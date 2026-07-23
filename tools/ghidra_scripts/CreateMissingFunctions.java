// Create functions at plan addresses (addr \t name) where none exists, then name them.
// Companion to ApplyNames.java: recovers the PDB entries it counted as fail= because
// Ghidra's auto-analysis never defined a function there. Addresses that fall inside an
// existing function body are counted (inExisting) but left alone — splitting is a
// separate decision. Args: [planTsvPath]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolUtilities;
import ghidra.program.model.mem.MemoryBlock;
import java.io.*;

public class CreateMissingFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) { println("CreateMissingFunctions: need plan path arg"); return; }
        BufferedReader br = new BufferedReader(new FileReader(args[0]));
        FunctionManager fm = currentProgram.getFunctionManager();
        Listing listing = currentProgram.getListing();
        String line;
        int created = 0, named = 0, already = 0, inExisting = 0, noMem = 0, failDisasm = 0, failName = 0;
        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty() || line.startsWith("#")) continue;
            String[] c = line.split("\t");
            if (c.length < 2) continue;
            Address a;
            try { a = currentProgram.getAddressFactory().getAddress(c[0]); }
            catch (Exception e) { continue; }
            if (a == null) continue;
            if (fm.getFunctionAt(a) != null) { already++; continue; }
            MemoryBlock mb = currentProgram.getMemory().getBlock(a);
            if (mb == null || !mb.isExecute()) { noMem++; continue; }
            if (fm.getFunctionContaining(a) != null) { inExisting++; continue; }
            if (listing.getInstructionAt(a) == null) {
                disassemble(a);
            }
            if (listing.getInstructionAt(a) == null) { failDisasm++; continue; }
            Function f = createFunction(a, null);
            if (f == null) {
                // createFunction can fail on flow-analysis conflicts even with code present
                failDisasm++;
                continue;
            }
            created++;
            String name = c[1].replaceAll("\\s+", "_");
            try {
                f.setName(name, SourceType.IMPORTED);
                f.setComment("[pdb name; fn created by CreateMissingFunctions]");
                named++;
            } catch (Exception e) {
                try {
                    String safe = SymbolUtilities.replaceInvalidChars(name, false);
                    f.setName(safe, SourceType.IMPORTED);
                    named++;
                } catch (Exception e2) {
                    try { f.setName(name + "_" + c[0], SourceType.IMPORTED); named++; }
                    catch (Exception e3) { failName++; }
                }
            }
        }
        br.close();
        println("CreateMissingFunctions: created=" + created + " named=" + named +
                " already=" + already + " inExisting=" + inExisting + " noMem=" + noMem +
                " failDisasm=" + failDisasm + " failName=" + failName);
    }
}
