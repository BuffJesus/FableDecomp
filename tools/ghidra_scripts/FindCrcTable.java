// Scan all initialized memory for a 256-entry CRC lookup table (u32).
// Derives the polynomial from the table (reflected: poly=table[0x80]; forward: poly=table[1])
// and verifies all 256 entries. Reports base address, endianness, direction, poly.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.*;

public class FindCrcTable extends GhidraScript {
  long rd(byte[] buf, int i, boolean be) {
    if (be) return ((buf[i]&0xffL)<<24)|((buf[i+1]&0xffL)<<16)|((buf[i+2]&0xffL)<<8)|(buf[i+3]&0xffL);
    return ((buf[i+3]&0xffL)<<24)|((buf[i+2]&0xffL)<<16)|((buf[i+1]&0xffL)<<8)|(buf[i]&0xffL);
  }
  boolean checkReflected(byte[] buf, int base, boolean be, long poly) {
    for (int i=0;i<256;i++){
      long c=i;
      for(int k=0;k<8;k++) c = ((c&1)!=0)?((c>>>1)^poly):(c>>>1);
      c&=0xffffffffL;
      if (rd(buf, base+i*4, be)!=c) return false;
    }
    return true;
  }
  boolean checkForward(byte[] buf, int base, boolean be, long poly) {
    for (int i=0;i<256;i++){
      long c=((long)i)<<24;
      for(int k=0;k<8;k++) c = ((c&0x80000000L)!=0)?(((c<<1)^poly)&0xffffffffL):((c<<1)&0xffffffffL);
      c&=0xffffffffL;
      if (rd(buf, base+i*4, be)!=c) return false;
    }
    return true;
  }
  @Override public void run() throws Exception {
    int found=0;
    for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
      if (!b.isInitialized()) continue;
      byte[] buf=new byte[(int)b.getSize()];
      try { b.getBytes(b.getStart(), buf); } catch(Exception e){ continue; }
      for (int off=0; off+1024<=buf.length; off+=4) {
        for (boolean be : new boolean[]{true,false}) {
          if (rd(buf,off,be)!=0) continue;              // table[0] must be 0
          long polyF=rd(buf,off+4,be);                  // forward poly candidate
          long polyR=rd(buf,off+0x80*4,be);             // reflected poly candidate
          if (polyF!=0 && checkForward(buf,off,be,polyF)) {
            println("CRC-FORWARD @ "+b.getStart().add(off)+" be="+be+" poly="+Long.toHexString(polyF)); found++;
          }
          if (polyR!=0 && checkReflected(buf,off,be,polyR)) {
            println("CRC-REFLECTED @ "+b.getStart().add(off)+" be="+be+" poly="+Long.toHexString(polyR)); found++;
          }
        }
      }
    }
    println("FindCrcTable: done, found="+found);
  }
}
