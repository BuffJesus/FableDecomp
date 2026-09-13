// Export decompiles of a quest script's thread bodies plus the direct callees of its lifecycle
// functions that live inside the script's own address range (one script class is emitted
// contiguously by the compiler, so "same class" == "same range" without trusting bsim labels).
// Usage: -postScript ExportScriptThreadBodies.java <job-tsv>
// Job TSV columns (tab separated, '#' comments):
//   script  class  rangeLo  rangeHi  lifecycle(role=addr,...)  threads(name=addr,...|-)  output-json
// Writes one JSON per row: {script, class, range, functions:[{name,address,decompile,...}]}.
// Run with -readOnly -noanalysis: a thread body with no function in the DB is defined in memory
// only (disassemble + createFunction, like ExportScriptTranslationUnit.java) and never saved.
//@category FableTLC
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;

public class ExportScriptThreadBodies extends GhidraScript {
    private String json(String value) {
        if (value == null) return "null";
        return "\"" + value.replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t") + "\"";
    }

    private String hex(long a) { return String.format("0x%08X", a); }

    private long parse(String s) { return Long.parseLong(s.trim().replace("0x", "").replace("0X", ""), 16); }

    /** Direct call targets in address order (static calls only; indirect calls have no target). */
    private List<Long> directCallees(Function f) {
        LinkedHashSet<Long> out = new LinkedHashSet<>();
        InstructionIterator ins = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ins.hasNext()) {
            Instruction i = ins.next();
            if (!i.getFlowType().isCall()) continue;
            for (Address target : i.getFlows()) {
                Function called = getFunctionAt(target);
                if (called != null) out.add(called.getEntryPoint().getOffset());
            }
        }
        return new ArrayList<>(out);
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("usage: <job-tsv>");
        DecompInterface decompiler = new DecompInterface();
        decompiler.setSimplificationStyle("decompile");
        DecompileOptions opts = new DecompileOptions();
        opts.setMaxPayloadMBytes(256);
        decompiler.setOptions(opts);
        decompiler.openProgram(currentProgram);
        try {
            for (String line : Files.readAllLines(new File(args[0]).toPath())) {
                String t = line.trim();
                if (t.isEmpty() || t.startsWith("#")) continue;
                String[] cols = t.split("\t");
                if (cols.length != 7) throw new IllegalArgumentException("bad job row: " + t);
                String script = cols[0], klass = cols[1];
                long lo = parse(cols[2]), hi = parse(cols[3]);
                Map<Long, String> lifecycle = new LinkedHashMap<>();
                for (String kv : cols[4].split(",")) {
                    String[] p = kv.split("=");
                    lifecycle.put(parse(p[1]), p[0]);
                }
                // selected address -> how it was selected (thread name or "callee of <role>")
                Map<Long, List<String>> why = new LinkedHashMap<>();
                LinkedHashSet<Long> defined = new LinkedHashSet<>();
                if (!cols[5].equals("-")) {
                    for (String kv : cols[5].split(",")) {
                        String[] p = kv.split("=");
                        long a = parse(p[1]);
                        Address addr = toAddr(a);
                        if (getFunctionAt(addr) == null) {
                            // Stored function pointers often land in unanalyzed bytes. Define the
                            // function in memory only: the run is -readOnly, nothing is saved.
                            if (getInstructionAt(addr) == null) disassemble(addr);
                            Function created = createFunction(addr, null);
                            println("DEFINE(in-memory) " + hex(a) + " -> " + (created == null ? "FAILED" : created.getName()));
                            if (created != null) defined.add(a);
                        }
                        why.computeIfAbsent(a, k -> new ArrayList<>()).add("thread:" + p[0]);
                    }
                }
                // direct callees of the lifecycle functions, then of the thread bodies (one level)
                List<long[]> seeds = new ArrayList<>();
                for (Map.Entry<Long, String> e : lifecycle.entrySet()) seeds.add(new long[] {e.getKey(), 0});
                for (Long a : new ArrayList<>(why.keySet())) seeds.add(new long[] {a, 1});
                for (long[] seed : seeds) {
                    Function f = getFunctionAt(toAddr(seed[0]));
                    String label = seed[1] == 0 ? lifecycle.get(seed[0]) : "thread " + hex(seed[0]);
                    if (f == null) { println("NO FUNCTION for " + label); continue; }
                    for (long callee : directCallees(f)) {
                        if (callee < lo || callee > hi) continue;
                        if (lifecycle.containsKey(callee)) continue;
                        why.computeIfAbsent(callee, k -> new ArrayList<>()).add("callee:" + label);
                    }
                }
                List<String> rows = new ArrayList<>();
                for (Map.Entry<Long, List<String>> e : why.entrySet()) {
                    Address addr = toAddr(e.getKey());
                    Function f = getFunctionAt(addr);
                    String body = null, name = null, ns = null, error = null;
                    long size = 0;
                    if (f == null) {
                        Function containing = getFunctionContaining(addr);
                        error = containing == null ? "no function at address"
                            : "address inside " + containing.getName(true) + " @ " + containing.getEntryPoint();
                    } else {
                        name = f.getName(true);
                        ns = f.getParentNamespace() == null ? null : f.getParentNamespace().getName(true);
                        size = f.getBody().getNumAddresses();
                        DecompileResults res = decompiler.decompileFunction(f, 120, monitor);
                        if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null)
                            body = res.getDecompiledFunction().getC();
                        else error = res == null ? "null decompile result" : res.getErrorMessage();
                    }
                    List<String> reasons = new ArrayList<>();
                    for (String r : e.getValue()) reasons.add(json(r));
                    String[] callees = new String[0];
                    List<String> calleeRows = new ArrayList<>();
                    if (f != null) {
                        for (long c : directCallees(f)) {
                            Function cf = getFunctionAt(toAddr(c));
                            calleeRows.add("{\"address\":" + json(hex(c)) + ",\"currentName\":" +
                                json(cf == null ? null : cf.getName(true)) + ",\"inRange\":" + (c >= lo && c <= hi) + "}");
                        }
                    }
                    rows.add("{\"name\":" + json(name) + ",\"address\":" + json(hex(e.getKey())) +
                        ",\"namespace\":" + json(ns) + ",\"size\":" + size +
                        ",\"selectedBy\":[" + String.join(",", reasons) + "]" +
                        ",\"calls\":[" + String.join(",", calleeRows) + "]" +
                        ",\"definedInMemory\":" + defined.contains(e.getKey()) +
                        ",\"error\":" + json(error) + ",\"decompile\":" + json(body) + "}");
                    println("TB " + script + " " + hex(e.getKey()) + " " + name + " " + e.getValue());
                }
                File out = new File(cols[6]);
                out.getParentFile().mkdirs();
                try (PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)))) {
                    w.println("{\"schema\":\"fable-script-thread-bodies/0.1\",\"program\":" + json(currentProgram.getName()) +
                        ",\"script\":" + json(script) + ",\"class\":" + json(klass) +
                        ",\"range\":[" + json(hex(lo)) + "," + json(hex(hi)) + "]" +
                        ",\"functionCount\":" + rows.size() + ",\"functions\":[\n" + String.join(",\n", rows) + "\n]}");
                }
                println("ExportScriptThreadBodies: " + script + " " + rows.size() + " functions -> " + out);
            }
        } finally {
            decompiler.dispose();
        }
    }
}
