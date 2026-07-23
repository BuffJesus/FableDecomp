// Retry decompilation of specific functions with a custom timeout.
// Args: <timeoutSeconds> <hexAddr> [hexAddr...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class RetryDecomp extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    int timeout = Integer.parseInt(args[0]);
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions options = new DecompileOptions();
    options.setMaxPayloadMBytes(512); // RunCutsceneMacro_Func overflows the 50MB default
    di.setOptions(options);
    di.openProgram(currentProgram);
    for (int i = 1; i < args.length; i++) {
      Address ad = toAddr(Long.parseLong(args[i].replace("0x", ""), 16));
      Function f = getFunctionAt(ad);
      if (f == null) { println("RETRY " + args[i] + ": no function"); continue; }
      long start = System.currentTimeMillis();
      DecompileResults res = di.decompileFunction(f, timeout, monitor);
      long ms = System.currentTimeMillis() - start;
      boolean ok = res != null && res.decompileCompleted()
          && res.getDecompiledFunction() != null;
      println("RETRY " + f.getName() + "@" + ad + ": " + (ok ? "OK" : "FAILED")
          + " in " + ms + "ms"
          + (ok ? " (" + res.getDecompiledFunction().getC().length() + " chars)"
                : " err=" + (res != null ? res.getErrorMessage() : "null")));
    }
    di.dispose();
  }
}
