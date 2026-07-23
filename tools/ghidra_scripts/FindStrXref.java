import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.data.StringDataInstance;

// Find defined strings containing any arg substring; print address, value, and
// the functions that reference them.
public class FindStrXref extends GhidraScript {
  @Override public void run() throws Exception {
    String[] needles = getScriptArgs();
    DataIterator it = currentProgram.getListing().getDefinedData(true);
    int shown = 0;
    while (it.hasNext() && !monitor.isCancelled() && shown < 200) {
      Data d = it.next();
      Object v = d.getValue();
      if (!(v instanceof String)) continue;
      String s = (String) v;
      boolean match = false;
      for (String n : needles) if (s.toLowerCase().contains(n.toLowerCase())) match = true;
      if (!match) continue;
      shown++;
      StringBuilder sb = new StringBuilder();
      Reference[] refs = getReferencesTo(d.getAddress());
      int c = 0;
      for (Reference r : refs) {
        if (c++ < 8) {
          Function f = getFunctionContaining(r.getFromAddress());
          sb.append("\n    <- ").append(r.getFromAddress())
            .append(f != null ? " in " + f.getName() : " (nofunc)");
        }
      }
      println("STR " + d.getAddress() + " = \"" + (s.length() > 40 ? s.substring(0, 40) : s)
              + "\"  refs=" + refs.length + sb);
    }
    println("TOTAL matched=" + shown);
  }
}
