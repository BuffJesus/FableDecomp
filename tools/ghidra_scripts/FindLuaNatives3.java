// Stage 3 of the Lua-native enumeration: the ~984 pushcclosure sites whose
// setfield NAME is not a site-local constant. Those sites live in template-
// instantiated registrar functions where name = *(*(argN)) — the constants
// live at the CALLERS. This script:
//   1. re-derives the unnamed sites (same tracker as FindLuaNatives2),
//   2. maps each site to its instantiation entry (backward scan for the
//      'mfspr r12,LR' prologue word 0x7D8802A6),
//   3. finds all callers of each entry, backward-parses r3..r7 constants,
//   4. statically derefs *(arg) and *(*(arg)) into .data looking for name
//      strings, and dumps descriptor words (exec-ptr candidates) when the
//      chain hits a non-string struct.
// Output: ghidra_out/lua_natives3_report.txt (full) + summary counts.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.*;
import java.util.*;

public class FindLuaNatives3 extends GhidraScript {
  static final long PUSHC = 0x8219AA80L;
  static final long SETF1 = 0x823CE420L;
  static final long SETF2 = 0x82A246C8L;
  static final long CLS1  = 0x823CE2E0L;
  static final long CLS2  = 0x823CE5C0L;
  static final long PROLOGUE = 0x7D8802A6L; // mfspr r12,LR

  String readStr(long addr) {
    if (addr < 0x82000000L || addr > 0x83500000L) return null;
    try {
      byte[] b = new byte[96];
      currentProgram.getMemory().getBytes(toAddr(addr), b);
      StringBuilder sb = new StringBuilder();
      for (byte x : b) { if (x == 0) break; if (x < 32 || x > 126) return null; sb.append((char) x); }
      return sb.length() > 0 ? sb.toString() : null;
    } catch (Exception e) { return null; }
  }
  long rd32(long addr) {
    if (addr < 0x82000000L || addr > 0x83500000L) return -1;
    try { return getInt(toAddr(addr)) & 0xffffffffL; } catch (Exception e) { return -1; }
  }
  boolean isExec(long addr) {
    if ((addr & 3) != 0) return false;
    MemoryBlock blk = currentProgram.getMemory().getBlock(toAddr(addr));
    return blk != null && blk.isExecute();
  }
  static boolean isIdent(String s) { return s != null && s.matches("[A-Za-z_][A-Za-z0-9_]*"); }

  static class Snap {
    long at, target;
    long[] r = new long[11];
    boolean[] full = new boolean[11];
    long stw0, stw4; boolean h0, h4;
  }

