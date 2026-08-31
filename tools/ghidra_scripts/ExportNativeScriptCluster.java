// Export allocator, constructor, vtable, and five script lifecycle functions as JSON.
// Usage: -postScript ExportNativeScriptCluster.java <script-name> <allocator-address> <output-json> [anchor1,anchor2,...]
//@category FableTLC
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ExportNativeScriptCluster extends GhidraScript {
    private static final String[] ROLES = {"destructor", "RegisterMain", "Main", "Init", "OnPersist"};

    private String json(String value) {
        if (value == null) return "null";
        return "\"" + value.replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t") + "\"";
    }

    private Function findConstructor(Function allocator) {
        Function result = null;
        InstructionIterator instructions = currentProgram.getListing().getInstructions(allocator.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            if (!instruction.getFlowType().isCall()) continue;
            for (Address target : instruction.getFlows()) {
                Function called = getFunctionAt(target);
                if (called == null) continue;
                String name = called.getName().toLowerCase();
                if (name.contains("operator_new") || name.equals("malloc")) continue;
                result = called;
            }
        }
        return result;
    }

    private boolean looksLikeLifecycleVtable(Address candidate) {
        Memory memory = currentProgram.getMemory();
        try {
            for (int slot = 0; slot < 5; slot++) {
                long pointer = memory.getInt(candidate.add(slot * 4L)) & 0xffffffffL;
                Address target = toAddr(pointer);
                if (memory.getBlock(target) == null || !memory.getBlock(target).isExecute()) return false;
            }
            return true;
        } catch (Exception ignored) {
            return false;
        }
    }

    private Address findVtable(Function constructor) {
        InstructionIterator instructions = currentProgram.getListing().getInstructions(constructor.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            Reference[] references = currentProgram.getReferenceManager().getReferencesFrom(instruction.getAddress());
            for (Reference reference : references) {
                Address candidate = reference.getToAddress();
                if (candidate != null && candidate.isMemoryAddress() && looksLikeLifecycleVtable(candidate)) {
                    return candidate;
                }
            }
        }
        return null;
    }

    private String decompile(DecompInterface decompiler, Function function) {
        if (function == null) return null;
        DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
        if (result == null || !result.decompileCompleted()) return null;
        return result.getDecompiledFunction().getC();
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 3 || args.length > 4) {
            throw new IllegalArgumentException("expected script-name allocator-address output-json [comma-separated-anchors]");
        }
        String scriptName = args[0];
        Address allocatorAddress = toAddr(Long.parseLong(args[1].replace("0x", ""), 16));
        Function allocator = getFunctionAt(allocatorAddress);
        if (allocator == null) throw new IllegalArgumentException("no allocator function at " + allocatorAddress);
        Function constructor = findConstructor(allocator);
        Address vtable = findVtable(allocator);
        if (vtable == null && constructor != null) vtable = findVtable(constructor);
        if (vtable == null) throw new IllegalStateException("could not resolve lifecycle vtable from allocator/constructor " + allocatorAddress);

        Memory memory = currentProgram.getMemory();
        List<Function> lifecycle = new ArrayList<>();
        for (int slot = 0; slot < 5; slot++) {
            long pointer = memory.getInt(vtable.add(slot * 4L)) & 0xffffffffL;
            Address target = toAddr(pointer);
            Function function = getFunctionAt(target);
            if (function == null) function = createFunction(target, null);
            if (function == null) function = getFunctionContaining(target);
            if (function == null) throw new IllegalStateException("could not create lifecycle function at " + target);
            lifecycle.add(function);
        }
        DecompInterface decompiler = new DecompInterface();
        decompiler.setSimplificationStyle("decompile");
        decompiler.openProgram(currentProgram);
        List<String> lifecycleDecompiles = new ArrayList<>();
        StringBuilder evidenceText = new StringBuilder();
        evidenceText.append(decompile(decompiler, allocator));
        if (constructor != null) evidenceText.append(decompile(decompiler, constructor));
        for (Function function : lifecycle) {
            String text = decompile(decompiler, function);
            lifecycleDecompiles.add(text);
            evidenceText.append(text);
        }
        List<String> anchors = args.length == 4 && !args[3].isBlank()
            ? Arrays.asList(args[3].split(",")) : new ArrayList<>();
        List<String> missingAnchors = new ArrayList<>();
        for (String anchor : anchors) {
            if (!evidenceText.toString().contains(anchor.trim())) missingAnchors.add(anchor.trim());
        }
        if (!missingAnchors.isEmpty()) {
            throw new IllegalStateException("candidate rejected; missing evidence anchors " + missingAnchors);
        }
        File outputFile = new File(args[2]);
        File parent = outputFile.getParentFile();
        if (parent != null && !parent.isDirectory() && !parent.mkdirs()) {
            throw new IllegalStateException("could not create output directory " + parent);
        }
        try (PrintWriter output = new PrintWriter(new BufferedWriter(new FileWriter(outputFile)))) {
            output.println("{");
            output.println("  \"schema\": \"fable-native-script-cluster/0.1\",");
            output.println("  \"script\": " + json(scriptName) + ",");
            output.println("  \"evidenceLevel\": \"native-decompile\",");
            output.println("  \"allocatorAddress\": " + json("0x" + allocatorAddress.toString().toUpperCase()) + ",");
            output.println("  \"constructorAddress\": " +
                (constructor == null ? "null" : json("0x" + constructor.getEntryPoint().toString().toUpperCase())) + ",");
            output.println("  \"vtableAddress\": " + json("0x" + vtable.toString().toUpperCase()) + ",");
            output.println("  \"evidenceAnchors\": [" + anchors.stream().map(this::json).reduce((a, b) -> a + ", " + b).orElse("") + "],");
            output.println("  \"lifecycle\": [");
            for (int slot = 0; slot < lifecycle.size(); slot++) {
                Function function = lifecycle.get(slot);
                output.println("    {");
                output.println("      \"slot\": " + slot + ",");
                output.println("      \"role\": " + json(ROLES[slot]) + ",");
                output.println("      \"address\": " + json("0x" + function.getEntryPoint().toString().toUpperCase()) + ",");
                output.println("      \"currentName\": " + json(function.getName()) + ",");
                output.println("      \"decompile\": " + json(lifecycleDecompiles.get(slot)));
                output.println("    }" + (slot + 1 == lifecycle.size() ? "" : ","));
            }
            output.println("  ]");
            output.println("}");
        } finally {
            decompiler.dispose();
        }
        println("ExportNativeScriptCluster: " + scriptName + " allocator=" + allocatorAddress +
            " constructor=" + (constructor == null ? "<inline>" : constructor.getEntryPoint()) +
            " vtable=" + vtable + " -> " + args[2]);
    }
}
