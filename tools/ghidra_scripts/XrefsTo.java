import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.*;

// Print all callers (xrefs) to each address arg.
public class XrefsTo extends GhidraScript {
  @Override public void run() throws Exception {
    var rm = currentProgram.getReferenceManager();
    var fm = currentProgram.getFunctionManager();
    for (String a : getScriptArgs()) {
      Address tgt = toAddr(Long.parseLong(a.replace("0x",""),16));
      println("==== XREFS TO " + a + " ====");
      ReferenceIterator it = rm.getReferencesTo(tgt);
      while (it.hasNext()) {
        Reference r = it.next();
        Address from = r.getFromAddress();
        Function cf = fm.getFunctionContaining(from);
        println("  from " + from + "  in " + (cf!=null?cf.getName()+" @ "+cf.getEntryPoint():"?") + "  ["+r.getReferenceType()+"]");
      }
    }
  }
}