  // Walk [lo, lo+4*n) tracking register constants; snapshot at every bl.
  List<Snap> walk(long lo, int n) throws Exception {
    int[] words = new int[n];
    for (int k = 0; k < n; k++) {
      try { words[k] = getInt(toAddr(lo + 4L * k)); } catch (Exception e) { words[k] = 0; }
    }
    long[] hi = new long[32]; boolean[] have = new boolean[32]; boolean[] full = new boolean[32];
    long stw0 = 0, stw4 = 0; boolean h0 = false, h4 = false;
    List<Snap> out = new ArrayList<>();
    for (int k = 0; k < n; k++) {
      int w = words[k];
      long ia = lo + 4L * k;
      int opc = (w >>> 26) & 0x3f;
      int rD = (w >>> 21) & 0x1f;
      int rA = (w >>> 16) & 0x1f;
      int imm = (short) (w & 0xffff);
      if (opc == 18 && (w & 1) == 1 && (w & 2) == 0) {
        long li = w & 0x03FFFFFC;
        if ((li & 0x02000000) != 0) li |= 0xFFFFFFFFFC000000L;
        Snap s = new Snap();
        s.at = ia; s.target = (ia + li) & 0xffffffffL;
        for (int g = 0; g < 11; g++) { s.r[g] = hi[g]; s.full[g] = have[g] && full[g]; }
        s.stw0 = stw0; s.h0 = h0; s.stw4 = stw4; s.h4 = h4;
        out.add(s);
        for (int g = 0; g <= 12; g++) if (g != 1 && g != 2) have[g] = false;
        stw0 = stw4 = 0; h0 = h4 = false;
        continue;
      }
      if (opc == 15) {
        long val = ((long)(w & 0xffff) << 16) & 0xffffffffL;
        if (rA == 0) { hi[rD] = val; have[rD] = true; full[rD] = false; }
        else if (have[rA]) { hi[rD] = (hi[rA] + ((long)imm << 16)) & 0xffffffffL; have[rD] = true; full[rD] = false; }
        else have[rD] = false;
      } else if (opc == 14) {
        if (rA == 0) { hi[rD] = imm & 0xffffffffL; have[rD] = true; full[rD] = true; }
        else if (have[rA]) { hi[rD] = (hi[rA] + imm) & 0xffffffffL; have[rD] = true; full[rD] = true; }
        else have[rD] = false;
      } else if (opc == 24) {
        if (have[rD]) { hi[rA] = (hi[rD] | (w & 0xffff)) & 0xffffffffL; have[rA] = true; full[rA] = true; }
        else have[rA] = false;
      } else if (opc == 36) {
        if (imm == 0 && have[rD] && full[rD]) { stw0 = hi[rD]; h0 = true; }
        if (imm == 4 && have[rD] && full[rD]) { stw4 = hi[rD]; h4 = true; }
      } else if (opc == 37) {
        have[rA] = false;
      } else if (opc == 32 || opc == 33 || opc == 34 || opc == 35 || opc == 40
              || opc == 41 || opc == 42 || opc == 43 || opc == 46) {
        have[rD] = false;
        if (opc == 33 || opc == 35 || opc == 41 || opc == 43) have[rA] = false;
      } else if (opc == 31) {
        int xo = (w >>> 1) & 0x3ff;
        if (xo == 444) {
          int rS = (w >>> 21) & 0x1f; int rB = (w >>> 11) & 0x1f; int dst = (w >>> 16) & 0x1f;
          if (rS == rB && have[rS]) { hi[dst] = hi[rS]; have[dst] = have[rS]; full[dst] = full[rS]; }
          else have[dst] = false;
        } else if (xo == 151 || xo == 183 || xo == 215 || xo == 247 || xo == 407 || xo == 439
                || xo == 661 || xo == 150) {
          if (xo == 183 || xo == 247 || xo == 439) have[rA] = false;
        } else {
          have[rD] = false;
        }
      }
    }
    return out;
  }

