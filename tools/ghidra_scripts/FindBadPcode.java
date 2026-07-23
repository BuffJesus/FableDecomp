// Locate instructions whose Pcode fails to decode inside a function body.
// Arg: hex address of the function. Prints each instruction that throws,
// with its bytes, so the bad disassembly can be patched.
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class FindBadPcode extends GhidraScript {
  @Override public void run() throws Exception {
    Address ad = toAddr(Long.parseLong(getScriptArgs()[0].replace("0x", ""), 16));
    Function f = getFunctionAt(ad);
    if (f == null) { println("no function at " + ad); return; }
    int bad = 0, total = 0;
    InstructionIterator it =
        currentProgram.getListing().getInstructions(f.getBody(), true);
    while (it.hasNext()) {
      Instruction ins = it.next();
      total++;
      try {
        ins.getPcode();
      } catch (Exception e) {
        bad++;
        StringBuilder hex = new StringBuilder();
        for (byte b : ins.getBytes()) hex.append(String.format("%02X ", b));
        println("BADPCODE " + ins.getAddress() + "  " + hex + " " + ins
            + "  err=" + e.getMessage());
      }
    }
    println("FindBadPcode: " + bad + " bad of " + total + " instructions in "
        + f.getName());
  }
}
