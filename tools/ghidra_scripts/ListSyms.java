import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.*;
import ghidra.program.model.listing.Function;

// List all symbols whose name contains any arg substring (case-insensitive).
// Prints address and name. Useful to see what's already named (e.g. FunctionID hits).
public class ListSyms extends GhidraScript {
  @Override public void run() throws Exception {
    String[] needles = getScriptArgs();
    SymbolTable st = currentProgram.getSymbolTable();
    SymbolIterator it = st.getAllSymbols(true);
    int n = 0;
    while (it.hasNext()) {
      Symbol s = it.next();
      String nm = s.getName();
      String low = nm.toLowerCase();
      boolean m = false;
      for (String x : needles) if (low.contains(x.toLowerCase())) m = true;
      if (!m) continue;
      // skip default names
      if (nm.startsWith("FUN_") || nm.startsWith("DAT_") || nm.startsWith("LAB_")
          || nm.startsWith("sub_") || nm.startsWith("Function_")) continue;
      println("SYM " + s.getAddress() + "  " + nm + "  (" + s.getSymbolType() + ")");
      n++;
    }
    println("ListSyms: " + n + " matches");
  }
}
