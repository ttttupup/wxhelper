import java.util.ArrayList;
import java.util.List;

import docking.widgets.filter.ContainsTextFilterFactory;
import docking.widgets.filter.TextFilter;
import docking.widgets.filter.TextFilterFactory;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;

public class FindCallScript extends GhidraScript {

	
	@Override
	protected void run() throws Exception {
		ContainsTextFilterFactory containsTextFilterFactory = new ContainsTextFilterFactory(false, true);
		
		find_send_msg(containsTextFilterFactory);
	}
	
	void find_send_msg(ContainsTextFilterFactory factory) {
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		List<Symbol> searchSymbol = searchSymbol(symbolTable,"SendMessageMgr::sendMsg",factory,"SendMessageMgr::sendMsg");
		for (Symbol symbol : searchSymbol) {
			println("found SendMessageMgr::sendMsg" + symbol.getAddress().toString());
			println("=================copy source========================");
			printf("const uint64_t kSendTextMsg = " + symbol.getAddress().toString() + "; \n");
		}
	}
	
	void find_sendMessageMgr(ContainsTextFilterFactory factory) {
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		List<Symbol> searchSymbol = searchSymbol(symbolTable,"SendMessageMgr::SendMessageMgr",factory,"SendMessageMgr::SendMessageMgr");
		for (Symbol symbol : searchSymbol) {
			
		}
	}
	
	List<Symbol> searchSymbol(SymbolTable symbolTable,String text,TextFilterFactory factory,String filterText) {
		List<Symbol> list = new ArrayList<>();
		TextFilter textFilter = factory.getTextFilter(filterText);
		SymbolIterator symbols = symbolTable.getSymbols(text);
		while (symbols.hasNext()) {
			Symbol next = symbols.next();
			if(textFilter.matches(next.getName())) {
				list.add(next);
			}
		}
		return list;
	}

}