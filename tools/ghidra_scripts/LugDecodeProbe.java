// Decompile the ADPCM decode + sound-buffer creation path.
// Args: <outCFile> <hexaddr1> [hexaddr2 ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.io.PrintWriter;

public class LugDecodeProbe extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);
    try (PrintWriter pw = new PrintWriter(args[0])) {
      for (int i=1;i<args.length;i++) {
        long addr = Long.parseLong(args[i].replace("0x",""),16);
        Address a = toAddr(addr);
        Function f = getFunctionContaining(a);
        if (f==null){pw.println("//=== no fn @ "+a+" ===");continue;}
        pw.println("//=== "+f.getName()+" @ "+f.getEntryPoint()+" (callers:");
        for (Reference r: getReferencesTo(f.getEntryPoint())) {
          Function cf=getFunctionContaining(r.getFromAddress());
          pw.println("//     <- "+r.getFromAddress()+(cf!=null?" "+cf.getName():""));
        }
        pw.println("// ) ===");
        DecompileResults res = di.decompileFunction(f, 120, monitor);
        if (res!=null && res.decompileCompleted() && res.getDecompiledFunction()!=null)
          pw.println(res.getDecompiledFunction().getC());
        else pw.println("// <decompile failed>");
        pw.println();
      }
    }
    di.dispose();
    println("done");
  }
}
