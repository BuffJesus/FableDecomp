// Export a content fingerprint for every function in the current program.
// Fingerprint = SHA-256 of the sorted set of {referenced string literals} +
// {called external/thunk names}. This is address-independent, so it is stable
// across two different-but-related builds (ego_r debug vs Fable.exe retail) —
// the basis for porting PDB names from the symbolized donor onto the stripped target.
// Only "distinctive" functions are emitted (enough string content) to keep precision high.
// Args: [outTsvPath]
// Row: fp \t name \t isRealName(0/1) \t nparts \t entryAddr \t sampleString
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.security.MessageDigest;
import java.util.*;

public class ExportFuncFingerprints extends GhidraScript {
    private String sha16(String s) throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        byte[] h = md.digest(s.getBytes("UTF-8"));
        StringBuilder sb = new StringBuilder();
        for (byte b : h) sb.append(String.format("%02x", b));
        return sb.substring(0, 16);
    }
    private boolean isDefault(String n) {
        return n.startsWith("FUN_") || n.startsWith("SUB_") || n.startsWith("sub_") || n.startsWith("thunk_");
    }
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String out = (args.length > 0) ? args[0]
            : "D:/Documents/FableTLC/ghidra_out/fp_" + currentProgram.getName() + ".tsv";
        PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(out)));
        Listing listing = currentProgram.getListing();
        ReferenceManager rm = currentProgram.getReferenceManager();
        FunctionManager fm = currentProgram.getFunctionManager();
        int emitted = 0;
        for (Function f : fm.getFunctions(true)) {
            if (f.isThunk()) continue;
            TreeSet<String> parts = new TreeSet<>();
            int strchars = 0, maxstr = 0;
            AddressSetView body = f.getBody();
            AddressIterator srcs = rm.getReferenceSourceIterator(body, true);
            while (srcs.hasNext()) {
                Address a = srcs.next();
                for (Reference r : rm.getReferencesFrom(a)) {
                    Address to = r.getToAddress();
                    if (to == null) continue;
                    Data d = listing.getDataAt(to);
                    if (d != null && d.hasStringValue()) {
                        Object v = d.getValue();
                        if (v != null) {
                            String s = v.toString();
                            if (s.length() >= 4) {
                                parts.add("s:" + s);
                                strchars += s.length();
                                if (s.length() > maxstr) maxstr = s.length();
                            }
                        }
                    } else if (r.getReferenceType().isCall()) {
                        Function tf = fm.getFunctionAt(to);
                        if (tf != null && (tf.isExternal() || tf.isThunk()))
                            parts.add("c:" + tf.getName());
                    }
                }
            }
            boolean distinctive = (strchars >= 12 && maxstr >= 5) || parts.size() >= 3;
            if (!distinctive) continue;
            StringBuilder key = new StringBuilder();
            for (String p : parts) { key.append(p); key.append('|'); }
            String fp = sha16(key.toString());
            String sample = "";
            for (String p : parts) { if (p.startsWith("s:")) { sample = p.substring(2); break; } }
            if (sample.length() > 40) sample = sample.substring(0, 40);
            sample = sample.replace('\t', ' ').replace('\n', ' ').replace('\r', ' ');
            String name = f.getName();
            pw.println(fp + "\t" + name + "\t" + (isDefault(name) ? 0 : 1) + "\t"
                     + parts.size() + "\t" + f.getEntryPoint() + "\t" + sample);
            emitted++;
        }
        pw.close();
        println("ExportFuncFingerprints: " + emitted + " fingerprints -> " + out);
    }
}
