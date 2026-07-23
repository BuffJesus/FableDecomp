// Dump N pointers from a data address and decompile each target fn.
// Args: <outCFile> <hexTableAddr> <count>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.io.PrintWriter;

public class DumpPtrTable extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    long base = Long.parseLong(a[1].replace("0x",""),16);
    int cnt = Integer.parseInt(a[2]);
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions(); opts.setMaxPayloadMBytes(128);
    di.setOptions(opts); di.openProgram(currentProgram);
    java.util.LinkedHashSet<Long> targets = new java.util.LinkedHashSet<>();
    for (int i=0;i<cnt;i++){
      long p = getInt(toAddr(base + i*4L)) & 0xffffffffL;
      println("slot["+i+"] @"+Long.toHexString(base+i*4L)+" -> "+Long.toHexString(p));
      if (p>0x400000L && p<0x1400000L) targets.add(p);
    }
    try (PrintWriter pw=new PrintWriter(a[0])) {
      for (long p: targets) {
        Function f=getFunctionContaining(toAddr(p));
        if (f==null){pw.println("//=== no fn @ "+Long.toHexString(p)+" ===");continue;}
        pw.println("//=== "+f.getName()+" @ "+f.getEntryPoint()+" ===");
        DecompileResults r=di.decompileFunction(f,120,monitor);
        pw.println(r!=null&&r.decompileCompleted()&&r.getDecompiledFunction()!=null
          ? r.getDecompiledFunction().getC() : "// <fail>");
        pw.println();
      }
    }
    di.dispose(); println("done");
  }
}
