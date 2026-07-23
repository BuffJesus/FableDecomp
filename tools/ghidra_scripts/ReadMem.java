// Dump u32 values (big-endian) over a guest address range. Args: startHex endHex.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;

public class ReadMem extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    long start = Long.parseLong(a[0], 16), end = Long.parseLong(a[1], 16);
    for (long x = start; x < end; x += 4) {
      Address ad = toAddr(x);
      int v = 0; boolean ok = true;
      try { v = getInt(ad); } catch (Exception e) { ok = false; }
      println(String.format("  %08X = %s", x, ok ? String.format("%08X", v) : "??"));
    }
    println("ReadMem: done");
  }
}
