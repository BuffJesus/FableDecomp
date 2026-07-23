// Pre-script: force Ghidra to load the matched PDB for ego_r.exe before analysis.
// Ghidra's own log suggests exactly this call. Run via -preScript so the
// PdbUniversalAnalyzer picks it up during -import auto-analysis.
import ghidra.app.script.GhidraScript;
import ghidra.app.plugin.core.analysis.PdbUniversalAnalyzer;
import java.io.File;

public class SetPdbEgoR extends GhidraScript {
    @Override
    public void run() throws Exception {
        File pdb = new File(
          "D:/Documents/FableTLC/symbols/Fable_Anniversary-2013-02-25/Fable/Ego_r.pdb");
        if (!pdb.exists()) { println("SetPdbEgoR: PDB NOT FOUND at " + pdb); return; }
        PdbUniversalAnalyzer.setPdbFileOption(currentProgram, pdb);
        println("SetPdbEgoR: PDB set -> " + pdb.getAbsolutePath());
    }
}
