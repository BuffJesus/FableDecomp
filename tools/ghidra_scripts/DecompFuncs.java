import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

// Decompile the functions whose hex addresses are passed as args, print C.
public class DecompFuncs extends GhidraScript {
  @Override public void run() throws Exception {
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);
    for (String a : getScriptArgs()) {
      Address ad = toAddr(Long.parseLong(a.replace("0x", ""), 16));
      Function f = getFunctionContaining(ad);
      if (f == null) {
        // Try to disassemble + create a function at this address.
        if (getInstructionAt(ad) == null) disassemble(ad);
        f = createFunction(ad, null);
        if (f == null) f = getFunctionContaining(ad);
      }
      if (f == null) { println("=== NO FUNC at " + a + " ==="); continue; }
      println("==================== " + f.getName() + " @ " + f.getEntryPoint() + " ====================");
      DecompileResults res = di.decompileFunction(f, 60, monitor);
      if (res != null && res.decompileCompleted()) {
        println(res.getDecompiledFunction().getC());
      } else {
        println("<decompile failed: " + (res != null ? res.getErrorMessage() : "null") + ">");
      }
    }
    di.dispose();
  }
}
