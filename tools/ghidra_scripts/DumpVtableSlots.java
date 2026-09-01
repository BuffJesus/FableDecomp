// Resolve vtable offsets to executable targets and current function names.
// Args: <vtable-base> <output-tsv> <offset> [offset ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import java.io.PrintWriter;

public class DumpVtableSlots extends GhidraScript {
  private long hex(String value) {
    return Long.parseLong(value.replace("0x", ""), 16);
  }

  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length < 3) throw new IllegalArgumentException("usage: <base> <output> <offset>...");
    Address base = toAddr(hex(args[0]));
    Memory memory = currentProgram.getMemory();
    int resolved = 0;
    try (PrintWriter out = new PrintWriter(args[1])) {
      out.println("vtable_base\toffset\tslot_address\ttarget_address\tcurrent_name\texecutable");
      for (int i = 2; i < args.length; ++i) {
        long offset = hex(args[i]);
        Address slot = base.add(offset);
        Address target = toAddr(memory.getInt(slot) & 0xffffffffL);
        Function function = getFunctionAt(target);
        boolean executable = memory.getExecuteSet().contains(target);
        if (executable) ++resolved;
        out.println(base + "\t0x" + Long.toHexString(offset) + "\t" + slot + "\t" + target +
                    "\t" + (function == null ? "" : function.getName()) + "\t" + executable);
      }
    }
    println("DumpVtableSlots: requested=" + (args.length - 2) + " executable=" + resolved);
  }
}
