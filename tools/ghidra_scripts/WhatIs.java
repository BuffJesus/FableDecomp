import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.listing.Function;

public class WhatIs extends GhidraScript {
  @Override public void run() throws Exception {
    for (String a : getScriptArgs()) {
      Address ad = toAddr(Long.parseLong(a.replace("0x", ""), 16));
      Reference[] refs = getReferencesTo(ad);
      StringBuilder sb = new StringBuilder();
      int c = 0;
      for (Reference r : refs) {
        if (c++ < 12) {
          Function f = getFunctionContaining(r.getFromAddress());
          sb.append("\n    ").append(r.getReferenceType())
            .append(" from ").append(r.getFromAddress())
            .append(f != null ? " in " + f.getName() : " (no func)");
        }
      }
      println("ADDR " + a + " nrefs=" + refs.length + sb.toString());
    }
  }
}
