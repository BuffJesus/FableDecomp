// Apply a rename plan (addr \t name) to the current program. Only renames
// functions still bearing a default name (FUN_/SUB_/sub_) — never clobbers an
// existing name. Marks each with a comment so ported names are auditable/reversible.
// Args: [planTsvPath]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolUtilities;
import java.io.*;

public class ApplyNames extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) { println("ApplyNames: need plan path arg"); return; }
        BufferedReader br = new BufferedReader(new FileReader(args[0]));
        FunctionManager fm = currentProgram.getFunctionManager();
        String line; int applied = 0, skipped = 0, fail = 0;
        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty() || line.startsWith("#")) continue;
            String[] c = line.split("\t");
            if (c.length < 2) { skipped++; continue; }
            Address a;
            try { a = currentProgram.getAddressFactory().getAddress(c[0]); }
            catch (Exception e) { fail++; continue; }
            Function f = (a == null) ? null : fm.getFunctionAt(a);
            if (f == null) { fail++; continue; }
            String cur = f.getName();
            if (!(cur.startsWith("FUN_") || cur.startsWith("SUB_") || cur.startsWith("sub_"))) { skipped++; continue; }
            // Ghidra rejects whitespace in symbol names (PDB has e.g. "dynamic initializer for 'x'")
            c[1] = c[1].replaceAll("\\s+", "_");
            try {
                f.setName(c[1], SourceType.IMPORTED);
                f.setComment("[ported from ego_r via strfp]");
                applied++;
            } catch (Exception e) {
                try {
                    String safe = SymbolUtilities.replaceInvalidChars(c[1], false);
                    f.setName(safe, SourceType.IMPORTED);
                    applied++;
                } catch (Exception e2) {
                    try { f.setName(c[1] + "_" + c[0], SourceType.IMPORTED); applied++; }
                    catch (Exception e3) { fail++; }
                }
            }
        }
        br.close();
        println("ApplyNames: applied=" + applied + " skipped=" + skipped + " fail=" + fail);
    }
}
