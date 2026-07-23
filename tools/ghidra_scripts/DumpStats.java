// Post-script: print coverage-baseline stats to the headless log.
// STATS lines are grep-able: total functions, still-default-named (FUN_/SUB_),
// thunks, and total symbols. Used to seed the pseudo-C coverage metric.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;

public class DumpStats extends GhidraScript {
    @Override
    public void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        int total = 0, def = 0, thunks = 0;
        for (Function f : fm.getFunctions(true)) {
            total++;
            if (f.isThunk()) thunks++;
            String n = f.getName();
            if (n.startsWith("FUN_") || n.startsWith("SUB_") || n.startsWith("sub_")) def++;
        }
        long syms = currentProgram.getSymbolTable().getNumSymbols();
        println("STATS program=" + currentProgram.getName()
              + " total_functions=" + total
              + " default_named=" + def
              + " named=" + (total - def)
              + " thunks=" + thunks
              + " total_symbols=" + syms);
    }
}
