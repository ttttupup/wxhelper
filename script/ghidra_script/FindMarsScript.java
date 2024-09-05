// find  mars
//@author ttttupup
//@category _NEW_
//@keybinding 
//@menupath 
//@toolbar 

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import docking.widgets.filter.ContainsTextFilterFactory;
import docking.widgets.filter.TextFilter;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.block.BasicBlockModel;
import ghidra.program.model.block.CodeBlock;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.PcodeBlockBasic;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.PcodeOpAST;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.util.string.FoundString;
import ghidra.sleigh.grammar.SleighParser.stringorident_return;
import ghidra.util.StringUtilities;
import ghidra.util.exception.CancelledException;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;

public class FindMarsScript extends GhidraScript {
	DecompInterface decompiler = new DecompInterface();
	BasicBlockModel basicBlockModel;

	@Override
	protected void run() throws Exception {
		DecompileOptions decompileOptions = new DecompileOptions();
		decompiler.setOptions(decompileOptions);
		decompiler.openProgram(currentProgram);
		basicBlockModel = new BasicBlockModel(currentProgram);
		Address findXScopeTracer = findXScopeTracer();
//		Address findXScopeTracer = currentProgram.getAddressFactory().getAddress("0x183d04d10");
		if (null != findXScopeTracer) {
			ReferenceIterator referenceIterator = currentProgram.getReferenceManager()
					.getReferencesTo(findXScopeTracer);
			while (referenceIterator.hasNext()) {
				monitor.checkCancelled();
				Reference next = referenceIterator.next();
				Address fromAddress = next.getFromAddress();

				Function functionContaining = currentProgram.getFunctionManager().getFunctionContaining(fromAddress);
				if (null == functionContaining) {
					println("no found function: " + fromAddress.toString());
					continue;
				}
				DecompileResults res = decompiler.decompileFunction(functionContaining, 20, null);
				if (!res.decompileCompleted()) {
					println("decompile error: " + res.getErrorMessage());
					continue;
				}
				HighFunction highFunction = res.getHighFunction();
				Iterator<PcodeOpAST> pcodeOps = highFunction.getPcodeOps();
				while (pcodeOps.hasNext()) {
					monitor.checkCancelled();
					PcodeOpAST opAST = pcodeOps.next();
					int opcode = opAST.getOpcode();
					if (PcodeOp.CALL == opcode || PcodeOp.CALLIND == opcode || PcodeOp.CALLOTHER == opcode) {
						Varnode input0 = opAST.getInput(0);
						boolean contains = input0.contains(findXScopeTracer);

						if (contains) {
							Varnode input3 = opAST.getInput(3);
							Varnode input4 = opAST.getInput(4);
							if (input4 == null || input3 == null) {
								continue;
							}
							String namespaceString = "";
							String funcNameString = "";
							if (input3.isUnique()) {
								namespaceString = getUniqueValue(input3);
							} else if (input3.isConstant()) {
								namespaceString = getConstact(input3);
							}
							if (input4.isUnique()) {
								funcNameString = getUniqueValue(input4);
							} else if (input4.isConstant()) {
								funcNameString = getConstact(input4);
							}
							if (StringUtilities.isAllBlank(namespaceString)
									|| StringUtilities.isAllBlank(funcNameString)) {
								continue;
							}
							String name = namespaceString.replace(" ", "") + "::" + funcNameString.replace(" ", "");
							println("find function :" + name);
							functionContaining.setName(name, SourceType.USER_DEFINED);
							currentProgram.getListing().setComment(functionContaining.getEntryPoint(),
									CodeUnit.EOL_COMMENT, "mars scipt auto  define: " + name);
						}

					}
				}
			}

		}

	}

	private String getConstact(Varnode input3) {
		Address address2 = input3.getAddress();
		Address address3 = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(address2.getOffset());
		Data dataAt2 = getDataAt(address3);
		if (null != dataAt2) {
			if (dataAt2.hasStringValue()) {
				return dataAt2.getValue().toString();
			}
		}
		return null;
	}

