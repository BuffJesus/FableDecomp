// Enumerate Lua natives registered via the closure-push pattern (the bulk of the
// scripting API — Debug/GUI/ScriptFunction/Inventory/...). Pattern per site:
//   bl 823CE2E0            ; get/create class table (r4 = class-path string)
//   bl 82227880            ; lua_newuserdata(L, 4)
//   stw rX, 0(r11)         ; store the REAL native fn ptr into the userdata
//   bl 8219AA80            ; lua_pushcclosure(L, trampoline(r4), 1)
//   bl 823CE420            ; (r3=&tmp, r4=L, r5=method-name string)
//   bl 82A246C8            ; lua_setfield(L, -2, name(r5))
// We drive from every ref to 8219AA80, walk a window of raw PPC words tracking
// GPR constants (lis/addi/ori/mr; volatiles r0,r3-r12 clobbered at each bl,
// snapshot taken first), and pair the last stw-to-offset-0 value (the cfunc)
// with r5 at the following 823CE420/82A246C8 call (the name) and r4 at the
// preceding 823CE2E0 call (the class path). Emits ghidra_out/lua_natives2.tsv.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.*;
import java.util.*;

public class FindLuaNatives2 extends GhidraScript {
  static final long PUSHC = 0x8219AA80L;   // lua_pushcclosure wrapper
  static final long SETF1 = 0x823CE420L;   // pre-setfield helper (r5=name)
  static final long SETF2 = 0x82A246C8L;   // lua_setfield (r5=name)
  static final long GETTB = 0x823CE2E0L;   // class-table lookup (r4=class path)

  String readStr(long addr) {
    try {
      byte[] b = new byte[96];
      currentProgram.getMemory().getBytes(toAddr(addr), b);
      StringBuilder sb = new StringBuilder();
      for (byte x : b) { if (x == 0) break; if (x < 32 || x > 126) return null; sb.append((char) x); }
      return sb.length() > 0 ? sb.toString() : null;
    } catch (Exception e) { return null; }
  }

  boolean isExec(long addr) {
    MemoryBlock blk = currentProgram.getMemory().getBlock(toAddr(addr));
    return blk != null && blk.isExecute() && (addr & 3) == 0;
  }

  static class BlSnap {
    long at, target;
    long[] r = new long[8];
    boolean[] full = new boolean[8];
    long stw0, stw4; boolean haveStw0, haveStw4;
  }

