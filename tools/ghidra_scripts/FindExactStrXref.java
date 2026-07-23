// Find defined strings exactly matching any arg and print referencing functions.
// This avoids substring noise from short cutscene macro verbs such as "Create".
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import java.util.HashSet;
import java.util.Set;

public class FindExactStrXref extends GhidraScript {
  @Override public void run() throws Exception {
    Set<String> needles = new HashSet<>();
    for (String arg : getScriptArgs()) needles.add(arg);

    DataIterator it = currentProgram.getListing().getDefinedData(true);
    int matched = 0;
    while (it.hasNext() && !monitor.isCancelled()) {
      Data d = it.next();
      Object value = d.getValue();
      if (!(value instanceof String)) continue;
      String s = (String) value;
      if (!needles.contains(s)) continue;

      ++matched;
      Reference[] refs = getReferencesTo(d.getAddress());
      println("EXACTSTR " + d.getAddress() + " = \"" + s + "\" refs=" + refs.length);
      for (int i = 0; i < refs.length && i < 20; ++i) {
        Reference r = refs[i];
        Function f = getFunctionContaining(r.getFromAddress());
        println("  <- " + r.getFromAddress() + " in " +
                (f != null ? f.getName() + "@" + f.getEntryPoint() : "?") +
                " type=" + r.getReferenceType());
      }
    }
    println("FindExactStrXref: matched=" + matched);
  }
}
