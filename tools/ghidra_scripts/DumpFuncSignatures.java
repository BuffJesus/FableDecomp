// DumpFuncSignatures — dump every function's recovered signature from a
// PDB-symbolized program (donor), for porting verified types onto the stripped
// retail Fable.exe (task #10, via donor-port instead of a blind analyzer pass).
//
// Also emits any available source-file association per function (task #12).
//
// Args: <sig-out-tsv> [<srcfile-out-tsv>]
// sig:  addr \t cc \t ret \t nParams \t paramTypes(csv) \t demangledName
// src:  addr \t sourceFile \t name   (only if the program carries source info)
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.DataType;
import ghidra.program.model.address.Address;
import java.io.*;

public class DumpFuncSignatures extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    String sigOut = a[0];
    String srcOut = a.length > 1 ? a[1] : null;
    var fm = currentProgram.getFunctionManager();
    PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(sigOut)));
    w.println("# addr\tcc\tret\tnParams\tparamTypes\tname");
    int n = 0;
    for (Function f : fm.getFunctions(true)) {
      DataType ret = f.getReturnType();
      Parameter[] ps = f.getParameters();
      StringBuilder sb = new StringBuilder();
      for (int i = 0; i < ps.length; i++) {
        if (i > 0) sb.append(",");
        DataType pt = ps[i].getDataType();
        sb.append(pt != null ? pt.getName() : "?");
      }
      String cc = f.getCallingConventionName();
      w.println(f.getEntryPoint() + "\t" + (cc != null ? cc : "unknown")
          + "\t" + (ret != null ? ret.getName() : "void")
          + "\t" + ps.length + "\t" + sb + "\t" + f.getName());
      n++;
    }
    w.close();
    println("DumpFuncSignatures: wrote " + n + " signatures -> " + sigOut);

    // Source-file association (task #12) — best-effort; Ghidra 11.2+ SourceFileManager.
    if (srcOut != null) {
      int sn = 0;
      try {
        var sfm = currentProgram.getClass().getMethod("getSourceFileManager").invoke(currentProgram);
        PrintWriter sw = new PrintWriter(new BufferedWriter(new FileWriter(srcOut)));
        sw.println("# addr\tsourceFile\tname");
        // Reflectively probe getSourceMapEntries(Address) per function entry.
        var getEntries = sfm.getClass().getMethod("getSourceMapEntries", Address.class);
        for (Function f : fm.getFunctions(true)) {
          Object entries = getEntries.invoke(sfm, f.getEntryPoint());
          if (entries instanceof java.util.List && !((java.util.List<?>) entries).isEmpty()) {
            Object e0 = ((java.util.List<?>) entries).get(0);
            Object sf = e0.getClass().getMethod("getSourceFile").invoke(e0);
            String path = (String) sf.getClass().getMethod("getPath").invoke(sf);
            sw.println(f.getEntryPoint() + "\t" + path + "\t" + f.getName());
            sn++;
          }
        }
        sw.close();
        println("DumpFuncSignatures: wrote " + sn + " source-file rows -> " + srcOut);
      } catch (Throwable t) {
        println("DumpFuncSignatures: source-file info unavailable (" + t.getClass().getSimpleName() + ": " + t.getMessage() + ")");
      }
    }
  }
}
