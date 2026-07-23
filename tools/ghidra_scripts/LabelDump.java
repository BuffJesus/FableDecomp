// LabelDump — self-contained headless labeler (gather step).
//
// Emits a labeling worklist: for each target function, its decompiled C plus
// context (callers, callees, referenced string/data) so a human/LLM can name
// it. Output is a delimited text file that is easy to read and to turn into a
// labels_in.tsv for LabelApply.
//
// Target selection (first match wins):
//   arg[0] = path to a file of addresses (one hex per line), OR
//   arg[0] = "auto:strings"   -> functions that reference string literals
//   arg[0] = "auto:calls:N"   -> the N most-called unnamed sub_ functions
//   default                    -> auto:strings, capped at 60
// arg[1] (optional) = output path (default ghidra_out\label_batch.txt)
//
//@category Fable2
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.StringDataInstance;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class LabelDump extends GhidraScript {
    DecompInterface dec;

    boolean unnamed(Function f) {
        String n = f.getName();
        return n.startsWith("sub_") || n.startsWith("FUN_") || n.startsWith("Function_");
    }

    List<Function> pickByStrings(int cap) {
        LinkedHashSet<Function> out = new LinkedHashSet<>();
        DataIterator it = currentProgram.getListing().getDefinedData(true);
        while (it.hasNext() && out.size() < cap) {
            Data d = it.next();
            if (!d.hasStringValue()) continue;
            ReferenceIterator ri = currentProgram.getReferenceManager()
                    .getReferencesTo(d.getAddress());
            while (ri.hasNext()) {
                Function rf = getFunctionContaining(ri.next().getFromAddress());
                if (rf != null && unnamed(rf)) out.add(rf);
            }
        }
        return new ArrayList<>(out);
    }

    List<Function> pickByCalls(int topN) {
        // One pass: for each function, tally its callees. Avoids a per-function
        // getReferencesTo scan (which is far too slow over ~42k functions).
        Map<Function,Integer> counts = new HashMap<>();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            for (Function callee : f.getCalledFunctions(monitor)) {
                if (unnamed(callee)) counts.merge(callee, 1, Integer::sum);
            }
        }
        List<Function> fns = new ArrayList<>(counts.keySet());
        fns.sort((a,b) -> counts.get(b) - counts.get(a));
        return fns.subList(0, Math.min(topN, fns.size()));
    }

    // Collect a string at addr, following one level of pointer indirection
    // (Xbox string refs are often via PTR_s_string_XXXX).
    void collectString(ghidra.program.model.address.Address addr, Set<String> out, int depth) {
        if (addr == null || depth > 1) return;
        Data d = getDataAt(addr);
        if (d == null) return;
        if (d.hasStringValue()) {
            String v = StringDataInstance.getStringDataInstance(d).getStringValue();
            if (v != null && v.length() >= 3) out.add(v.replace("\n", "\\n"));
            return;
        }
        if (d.isPointer()) {
            Object val = d.getValue();
            if (val instanceof ghidra.program.model.address.Address)
                collectString((ghidra.program.model.address.Address) val, out, depth + 1);
        }
    }

    void emit(PrintWriter pw, Function f) {
        pw.println("################################################################");
        pw.println("ADDR: " + f.getEntryPoint() + "   CURRENT: " + f.getName());
        // string refs inside the body
        LinkedHashSet<String> strs = new LinkedHashSet<>();
        InstructionIterator it = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (it.hasNext()) {
            for (Reference r : it.next().getReferencesFrom()) {
                collectString(r.getToAddress(), strs, 0);
            }
        }
        if (!strs.isEmpty()) pw.println("STRINGS: " + strs);
        // callees / callers (names)
        List<String> callees = new ArrayList<>();
        for (Function c : f.getCalledFunctions(monitor)) callees.add(c.getName());
        List<String> callers = new ArrayList<>();
        for (Function c : f.getCallingFunctions(monitor)) callers.add(c.getName());
        pw.println("CALLS: " + callees);
        pw.println("CALLED_BY: " + callers);
        pw.println("--- decompile ---");
        DecompileResults r = dec.decompileFunction(f, 60, monitor);
        pw.println(r.decompileCompleted() ? r.getDecompiledFunction().getC()
                                          : "// DECOMP FAILED");
        pw.println();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String sel = (args.length > 0) ? args[0] : "auto:strings";
        String out = (args.length > 1) ? args[1]
            : "D:\\Documents\\Fable2RE\\ghidra_out\\label_batch.txt";
        new File(out).getParentFile().mkdirs();

        List<Function> targets = new ArrayList<>();
        if (sel.startsWith("auto:strings")) {
            targets = pickByStrings(60);
        } else if (sel.startsWith("auto:calls:")) {
            targets = pickByCalls(Integer.parseInt(sel.substring("auto:calls:".length())));
        } else {
            for (String line : Files.readAllLines(new File(sel).toPath())) {
                String s = line.trim().replaceFirst("^0x", "");
                if (s.isEmpty() || s.startsWith("#")) continue;
                try {
                    Function f = getFunctionContaining(toAddr(Long.parseLong(s, 16)));
                    if (f != null) targets.add(f);
                } catch (NumberFormatException ignore) {}
            }
        }

        dec = new DecompInterface();
        dec.openProgram(currentProgram);
        try (PrintWriter pw = new PrintWriter(new FileWriter(out))) {
            pw.println("# LabelDump worklist: " + targets.size() + " function(s), selector=" + sel);
            pw.println("# Turn into labels_in.tsv lines:  <addr>\\t<name>\\t<comment>");
            pw.println();
            for (Function f : targets) emit(pw, f);
        }
        dec.dispose();
        println("LabelDump: wrote " + targets.size() + " functions to " + out);
    }
}
