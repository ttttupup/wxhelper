/* ###
 * IP: GHIDRA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//
// 根据一些日志类的相关函数的引用来获取当前函数的函数名
// 输入被调用函数的地址和可以作为函数名称的参数的位置
//
//@category Functions
import java.util.Iterator;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.PcodeOpAST;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.StringUtilities;

public class RenameScript extends GhidraScript {

	@Override
	protected void run() throws Exception {
		Address askAddress = askAddress("输入函数地址", "引用的函数地址");
		int askInt = askInt("输入参数位置", "函数名称的参数位置");
		DecompInterface decompiler = getDecompInterface();

		ReferenceIterator referenceIterator = currentProgram.getReferenceManager().getReferencesTo(askAddress);
		for (Reference ref : referenceIterator) {
			monitor.checkCancelled();
			Address fromAddress = ref.getFromAddress();
			Function functionContaining = currentProgram.getFunctionManager().getFunctionContaining(fromAddress);
			if (null == functionContaining) {
				println("no found function: " + fromAddress.toString());
				continue;
			}
			DecompileResults res = decompiler.decompileFunction(functionContaining, 30, null);
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
					boolean contains = input0.contains(askAddress);
					if (!contains) {
						continue;
					}
					Varnode inputVarnode = opAST.getInput(askInt);
					String nameString = null;
					if (inputVarnode.isUnique()) {
						nameString = getUniqueValue(inputVarnode);

					} else if (inputVarnode.isConstant()) {
						nameString = getConstact(inputVarnode);
					}

					if (StringUtilities.isAllBlank(nameString)) {
						continue;
					}
					String name = nameString.replace(" ", "");
					println("find function :" + name);
					SourceType signatureSource = functionContaining.getSignatureSource();
					if (signatureSource.isLowerPriorityThan(SourceType.USER_DEFINED)) {
						functionContaining.setName(name, SourceType.USER_DEFINED);
						currentProgram.getListing().setComment(functionContaining.getEntryPoint(), CodeUnit.POST_COMMENT,
								"script rename: " + name);
					}
					break;
				}
			}
		}
	}

	private DecompInterface getDecompInterface() {
		DecompInterface decompiler = new DecompInterface();
		DecompileOptions decompileOptions = new DecompileOptions();
		decompiler.setOptions(decompileOptions);
		decompiler.openProgram(currentProgram);
		return decompiler;
	}

	private String getConstact(Varnode input3) {
		Address address2 = input3.getAddress();
		Address address3 = toAddr(address2.getOffset());
		Data dataAt2 = getDataAt(address3);
		if (null != dataAt2) {
			if (dataAt2.hasStringValue()) {
				return dataAt2.getValue().toString();
			}
		}
		return null;
	}

	private String getUniqueValue(Varnode input3) {
		PcodeOp def = input3.getDef();
		Varnode[] inputs = def.getInputs();
		if (null != inputs && inputs.length > 0) {
			if (inputs[0].isConstant()) {
				Address address2 = inputs[0].getAddress();
				Address address3 = toAddr(address2.getOffset());
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
}
