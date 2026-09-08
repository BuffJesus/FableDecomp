// Export every function in an address range (one script translation unit) with
// decompilation, direct calls, string references, and the data references that
// point AT each function (vtable slots / stored function pointers), so entity
// classes can be tied to their lifecycle functions without trusting donor names.
// Usage: -postScript ExportScriptTranslationUnit.java <lo-hex> <hi-hex> <output-json> [define-list]
// The optional define-list is a text file of hex addresses (one per line, '#' comments). Each is
// disassembled and turned into a function if the database has none there (vtable slots and stored
// function pointers frequently land in unanalyzed bytes), and it is exported even when outside the range.
//@category FableTLC
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataType;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.RefType;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class ExportScriptTranslationUnit extends GhidraScript {
    private String json(String value) {
        if (value == null) return "null";
        return "\"" + value.replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t") + "\"";
    }

    private String hex(Address a) { return "0x" + a.toString().toUpperCase(); }

    private String stringAt(Address a) {
        Data d = getDataAt(a);
        if (d == null) d = getDataContaining(a);
        if (d == null) return null;
        Object v = d.getValue();
        if (v instanceof String) return (String) v;
        DataType t = d.getDataType();
        if (t != null && t.getName().toLowerCase().contains("string")) return d.getDefaultValueRepresentation();
        return null;
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 3 || args.length > 4) throw new IllegalArgumentException("expected lo hi output-json [define-list]");
        java.util.LinkedHashSet<Long> extra = new java.util.LinkedHashSet<>();
        if (args.length == 4) {
            for (String line : java.nio.file.Files.readAllLines(new File(args[3]).toPath())) {
                String t = line.trim();
                if (t.isEmpty() || t.startsWith("#")) continue;
                long a = Long.parseLong(t.split("\s+")[0].replace("0x", ""), 16);
                Address addr = toAddr(a);
                if (getFunctionAt(addr) == null) {
                    if (getInstructionAt(addr) == null) disassemble(addr);
                    Function created = createFunction(addr, null);
                    println("DEFINE " + addr + " -> " + (created == null ? "FAILED" : created.getName()));
                }
                extra.add(a);
            }
        }
        long lo = Long.parseLong(args[0].replace("0x", ""), 16);
        long hi = Long.parseLong(args[1].replace("0x", ""), 16);
        File outputFile = new File(args[2]);
        DecompInterface decompiler = new DecompInterface();
        decompiler.setSimplificationStyle("decompile");
        decompiler.openProgram(currentProgram);
        var fm = currentProgram.getFunctionManager();
        var rm = currentProgram.getReferenceManager();
        List<String> rows = new ArrayList<>();
        List<Function> selected = new ArrayList<>();
        FunctionIterator it = fm.getFunctions(toAddr(lo), true);
        while (it.hasNext()) {
            Function f = it.next();
            if (f.getEntryPoint().getOffset() >= hi) break;
            selected.add(f);
        }
        for (long a : extra) {
            Function f = getFunctionAt(toAddr(a));
            if (f != null && !selected.contains(f)) selected.add(f);
        }
        int count = 0;
        for (Function f : selected) {
            count++;
            List<String> calls = new ArrayList<>();
            List<String> strings = new ArrayList<>();
            List<String> immediates = new ArrayList<>();
            InstructionIterator ins = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ins.hasNext()) {
                Instruction i = ins.next();
                if (i.getFlowType().isCall()) {
                    for (Address target : i.getFlows()) {
                        Function called = getFunctionAt(target);
                        if (called == null) called = getFunctionContaining(target);
                        calls.add("{\"site\":" + json(hex(i.getAddress())) + ",\"target\":" + json(hex(target)) +
                            ",\"currentName\":" + json(called == null ? null : called.getName(true)) + "}");
                    }
                }
                for (Reference r : i.getReferencesFrom()) {
                    Address to = r.getToAddress();
                    if (r.getReferenceType().isData() || r.getReferenceType() == RefType.DATA) {
                        String s = stringAt(to);
                        if (s != null) {
                            strings.add("{\"site\":" + json(hex(i.getAddress())) + ",\"address\":" + json(hex(to)) + ",\"value\":" + json(s) + "}");
                        } else {
                            Function pointee = getFunctionAt(to);
                            immediates.add("{\"site\":" + json(hex(i.getAddress())) + ",\"address\":" + json(hex(to)) +
                                ",\"function\":" + json(pointee == null ? null : pointee.getName(true)) + "}");
                        }
                    }
                }
            }
            List<String> pointedFrom = new ArrayList<>();
            List<String> callers = new ArrayList<>();
            ReferenceIterator refs = rm.getReferencesTo(f.getEntryPoint());
            while (refs.hasNext()) {
                Reference r = refs.next();
                Address from = r.getFromAddress();
                Function cf = fm.getFunctionContaining(from);
                if (r.getReferenceType().isCall()) {
                    callers.add("{\"site\":" + json(hex(from)) + ",\"function\":" + json(cf == null ? null : cf.getName(true)) +
                        ",\"functionAddress\":" + json(cf == null ? null : hex(cf.getEntryPoint())) + "}");
                } else {
                    pointedFrom.add("{\"site\":" + json(hex(from)) + ",\"inFunction\":" + json(cf == null ? null : hex(cf.getEntryPoint())) + "}");
                }
            }
            String body;
            DecompileResults res = decompiler.decompileFunction(f, 120, monitor);
            if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null)
                body = res.getDecompiledFunction().getC();
            else body = null;
            rows.add("{\"address\":" + json(hex(f.getEntryPoint())) + ",\"size\":" + f.getBody().getNumAddresses() +
                ",\"currentName\":" + json(f.getName(true)) +
                ",\"calls\":[" + String.join(",", calls) + "]" +
                ",\"strings\":[" + String.join(",", strings) + "]" +
                ",\"immediates\":[" + String.join(",", immediates) + "]" +
                ",\"callers\":[" + String.join(",", callers) + "]" +
                ",\"pointedFrom\":[" + String.join(",", pointedFrom) + "]" +
                ",\"decompile\":" + json(body) + "}");
            println("TU " + hex(f.getEntryPoint()) + " " + f.getName(true));
        }
        try (PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(outputFile)))) {
            w.println("{\"schema\":\"fable-script-translation-unit/0.1\",\"program\":" + json(currentProgram.getName()) +
                ",\"range\":[" + json(args[0]) + "," + json(args[1]) + "],\"functionCount\":" + count +
                ",\"functions\":[" + String.join(",\n", rows) + "]}");
        }
        println("ExportScriptTranslationUnit: " + count + " functions -> " + outputFile);
    }
}
