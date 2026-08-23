// Find callers of a target address, decompile each caller to one file.
// Args: <outCFile> <targetHex>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.PrintWriter;
import java.util.*;

public class FindCallersDecomp extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    String outPath = args[0];
    long ta = Long.parseLong(args[1].replace("0x",""), 16);
    Address target = toAddr(ta);
    Function tf = getFunctionContaining(target);
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);
    Set<Function> callers = new LinkedHashSet<>();
    if (tf != null) callers.addAll(tf.getCallingFunctions(monitor));
    try (PrintWriter pw = new PrintWriter(outPath)) {
      pw.println("// callers of " + (tf!=null?tf.getName():args[1]) + " (" + callers.size() + ")");
      for (Function f : callers) {
        pw.println("//=== " + f.getName() + " @ " + f.getEntryPoint() + " ===");
        DecompileResults res = di.decompileFunction(f, 120, monitor);
        if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null) {
          pw.println(res.getDecompiledFunction().getC());
        } else {
          pw.println("// <decompile failed>");
        }
        pw.println();
      }
    }
    di.dispose();
    println("done -> " + outPath);
  }
}
