// Repair a function whose decompilation fails on metadata (e.g. "Attribute
// size is not present") even though every instruction's Pcode decodes.
// Prints the current signature/storage, clears custom variable storage and
// parameters back to defaults, then re-tries decompilation transiently.
// Arg: hex address.
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.symbol.SourceType;

public class FixDecompMeta extends GhidraScript {
  @Override public void run() throws Exception {
    Address ad = toAddr(Long.parseLong(getScriptArgs()[0].replace("0x", ""), 16));
    Function f = getFunctionAt(ad);
    if (f == null) { println("no function at " + ad); return; }

    println("FIXMETA before: " + f.getPrototypeString(true, true)
        + " customStorage=" + f.hasCustomVariableStorage()
        + " cc=" + f.getCallingConventionName());
    for (Parameter p : f.getParameters()) {
      println("  param " + p.getName() + " storage=" + p.getVariableStorage());
    }

    f.setCustomVariableStorage(false);
    f.replaceParameters(Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
        true, SourceType.DEFAULT);
    f.setReturnType(ghidra.program.model.data.DataType.DEFAULT,
        SourceType.DEFAULT);

    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);
    DecompileResults res = di.decompileFunction(f, 120, monitor);
    boolean ok = res != null && res.decompileCompleted()
        && res.getDecompiledFunction() != null;
    println("FIXMETA after reset: " + (ok ? "OK ("
        + res.getDecompiledFunction().getC().length() + " chars)"
        : "STILL FAILED err=" + (res != null ? res.getErrorMessage() : "null")));
    di.dispose();
  }
}
