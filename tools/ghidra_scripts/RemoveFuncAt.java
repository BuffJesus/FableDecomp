// Delete the function DEFINED AT each given hex address (use to undo accidental
// mid-function createFunction artifacts). Does not touch code or other symbols.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class RemoveFuncAt extends GhidraScript {
  @Override public void run() throws Exception {
    for (String a : getScriptArgs()) {
      Address ad = toAddr(Long.parseLong(a.replace("0x", ""), 16));
      Function f = getFunctionAt(ad);
      if (f == null) { println("RemoveFuncAt " + ad + ": no function defined at this exact address"); continue; }
      String name = f.getName();
      removeFunctionAt(ad);
      println("RemoveFuncAt " + ad + " (" + name + "): " + (getFunctionAt(ad) == null ? "removed" : "FAILED"));
    }
  }
}