  @Override public void run() throws Exception {
    ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(toAddr(PUSHC));
    List<Long> sites = new ArrayList<>();
    while (it.hasNext()) {
      Reference r = it.next();
      if (r.getReferenceType().isCall()) sites.add(r.getFromAddress().getOffset());
    }
    println("FindLuaNatives2: " + sites.size() + " pushcclosure call sites");

    // cfunc -> list of "Class.Name" strings
    Map<Long, LinkedHashSet<String>> byFunc = new LinkedHashMap<>();
    Map<Long, Integer> trampCount = new LinkedHashMap<>();
    int okSites = 0, noName = 0, noFunc = 0;

    int BACK = 96, FWD = 16;
    for (long call : sites) {
      long lo = call - 4L * BACK;
      int n = BACK + 1 + FWD;
      int[] words = new int[n];
      for (int k = 0; k < n; k++) {
        try { words[k] = getInt(toAddr(lo + 4L * k)); } catch (Exception e) { words[k] = 0; }
      }
      long[] hi = new long[32]; boolean[] have = new boolean[32]; boolean[] full = new boolean[32];
      long stw0 = 0, stw4 = 0; boolean haveStw0 = false, haveStw4 = false;
      List<BlSnap> bls = new ArrayList<>();
      for (int k = 0; k < n; k++) {
        int w = words[k];
        long ia = lo + 4L * k;
        int opc = (w >>> 26) & 0x3f;
        int rD = (w >>> 21) & 0x1f;
        int rA = (w >>> 16) & 0x1f;
        int imm = (short) (w & 0xffff);
        if (opc == 18 && (w & 1) == 1 && (w & 2) == 0) {   // bl (relative)
          long li = w & 0x03FFFFFC;
          if ((li & 0x02000000) != 0) li |= 0xFFFFFFFFFC000000L;
          long tgt = (ia + li) & 0xffffffffL;
          BlSnap s = new BlSnap();
          s.at = ia; s.target = tgt;
          for (int g = 0; g < 8; g++) { s.r[g] = hi[g]; s.full[g] = have[g] && full[g]; }
          s.stw0 = stw0; s.haveStw0 = haveStw0;
          s.stw4 = stw4; s.haveStw4 = haveStw4;
          bls.add(s);
          for (int g = 0; g <= 12; g++) if (g != 1 && g != 2) { have[g] = false; }  // clobber volatiles
          have[0] = false;
          stw0 = stw4 = 0; haveStw0 = haveStw4 = false;  // stw candidates don't survive a call
          continue;
        }
        if (opc == 15) {                 // addis/lis
          long val = ((long)(w & 0xffff) << 16) & 0xffffffffL;
          if (rA == 0) { hi[rD] = val; have[rD] = true; full[rD] = false; }
          else if (have[rA]) { hi[rD] = (hi[rA] + ((long)imm << 16)) & 0xffffffffL; have[rD] = true; full[rD] = false; }
          else have[rD] = false;
        } else if (opc == 14) {          // addi/li
          if (rA == 0) { hi[rD] = imm & 0xffffffffL; have[rD] = true; full[rD] = true; }
          else if (have[rA]) { hi[rD] = (hi[rA] + imm) & 0xffffffffL; have[rD] = true; full[rD] = true; }
          else have[rD] = false;
        } else if (opc == 24) {          // ori rA,rS,UIMM (dest = rA)
          if (have[rD]) { hi[rA] = (hi[rD] | (w & 0xffff)) & 0xffffffffL; have[rA] = true; full[rA] = true; }
          else have[rA] = false;
        } else if (opc == 36) {          // stw rS, d(rA)
          if (imm == 0 && have[rD] && full[rD]) { stw0 = hi[rD]; haveStw0 = true; }
          if (imm == 4 && have[rD] && full[rD]) { stw4 = hi[rD]; haveStw4 = true; }
        } else if (opc == 37) {          // stwu writes rA
          have[rA] = false;
        } else if (opc == 32 || opc == 33 || opc == 34 || opc == 35 || opc == 40
                || opc == 41 || opc == 42 || opc == 43 || opc == 46) {  // loads write rD
          have[rD] = false;
          if (opc == 33 || opc == 35 || opc == 41 || opc == 43) have[rA] = false;
        } else if (opc == 31) {
          int xo = (w >>> 1) & 0x3ff;
          if (xo == 444) {               // or rA,rS,rB (mr when rS==rB)
            int rS = (w >>> 21) & 0x1f; int rB = (w >>> 11) & 0x1f; int dst = (w >>> 16) & 0x1f;
            if (rS == rB && have[rS]) { hi[dst] = hi[rS]; have[dst] = have[rS]; full[dst] = full[rS]; }
            else have[dst] = false;
          } else if (xo == 151 || xo == 183 || xo == 215 || xo == 247 || xo == 407 || xo == 439
                  || xo == 661 || xo == 150) {
            // x-form stores (stwx/stwux/stbx/...) — no GPR write (update forms write rA)
            if (xo == 183 || xo == 247 || xo == 439) have[rA] = false;
          } else {
            have[rD] = false;            // conservative: arith/loads write rD
          }
        }
      }
      // locate the pushcclosure bl at `call`, the following name call, preceding table call
      BlSnap push = null; int pushIdx = -1;
      for (int i = 0; i < bls.size(); i++) if (bls.get(i).at == call) { push = bls.get(i); pushIdx = i; break; }
      if (push == null) continue;
      Long nameA = null;
      for (int i = pushIdx + 1; i < bls.size(); i++) {
        BlSnap s = bls.get(i);
        if ((s.target == SETF1 || s.target == SETF2) && s.full[5]) { nameA = s.r[5]; break; }
      }
      if (push.full[4]) trampCount.merge(push.r[4], 1, Integer::sum);
      // 8-byte userdata (bound method): [this][method] -> method at +4;
      // 4-byte userdata (plain fn): fn at +0.
      long cfunc;
      if (push.haveStw4 && isExec(push.stw4)) cfunc = push.stw4;
      else if (push.haveStw0 && isExec(push.stw0)) cfunc = push.stw0;
      else {
        noFunc++;
        if (noFunc <= 20) println(String.format("  [noFunc] site 0x%08X stw0=%b/0x%08X stw4=%b/0x%08X",
            call, push.haveStw0, push.stw0, push.haveStw4, push.stw4));
        continue;
      }
      String name = nameA != null ? readStr(nameA) : null;
      if (name == null || !name.matches("[A-Za-z_][A-Za-z0-9_]*")) {
        noName++;
        if (noName <= 20) {
          StringBuilder ahead = new StringBuilder();
          for (int i = pushIdx + 1; i < bls.size(); i++)
            ahead.append(String.format(" bl@+0x%X->0x%08X(r5 full=%b val=0x%08X)",
                bls.get(i).at - call, bls.get(i).target, bls.get(i).full[5], bls.get(i).r[5]));
          println(String.format("  [noName] site 0x%08X nameA=%s cfunc=0x%08X ahead:%s",
              call, nameA == null ? "null" : String.format("0x%08X", nameA), cfunc, ahead));
        }
        continue;
      }
      byFunc.computeIfAbsent(cfunc, k -> new LinkedHashSet<>()).add(name);
      okSites++;
    }

    StringBuilder tsv = new StringBuilder();
    Set<String> used = new HashSet<>();
    for (Map.Entry<Long, LinkedHashSet<String>> e : byFunc.entrySet()) {
      List<String> names = new ArrayList<>(e.getValue());
      String label = "lua_native_" + names.get(0);
      if (!used.add(label)) {
        label = String.format("%s_%04X", label, e.getKey() & 0xffff);
        used.add(label);
      }
      String all = String.join(", ", names);
      tsv.append(String.format("0x%08X\t%s\tLua native method '%s' (closure-push registration).%n",
          e.getKey(), label, all));
      println(String.format("0x%08X\t%s", e.getKey(), all));
    }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives2.tsv"))) { w.write(tsv.toString()); }
    println("FindLuaNatives2: sites=" + sites.size() + " ok=" + okSites + " noName=" + noName
        + " noFunc=" + noFunc + " uniqueFuncs=" + byFunc.size() + " -> ghidra_out/lua_natives2.tsv");
    for (Map.Entry<Long, Integer> t : trampCount.entrySet())
      println(String.format("  trampoline 0x%08X used by %d sites", t.getKey(), t.getValue()));
  }
}
