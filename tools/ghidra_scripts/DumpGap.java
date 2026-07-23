// Disassemble a guest address range and report function starts, to identify
// codegen-missed indirect-call targets. Args: startHex endHex (defaults cover
// the 0x82CB2C84..0x82CB2CC8 CRT gap).
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;

public class DumpGap extends GhidraScript {
  @Override
  public void run() throws Exception {
    String[] a = getScriptArgs();
    long start = a.length > 0 ? Long.parseLong(a[0], 16) : 0x82CB2C84L;
    long end   = a.length > 1 ? Long.parseLong(a[1], 16) : 0x82CB2CC8L;
    Listing lst = currentProgram.getListing();
    Address addr = toAddr(start);
    Address endA = toAddr(end);
    println("DumpGap: range " + Long.toHexString(start) + ".." + Long.toHexString(end));
    while (addr.compareTo(endA) < 0) {
      Function f = getFunctionAt(addr);
      String fmark = (f != null) ? ("  <<< FUNC START: " + f.getName() + " len=0x" + Long.toHexString(f.getBody().getNumAddresses())) : "";
      Instruction ins = lst.getInstructionAt(addr);
      if (ins == null) {
        println(addr + ":  (no instruction / data)" + fmark);
        addr = addr.add(4);
        continue;
      }
      println(addr + ":  " + ins.toString() + fmark);
      addr = ins.getMaxAddress().add(1);
    }
    println("DumpGap: done");
  }
}
