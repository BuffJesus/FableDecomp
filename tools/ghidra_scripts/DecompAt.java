// Decompile functions at the given hex addresses to one file with headers,
// and list each function's direct callees (call targets) after its body.
// Args: <outCFile> <addrHex1> [addrHex2 ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.PrintWriter;
import java.util.*;

public class DecompAt extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    String outPath = args[0];
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);
    try (PrintWriter pw = new PrintWriter(outPath)) {
      for (int i = 1; i < args.length; i++) {
        long a = Long.parseLong(args[i].replace("0x",""), 16);
        Address addr = toAddr(a);
        Function f = getFunctionContaining(addr);
        if (f == null) { pw.println("//=== " + args[i] + " : NO FUNCTION ==="); continue; }
        pw.println("//=== " + f.getName() + " @ " + f.getEntryPoint() + " (seed " + args[i] + ") ===");
        DecompileResults res = di.decompileFunction(f, 120, monitor);
        if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null) {
          pw.println(res.getDecompiledFunction().getC());
        } else {
          pw.println("// <decompile failed: " + (res!=null?res.getErrorMessage():"null") + ">");
        }
        // direct callees
        pw.println("// CALLEES of " + f.getName() + ":");
        Set<Function> callees = f.getCalledFunctions(monitor);
        for (Function c : callees) {
          pw.println("//   " + c.getEntryPoint() + "  " + c.getName());
        }
        pw.println();
      }
    }
    di.dispose();
    println("DecompAt done -> " + outPath);
  }
}
