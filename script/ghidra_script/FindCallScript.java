import static org.junit.Assert.assertNotNull;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.collections4.BidiMap;
import org.apache.commons.collections4.bidimap.DualHashBidiMap;
import docking.widgets.filter.ContainsTextFilterFactory;
import docking.widgets.filter.TextFilter;
import docking.widgets.filter.TextFilterFactory;
import edu.uci.ics.jung.graph.Graph;
import ghidra.app.script.GhidraScript;
import ghidra.feature.fid.db.FidFile;
import ghidra.feature.fid.db.FidFileManager;
import ghidra.graph.GDirectedGraph;
import ghidra.graph.GEdge;
import ghidra.graph.jung.JungDirectedGraph;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.block.BasicBlockModel;
import ghidra.program.model.block.CodeBlock;
import ghidra.program.model.block.CodeBlockIterator;
import ghidra.program.model.block.CodeBlockModel;
import ghidra.program.model.block.CodeBlockReference;
import ghidra.program.model.block.CodeBlockReferenceIterator;
import ghidra.program.model.block.graph.CodeBlockVertex;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.FlowType;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

public class FindCallScript extends GhidraScript {

	@Override
	protected void run() throws Exception {
		ContainsTextFilterFactory factory = new ContainsTextFilterFactory(false, true);

		find_send_msg(factory);
		findFunction(factory,"SendMessageMgr::sendMsg");
		findFunction(factory,"AppMsgMgr::sendFile");
		findFunction(factory,"SendMessageMgr::sendImageMsg");
		findFunction(factory,"SyncMgr::doAddMsg");
		findFunction(factory,"ChatRoomMgr::doModChatRoomMemberNickName");
		
		findFunction2(factory,"New_ChatMsg");
		findFunction2(factory,"Free_ChatMsg");
		findFunction2(factory,"New_ChatRoomInfo");
		findFunction2(factory,"Free_ChatRoomInfo");
		findFunction2(factory,"New_ChatRoom");
		findFunction2(factory,"Free_ChatRoom");
		findFunction2(factory,"New_Contact");
		findFunction2(factory,"Free_Contact");
		findFunction2(factory,"New_WAUpdatableMsgInfo");
		findFunction2(factory,"Free_WAUpdatableMsgInfo");
		findFunction2(factory,"New_AppMsgInfo");
		findFunction2(factory,"Free_AppMsgInfo");
		findFunction2(factory,"New_MMReaderItem");
		findFunction2(factory,"Free_MMReaderItem");
		findFunction2(factory,"New_WCPayInfo");
		findFunction2(factory,"Free_WCPayInfo");
		findFunction2(factory,"New_WebViewPageConfig");
		findFunction2(factory,"Free_WebViewPageConfig");
		
		findMgr(factory, "SendMessageMgr::SendMessageMgr", "GetSendMessageMgr");
		findMgr(factory, "AccountServiceMgr::AccountServiceMgr", "GetAccountServiceMgr");
		findMgr(factory, "AppMsgMgr::AppMsgMgr", "GetAppMsgMgr");
		findMgr(factory, "ContactMgr::ContactMgr", "GetContactMgr");
		findMgr(factory, "ChatRoomMgr::ChatRoomMgr", "GetChatRoomMgr");
		findMgr(factory, "SNSDataMgr::SNSDataMgr", "GetSNSDataMgr");
		findMgr(factory, "SNSTimeLineMgr::SNSTimeLineMgr", "GetSNSTimeLineMgr");
		findMgr(factory, "ChatMgr::ChatMgr", "GetChatMgr");
		findMgr(factory, "FavoriteMgr::FavoriteMgr", "GetFavoriteMgr");
		findMgr(factory, "PreDownLoadMgr::PreDownLoadMgr", "GetPreDownLoadMgr");
		findMgr(factory, "CustomSmileyMgr::CustomSmileyMgr", "GetCustomSmileyMgr");
		findMgr(factory, "LockWechatMgr::LockWechatMgr", "GetLockWechatMgr");
		findMgr(factory, "QRCodeLoginMgr::QRCodeLoginMgr", "GetQRCodeLoginMgr");
		findMgr(factory, "VoiceMgr::VoiceMgr", "GetVoiceMgr");
		findMgr(factory, "WebViewMgr::WebViewMgr", "GetWebViewMgr");
		findMgr(factory, "SearchContactMgr::SearchContactMgr", "GetSearchContactMgr");
		findMgr(factory, "PublicMsgMgr::PublicMsgMgr", "GetPublicMsgMgr");
		findMgr(factory, "MultiDBMsgMgr::MultiDBMsgMgr", "GetMultiDBMsgMgr");
		findMgr(factory, "FavoriteStorageMgr::FavoriteStorageMgr", "GetFavoriteStorageMgr");
		findMgr(factory, "HardLinkMgr::HardLinkMgr", "GetHardLinkMgr");

	}