	private void easy(Address findXScopeTracer) throws CancelledException {
		if (null != findXScopeTracer) {
			ReferenceIterator referenceIterator = currentProgram.getReferenceManager()
					.getReferencesTo(findXScopeTracer);
			while (referenceIterator.hasNext()) {
				monitor.checkCancelled();
				Reference next = referenceIterator.next();
				Address fromAddress = next.getFromAddress();
				Instruction instructionAt = currentProgram.getListing().getInstructionAt(fromAddress);
				if (null != instructionAt) {
					PcodeOp[] pcode = instructionAt.getPcode();
					if (null != pcode) {
						for (PcodeOp pcodeOp : pcode) {
							if (pcodeOp.getOpcode() == PcodeOp.CALL) {
								Varnode namespace = pcodeOp.getInput(3);
								Varnode funcName = pcodeOp.getInput(4);
								if (null != namespace) {
									if (namespace.isConstant()) {
										long offset = namespace.getAddress().getOffset();
										Address address = currentProgram.getAddressFactory().getDefaultAddressSpace()
												.getAddress(offset);
										Data dataAt = currentProgram.getListing().getDataAt(address);
										if (null != dataAt) {
											if (dataAt.hasStringValue()) {
												dataAt.getValue().toString();
											}
										}
									}
								}
								if (null != funcName) {
									if (funcName.isConstant()) {
										long offset = funcName.getAddress().getOffset();
										Address address = currentProgram.getAddressFactory().getDefaultAddressSpace()
												.getAddress(offset);
										Data dataAt = currentProgram.getListing().getDataAt(address);
										if (null != dataAt) {
											if (dataAt.hasStringValue()) {
												dataAt.getValue().toString();
											}
										}
									}
								}
							}
						}
					}

					println("caller address: " + fromAddress.toString());
				}
			}
		}
	}

	private void find() throws Exception {
		Function findXLogger = findXLogger();
		if (null != findXLogger) {
			if (findXLogger.isThunk()) {
				currentProgram.getFunctionManager().createFunction("XLogger::operator()", findXLogger.getEntryPoint(),
						findXLogger.getBody(), findXLogger.getSignatureSource());
			} else {

				findXLogger.setName("XLogger::operator()", SourceType.USER_DEFINED);
				currentProgram.getListing().setComment(findXLogger.getEntryPoint(), CodeUnit.EOL_COMMENT,
						"rename function: XLogger::operator()");

				ReferenceIterator referenceIterator = currentProgram.getReferenceManager()
						.getReferencesTo(findXLogger.getEntryPoint());
				while (referenceIterator.hasNext()) {
					monitor.checkCancelled();
					Reference next = referenceIterator.next();
					Address fromAddress = next.getFromAddress();
					println("caller address: " + fromAddress.toString());
					handle(fromAddress, findXLogger.getEntryPoint());
				}
			}
		}
	}

	private Address findXScopeTracer() throws Exception {
		List<FoundString> list = findStrings(null, 5, 1, true, false);
		ContainsTextFilterFactory factory = new ContainsTextFilterFactory(false, true);
		TextFilter textFilter = factory.getTextFilter("mars::comm::DNS::GetHostByName");
		for (FoundString foundString : list) {
			monitor.checkCancelled();
			String string = foundString.getString(currentProgram.getMemory());
			if (textFilter.matches(string)) {
				Address address = foundString.getAddress();
				println("=====text addr :" + address.toString());

				ReferenceIterator referencesTo = currentProgram.getReferenceManager().getReferencesTo(address);
				while (referencesTo.hasNext()) {
					Reference ref = referencesTo.next();
					Address textAddress = ref.getFromAddress();
					Function func = currentProgram.getFunctionManager().getFunctionContaining(textAddress);
					if (null == func) {
						continue;
					}
					CodeBlock[] codeBlocksContaining = basicBlockModel.getCodeBlocksContaining(textAddress, monitor);
					for (CodeBlock codeBlock : codeBlocksContaining) {
						AddressIterator addressIterator = codeBlock.getAddresses(true);
						while (addressIterator.hasNext()) {
							Address next = addressIterator.next();
							CodeUnit codeUnitAt = currentProgram.getListing().getCodeUnitAt(next);
							if (null != codeUnitAt) {
								if (codeUnitAt.getMnemonicString().equals("CALL")) {
									Instruction instructionAt = currentProgram.getListing().getInstructionAt(next);
									PcodeOp[] pcode = instructionAt.getPcode();
									PcodeOp pcodeOp = pcode[pcode.length - 1];
									Varnode input = pcodeOp.getInput(0);
									Address address2 = input.getAddress();
									println("===== XScopeTracer::XScopeTracer addr :" + address2.toString());
									Function functionAt = currentProgram.getFunctionManager().getFunctionAt(address2);
									if (null != functionAt) {
										functionAt.setName("XScopeTracer::XScopeTracer", SourceType.USER_DEFINED);
										currentProgram.getListing().setComment(functionAt.getEntryPoint(),
												CodeUnit.EOL_COMMENT, "scipt auto  define: XScopeTracer::XScopeTracer");
									} else {
										createFunction(address2, "XScopeTracer::XScopeTracer");
										currentProgram.getListing().setComment(address2, CodeUnit.EOL_COMMENT,
												"scipt auto create: XScopeTracer::XScopeTracer");
									}

									return address2;
								}
							}

						}

					}
				}
			}
		}
		return null;

	}

