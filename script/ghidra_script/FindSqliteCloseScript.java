import java.util.List;

import docking.widgets.filter.ContainsTextFilterFactory;
import docking.widgets.filter.TextFilter;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.script.GhidraScript;
import ghidra.feature.fid.hash.FidHashQuad;
import ghidra.feature.fid.service.FidService;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.util.string.FoundString;
import ghidra.util.UndefinedFunction;

public class FindSqliteCloseScript extends GhidraScript {
	FidService service;
	DecompInterface decompiler;
	
	long sql3close_full_hash =   -1574752015815545339L;
	long sql3close_spec_hash =  -1262376715505923342L;

	@Override
	protected void run() throws Exception {
		service = new FidService();

		decompiler = new DecompInterface();
		DecompileOptions decompileOptions = new DecompileOptions();
		decompiler.setOptions(decompileOptions);
		decompiler.openProgram(currentProgram);

		List<FoundString> findStrings = findStrings(null, 5, 1, true, false);
		ContainsTextFilterFactory containsTextFilterFactory = new ContainsTextFilterFactory(false, true);
		find_sqlite3Close(findStrings,containsTextFilterFactory);

	}
	


	void find_sqlite3Close(List<FoundString> list, ContainsTextFilterFactory factory) throws MemoryAccessException {
		int size = currentProgram.getLanguage().getLanguageDescription().getSize() / 8;
		FunctionManager functionManager = currentProgram.getFunctionManager();
		ReferenceManager referenceManager = currentProgram.getReferenceManager();
		TextFilter textFilter = factory.getTextFilter("unable to close due to unfinalized statements or unfinished backups");
		for (FoundString foundString : list) {
			if(monitor.isCancelled()) {
				break;
			}
			String string = foundString.getString(currentProgram.getMemory());
			if(textFilter.matches(string)) {
				Address address = foundString.getAddress();
				println("=====text addr :"+address.toString() );
				ReferenceIterator referencesTo = referenceManager.getReferencesTo(address);
				while (referencesTo.hasNext()) {
					Reference ref = referencesTo.next();
					Address sql3CloseAddr = ref.getFromAddress();
					Function sql3CloseFunction = functionManager.getFunctionContaining(sql3CloseAddr);
					if(null ==sql3CloseFunction ) {
						continue;
					}
					FidHashQuad hashQuad = service.hashFunction(sql3CloseFunction);
					if(hashQuad.getFullHash() == sql3close_full_hash && hashQuad.getSpecificHash() == sql3close_spec_hash) {
						println("found sql3Close :" + sql3CloseFunction.getName() + " at " + sql3CloseFunction.getEntryPoint() +
								" full hash   " + hashQuad.getFullHash()   + " specific hash " + hashQuad.getSpecificHash());
						Address entryPoint = sql3CloseFunction.getEntryPoint();
						ReferenceIterator referencesTo2 = referenceManager.getReferencesTo(entryPoint);
						while (referencesTo2.hasNext()) {
							Reference next = referencesTo2.next();
							Address  addr = next.getFromAddress();
							Function sql_close_func = functionManager.getFunctionContaining(addr);
							if(null !=sql_close_func ) {
								continue;
							}

							CodeUnit codeUnitAt = currentProgram.getListing().getCodeUnitAt(addr);
							if(null != codeUnitAt) {
								String mnemonicString = codeUnitAt.getMnemonicString();
								if("JMP".equals(mnemonicString)) {
									CodeUnit codeUnitBefore = currentProgram.getListing().getCodeUnitBefore(addr);
									String mnemonicString2 = codeUnitBefore.getMnemonicString();
									Function function = UndefinedFunction.findFunction(currentProgram, addr, monitor);
									if (null != function) {
										Address entryPoint2 = function.getEntryPoint();
										if("XOR".equals(mnemonicString2)) {
											createFunction(entryPoint2, "sqlite3_close");
											currentProgram.getListing().setComment(entryPoint2, CodeUnit.POST_COMMENT,
													"auto define function: sqlite3_close");
											println("sqlite3_close  addr: " + entryPoint2.toString());
										}else if ("MOV".equals(mnemonicString2)) {
											createFunction(entryPoint2, "sqlite3_close_v2");
											currentProgram.getListing().setComment(entryPoint2, CodeUnit.POST_COMMENT,
													"auto define function : sqlite3_close_v2" );
											println("sqlite3_close_v2  addr: " + entryPoint2.toString());
										}
										ReferenceIterator referencesTo3 = referenceManager.getReferencesTo(entryPoint2);
										while (referencesTo3.hasNext()) {
											Reference next2 = referencesTo3.next();
											Address fromAddress = next2.getFromAddress();
											if("XOR".equals(mnemonicString2)) {
												long l = fromAddress.getOffset() - 16*size;
												println("sqlite3_close  xref:" + fromAddress.toString());
												printf("sqlite3_api_routines  addr: %x \n", l);
											}else if ("MOV".equals(mnemonicString2)) {
												long l = fromAddress.getOffset()- 179*size;
												println("sqlite3_close_v2  xref:" +fromAddress.toString());
												printf("sqlite3_api_routines  addr: %x \n", l);
											}
											
										}
									}
									
								}
								
							}
							
						}
					}
			
				}
			
			}
		}
		
		
		
	}

}
