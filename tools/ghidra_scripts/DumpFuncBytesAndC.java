// DumpFuncBytesAndC — for a target function: print its decompiled C, its exact
// byte range, and a hex dump of its instruction bytes (for the lift-harness diff
// oracle). Arg: <hexaddr> [<hexaddr>...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;

public class DumpFuncBytesAndC extends GhidraScript {
  @Override public void run() throws Exception {
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);
    var mem = currentProgram.getMemory();
    for (String a : getScriptArgs()) {
      Address ad = toAddr(Long.parseLong(a.replace("0x",""),16));
      Function f = getFunctionContaining(ad);
      if (f == null) { println("NO FUNC at " + a); continue; }
      long start = f.getEntryPoint().getOffset();
      long end = f.getBody().getMaxAddress().getOffset();
      int len = (int)(end - start + 1);
      // Include the namespace/class so downstream lift agents receive useful
      // target metadata instead of only an overloaded leaf name.
      println("==== " + f.getName(true) + " @ " + f.getEntryPoint() + " len=" + len + " ====");
      DecompileResults r = di.decompileFunction(f, 60, monitor);
      if (r != null && r.decompileCompleted() && r.getDecompiledFunction()!=null)
        println("---C---\n" + r.getDecompiledFunction().getC());
      // hex bytes
      byte[] buf = new byte[len];
      mem.getBytes(f.getEntryPoint(), buf);
      StringBuilder hex = new StringBuilder("---BYTES---\n");
      for (int i = 0; i < len; i++) {
        hex.append(String.format("%02x", buf[i] & 0xFF));
        if ((i & 15) == 15) hex.append("\n"); else hex.append(' ');
      }
      println(hex.toString());
    }
    di.dispose();
  }
}
