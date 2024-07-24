import java.util.Iterator;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.PcodeOpAST;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;

public class NamedScript extends GhidraScript {

	@Override
	protected void run() throws Exception {
		Address selectAddress = askAddress("选择log地址", "wechat  log地址：");
		Listing listing = currentProgram.getListing();
		FunctionManager functionManager = currentProgram.getFunctionManager();
		ReferenceManager referenceManager = currentProgram.getReferenceManager();
		AddressFactory addressFactory = currentProgram.getAddressFactory();

		AddressSpace space = addressFactory.getDefaultAddressSpace();
		AddressSpace[] addressSpaces = addressFactory.getAddressSpaces();

		DecompInterface decompiler = new DecompInterface();
		DecompileOptions decompileOptions = new DecompileOptions();
		decompiler.setOptions(decompileOptions);
		decompiler.openProgram(currentProgram);

	
		Address funcAddress = selectAddress;
		Function logFunction = functionManager.getFunctionAt(funcAddress);
//		for (AddressSpace sp : addressSpaces) {
//			funcAddress = sp.getAddress(selectAddress.getOffset());
//			logFunction = functionManager.getFunctionAt(funcAddress);
//			space = sp;
//			if (null != logFunction) {
//				break;
//			}
//		}

		ReferenceIterator referenceIterator = referenceManager.getReferencesTo(logFunction.getEntryPoint());
		for (Reference ref : referenceIterator) {
			Address fromAddress = ref.getFromAddress();
//			if (fromAddress.getOffset() != 0x18284bb19L) {
//				continue;
//			}
			handle(fromAddress, functionManager, decompiler, space, listing,selectAddress);
			println("caller address: " + fromAddress);
		}

	}

	private void handle(Address address, FunctionManager functionManager, DecompInterface decompiler,
			AddressSpace space, Listing listing,Address selectAddress) {
		Function functionContaining = functionManager.getFunctionContaining(address);
		if (null == functionContaining) {
			println("no found function: " + address);
			return;
		}
		DecompileResults res = decompiler.decompileFunction(functionContaining, 20, null);
		if (!res.decompileCompleted()) {
			println("Symbol " + res.getErrorMessage());
			return;
		}
		HighFunction highFunction = res.getHighFunction();
		Iterator<PcodeOpAST> pcodeOps = highFunction.getPcodeOps();
		while (pcodeOps.hasNext()) {
			PcodeOpAST next = pcodeOps.next();
			int opcode = next.getOpcode();
			int numInputs = next.getNumInputs();
			if (PcodeOp.CALL == opcode || PcodeOp.CALLIND == opcode || PcodeOp.CALLOTHER == opcode) {
				Varnode input0 = next.getInput(0);
				boolean contains = input0.contains(selectAddress);

				if (contains) {
					Varnode input4 = next.getInput(4);
					if (input4 == null) {
						continue;
					}
					PcodeOp def4 = input4.getDef();
					if (null != def4) {
						Address param4Address = null;

						if (def4.getOpcode() == PcodeOp.COPY) {
							Varnode vn0 = def4.getInput(0);
							param4Address = space.getAddress(vn0.getAddress().getOffset());

							Data dataAt = listing.getDataAt(param4Address);
							if (dataAt.hasStringValue()) {
								Object value = dataAt.getValue();
								String funcStr = value.toString();
								println("func:" + funcStr);
								try {
									functionContaining.setName(funcStr, SourceType.USER_DEFINED);
									listing.setComment(functionContaining.getEntryPoint(), CodeUnit.EOL_COMMENT,
											"auto define:" + funcStr);
								} catch (DuplicateNameException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								} catch (InvalidInputException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
							}
						}

					}

				}

			}
		}
	}

}