  @Override public void run() throws Exception {
    // ---- stage 1: unnamed pushcclosure sites (same criteria as FindLuaNatives2)
    ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(toAddr(PUSHC));
    List<Long> sites = new ArrayList<>();
    while (it.hasNext()) {
      Reference r = it.next();
      if (r.getReferenceType().isCall()) sites.add(r.getFromAddress().getOffset());
    }
    int BACK = 96, FWD = 16;
    // entry -> thunk (cfunc constant stored in the userdata by the instantiation)
    Map<Long, Long> entryThunk = new LinkedHashMap<>();
    Map<Long, Integer> entrySites = new LinkedHashMap<>();
    int named = 0, unnamed = 0, noEntry = 0;
    for (long call : sites) {
      long lo = call - 4L * BACK;
      List<Snap> bls = walk(lo, BACK + 1 + FWD);
      Snap push = null; int pushIdx = -1;
      for (int i = 0; i < bls.size(); i++) if (bls.get(i).at == call) { push = bls.get(i); pushIdx = i; break; }
      if (push == null) continue;
      Long nameA = null;
      for (int i = pushIdx + 1; i < bls.size(); i++) {
        Snap s = bls.get(i);
        if ((s.target == SETF1 || s.target == SETF2) && s.full[5]) { nameA = s.r[5]; break; }
      }
      if (nameA != null && isIdent(readStr(nameA))) { named++; continue; }  // already covered
      unnamed++;
      long cfunc = push.h4 && isExec(push.stw4) ? push.stw4
                 : push.h0 && isExec(push.stw0) ? push.stw0 : 0;
      // find the instantiation entry: nearest preceding 'mfspr r12,LR'
      long entry = 0;
      for (long a = call; a >= call - 4L * 1024 && a >= 0x82000000L; a -= 4) {
        long w = rd32(a);
        if (w == PROLOGUE) { entry = a; break; }
      }
      if (entry == 0) { noEntry++; continue; }
      entryThunk.putIfAbsent(entry, cfunc);
      entrySites.merge(entry, 1, Integer::sum);
    }
    println("FindLuaNatives3: sites=" + sites.size() + " named(skip)=" + named
        + " unnamed=" + unnamed + " noEntry=" + noEntry + " entries=" + entryThunk.size());

    // ---- stage 2: callers of each entry; deref arg constants
    StringBuilder rep = new StringBuilder();
    StringBuilder cat = new StringBuilder();   // catalog TSV: name, class?, entry, thunk, caller
    int callersTotal = 0, callersNamed = 0, callersDark = 0;
    Set<String> catalogSeen = new HashSet<>();
    for (Map.Entry<Long, Long> e : entryThunk.entrySet()) {
      long entry = e.getKey(), thunk = e.getValue();
      rep.append(String.format("%n== registrar 0x%08X (thunk 0x%08X, %d pushc sites)%n",
          entry, thunk, entrySites.get(entry)));
      ReferenceIterator rit = currentProgram.getReferenceManager().getReferencesTo(toAddr(entry));
      int nc = 0;
      while (rit.hasNext()) {
        Reference r = rit.next();
        if (!r.getReferenceType().isCall()) continue;
        long cs = r.getFromAddress().getOffset();
        nc++; callersTotal++;
        List<Snap> bls = walk(cs - 4L * BACK, BACK + 1);
        Snap callSnap = null; int ci = -1;
        for (int i = 0; i < bls.size(); i++) if (bls.get(i).at == cs) { callSnap = bls.get(i); ci = i; break; }
        if (callSnap == null) { rep.append(String.format("  caller 0x%08X: <no snap>%n", cs)); continue; }
        // class-name candidate: nearest preceding class-table lookup with const r4
        String cls = null;
        for (int i = ci - 1; i >= 0; i--) {
          Snap s = bls.get(i);
          if ((s.target == CLS1 || s.target == CLS2) && s.full[4]) {
            String c = readStr(s.r[4]);
            if (isIdent(c)) cls = c;
            break;
          }
        }
        StringBuilder line = new StringBuilder(String.format("  caller 0x%08X:", cs));
        String bestName = null;
        for (int g = 3; g <= 7; g++) {
          if (!callSnap.full[g]) continue;
          long v = callSnap.r[g];
          String direct = readStr(v);
          long p1 = rd32(v);
          String s1 = p1 > 0 ? readStr(p1) : null;
          long p2 = p1 > 0 ? rd32(p1) : -1;
          String s2 = p2 > 0 ? readStr(p2) : null;
          line.append(String.format(" r%d=0x%08X", g, v));
          if (isIdent(direct)) line.append("('").append(direct).append("')");
          else if (isIdent(s1)) line.append("(*->'").append(s1).append("')");
          else if (isIdent(s2)) line.append("(**->'").append(s2).append("')");
          else if (p1 > 0) {
            // descriptor dump: first 8 words, flag exec ptrs
            line.append("(desc:");
            for (int wq = 0; wq < 8; wq++) {
              long dv = rd32(p1 + 4L * wq);
              if (dv <= 0) break;
              line.append(String.format(" %08X%s", dv, isExec(dv) ? "*X*" : ""));
            }
            line.append(")");
          }
          String cand = isIdent(s2) ? s2 : (isIdent(s1) ? s1 : null);
          if (bestName == null && cand != null) bestName = cand;
          if (bestName == null && isIdent(direct)) bestName = direct;
        }
        if (cls != null) line.append("  class='" + cls + "'");
        rep.append(line).append("\n");
        if (bestName != null) {
          callersNamed++;
          String key = (cls != null ? cls + "." : "") + bestName + "@" + Long.toHexString(entry);
          if (catalogSeen.add(key))
            cat.append(String.format("%s\t%s\t0x%08X\t0x%08X\t0x%08X%n",
                bestName, cls != null ? cls : "?", entry, thunk, cs));
        } else callersDark++;
      }
      if (nc == 0) rep.append("  <no callers found>\n");
    }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives3_report.txt"))) { w.write(rep.toString()); }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives3_catalog.tsv"))) { w.write(cat.toString()); }
    println("FindLuaNatives3: callers=" + callersTotal + " named=" + callersNamed + " dark=" + callersDark
        + " -> ghidra_out/lua_natives3_report.txt + lua_natives3_catalog.tsv");
  }
}
