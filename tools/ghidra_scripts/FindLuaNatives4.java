// Stage 4: find the reflection/binding descriptor structs in .data.
// Recomputes the unnamed registrar instantiation entries (as FindLuaNatives3),
// then scans every initialized non-executable memory block for 32-bit words
// equal to any entry address. For each hit, dumps a 16-word neighborhood with
// string/exec annotations so the descriptor layout can be learned, and tries
// name extraction (nearest identifier-string pointer in the struct).
// Output: ghidra_out/lua_natives4_hits.txt
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.*;
import java.util.*;

public class FindLuaNatives4 extends GhidraScript {
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

  @Override public void run() throws Exception {
    // stage 1: unnamed-site instantiation entries (same walk as before, trimmed:
    // just need call sites whose following setfield r5 is not a const identifier)
    ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(toAddr(PUSHC));
    List<Long> sites = new ArrayList<>();
    while (it.hasNext()) {
      Reference r = it.next();
      if (r.getReferenceType().isCall()) sites.add(r.getFromAddress().getOffset());
    }
    Set<Long> entries = new LinkedHashSet<>();
    int BACK = 96, FWD = 16;
    for (long call : sites) {
      // quick named-check: scan forward FWD words for SETF bl and see if a
      // preceding lis/addi chain gave r5 a string — reuse of the full tracker is
      // overkill here; instead mark named iff FindLuaNatives2 succeeded, which we
      // approximate by checking the 8 words before the setfield for lis+addi into
      // r5/r30/r31 producing an identifier. Simpler and safe: recompute entries for
      // ALL sites; named ones' entries just won't produce new catalog rows later.
      long entry = 0;
      for (long a = call; a >= call - 4L * 1024 && a >= 0x82000000L; a -= 4) {
        if (rd32(a) == PROLOGUE) { entry = a; break; }
      }
      if (entry != 0) entries.add(entry);
    }
    println("FindLuaNatives4: " + entries.size() + " registrar entries from " + sites.size() + " sites");

    // stage 2: scan initialized, non-exec blocks for pointers to any entry
    StringBuilder rep = new StringBuilder();
    int hits = 0, dumped = 0;
    Map<Long, Integer> perEntry = new HashMap<>();
    for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
      if (!blk.isInitialized() || blk.isExecute()) continue;
      long start = blk.getStart().getOffset(), size = blk.getSize();
      if (start < 0x82000000L || start > 0x83500000L) continue;
      byte[] buf = new byte[(int) Math.min(size, Integer.MAX_VALUE - 8)];
      try { blk.getBytes(blk.getStart(), buf); } catch (Exception ex) { continue; }
      for (int off = 0; off + 4 <= buf.length; off += 4) {
        long w = ((buf[off] & 0xffL) << 24) | ((buf[off+1] & 0xffL) << 16)
               | ((buf[off+2] & 0xffL) << 8) | (buf[off+3] & 0xffL);
        if (!entries.contains(w)) continue;
        hits++;
        perEntry.merge(w, 1, Integer::sum);
        long D = start + off;
        if (dumped < 40) {
          dumped++;
          rep.append(String.format("%nHIT %d: entry 0x%08X pointed to from data 0x%08X (block %s)%n",
              hits, w, D, blk.getName()));
          for (int q = -8; q <= 8; q++) {
            long a = D + 4L * q;
            long v = rd32(a);
            if (v < 0) continue;
            String note = "";
            String s = readStr(v);
            if (isIdent(s)) note = "  -> '" + s + "'";
            else if (v == w) note = "  <== registrar";
            else if (isExec(v)) note = "  (code)";
            rep.append(String.format("  0x%08X: %08X%s%n", a, v, note));
          }
        }
      }
    }
    rep.insert(0, String.format("hits=%d over %d distinct entries (of %d)%n",
        hits, perEntry.size(), entries.size()));
    try (java.io.FileWriter w = new java.io.FileWriter(new java.io.File("ghidra_out/lua_natives4_hits.txt"))) { w.write(rep.toString()); }
    println("FindLuaNatives4: data hits=" + hits + " entriesWithHits=" + perEntry.size()
        + "/" + entries.size() + " -> ghidra_out/lua_natives4_hits.txt");
  }
}
