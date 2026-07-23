import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.util.*;

// Decompile functions by substring match on function name.
public class DecompByName extends GhidraScript {
  @Override public void run() throws Exception {
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);
    String[] needles = getScriptArgs();
    Set<Address> done = new HashSet<>();
    var fm = currentProgram.getFunctionManager();
    for (Function f : fm.getFunctions(true)) {
      String nm = f.getName();
      for (String needle : needles) {
        if (nm.toLowerCase().contains(needle.toLowerCase())) {
          if (!done.add(f.getEntryPoint())) continue;
          println("==================== " + nm + " @ " + f.getEntryPoint() + " ====================");
          DecompileResults res = di.decompileFunction(f, 60, monitor);
          if (res != null && res.decompileCompleted()) {
            println(res.getDecompiledFunction().getC());
          } else {
            println("<decompile failed: " + (res != null ? res.getErrorMessage() : "null") + ">");
          }
          break;
        }
      }
    }
    di.dispose();
  }
}
