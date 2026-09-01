// Export exact direct-call sites/targets for every recovered retail script lifecycle.
// Usage: -postScript ExportScriptLifecycleDirectCalls.java <allocator-tsv> <output-json> [name address ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class ExportScriptLifecycleDirectCalls extends GhidraScript {
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
                Address target = toAddr(memory.getInt(candidate.add(slot * 4L)) & 0xffffffffL);
                if (memory.getBlock(target) == null || !memory.getBlock(target).isExecute()) return false;
            }
            return true;
        } catch (Exception ignored) {
            return false;
        }
    }

    private Address findVtable(Function function) {
        InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            for (Reference reference : currentProgram.getReferenceManager().getReferencesFrom(instruction.getAddress())) {
                Address candidate = reference.getToAddress();
                if (candidate != null && candidate.isMemoryAddress() && looksLikeLifecycleVtable(candidate)) return candidate;
            }
        }
        return null;
    }

    private String directCallsJson(Function function) {
        List<String> rows = new ArrayList<>();
        InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            if (!instruction.getFlowType().isCall()) continue;
            for (Address target : instruction.getFlows()) {
                Function called = getFunctionAt(target);
                if (called == null) called = getFunctionContaining(target);
                String name = called == null ? null : called.getName(true);
                rows.add("{\"site\":" + json("0x" + instruction.getAddress().toString().toUpperCase()) +
                    ",\"target\":" + json("0x" + target.toString().toUpperCase()) +
                    ",\"currentName\":" + json(name) + "}");
            }
        }
        return "[" + String.join(",", rows) + "]";
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException("expected allocator-tsv output-json [name address ...]");
        }
        List<String[]> targets = new ArrayList<>();
        try (BufferedReader input = new BufferedReader(new FileReader(args[0]))) {
            String line;
            boolean header = true;
            while ((line = input.readLine()) != null) {
                if (header) { header = false; continue; }
                String[] fields = line.split("\\t", -1);
                if (fields.length >= 2 && !fields[0].isBlank() && !fields[1].isBlank()) targets.add(fields);
            }
        }
        // Registry-string correlation covers 160/161 scripts. Expression_Follow's
        // allocator is a direct registry operand, so allow that independently
        // evidenced address (or future equivalents) to be supplied explicitly.
        if ((args.length - 2) % 2 != 0) {
            throw new IllegalArgumentException("supplemental allocators must be name/address pairs");
        }
        for (int index = 2; index < args.length; index += 2) {
            String[] supplemental = {args[index], args[index + 1]};
            if (supplemental[0].isBlank() || supplemental[1].isBlank()) {
                throw new IllegalArgumentException("invalid supplemental allocator pair");
            }
            targets.add(supplemental);
        }
        File outputFile = new File(args[1]);
        File parent = outputFile.getParentFile();
        if (parent != null && !parent.isDirectory() && !parent.mkdirs()) {
            throw new IllegalStateException("could not create output directory " + parent);
        }
        Memory memory = currentProgram.getMemory();
        try (PrintWriter output = new PrintWriter(new BufferedWriter(new FileWriter(outputFile)))) {
            output.println("{\"schema\":\"fable-script-lifecycle-direct-calls/0.1\",\"scripts\":[");
            for (int index = 0; index < targets.size(); index++) {
                String[] row = targets.get(index);
                Address allocatorAddress = toAddr(Long.parseLong(row[1].replace("0x", ""), 16));
                Function allocator = getFunctionAt(allocatorAddress);
                if (allocator == null) allocator = createFunction(allocatorAddress, null);
                if (allocator == null) throw new IllegalStateException("allocator missing: " + allocatorAddress);
                Function constructor = findConstructor(allocator);
                Address vtable = findVtable(allocator);
                if (vtable == null && constructor != null) vtable = findVtable(constructor);
                if (vtable == null) throw new IllegalStateException("vtable missing: " + row[0]);
                output.println("{\"script\":" + json(row[0]) + ",\"allocatorAddress\":" +
                    json("0x" + allocatorAddress.toString().toUpperCase()) + ",\"lifecycle\":[");
                for (int slot = 0; slot < ROLES.length; slot++) {
                    Address address = toAddr(memory.getInt(vtable.add(slot * 4L)) & 0xffffffffL);
                    Function function = getFunctionAt(address);
                    if (function == null) function = getFunctionContaining(address);
                    if (function == null) throw new IllegalStateException("lifecycle missing: " + address);
                    output.print("{\"role\":" + json(ROLES[slot]) + ",\"address\":" +
                        json("0x" + address.toString().toUpperCase()) + ",\"directCalls\":" +
                        directCallsJson(function) + "}");
                    output.println(slot + 1 == ROLES.length ? "" : ",");
                }
                output.print("]}");
                output.println(index + 1 == targets.size() ? "" : ",");
            }
            output.println("]}");
        }
        println("ExportScriptLifecycleDirectCalls: scripts=" + targets.size() + " -> " + args[1]);
    }
}
