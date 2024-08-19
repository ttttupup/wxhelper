import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import docking.widgets.filter.ContainsTextFilterFactory;
import docking.widgets.filter.TextFilter;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.feature.fid.hash.FidHashQuad;
import ghidra.feature.fid.service.FidService;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.PcodeOpAST;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.util.string.FoundString;
import ghidra.util.exception.CancelledException;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;

public class NamedScript extends GhidraScript {

	long wxlog_full_hash = 0xf9f7e7c3081f5aa3L;
	long wxlog_spec_hash = 0x9e71b418c749e4faL;
	FidService service;

	@Override
	protected void run() throws Exception {
		service = new FidService();

		Address selectAddress = askAddress("填写手动确认的wxlog地址", "wxlog地址或者任意值进行搜索：");
		Listing listing = currentProgram.getListing();
		FunctionManager functionManager = currentProgram.getFunctionManager();
		ReferenceManager referenceManager = currentProgram.getReferenceManager();
		AddressFactory addressFactory = currentProgram.getAddressFactory();

		AddressSpace space = addressFactory.getDefaultAddressSpace();
		DecompInterface decompiler = new DecompInterface();
		DecompileOptions decompileOptions = new DecompileOptions();
		decompiler.setOptions(decompileOptions);
		decompiler.openProgram(currentProgram);

		Address funcAddress = selectAddress;

		Function logFunction = functionManager.getFunctionAt(funcAddress);

		if (null == logFunction) {
			logFunction = findLogFunc();
			if (null == logFunction) {
				printerr("未能成功自动匹配wxlog函数,请手动查找后填写正确地址 ");
				return;
			}

		}

		ReferenceIterator referenceIterator = referenceManager.getReferencesTo(logFunction.getEntryPoint());
		while (referenceIterator.hasNext()) {
			monitor.checkCancelled();
			Reference next = referenceIterator.next();
			Address fromAddress = next.getFromAddress();
			println("lookup address: " + fromAddress.toString());
			handle(fromAddress, functionManager, decompiler, space, listing, selectAddress);
			println("caller address: " + fromAddress.toString());
		}

	}

	private Function findLogFunc() throws MemoryAccessException, CancelledException {

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
			TextFilter textFilter = containsTextFilterFactory.getTextFilter("(%d-%d-%d:%d:%02d:%02d:%03d %05d)-%s/%s:");
			for (FoundString foundString : findStrings) {
				if (monitor.isCancelled()) {
					break;
				}
				String string = foundString.getString(currentProgram.getMemory());
				if (textFilter.matches(string)) {
					Address address = foundString.getAddress();

					ReferenceIterator referencesTo = referenceManager.getReferencesTo(address);
					while (referencesTo.hasNext()) {
						if (monitor.isCancelled()) {
							break;
						}
						Reference ref = referencesTo.next();
						Address wxlogAddr = ref.getFromAddress();
						Function targetFunction = functionManager.getFunctionContaining(wxlogAddr);
						if (null == targetFunction) {
							continue;
						}
						FidHashQuad hashQuad = service.hashFunction(targetFunction);
						if (hashQuad.getFullHash() == wxlog_full_hash
								&& hashQuad.getSpecificHash() == wxlog_spec_hash) {
							return targetFunction;
						}
					}
				}
			}
		}
		return null;
	}

	private void handle(Address address, FunctionManager functionManager, DecompInterface decompiler,
			AddressSpace space, Listing listing, Address selectAddress) {
		Function functionContaining = functionManager.getFunctionContaining(address);
		if (null == functionContaining) {
			println("no found function: " + address.toString());
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
