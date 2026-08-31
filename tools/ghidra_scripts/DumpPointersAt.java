// Dump pointer-sized entries and resolved function names at an address.
// Usage: -postScript DumpPointersAt.java <address> <count>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DumpPointersAt extends GhidraScript {
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        Address cursor = toAddr(Long.parseLong(args[0].replace("0x", ""), 16));
        int count = Integer.parseInt(args[1]);
        for (int slot = 0; slot < count; slot++, cursor = cursor.add(4)) {
            long value = currentProgram.getMemory().getInt(cursor) & 0xffffffffL;
            Address target = toAddr(value);
            Function function = getFunctionAt(target);
            println(String.format("slot=%d at=%s value=0x%08X function=%s", slot, cursor, value,
                function == null ? "<nonfunc>" : function.getName()));
        }
    }
}
