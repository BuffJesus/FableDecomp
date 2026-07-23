// ExportEngineApi — export named engine functions (retail Fable.exe) as a
// signature table, so injectors / ForgeFSE / FableForge can bind engine
// internals by absolute VA with known prototypes.
//
// Emits TSV: addr \t callingConv \t retType \t retLen \t nParams \t paramTypes(csv) \t demangledName
// Only real names (skips FUN_* and default thunks). One row per function.
//
// Arg: <out-tsv>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.DataType;
import java.io.*;

public class ExportEngineApi extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    String out = a.length > 0 ? a[0] : "D:\\Documents\\FableTLC\\ghidra_out\\engine_api.tsv";
    var fm = currentProgram.getFunctionManager();
    PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)));
    w.println("# addr\tcc\tret\tretLen\tnParams\tparamTypes\tname");
    int n = 0;
    for (Function f : fm.getFunctions(true)) {
      String nm = f.getName();
      if (nm.startsWith("FUN_")) continue;
      DataType ret = f.getReturnType();
      StringBuilder ps = new StringBuilder();
      Parameter[] params = f.getParameters();
      for (int i = 0; i < params.length; i++) {
        DataType pt = params[i].getDataType();
        if (i > 0) ps.append(",");
        ps.append(pt != null ? pt.getName() : "?");
      }
      String cc = f.getCallingConventionName();
      w.println(f.getEntryPoint() + "\t" + (cc != null ? cc : "unknown")
        + "\t" + (ret != null ? ret.getName() : "void")
        + "\t" + (ret != null ? ret.getLength() : 0)
        + "\t" + params.length
        + "\t" + ps
        + "\t" + nm);
      n++;
    }
    w.close();
    println("ExportEngineApi: wrote " + n + " named functions to " + out);
  }
}
