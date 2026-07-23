// Diagnose PDB symbol application: how many symbols/functions carry real (mangled or
// demangled game) names vs default. Prints samples. Read-only.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.*;
import ghidra.program.model.listing.*;

public class ProbeSymbols extends GhidraScript {
    @Override
    public void run() throws Exception {
        SymbolTable st = currentProgram.getSymbolTable();
        long total = 0, mangled = 0, gameish = 0;
        int shown = 0;
        SymbolIterator it = st.getAllSymbols(false);
        while (it.hasNext()) {
            Symbol s = it.next();
            total++;
            String n = s.getName();
            if (n.contains("@@") || (n.startsWith("?") && n.contains("@"))) {
                mangled++;
                if (shown < 15) { println("MANGLED sym: " + n + "  @" + s.getAddress()
                        + "  type=" + s.getSymbolType() + "  primary=" + s.isPrimary()); shown++; }
            }
            if (n.startsWith("CThing") || n.startsWith("CHero") || n.startsWith("CWorld")
                || n.startsWith("CScriptBase") || n.startsWith("CQuest")) gameish++;
        }
        println("PROBE total_symbols=" + total + " mangled_symbols=" + mangled
              + " gameish_symbols=" + gameish);
        // functions carrying a mangled/game name
        FunctionManager fm = currentProgram.getFunctionManager();
        long fnMangled = 0, fnTotal = 0;
        for (Function f : fm.getFunctions(true)) {
            fnTotal++;
            String n = f.getName();
            if (n.contains("@@") || n.contains("::") || n.startsWith("?")) fnMangled++;
        }
        println("PROBE functions_total=" + fnTotal + " functions_with_mangled_or_method_name=" + fnMangled);
    }
}
