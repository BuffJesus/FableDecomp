// Scan all initialized memory blocks for a 32-bit big-endian value; print hits
// and, for each hit, dump the 8 32-bit words around it (context for tables).
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.*;

public class FindValueInData extends GhidraScript {
  @Override public void run() throws Exception {
    for (String a : getScriptArgs()) {
      long val = Long.parseLong(a.replace("0x", ""), 16);
      byte[] needle = new byte[] {
        (byte)(val >> 24), (byte)(val >> 16), (byte)(val >> 8), (byte)val };
      println("SCAN for BE32 " + a);
      for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
        if (!b.isInitialized()) continue;
        byte[] buf = new byte[(int)b.getSize()];
        b.getBytes(b.getStart(), buf);
        for (int i = 0; i + 4 <= buf.length; i += 4) {
          if (buf[i]==needle[0] && buf[i+1]==needle[1] && buf[i+2]==needle[2] && buf[i+3]==needle[3]) {
            Address hit = b.getStart().add(i);
            StringBuilder ctx = new StringBuilder();
            for (int w = -3; w <= 4; w++) {
              int off = i + w*4;
              if (off < 0 || off + 4 > buf.length) continue;
              long v = ((buf[off]&0xffL)<<24)|((buf[off+1]&0xffL)<<16)|((buf[off+2]&0xffL)<<8)|(buf[off+3]&0xffL);
              ctx.append(String.format(" %s%08x", (w==0?"[":""), v)).append(w==0?"]":"");
            }
            println("  HIT " + hit + " in " + b.getName() + " ctx:" + ctx);
          }
        }
      }
    }
    println("SCAN done");
  }
}
