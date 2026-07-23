// Dump N bytes at a virtual address as hex to a file (one u32 per line, LE).
// Args: <hexAddr> <count> <outFile>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.io.PrintWriter;

public class DumpBytes extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    Address ad = toAddr(Long.parseLong(a[0].replace("0x", ""), 16));
    int n = Integer.parseInt(a[1]);
    byte[] buf = new byte[n];
    currentProgram.getMemory().getBytes(ad, buf);
    try (PrintWriter pw = new PrintWriter(a[2])) {
      for (int i = 0; i + 4 <= n; i += 4) {
        long v = (buf[i] & 0xffL) | ((buf[i + 1] & 0xffL) << 8)
               | ((buf[i + 2] & 0xffL) << 16) | ((buf[i + 3] & 0xffL) << 24);
        pw.printf("%08x%n", v);
      }
    }
    println("DumpBytes: wrote " + (n / 4) + " u32 from " + ad + " -> " + a[2]);
  }
}
