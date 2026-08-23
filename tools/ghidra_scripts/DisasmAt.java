// Dump linear disassembly of the function containing each given hex address.
// Args: <outFile> <addrHex1> [addrHex2 ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.PrintWriter;

public class DisasmAt extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    String outPath = args[0];
    Listing lst = currentProgram.getListing();
    try (PrintWriter pw = new PrintWriter(outPath)) {
      for (int i = 1; i < args.length; i++) {
        long a = Long.parseLong(args[i].replace("0x",""), 16);
        Address addr = toAddr(a);
        Function f = getFunctionContaining(addr);
        if (f == null) { pw.println("//=== " + args[i] + " : NO FUNCTION ==="); continue; }
        pw.println("//=== " + f.getName() + " @ " + f.getEntryPoint() + " (seed " + args[i] + ") ===");
        InstructionIterator it = lst.getInstructions(f.getBody(), true);
        while (it.hasNext()) {
          Instruction ins = it.next();
          Address ea = ins.getAddress();
          StringBuilder bytes = new StringBuilder();
          try { for (byte b : ins.getBytes()) bytes.append(String.format("%02x", b & 0xff)); }
          catch (Exception e) { bytes.append("??"); }
          String ref = "";
          Reference[] refs = ins.getReferencesFrom();
          for (Reference r : refs) {
            if (r.getReferenceType().isCall() || r.getReferenceType().isData()) {
              Symbol s = getSymbolAt(r.getToAddress());
              if (s != null) { ref = "  ; " + s.getName(); break; }
            }
          }
          pw.println(ea + ": " + String.format("%-20s", bytes.toString()) + ins.toString() + ref);
        }
        pw.println();
      }
    }
    println("DisasmAt done -> " + outPath);
  }
}
