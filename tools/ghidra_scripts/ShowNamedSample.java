// Print a sample of demangled game-class functions to verify port/demangle quality. Read-only.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import java.util.*;

public class ShowNamedSample extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] classes = {"CWorld::", "CHero::", "CThing::", "CQuestManager::",
                            "CScriptThing::", "CCombat", "CThingManager::"};
        Map<String, Integer> counts = new TreeMap<>();
        List<String> samples = new ArrayList<>();
        FunctionManager fm = currentProgram.getFunctionManager();
        for (Function f : fm.getFunctions(true)) {
            String n = f.getName(true); // include namespace
            for (String c : classes) {
                if (n.contains(c)) {
                    counts.merge(c, 1, Integer::sum);
                    if (samples.size() < 25 && n.contains(c)) samples.add(f.getEntryPoint() + "  " + n);
                    break;
                }
            }
        }
        println("=== class function counts ===");
        for (Map.Entry<String, Integer> e : counts.entrySet())
            println("  " + e.getKey() + " : " + e.getValue());
        println("=== samples ===");
        for (String s : samples) println("  " + s);
    }
}