	private boolean checkFidFile() {
		FidFileManager fidFileManager = FidFileManager.getInstance();
		List<FidFile> userFid = fidFileManager.getFidFiles();
		if (userFid.isEmpty()) {
			printerr("please import 3.9.11.25.fid  fid file.");
			return false;
		}
		return true;
	}

	void find_send_msg(ContainsTextFilterFactory factory) {
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		List<Symbol> searchSymbol = searchSymbol(symbolTable, "SendMessageMgr::sendMsg", factory,
				"SendMessageMgr::sendMsg");
		for (Symbol symbol : searchSymbol) {
			println("const uint64_t kSendTextMsg = " + symbol.getAddress().toString());
		}
	}

	void findFunction(ContainsTextFilterFactory factory, String functionName) {
		assertNotNull(factory);
		assertNotNull(functionName);
		String name = "";
		String[] split = functionName.split("::");
		if (split.length > 1) {
			name = split[1];
			char upperCase = Character.toUpperCase(split[1].charAt(0));
			name = upperCase + split[1].substring(1);
		} else {
			char upperCase = Character.toUpperCase(functionName.charAt(0));
			name = upperCase + functionName.substring(1);
		}
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		List<Symbol> searchSymbol = searchSymbol(symbolTable, functionName, factory, functionName);
		for (Symbol symbol : searchSymbol) {
			println("const uint64_t k" + name + " = " + symbol.getAddress().toString());
		}
	}
	
	void findFunction2(ContainsTextFilterFactory factory, String functionName) {
		assertNotNull(factory);
		assertNotNull(functionName);
		String name = "";
		String split = functionName.replace("_", "");
		char upperCase = Character.toUpperCase(split.charAt(0));
		name = upperCase + split.substring(1);
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		List<Symbol> searchSymbol = searchSymbol(symbolTable, functionName, factory, functionName);
		for (Symbol symbol : searchSymbol) {
			println("const uint64_t k" + name + " = " + symbol.getAddress().toString());
		}
	}

	void find_Mgr(ContainsTextFilterFactory factory) throws CancelledException {
		if (!checkFidFile()) {
			return;
		}
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		FunctionManager functionManager = currentProgram.getFunctionManager();
		ReferenceManager referenceManager = currentProgram.getReferenceManager();

		List<Address> addresses = new ArrayList<>();
		List<Symbol> searchMgr = searchSymbol(symbolTable, "SendMessageMgr::SendMessageMgr", factory,
				"SendMessageMgr::SendMessageMgr");
		for (Symbol symbol : searchMgr) {
			Address address = symbol.getAddress();
			ReferenceIterator referencesTo = referenceManager.getReferencesTo(address);
			while (referencesTo.hasNext()) {
				Reference next = referencesTo.next();
				Address fromAddress = next.getFromAddress();
				println("fromAddress  " + fromAddress.toString());
				Function functionAt = functionManager.getFunctionContaining(fromAddress);
				if (null != functionAt) {
					addresses.add(functionAt.getEntryPoint());
				}
			}
		}

		List<Symbol> searchSymbol = searchSymbol(symbolTable, "GetSendMessageMgr", factory, "GetSendMessageMgr");
		for (Symbol symbol : searchSymbol) {
			Address address = symbol.getAddress();
			if (addresses.contains(address)) {
				println("const uint64_t kGetSendMessageMgr = " + address.toString() + ";");
			}
		}

	}

