// Demangle all MSVC-mangled function names (?Foo@CBar@@...) into readable CBar::Foo,
// applying signature/namespace info where the demangler recovers it. Uses the high-level
// DemanglerCmd so it picks the right demangler and applies safely. Saves changes.
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.label.DemanglerCmd;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;

public class DemangleAll extends GhidraScript {
    @Override
    public void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        int done = 0, failed = 0, skipped = 0;
        for (Function f : fm.getFunctions(true)) {
            String n = f.getName();
            if (!(n.startsWith("?") || n.contains("@@") || n.startsWith("__") && n.contains("@"))) {
                skipped++;
                continue;
            }
            try {
                DemanglerCmd cmd = new DemanglerCmd(f.getEntryPoint(), n);
                if (cmd.applyTo(currentProgram, monitor)) done++;
                else failed++;
            } catch (Exception e) {
                failed++;
            }
        }
        println("DemangleAll: demangled=" + done + " failed=" + failed + " skipped=" + skipped);
    }
}
