// For every function, record the set of referenced source-file strings
// (assert/log macros embed __FILE__). These are stable across builds and map a
// function to its origin source file => the engine's module structure.
// On ego_r (PDB-named) this yields file -> real class/method names (a module dossier);
// on Fable.exe it yields file -> function addresses (where to look).
// Args: [outTsvPath]
// Row: entryAddr \t name \t isRealName(0/1) \t comma-separated source files
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;
import java.util.regex.*;

public class MapSourceFiles extends GhidraScript {
    // matches things like  CHero.cpp  .\PKCTRRedblackTree.cpp  foo_bar.h  x.inl  (no spaces)
    private static final Pattern SRC =
        Pattern.compile("([A-Za-z0-9_\\\\./]+\\.(?:cpp|h|hpp|inl|c))", Pattern.CASE_INSENSITIVE);
    private boolean isDefault(String n) {
        return n.startsWith("FUN_") || n.startsWith("SUB_") || n.startsWith("sub_") || n.startsWith("thunk_");
    }
    private String norm(String f) {
        int i = Math.max(f.lastIndexOf('\\'), f.lastIndexOf('/'));
        return (i >= 0 ? f.substring(i + 1) : f).toLowerCase();  // basename only
    }
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String out = (args.length > 0) ? args[0]
            : "D:/Documents/FableTLC/ghidra_out/srcmap_" + currentProgram.getName() + ".tsv";
        PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(out)));
        Listing listing = currentProgram.getListing();
        ReferenceManager rm = currentProgram.getReferenceManager();
        FunctionManager fm = currentProgram.getFunctionManager();
        int emitted = 0;
        for (Function f : fm.getFunctions(true)) {
            if (f.isThunk()) continue;
            TreeSet<String> files = new TreeSet<>();
            AddressIterator srcs = rm.getReferenceSourceIterator(f.getBody(), true);
            while (srcs.hasNext()) {
                Address a = srcs.next();
                for (Reference r : rm.getReferencesFrom(a)) {
                    Address to = r.getToAddress();
                    if (to == null) continue;
                    Data d = listing.getDataAt(to);
                    if (d != null && d.hasStringValue()) {
                        Object v = d.getValue();
                        if (v == null) continue;
                        Matcher m = SRC.matcher(v.toString());
                        while (m.find()) files.add(norm(m.group(1)));
                    }
                }
            }
            if (files.isEmpty()) continue;
            String name = f.getName();
            pw.println(f.getEntryPoint() + "\t" + name + "\t" + (isDefault(name) ? 0 : 1)
                     + "\t" + String.join(",", files));
            emitted++;
        }
        pw.close();
        println("MapSourceFiles: " + emitted + " functions with source-file refs -> " + out);
    }
}
