import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import java.io.PrintWriter;

// Like DecompAt, but creates (disassembles + defines) a function at each seed address
// when none exists. Args: <out-path> <addr>... . Meant for -readOnly runs on PDB-named
// binaries where not every function has been carved yet.
public class DecompAtCreate extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);
    try (PrintWriter pw = new PrintWriter(args[0])) {
      for (int i = 1; i < args.length; i++) {
        Address addr = toAddr(Long.parseLong(args[i].replace("0x", ""), 16));
        Function f = getFunctionAt(addr);
        if (f == null) {
          new DisassembleCommand(addr, null, true).applyTo(currentProgram, monitor);
          new CreateFunctionCmd(addr).applyTo(currentProgram, monitor);
          f = getFunctionAt(addr);
        }
        if (f == null) { pw.println("//=== " + args[i] + " : NO FUNCTION ==="); continue; }
        pw.println("//=== " + f.getName() + " @ " + f.getEntryPoint() + " ===");
        DecompileResults res = di.decompileFunction(f, 180, monitor);
        if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null)
          pw.println(res.getDecompiledFunction().getC());
        else
          pw.println("// <decompile failed: " + (res != null ? res.getErrorMessage() : "null") + ">");
        pw.println();
      }
    }
    di.dispose();
  }
}
