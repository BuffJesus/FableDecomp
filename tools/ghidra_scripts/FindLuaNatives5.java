// Stage 5 (the one that works around undisassembled callers): enumerate the
// remaining Lua natives by RAW-SCANNING .text for `bl <registrar>` words —
// Ghidra references are useless here because most caller code was never
// disassembled. Pipeline:
//   1. unnamed-site registrar entries (full tracker, as FindLuaNatives2/3),
//   2. raw bl-scan of all executable blocks -> caller sites per entry,
//   3. per-caller backward register parse: name = identifier string among
//      r3..r7 (direct or via * / **), method = exec ptr among r5..r10,
//   4. catalog + label TSV (method -> lua_native_<name>), aggregated per method.
// Outputs: ghidra_out/lua_natives5_catalog.tsv (name, method, entry, caller)
//          ghidra_out/lua_natives5_labels.tsv  (LabelApply format, deduped)
//          ghidra_out/lua_natives5_report.txt  (first 30 callers, full regs)
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.*;
import java.util.*;

public class FindLuaNatives5 extends GhidraScript {
  static final long PUSHC = 0x8219AA80L;
  static final long SETF1 = 0x823CE420L;
  static final long SETF2 = 0x82A246C8L;
  static final long PROLOGUE = 0x7D8802A6L;

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
    if ((addr & 3) != 0 || addr < 0x82000000L || addr > 0x83500000L) return false;
    MemoryBlock blk = currentProgram.getMemory().getBlock(toAddr(addr));
    return blk != null && blk.isExecute();
  }
  static boolean isIdent(String s) { return s != null && s.matches("[A-Za-z_][A-Za-z0-9_]*"); }

  static class Snap {
    long at, target;
    long[] r = new long[12];
    boolean[] full = new boolean[12];
    boolean[] sp = new boolean[12];   // value is r1+offset (offset in r[])
    Map<Long, Long> spSlots;          // r1+offset -> stored constant (stw)
    long stw0, stw4; boolean h0, h4;  // last const stw to 0(rX)/4(rX), rX != r1
  }

  List<Snap> walk(long lo, int n) throws Exception {
    long[] hi = new long[32]; boolean[] have = new boolean[32]; boolean[] full = new boolean[32];
    boolean[] sp = new boolean[32];   // symbolic r1-relative value
    Map<Long, Long> spSlots = new HashMap<>();  // persists across bl (caller frame)
    long stw0 = 0, stw4 = 0; boolean h0 = false, h4 = false;
    List<Snap> out = new ArrayList<>();
    for (int k = 0; k < n; k++) {
      long ia = lo + 4L * k;
      int w;
      try { w = getInt(toAddr(ia)); } catch (Exception e) { w = 0; }
      int opc = (w >>> 26) & 0x3f;
      int rD = (w >>> 21) & 0x1f;
      int rA = (w >>> 16) & 0x1f;
      int imm = (short) (w & 0xffff);
      if (opc == 18 && (w & 1) == 1 && (w & 2) == 0) {
        long li = w & 0x03FFFFFC;
        if ((li & 0x02000000) != 0) li |= 0xFFFFFFFFFC000000L;
        Snap s = new Snap();
        s.at = ia; s.target = (ia + li) & 0xffffffffL;
        for (int g = 0; g < 12; g++) { s.r[g] = hi[g]; s.full[g] = have[g] && full[g]; s.sp[g] = have[g] && sp[g]; }
        s.spSlots = new HashMap<>(spSlots);
        s.stw0 = stw0; s.h0 = h0; s.stw4 = stw4; s.h4 = h4;
        out.add(s);
        for (int g = 0; g <= 12; g++) if (g != 1 && g != 2) { have[g] = false; sp[g] = false; }
        stw0 = stw4 = 0; h0 = h4 = false;
        continue;
      }
      if (opc == 15) {
        long val = ((long)(w & 0xffff) << 16) & 0xffffffffL;
        if (rA == 0) { hi[rD] = val; have[rD] = true; full[rD] = false; sp[rD] = false; }
        else if (have[rA] && !sp[rA]) { hi[rD] = (hi[rA] + ((long)imm << 16)) & 0xffffffffL; have[rD] = true; full[rD] = false; sp[rD] = false; }
        else have[rD] = false;
      } else if (opc == 14) {
        if (rA == 0) { hi[rD] = imm & 0xffffffffL; have[rD] = true; full[rD] = true; sp[rD] = false; }
        else if (rA == 1) { hi[rD] = imm; have[rD] = true; full[rD] = true; sp[rD] = true; }
        else if (have[rA]) { hi[rD] = (hi[rA] + imm) & 0xffffffffL; have[rD] = true; full[rD] = true; sp[rD] = sp[rA]; }
        else have[rD] = false;
      } else if (opc == 24) {
        if (have[rD] && !sp[rD]) { hi[rA] = (hi[rD] | (w & 0xffff)) & 0xffffffffL; have[rA] = true; full[rA] = true; sp[rA] = false; }
        else have[rA] = false;
      } else if (opc == 36) {           // stw rS, d(rA)
        if (rA == 1) {                  // frame-slot constants
          if (have[rD] && full[rD] && !sp[rD]) spSlots.put((long) imm, hi[rD]);
          else spSlots.remove((long) imm);
        } else if (have[rD] && full[rD] && !sp[rD]) {  // userdata init (direct sites)
          if (imm == 0) { stw0 = hi[rD]; h0 = true; }
          if (imm == 4) { stw4 = hi[rD]; h4 = true; }
        }
      } else if (opc == 58) {           // ld rD, ds(rA) — 64-bit load (DS-form)
        int ds = (short) (w & 0xFFFC);
        Long slot = rA == 1 ? spSlots.get((long) ds) : null;
        if (slot != null && (w & 3) == 0) {
          // big-endian: high word of the packed 64-bit value = the 32-bit ptr
          hi[rD] = slot; have[rD] = true; full[rD] = true; sp[rD] = false;
        } else {
          have[rD] = false;
          if ((w & 3) == 1) have[rA] = false;   // ldu updates rA
        }
      } else if (opc == 62) {           // std rS, ds(r1): 64-bit store — drop slots
        if (rA == 1) {
          int ds = (short) (w & 0xFFFC);
          spSlots.remove((long) ds); spSlots.remove((long) ds + 4);
        }
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
          if (rS == rB && have[rS]) { hi[dst] = hi[rS]; have[dst] = have[rS]; full[dst] = full[rS]; sp[dst] = sp[rS]; }
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
    // ---- stage 1: ALL pushcclosure sites via raw bl-scan. Ghidra refs only
    // cover disassembled code — the Debug/GUI/QuestManager registrars live in
    // undisassembled regions and were invisible to getReferencesTo(PUSHC).
    List<Long> sites = new ArrayList<>();
    for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
      if (!blk.isInitialized() || !blk.isExecute()) continue;
      long start = blk.getStart().getOffset(), size = blk.getSize();
      byte[] buf = new byte[(int) Math.min(size, Integer.MAX_VALUE - 8)];
      try { blk.getBytes(blk.getStart(), buf); } catch (Exception ex) { continue; }
      for (int off = 0; off + 4 <= buf.length; off += 4) {
        int w = ((buf[off] & 0xff) << 24) | ((buf[off+1] & 0xff) << 16)
              | ((buf[off+2] & 0xff) << 8) | (buf[off+3] & 0xff);
        if (((w >>> 26) & 0x3f) != 18 || (w & 1) != 1 || (w & 2) != 0) continue;
        long li = w & 0x03FFFFFC;
        if ((li & 0x02000000) != 0) li |= 0xFFFFFFFFFC000000L;
        if (((start + off + li) & 0xffffffffL) == PUSHC) sites.add(start + off);
      }
    }
    Set<Long> entries = new LinkedHashSet<>();
    Map<Long, LinkedHashSet<String>> byMethod = new LinkedHashMap<>();
    StringBuilder cat = new StringBuilder("class\tname\tmethod\tentry\tcaller\n");
    int BACK = 96, FWD = 16, named = 0, namedNoFunc = 0;
    for (long call : sites) {
      List<Snap> bls = walk(call - 4L * BACK, BACK + 1 + FWD);
      Snap push = null; int pushIdx = -1;
      for (int i = 0; i < bls.size(); i++) if (bls.get(i).at == call) { push = bls.get(i); pushIdx = i; break; }
      if (pushIdx < 0) continue;
      Long nameA = null;
      for (int i = pushIdx + 1; i < bls.size(); i++) {
        Snap s = bls.get(i);
        if ((s.target == SETF1 || s.target == SETF2) && s.full[5]) { nameA = s.r[5]; break; }
      }
      String siteName = nameA != null ? readStr(nameA) : null;
      if (isIdent(siteName)) {
        // direct-named site (FLN2 style): pair with the userdata-stored fn ptr
        long cfunc = push.h4 && isExec(push.stw4) ? push.stw4
                   : push.h0 && isExec(push.stw0) ? push.stw0 : 0;
        named++;
        if (cfunc != 0) {
          byMethod.computeIfAbsent(cfunc, k -> new LinkedHashSet<>()).add(siteName);
          cat.append(String.format("?\t%s\t0x%08X\t0x%08X\t0x%08X%n", siteName, cfunc, call, call));
        } else namedNoFunc++;
        continue;
      }
      for (long a = call; a >= call - 4L * 1024 && a >= 0x82000000L; a -= 4) {
        if (rd32(a) == PROLOGUE) { entries.add(a); break; }
      }
    }
    println("FindLuaNatives5: sites=" + sites.size() + " direct-named=" + named
        + " (noFunc=" + namedNoFunc + "), registrar entries=" + entries.size());

    // ---- stage 2: raw bl-scan of executable blocks for callers
    Map<Long, List<Long>> callers = new LinkedHashMap<>();  // entry -> caller sites
    int callerSites = 0;
    for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
      if (!blk.isInitialized() || !blk.isExecute()) continue;
      long start = blk.getStart().getOffset(), size = blk.getSize();
      byte[] buf = new byte[(int) Math.min(size, Integer.MAX_VALUE - 8)];
      try { blk.getBytes(blk.getStart(), buf); } catch (Exception ex) { continue; }
      for (int off = 0; off + 4 <= buf.length; off += 4) {
        int w = ((buf[off] & 0xff) << 24) | ((buf[off+1] & 0xff) << 16)
              | ((buf[off+2] & 0xff) << 8) | (buf[off+3] & 0xff);
        if (((w >>> 26) & 0x3f) != 18 || (w & 1) != 1 || (w & 2) != 0) continue;
        long li = w & 0x03FFFFFC;
        if ((li & 0x02000000) != 0) li |= 0xFFFFFFFFFC000000L;
        long tgt = (start + off + li) & 0xffffffffL;
        if (!entries.contains(tgt)) continue;
        long cs = start + off;
        if (entries.contains(cs)) continue;   // ignore self/adjacent weirdness
        callers.computeIfAbsent(tgt, k -> new ArrayList<>()).add(cs);
        callerSites++;
      }
    }
    println("FindLuaNatives5: raw scan found " + callerSites + " caller sites for "
        + callers.size() + "/" + entries.size() + " entries");

    // ---- stage 3: parse each caller (byMethod/cat continue from stage 1)
    StringBuilder rep = new StringBuilder();
    int okC = 0, darkC = 0, reported = 0;
    for (Map.Entry<Long, List<Long>> e : callers.entrySet()) {
      long entry = e.getKey();
      for (long cs : e.getValue()) {
        List<Snap> bls = walk(cs - 4L * BACK, BACK + 1);
        Snap sn = null; int callIdx = -1;
        for (int i = 0; i < bls.size(); i++) if (bls.get(i).at == cs) { sn = bls.get(i); callIdx = i; break; }
        if (sn == null) { darkC++; continue; }
        // slot map: r1+offset -> last cstring assigned into it via
        // lh_string_assign_cstr(&slot, cstr, -1) before this call. Track the
        // assign ORDER so "nearest to the call wins" for name preference.
        Map<Long, String> slotStr = new HashMap<>();
        Map<Long, Integer> slotOrd = new HashMap<>();
        int ord = 0;
        for (int i = 0; i < callIdx; i++) {
          Snap a = bls.get(i);
          if (a.target != 0x8222CED0L) continue;
          if (!a.sp[3] || !a.full[3]) continue;
          ord++;
          if (a.full[4] && !a.sp[4]) {
            String s = readStr(a.r[4]);
            if (isIdent(s)) { slotStr.put(a.r[3], s); slotOrd.put(a.r[3], ord); continue; }
          }
          slotStr.remove(a.r[3]); slotOrd.remove(a.r[3]);   // overwritten with unknown
        }
        // Register convention (validated on Inventory.AddItemOfType /
        // PlayerWebsiteUnlocks.IsItemUnlocked / the 823ABxxx family):
        // r5 slot = METHOD name, r4 slot = CLASS string.
        String name = null, cls = null; int nameReg = -1;
        int[] slotPrio = {5, 4, 6, 3, 7};
        for (int g : slotPrio) {
          if (!sn.full[g] || !sn.sp[g]) continue;
          String s = slotStr.get(sn.r[g]);
          if (s == null) continue;
          if (name == null) { name = s; nameReg = g; }
          else if (cls == null && g < nameReg) cls = s;   // lower reg (r4) = class
        }
        // class candidate = r4 slot when the name came from r5
        if (name != null && nameReg == 5 && sn.full[4] && sn.sp[4]) {
          String s4 = slotStr.get(sn.r[4]);
          if (s4 != null && !s4.equals(name)) cls = s4;
        }
        // 2) direct constants (tutorial-family convention: r4 = char*)
        if (name == null) {
          for (int g : new int[]{4, 5, 3, 6, 7}) {
            if (!sn.full[g] || sn.sp[g]) continue;
            String d = readStr(sn.r[g]);
            if (isIdent(d)) { name = d; nameReg = g; break; }
          }
        }
        // 3) indirect: *(r) or **(r)
        if (name == null) {
          for (int g : new int[]{4, 5, 3, 6, 7}) {
            if (!sn.full[g] || sn.sp[g]) continue;
            long p1 = rd32(sn.r[g]);
            String s1 = p1 > 0 ? readStr(p1) : null;
            if (isIdent(s1)) { name = s1; nameReg = g; break; }
            long p2 = p1 > 0 ? rd32(p1) : -1;
            String s2 = p2 > 0 ? readStr(p2) : null;
            if (isIdent(s2)) { name = s2; nameReg = g; break; }
          }
        }
        long method = 0;
        int[] methOrder = {6, 7, 5, 8, 9, 10, 4, 3};
        for (int g : methOrder) {
          if (g == nameReg || !sn.full[g] || sn.sp[g]) continue;
          if (isExec(sn.r[g])) { method = sn.r[g]; break; }
        }
        if (method == 0) {
          // member-fn-ptr struct passed BY STACK POINTER: reg = sp+off, frame
          // slot [off] (or [off+4]) holds the code address (stored via stw).
          for (int g : methOrder) {
            if (g == nameReg || !sn.full[g] || !sn.sp[g]) continue;
            Long v0 = sn.spSlots.get(sn.r[g]);
            Long v4 = sn.spSlots.get(sn.r[g] + 4);
            if (v0 != null && isExec(v0)) { method = v0; break; }
            if (v4 != null && isExec(v4)) { method = v4; break; }
          }
        }
        if (reported < 30) {
          reported++;
          StringBuilder line = new StringBuilder(String.format("caller 0x%08X -> entry 0x%08X:", cs, entry));
          for (int g = 3; g <= 10; g++) if (sn.full[g]) {
            String d = readStr(sn.r[g]);
            line.append(String.format(" r%d=0x%08X%s%s", g, sn.r[g],
                isIdent(d) ? "('" + d + "')" : "", isExec(sn.r[g]) ? "[X]" : ""));
          }
          line.append("  => name=").append(name).append(" method=").append(
              method != 0 ? String.format("0x%08X", method) : "-");
          rep.append(line).append("\n");
        }
        if (name != null && method != 0) {
          okC++;
          byMethod.computeIfAbsent(method, k -> new LinkedHashSet<>())
              .add((cls != null ? cls + "." : "") + name);
          cat.append(String.format("%s\t%s\t0x%08X\t0x%08X\t0x%08X%n",
              cls != null ? cls : "?", name, method, entry, cs));
        } else {
          darkC++;
          if (darkC <= 40) {
            StringBuilder dl = new StringBuilder(String.format(
                "DARK caller 0x%08X -> entry 0x%08X: name=%s method=%s regs:",
                cs, entry, name, method != 0 ? String.format("0x%08X", method) : "-"));
            for (int g = 3; g <= 10; g++) if (sn.full[g])
              dl.append(String.format(" r%d=%s0x%08X", g, sn.sp[g] ? "sp+" : "", sn.r[g]));
            // dump slot map for context
            for (Map.Entry<Long, String> sm : slotStr.entrySet())
              dl.append(String.format(" [sp+0x%X='%s']", sm.getKey(), sm.getValue()));
            rep.append(dl).append("\n");
          }
        }
      }
    }

    StringBuilder tsv = new StringBuilder();
    Set<String> used = new HashSet<>();
    for (Map.Entry<Long, LinkedHashSet<String>> m : byMethod.entrySet()) {
      List<String> names = new ArrayList<>(m.getValue());
      String first = names.get(0);
      String plain = first.contains(".") ? first.substring(first.indexOf('.') + 1) : first;
      String label = "lua_native_" + plain;
      if (!used.add(label)) { label = String.format("%s_%04X", label, m.getKey() & 0xffff); used.add(label); }
      tsv.append(String.format("0x%08X\t%s\tLua native '%s' (reflection-registrar caller parse).%n",
          m.getKey(), label, String.join(", ", names)));
    }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives5_report.txt"))) { w.write(rep.toString()); }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives5_catalog.tsv"))) { w.write(cat.toString()); }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives5_labels.tsv"))) { w.write(tsv.toString()); }
    println("FindLuaNatives5: callersParsed ok=" + okC + " dark=" + darkC
        + " uniqueMethods=" + byMethod.size()
        + " -> lua_natives5_{report,catalog,labels}");
  }
}