	private void handle(Address address, Address calleeAddress) throws CancelledException {
		Function functionContaining = currentProgram.getFunctionManager().getFunctionContaining(address);
		if (null == functionContaining) {
			println("no found function: " + address.toString());
			return;
		}

		DecompileResults res = decompiler.decompileFunction(functionContaining, 20, null);
		if (!res.decompileCompleted()) {
			println("decompile error: " + res.getErrorMessage());
			return;
		}

		HighFunction highFunction = res.getHighFunction();
		ArrayList<PcodeBlockBasic> basicBlocks = highFunction.getBasicBlocks();

		for (PcodeBlockBasic pcodeBlockBasic : basicBlocks) {
			monitor.checkCancelled();
			boolean contains = pcodeBlockBasic.contains(address);
			if (contains) {
				Iterator<PcodeOp> iterator = pcodeBlockBasic.getIterator();
				for (; iterator.hasNext();) {
					monitor.checkCancelled();
					PcodeOp next = iterator.next();
					if (next.getOpcode() == PcodeOp.CALL) {
						if (next.getNumInputs() > 4) {
							Varnode input3 = next.getInput(3);
							if (null != input3) {
								if (input3.isUnique()) {
									String uniqueValue = getUniqueValue(input3);
									if (null != uniqueValue) {
										println(uniqueValue);
									}
								}
							}
						}
					}
				}
			}
		}

	}

	private String getUniqueValue(Varnode input3) {
		PcodeOp def = input3.getDef();
		Varnode[] inputs = def.getInputs();
		if (null != inputs && inputs.length > 0) {
			if (inputs[0].isConstant()) {
				Address address2 = inputs[0].getAddress();
				Address address3 = currentProgram.getAddressFactory().getDefaultAddressSpace()
						.getAddress(address2.getOffset());
				Data dataAt2 = currentProgram.getListing().getDataAt(address3);
				if (null != dataAt2) {
					if (dataAt2.hasStringValue()) {
						return dataAt2.getValue().toString();
					}
				}
			}
		}
		return null;
	}

	private Function findXLogger() throws MemoryAccessException, CancelledException {

		FunctionManager functionManager = currentProgram.getFunctionManager();
		ReferenceManager referenceManager = currentProgram.getReferenceManager();
		AddressFactory factory = currentProgram.getAddressFactory();
		AddressSpace[] addressSpaces = factory.getAddressSpaces();
		AddressSetView initializedMemory = currentProgram.getMemory().getLoadedAndInitializedAddressSet();

		AddressSet searchSet = initializedMemory.intersect(new AddressSet(initializedMemory));
		List<FoundString> findStrings = new ArrayList<>();
		println("初次查询速度较慢,请耐心等待。");
		for (AddressSpace space : addressSpaces) {
			if (monitor.isCancelled()) {
				break;
			}
			AddressSet intersecting = searchSet.intersectRange(space.getMinAddress(), space.getMaxAddress());
			findStrings.addAll(findStrings(null, 10, 1, true, true));
		}

		if (null != findStrings && findStrings.size() > 0) {
			ContainsTextFilterFactory containsTextFilterFactory = new ContainsTextFilterFactory(false, true);
			TextFilter textFilter = containsTextFilterFactory
					.getTextFilter("XLogger& XLogger::operator()(const char* _format, ...)");
			for (FoundString foundString : findStrings) {
				if (monitor.isCancelled()) {
					break;
				}
				String string = foundString.getString(currentProgram.getMemory());
				if (textFilter.matches(string)) {
					Address address = foundString.getAddress();
					println(address.toString());

					ReferenceIterator referencesTo = referenceManager.getReferencesTo(address);
					while (referencesTo.hasNext()) {
						if (monitor.isCancelled()) {
							break;
						}
						Reference ref = referencesTo.next();
						Address xLoggerAddr = ref.getFromAddress();
						Function targetFunction = functionManager.getFunctionContaining(xLoggerAddr);
						if (null == targetFunction) {
							continue;
						}
						println("XLogger::operator() ：" + targetFunction.getEntryPoint().toString());
						return targetFunction;
					}
				}
			}
		}
		return null;
	}

}
