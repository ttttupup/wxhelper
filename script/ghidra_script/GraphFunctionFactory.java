import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.apache.commons.collections4.BidiMap;
import org.apache.commons.collections4.bidimap.DualHashBidiMap;

import edu.uci.ics.jung.graph.Graph;
import generic.stl.Pair;
import ghidra.graph.GDirectedGraph;
import ghidra.graph.GEdge;
import ghidra.graph.jung.JungDirectedGraph;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.block.BasicBlockModel;
import ghidra.program.model.block.CodeBlock;
import ghidra.program.model.block.CodeBlockIterator;
import ghidra.program.model.block.CodeBlockModel;
import ghidra.program.model.block.CodeBlockReference;
import ghidra.program.model.block.CodeBlockReferenceIterator;
import ghidra.program.model.block.graph.CodeBlockVertex;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.FlowType;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

public class GraphFunctionFactory {

	public static Pair<GDirectedGraph<FunctionVertex, FunctionEdge>, BidiMap<CodeBlock, FunctionVertex>> createGraph(
			Function f, TaskMonitor monitor) throws CancelledException {
		if (monitor == null) {
			monitor = TaskMonitor.DUMMY;
		}

		JungDirectedGraph<FunctionVertex, FunctionEdge> graph = new JungDirectedGraph<>();

		BidiMap<CodeBlock, FunctionVertex> vertices = createVertices(f, monitor);
		for (FunctionVertex v : vertices.values()) {
			graph.addVertex(v);
		}

		Collection<FunctionEdge> edges = createdEdges(vertices, monitor);
		for (FunctionEdge e : edges) {
			graph.addEdge(e);
		}

		// this must be done after the edges are added to the graph
		for (FunctionVertex vertex : vertices.values()) {
			vertex.setType(getVertexType(graph, vertex));
		}
		return new Pair<GDirectedGraph<FunctionVertex, FunctionEdge>, BidiMap<CodeBlock, FunctionVertex>>(graph,
				vertices);
	}

	private static BidiMap<CodeBlock, FunctionVertex> createVertices(Function f, TaskMonitor monitor)
			throws CancelledException {

		BidiMap<CodeBlock, FunctionVertex> vertices = new DualHashBidiMap<>();
		Program p = f.getProgram();
		CodeBlockModel blockModel = new BasicBlockModel(p);

		AddressSetView addresses = f.getBody();
		CodeBlockIterator iterator = blockModel.getCodeBlocksContaining(addresses, monitor);
		monitor.initialize(addresses.getNumAddresses());

		for (; iterator.hasNext();) {
			CodeBlock codeBlock = iterator.next();

			FlowType flowType = codeBlock.getFlowType();
			boolean isEntry = isEntry(codeBlock);

			FunctionVertex vertex = new FunctionVertex(codeBlock, flowType, isEntry);

			vertices.put(codeBlock, vertex);

			long blockAddressCount = codeBlock.getNumAddresses();
			long currentProgress = monitor.getProgress();
			monitor.setProgress(currentProgress + blockAddressCount);
		}

		return vertices;
	}

	private static Collection<FunctionEdge> createdEdges(BidiMap<CodeBlock, FunctionVertex> vertices,
			TaskMonitor monitor) throws CancelledException {

		List<FunctionEdge> edges = new ArrayList<>();
		for (FunctionVertex startVertex : vertices.values()) {
			Collection<FunctionEdge> vertexEdges = getEdgesForStartVertex(vertices, startVertex, monitor);

			edges.addAll(vertexEdges);
		}

		return edges;
	}

	private static Collection<FunctionEdge> getEdgesForStartVertex(BidiMap<CodeBlock, FunctionVertex> blockToVertexMap,
			FunctionVertex startVertex, TaskMonitor monitor) throws CancelledException {

		List<FunctionEdge> edges = new ArrayList<>();
		CodeBlock codeBlock = blockToVertexMap.getKey(startVertex);
		CodeBlockReferenceIterator destinations = codeBlock.getDestinations(monitor);
		for (; destinations.hasNext();) {
			CodeBlockReference reference = destinations.next();
			CodeBlock destinationBlock = reference.getDestinationBlock();
			FunctionVertex destinationVertex = blockToVertexMap.get(destinationBlock);
			if (destinationVertex == null) {
				continue;// no vertex means the code block is not in our function
			}

			edges.add(new FunctionEdge(startVertex, destinationVertex, reference.getFlowType()));
		}
		return edges;
	}

	private static VertexType getVertexType(Graph<FunctionVertex, FunctionEdge> graph, FunctionVertex v) {

		boolean isEntry = v.isEntry();
		boolean isExit = false;

		FlowType flowType = v.getFlowType();
		if (flowType.isTerminal()) {
			isExit = true;
		}

		Collection<FunctionEdge> outs = graph.getOutEdges(v);
		if (outs == null || outs.isEmpty()) {
			isExit = true;
		}

		VertexType type = VertexType.BODY;
		if (isEntry) {
			if (isExit) {
				type = VertexType.SINGLETON;
			} else {
				type = VertexType.ENTRY;
			}
		} else if (isExit) {
			type = VertexType.EXIT;
		}
		return type;
	}

	private static boolean isEntry(CodeBlock codeBlock) {
		boolean isSource = true;
		try {
			CodeBlockReferenceIterator iter = codeBlock.getSources(TaskMonitor.DUMMY);
			while (iter.hasNext()) {
				isSource = false;
				if (iter.next().getFlowType().isCall()) {
					// any calls into a code block will make it an 'entry'
					return true;
				}
			}
		} catch (CancelledException e) {
			// will never happen, because I don't have a monitor
		}
		return isSource;
	}
}

class FunctionVertex extends CodeBlockVertex {

	private VertexType type = VertexType.BODY;
	private FlowType flowType;
	private boolean isEntry;

	public FunctionVertex(CodeBlock codeBlock, FlowType flowType, boolean isEntry) {
		super(codeBlock);
		this.flowType = flowType;
		this.isEntry = isEntry;
	}

	public boolean isEntry() {
		return isEntry;
	}

	public FlowType getFlowType() {
		return flowType;
	}

	public VertexType getType() {
		return type;
	}

	public void setType(VertexType type) {
		this.type = type;
	}
}

class FunctionEdge implements GEdge<FunctionVertex> {

	private FunctionVertex start;
	private FunctionVertex end;
	private FlowType flowType;

	public FunctionEdge(FunctionVertex start, FunctionVertex end, FlowType flowType) {
		this.start = start;
		this.end = end;
		this.flowType = flowType;
	}

	public FlowType getFlowType() {
		return flowType;
	}

	@Override
	public FunctionVertex getStart() {
		return start;
	}

	@Override
	public FunctionVertex getEnd() {
		return end;
	}
}

enum VertexType {

	//@formatter:off
	BODY,
	ENTRY,	
	EXIT,
	GROUP,		
	SINGLETON;
	//@formatter:on

	public boolean isEntry() {
		return this == ENTRY || this == SINGLETON;
	}

	public boolean isExit() {
		return this == EXIT || this == SINGLETON;
	}

}
