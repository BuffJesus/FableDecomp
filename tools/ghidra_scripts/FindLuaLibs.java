// Label the Lua 5.1 STANDARD LIBRARY: raw-scan .text for calls to
// luaL_register (0x82A25200), backward-parse r4 (lib name) + r5 (luaL_Reg
// array), then walk each null-terminated {char* name, lua_CFunction fn} array
// in .rdata and emit lua_<lib>_<name> labels. Also labels luaL_register.
// Output: ghidra_out/lua_stdlib_labels.tsv
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import java.util.*;

public class FindLuaLibs extends GhidraScript {
  static final long LREG = 0x82A25200L;

  String readStr(long addr) {
    if (addr < 0x82000000L || addr > 0x83500000L) return null;
    try {
      byte[] b = new byte[64];
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

  @Override public void run() throws Exception {
    // find all bl -> luaL_register via raw scan
    List<Long> calls = new ArrayList<>();
    for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
      if (!blk.isInitialized() || !blk.isExecute()) continue;
      long start = blk.getStart().getOffset();
      byte[] buf = new byte[(int) Math.min(blk.getSize(), Integer.MAX_VALUE - 8)];
      try { blk.getBytes(blk.getStart(), buf); } catch (Exception ex) { continue; }
      for (int off = 0; off + 4 <= buf.length; off += 4) {
        int w = ((buf[off] & 0xff) << 24) | ((buf[off+1] & 0xff) << 16)
              | ((buf[off+2] & 0xff) << 8) | (buf[off+3] & 0xff);
        if (((w >>> 26) & 0x3f) != 18 || (w & 1) != 1 || (w & 2) != 0) continue;
        long li = w & 0x03FFFFFC;
        if ((li & 0x02000000) != 0) li |= 0xFFFFFFFFFC000000L;
        if (((start + off + li) & 0xffffffffL) == LREG) calls.add(start + off);
      }
    }
    println("FindLuaLibs: " + calls.size() + " luaL_register call sites");

    StringBuilder tsv = new StringBuilder();
    tsv.append(String.format("0x%08X\tluaL_register\tLua 5.1 luaL_register(L, libname, luaL_Reg*) — registers the stdlib tables (debug/math/base/coroutine/table/string/package).%n", LREG));
    Map<Long, String> byFunc = new LinkedHashMap<>();
    int libs = 0;
    for (long call : calls) {
      // backward constant parse for r4 (libname) and r5 (array) — simple lis/addi
      long[] hi = new long[32]; boolean[] have = new boolean[32]; boolean[] full = new boolean[32];
      for (long a = call - 4L * 24; a < call; a += 4) {
        long lw = rd32(a);
        if (lw < 0) continue;
        int w = (int) lw;
        int opc = (w >>> 26) & 0x3f;
        int rD = (w >>> 21) & 0x1f;
        int rA = (w >>> 16) & 0x1f;
        int imm = (short) (w & 0xffff);
        if (opc == 15 && rA == 0) { hi[rD] = ((long)(w & 0xffff) << 16) & 0xffffffffL; have[rD] = true; full[rD] = false; }
        else if (opc == 15 && have[rA]) { hi[rD] = (hi[rA] + ((long) imm << 16)) & 0xffffffffL; have[rD] = true; full[rD] = false; }
        else if (opc == 14 && rA == 0) { hi[rD] = imm & 0xffffffffL; have[rD] = true; full[rD] = true; }
        else if (opc == 14 && have[rA]) { hi[rD] = (hi[rA] + imm) & 0xffffffffL; have[rD] = true; full[rD] = true; }
        else if (opc == 18 && (w & 1) == 1) { for (int g = 0; g <= 12; g++) if (g != 1 && g != 2) have[g] = false; }
      }
      String lib = have[4] && full[4] ? readStr(hi[4]) : null;
      long arr = have[5] && full[5] ? hi[5] : 0;
      if (arr == 0) { println(String.format("  call 0x%08X: no array (lib=%s)", call, lib)); continue; }
      String libTag = lib == null ? "base" : ("_G".equals(lib) ? "base" : lib);
      int n = 0;
      for (long p = arr; ; p += 8) {
        long np = rd32(p), fp = rd32(p + 4);
        if (np <= 0 || fp <= 0) break;
        String fname = readStr(np);
        if (fname == null || !fname.matches("[A-Za-z_][A-Za-z0-9_]*") || !isExec(fp)) break;
        byFunc.putIfAbsent(fp, "lua_" + libTag + "_" + fname);
        n++;
        if (n > 64) break;
      }
      libs++;
      println(String.format("  lib '%s' array 0x%08X: %d entries (call 0x%08X)", lib, arr, n, call));
    }
    Set<String> used = new HashSet<>();
    for (Map.Entry<Long, String> e : byFunc.entrySet()) {
      String label = e.getValue();
      if (!used.add(label)) { label = String.format("%s_%04X", label, e.getKey() & 0xffff); used.add(label); }
      tsv.append(String.format("0x%08X\t%s\tLua 5.1 stdlib C function (from the luaL_Reg array).%n", e.getKey(), label));
    }
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_stdlib_labels.tsv"))) { w.write(tsv.toString()); }
    println("FindLuaLibs: libs=" + libs + " stdlib funcs=" + byFunc.size() + " -> ghidra_out/lua_stdlib_labels.tsv");
  }
}
