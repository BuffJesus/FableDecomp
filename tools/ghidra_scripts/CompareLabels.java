// Compare a proposed labels TSV against current function names.
// Categorizes each row as: default (current is FUN_/SUB_), agree (demangled
// current == demangled proposed base), or disagree. Writes a report TSV
// <addr>\t<category>\t<current>\t<proposed> next to the input.
// TSV format: <hex-address> \t <decorated-name> [\t comment]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.io.File;
import java.io.PrintWriter;
import java.nio.file.Files;

public class CompareLabels extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            println("Usage: CompareLabels.java <labels.tsv> [out.tsv]");
            return;
        }
        File input = new File(args[0]);
        File out = new File(args.length > 1 ? args[1] : args[0] + ".compare.tsv");

        int rows = 0, isDefault = 0, agree = 0, disagree = 0, missing = 0;
        try (PrintWriter pw = new PrintWriter(Files.newBufferedWriter(out.toPath()))) {
            pw.println("address\tcategory\tcurrent\tproposed");
            for (String line : Files.readAllLines(input.toPath())) {
                String s = line.trim();
                if (s.isEmpty() || s.startsWith("#")) continue;
                String[] cols = line.split("\t", -1);
                if (cols.length < 2) continue;
                rows++;

                String addrText = cols[0].trim().replaceFirst("^0x", "");
                String proposed = cols[1].trim();
                Address address;
                try {
                    address = toAddr(Long.parseLong(addrText, 16));
                } catch (NumberFormatException e) {
                    continue;
                }
                Function fn = getFunctionAt(address);
                if (fn == null) {
                    missing++;
                    pw.println(addrText + "\tmissing\t\t" + proposed);
                    continue;
                }
                String current = fn.getName();
                boolean def = current.startsWith("FUN_") || current.startsWith("SUB_")
                    || current.startsWith("sub_");

                // Compare on the class+method core of the proposed decorated name.
                String proposedCore = coreOf(proposed);
                String currentCore = coreOf(current);
                String category;
                if (def) {
                    category = "default";
                    isDefault++;
                } else if (currentCore.equals(proposedCore) || current.contains(proposedCore)) {
                    category = "agree";
                    agree++;
                } else {
                    category = "disagree";
                    disagree++;
                }
                pw.println(addrText + "\t" + category + "\t" + current + "\t" + proposed);
            }
        }

        println(String.format(
            "CompareLabels: rows=%d default=%d agree=%d disagree=%d missing=%d -> %s",
            rows, isDefault, agree, disagree, missing, out.getAbsolutePath()));
    }

    // Extract "Class::Method" or the method token from either a decorated MSVC
    // name (?Method@Class@@...) or an already-demangled Ghidra name.
    private String coreOf(String name) {
        if (name.startsWith("?")) {
            String body = name.substring(1);
            int at = body.indexOf('@');
            if (at < 0) return body;
            String method = body.substring(0, at);
            String rest = body.substring(at + 1);
            int end = rest.indexOf("@@");
            String cls = end > 0 ? rest.substring(0, end) : rest;
            // cls may be scoped as Inner@Outer; take innermost.
            int inner = cls.indexOf('@');
            if (inner > 0) cls = cls.substring(0, inner);
            return cls + "::" + method;
        }
        // Demangled: keep last "Class::Method" pair if present.
        String n = name;
        int paren = n.indexOf('(');
        if (paren > 0) n = n.substring(0, paren);
        int sp = n.lastIndexOf(' ');
        if (sp > 0) n = n.substring(sp + 1);
        return n;
    }
}
