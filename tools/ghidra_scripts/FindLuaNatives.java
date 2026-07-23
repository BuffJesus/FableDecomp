// Enumerate Lua native registrations by parsing RAW PPC instruction words
// (independent of Ghidra disassembly). For every reference to lua_register_native
// (0x82353EF8), scan backward reconstructing GPRs set by lis/addi/ori, and read
// r4 (name string) + r6 (cfunc). Emits ghidra_out/lua_natives.tsv (cfunc -> name).
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.*;
import java.util.*;

public class FindLuaNatives extends GhidraScript {
  int rd(Address a) throws Exception { return getInt(a); }
  String readStr(long addr) {
    try {
      byte[] b = new byte[80];
      currentProgram.getMemory().getBytes(toAddr(addr), b);
      StringBuilder sb = new StringBuilder();
      for (byte x : b) { if (x == 0) break; if (x < 32 || x > 126) return null; sb.append((char) x); }
      return sb.toString();
    } catch (Exception e) { return null; }
  }
  @Override public void run() throws Exception {
    long REG = 0x82353EF8L;
    ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(toAddr(REG));
    int found = 0, nrefs = 0, gotBoth = 0;
    StringBuilder tsv = new StringBuilder();
    Set<String> seen = new HashSet<>();
    while (it.hasNext()) {
      Reference r = it.next();
      nrefs++;
      long call = r.getFromAddress().getOffset();
      long[] hi = new long[32]; boolean[] have = new boolean[32]; boolean[] full = new boolean[32];
      Long nameA = null, cfuncA = null;
      // collect the 30 words before the call, then process FORWARD (register
      // values flow forward: addi reads the rA set by an earlier lis).
      int WIN = 30;
      int[] words = new int[WIN];
      boolean ok = true;
      for (int k = 0; k < WIN; k++) {
        long ia = call - 4L * (WIN - k);   // oldest first
        try { words[k] = getInt(toAddr(ia)); } catch (Exception e) { words[k] = 0; }
      }
      for (int k = 0; k < WIN; k++) {
        int w = words[k];
        int opc = (w >>> 26) & 0x3f;
        int rD = (w >>> 21) & 0x1f;
        int rA = (w >>> 16) & 0x1f;
        int imm = (short) (w & 0xffff);
        if (opc == 15) {                 // addis/lis
          long val = ((long)(w & 0xffff) << 16) & 0xffffffffL;
          if (rA == 0) { hi[rD] = val; have[rD] = true; full[rD] = false; }
          else if (have[rA]) { hi[rD] = (hi[rA] + ((long)imm << 16)) & 0xffffffffL; have[rD] = true; full[rD] = false; }
        } else if (opc == 14) {          // addi
          if (rA == 0) { hi[rD] = imm & 0xffffffffL; have[rD] = true; full[rD] = true; }
          else if (have[rA]) { hi[rD] = (hi[rA] + imm) & 0xffffffffL; have[rD] = true; full[rD] = true; }
        } else if (opc == 24) {          // ori rA,rS,UIMM  (dest = rA)
          if (have[rD]) { hi[rA] = (hi[rD] | (w & 0xffff)) & 0xffffffffL; have[rA] = true; full[rA] = true; }
        }
        // r-mr via 'or rA,rS,rS' (opc 31, xo 444): copy
        if (opc == 31 && ((w >>> 1) & 0x3ff) == 444) {
          int rS = (w >>> 21) & 0x1f; int rB = (w >>> 11) & 0x1f; int dst = (w >>> 16) & 0x1f;
          if (rS == rB && have[rS]) { hi[dst] = hi[rS]; have[dst] = have[rS]; full[dst] = full[rS]; }
        }
      }
      // at the call, r4=name r6=cfunc
      if (full[4]) nameA = hi[4];
      if (full[6]) cfuncA = hi[6];
      if (nameA == null || cfuncA == null) continue;
      gotBoth++;
      String name = readStr(nameA);
      if (name == null || !name.matches("[A-Za-z_][A-Za-z0-9_]*")) continue;
      String key = Long.toHexString(cfuncA) + ":" + name;
      if (!seen.add(key)) continue;
      found++;
      println(String.format("0x%08X\t%s", cfuncA, name));
      tsv.append(String.format("0x%08X\tlua_native_%s\tLua C-func for native '%s'.%n", cfuncA, name, name));
    }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives.tsv"))) { w.write(tsv.toString()); }
    println("FindLuaNatives: refs=" + nrefs + " gotBoth=" + gotBoth + " found=" + found + " -> ghidra_out/lua_natives.tsv");
  }
}