	void find_SendMessageMgr(ContainsTextFilterFactory factory) throws CancelledException {
		if (!checkFidFile()) {
			return;
		}
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		FunctionManager functionManager = currentProgram.getFunctionManager();

		List<Address> addresses = new ArrayList<>();
		List<Symbol> searchMgr = searchSymbol(symbolTable, "SendMessageMgr::SendMessageMgr", factory,
				"SendMessageMgr::SendMessageMgr");
		for (Symbol symbol : searchMgr) {
			Address address = symbol.getAddress();
			addresses.add(address);
		}

		List<Symbol> searchSymbol = searchSymbol(symbolTable, "GetSendMessageMgr", factory, "GetSendMessageMgr");
		for (Symbol symbol : searchSymbol) {
			Address address = symbol.getAddress();
			Function function = functionManager.getFunctionAt(address);
			if (null != function) {
				AddressSetView functionBody = function.getBody();
				Address entryPoint = function.getEntryPoint();
				Set<Reference> references = getReferencesFrom(currentProgram, functionBody, monitor);
				for (Reference reference : references) {
					monitor.checkCancelled();
					Address toAddress = reference.getToAddress();
					if (toAddress.equals(entryPoint)) {
						continue;
					}
					Function calledFunction = functionManager.getFunctionAt(toAddress);
					if (null != calledFunction) {
						if (addresses.contains(toAddress)) {
							println("const uint64_t kGetSendMessageMgr = " + address.toString() + ";");
						}
					}
				}
			}

		}

	}

	void findMgr(ContainsTextFilterFactory factory, String mgr, String getMgr) throws CancelledException {
		if (!checkFidFile()) {
			return;
		}
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		FunctionManager functionManager = currentProgram.getFunctionManager();

		List<Address> addresses = new ArrayList<>();
		List<Symbol> searchMgr = searchSymbol(symbolTable, mgr, factory, mgr);
		for (Symbol symbol : searchMgr) {
			Address address = symbol.getAddress();
			addresses.add(address);
		}

		List<Symbol> searchSymbol = searchSymbol(symbolTable, getMgr, factory, getMgr);
		for (Symbol symbol : searchSymbol) {
			Address address = symbol.getAddress();
			Function function = functionManager.getFunctionAt(address);
			if (null != function) {
				AddressSetView functionBody = function.getBody();
				Address entryPoint = function.getEntryPoint();
				Set<Reference> references = getReferencesFrom(currentProgram, functionBody, monitor);
				for (Reference reference : references) {
					monitor.checkCancelled();
					Address toAddress = reference.getToAddress();
					if (toAddress.equals(entryPoint)) {
						continue;
					}
					Function calledFunction = functionManager.getFunctionAt(toAddress);
					if (null != calledFunction) {
						if (addresses.contains(toAddress)) {
							println("const uint64_t k" + getMgr + " = " + address.toString() + " ;");
						}
					}
				}
			}

		}

	}

	protected Set<Reference> getReferencesFrom(Program program, AddressSetView addresses, TaskMonitor monitor)
			throws CancelledException {
		Set<Reference> set = new HashSet<>();
		ReferenceManager referenceManager = program.getReferenceManager();
		AddressIterator addressIterator = addresses.getAddresses(true);
		while (addressIterator.hasNext()) {
			monitor.checkCancelled();
			Address address = addressIterator.next();
			Reference[] referencesFrom = referenceManager.getReferencesFrom(address);
			if (referencesFrom != null) {
				for (Reference reference : referencesFrom) {
					set.add(reference);
				}
			}
		}
		return set;
	}

	List<Symbol> searchSymbol(SymbolTable symbolTable, String text, TextFilterFactory factory, String filterText) {
		List<Symbol> list = new ArrayList<>();
		TextFilter textFilter = factory.getTextFilter(filterText);
		SymbolIterator symbols = symbolTable.getSymbols(text);
		while (symbols.hasNext()) {
			Symbol next = symbols.next();
			if (textFilter.matches(next.getName())) {
				list.add(next);
			}
		}
		return list;
	}

}

class GraphFunctionFactory {

	public static GDirectedGraph<FunctionVertex, FunctionEdge> createGraph(Function f, TaskMonitor monitor)
			throws CancelledException {

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

		return graph;
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