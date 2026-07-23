// Raw string hunter: for each arg string, find its bytes in ANY initialized
// block (defined or not), then raw-scan executable blocks for lis/addi pairs
// that materialize each found address (the standard PPC address build), and
// print the code locations. Works where FindStrXref (defined-data only) fails.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;
import java.util.*;

public class FindStrRaw extends GhidraScript {
  @Override public void run() throws Exception {
    for (String needle : getScriptArgs()) {
      byte[] pat = (needle + "\0").getBytes();
      List<Long> strAddrs = new ArrayList<>();
      for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
        if (!blk.isInitialized()) continue;
        long start = blk.getStart().getOffset();
        byte[] buf = new byte[(int) Math.min(blk.getSize(), Integer.MAX_VALUE - 8)];
        try { blk.getBytes(blk.getStart(), buf); } catch (Exception ex) { continue; }
        outer:
        for (int i = 0; i + pat.length <= buf.length; i++) {
          for (int j = 0; j < pat.length; j++) if (buf[i + j] != pat[j]) continue outer;
          if (i > 0 && buf[i - 1] != 0) continue;   // require preceding NUL/edge
          strAddrs.add(start + i);
        }
      }
      println("STR '" + needle + "' at: " + strAddrs.stream().map(Long::toHexString).reduce("", (a, b) -> a + " " + b));
      for (long sa : strAddrs) {
        int lo = (int) (sa & 0xffff);
        int hiAdj = (int) ((sa >> 16) + ((sa & 0x8000) != 0 ? 1 : 0)) & 0xffff;
        for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
          if (!blk.isInitialized() || !blk.isExecute()) continue;
          long start = blk.getStart().getOffset();
          byte[] buf = new byte[(int) Math.min(blk.getSize(), Integer.MAX_VALUE - 8)];
          try { blk.getBytes(blk.getStart(), buf); } catch (Exception ex) { continue; }
          for (int off = 0; off + 4 <= buf.length; off += 4) {
            int w = ((buf[off] & 0xff) << 24) | ((buf[off+1] & 0xff) << 16)
                  | ((buf[off+2] & 0xff) << 8) | (buf[off+3] & 0xff);
            int opc = (w >>> 26) & 0x3f;
            if (opc != 14) continue;                       // addi
            if ((w & 0xffff) != (lo & 0xffff)) continue;
            int rA = (w >>> 16) & 0x1f;
            if (rA == 0) continue;
            // look back up to 16 instrs for lis rA, hiAdj
            for (int k = 1; k <= 16 && off - 4 * k >= 0; k++) {
              int w2 = ((buf[off-4*k] & 0xff) << 24) | ((buf[off-4*k+1] & 0xff) << 16)
                     | ((buf[off-4*k+2] & 0xff) << 8) | (buf[off-4*k+3] & 0xff);
              if (((w2 >>> 26) & 0x3f) == 15 && ((w2 >>> 16) & 0x1f) == 0
                  && ((w2 >>> 21) & 0x1f) == rA && (w2 & 0xffff) == hiAdj) {
                println(String.format("  ref: addi at 0x%08X (lis %d back) -> str 0x%08X",
                    start + off, k, sa));
                break;
              }
            }
          }
        }
      }
    }
  }
}
