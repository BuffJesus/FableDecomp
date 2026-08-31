// Export functions owning an exact string xref, bootstrapping from nearby executable pointer tables.
// Usage: -postScript ExportAnchoredFunction.java <exact-anchor> <output-json>
//@category FableTLC
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class ExportAnchoredFunction extends GhidraScript {
    private String json(String value) {
        if (value == null) return "null";
        return "\"" + value.replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t") + "\"";
    }

    private Function bootstrap(Address xref, Address stringAddress) {
        Memory memory = currentProgram.getMemory();
        for (long delta = -0x800; delta <= 0x800; delta += 4) {
            try {
                Address pointerAddress = stringAddress.add(delta);
                Address target = toAddr(memory.getInt(pointerAddress) & 0xffffffffL);
                if (memory.getBlock(target) == null || !memory.getBlock(target).isExecute()) continue;
                Function function = getFunctionAt(target);
                if (function == null) function = createFunction(target, null);
                if (function != null && function.getBody().contains(xref)) return function;
            } catch (Exception ignored) {
                // Not a viable pointer/function boundary.
            }
        }
        return null;
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("expected exact-anchor output-json");
        String anchor = args[0];
        Set<Function> functions = new LinkedHashSet<>();
        DataIterator data = currentProgram.getListing().getDefinedData(true);
        while (data.hasNext() && !monitor.isCancelled()) {
            Data item = data.next();
            if (!anchor.equals(item.getValue())) continue;
            for (Reference reference : getReferencesTo(item.getAddress())) {
                Function function = getFunctionContaining(reference.getFromAddress());
                if (function == null) function = bootstrap(reference.getFromAddress(), item.getAddress());
                if (function != null && function.getBody().contains(reference.getFromAddress())) functions.add(function);
            }
        }
        if (functions.isEmpty()) throw new IllegalStateException("no anchored function found for " + anchor);

        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        File outputFile = new File(args[1]);
        File parent = outputFile.getParentFile();
        if (parent != null && !parent.isDirectory() && !parent.mkdirs()) {
            throw new IllegalStateException("could not create output directory " + parent);
        }
        try (PrintWriter output = new PrintWriter(new BufferedWriter(new FileWriter(outputFile)))) {
            output.println("{");
            output.println("  \"schema\": \"fable-native-anchored-functions/0.1\",");
            output.println("  \"anchor\": " + json(anchor) + ",");
            output.println("  \"functions\": [");
            int index = 0;
            for (Function function : functions) {
                DecompileResults result = decompiler.decompileFunction(function, 90, monitor);
                String text = result != null && result.decompileCompleted() ? result.getDecompiledFunction().getC() : null;
                output.println("    {\"address\": " + json("0x" + function.getEntryPoint().toString().toUpperCase()) +
                    ", \"currentName\": " + json(function.getName()) + ", \"decompile\": " + json(text) + "}" +
                    (++index == functions.size() ? "" : ","));
            }
            output.println("  ]");
            output.println("}");
        } finally {
            decompiler.dispose();
        }
        println("ExportAnchoredFunction: anchor=" + anchor + " functions=" + functions.size() + " -> " + args[1]);
    }
}
