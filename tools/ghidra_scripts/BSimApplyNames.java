// Port function names from a populated BSim H2 database onto the current program.
// Prereq: donor signatures are committed to the H2 DB (see docs/BSIM_PORT.md).
// For each still-default-named function in currentProgram (Fable.exe): generate its BSim
// signature, QueryNearest the DB, and if the top match exceeds thresholds AND has a real name,
// apply that name (marked with a reversible comment). Writes an audit TSV of every match.
//
// Run headless on Fable.exe (NOT read-only — it saves renames):
//   analyzeHeadless <proj> FableTLC -process Fable.exe -noanalysis \
//     -postScript BSimApplyNames.java file:/D:/Documents/FableTLC/bsim/fabletlc 0.7 0.0 \
//     -scriptPath tools/ghidra_scripts
// Args: <bsimURL> [simThresh=0.7] [sigThresh=0.0] [auditTsv]
//
// NOTE: untested until the DB is populated — verify it compiles/loads on first real run.
import ghidra.app.script.GhidraScript;
import generic.lsh.vector.LSHVectorFactory;
import ghidra.features.bsim.gui.filters.*;
import ghidra.features.bsim.query.*;
import ghidra.features.bsim.query.description.*;
import ghidra.features.bsim.query.protocol.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolUtilities;
import java.io.*;
import java.net.URL;
import java.util.Iterator;

public class BSimApplyNames extends GhidraScript {
    private boolean isDefault(String n) {
        return n != null && (n.startsWith("FUN_") || n.startsWith("SUB_") || n.startsWith("sub_"));
    }
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        String urlStr = a.length > 0 ? a[0] : "file:/D:/Documents/FableTLC/bsim/fabletlc";
        double SIM = a.length > 1 ? Double.parseDouble(a[1]) : 0.7;
        double SIG = a.length > 2 ? Double.parseDouble(a[2]) : 0.0;
        String audit = a.length > 3 ? a[3] : "D:/Documents/FableTLC/ghidra_out/bsim_port_audit.tsv";
        PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(audit)));
        pw.println("#target_addr\ttarget_old\tdonor_name\tsimilarity\tsignificance\tapplied");

        URL url = BSimClientFactory.deriveBSimURL(urlStr);
        int applied = 0, matched = 0;
        try (FunctionDatabase db = BSimClientFactory.buildClient(url, false)) {
            if (!db.initialize()) { println("BSim connect FAILED: " + db.getLastError()); pw.close(); return; }
            LSHVectorFactory vf = db.getLSHVectorFactory();

            // Generate signatures for ALL functions in the target (this decompiles them - long).
            FunctionManager fm = currentProgram.getFunctionManager();
            GenSignatures gensig = new GenSignatures(false);
            gensig.setVectorFactory(vf);
            gensig.openProgram(currentProgram, null, null, null, null, null);
            gensig.scanFunctions(fm.getFunctions(true), fm.getFunctionCount(), monitor);

            DescriptionManager mgr = gensig.getDescriptionManager();
            int signedCount = 0;
            for (Iterator<FunctionDescription> di = mgr.listAllFunctions(); di.hasNext(); di.next()) signedCount++;
            println("DIAG signed_functions=" + signedCount);

            QueryNearest query = new QueryNearest();
            query.manage = mgr;
            query.max = 5;
            query.thresh = SIM;
            query.signifthresh = SIG;
            ResponseNearest response = query.execute(db);
            if (response == null) { println("Query FAILED: " + db.getLastError()); pw.close(); return; }
            println("DIAG response_results=" + response.result.size());

            int resHadNamed = 0, resOnlyUnnamed = 0, resTgtAlreadyNamed = 0;
            for (SimilarityResult sr : response.result) {
                FunctionDescription base = sr.getBase();
                Address addr = currentProgram.getAddressFactory()
                        .getDefaultAddressSpace().getAddress(base.getAddress());
                Function tf = fm.getFunctionAt(addr);
                if (tf == null || !isDefault(tf.getName())) { resTgtAlreadyNamed++; continue; }
                boolean anyNamed = false;
                Iterator<SimilarityNote> it = sr.iterator();
                while (it.hasNext()) {
                    SimilarityNote note = it.next();
                    FunctionDescription d = note.getFunctionDescription();
                    String nm = d.getFunctionName();
                    if (isDefault(nm) || nm == null) continue;   // want a real donor name
                    nm = nm.replaceAll("\\s+", "_");
                    anyNamed = true;
                    matched++;
                    boolean ok = false;
                    try { tf.setName(nm, SourceType.IMPORTED);
                          tf.setComment("[bsim sim=" + note.getSimilarity() + " <- donor]"); ok = true; }
                    catch (Exception e) {
                        try {
                            String safe = SymbolUtilities.replaceInvalidChars(nm, false);
                            tf.setName(safe, SourceType.IMPORTED);
                            ok = true;
                        }
                        catch (Exception e2) {
                            try { tf.setName(nm + "_" + Long.toHexString(base.getAddress()), SourceType.IMPORTED); ok = true; }
                            catch (Exception e3) { ok = false; }
                        }
                    }
                    if (ok) applied++;
                    pw.println(Long.toHexString(base.getAddress()) + "\t" + tf.getName() + "\t" + nm
                             + "\t" + note.getSimilarity() + "\t" + note.getSignificance() + "\t" + ok);
                    break; // best qualifying match only
                }
                if (anyNamed) resHadNamed++; else resOnlyUnnamed++;
            }
            println("DIAG results_with_named_match=" + resHadNamed
                  + " results_only_unnamed_donor=" + resOnlyUnnamed
                  + " results_target_already_named=" + resTgtAlreadyNamed);
        }
        pw.close();
        println("BSimApplyNames: matched=" + matched + " applied=" + applied + " audit=" + audit);
    }
}
