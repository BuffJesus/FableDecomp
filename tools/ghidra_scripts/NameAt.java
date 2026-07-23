// Print the primary symbol (if any) at each given hex address.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.listing.Function;

public class NameAt extends GhidraScript {
  @Override public void run() throws Exception {
    for (String a : getScriptArgs()) {
      Address ad = toAddr(Long.parseLong(a.replace("0x", ""), 16));
      Symbol s = getSymbolAt(ad);
      Function f = getFunctionAt(ad);
      println("NAMEAT " + ad + "  sym=" + (s != null ? s.getName() : "-")
          + "  func=" + (f != null ? f.getName() : "-"));
    }
  }
}
