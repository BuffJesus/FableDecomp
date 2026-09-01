// Decompile address-keyed native helpers selected by the conversion-readiness queue.
// Usage: -postScript ExportNativeHelperTargets.java <queue-tsv> <output-json>
//@category FableTLC
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class ExportNativeHelperTargets extends GhidraScript {
    private String json(String value) {
        if (value == null) return "null";
        return "\"" + value.replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t") + "\"";
    }

    private String directCallsJson(Function function) {
        if (function == null) return "[]";
        List<String> rows = new ArrayList<>();
        InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            if (!instruction.getFlowType().isCall()) continue;
            for (Address target : instruction.getFlows()) {
                Function called = getFunctionAt(target);
                if (called == null) called = getFunctionContaining(target);
                rows.add("{\"site\":" + json("0x" + instruction.getAddress().toString().toUpperCase()) +
                    ",\"target\":" + json("0x" + target.toString().toUpperCase()) +
                    ",\"currentName\":" + json(called == null ? null : called.getName(true)) + "}");
            }
        }
        return "[" + String.join(",", rows) + "]";
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("expected queue-tsv output-json");
        List<String[]> targets = new ArrayList<>();
        try (BufferedReader input = new BufferedReader(new FileReader(args[0]))) {
            String line;
            boolean header = true;
            while ((line = input.readLine()) != null) {
                if (header) { header = false; continue; }
                String[] fields = line.split("\\t", -1);
                if (fields.length != 6) throw new IllegalArgumentException("invalid queue row: " + line);
                targets.add(fields);
            }
        }
        File outputFile = new File(args[1]);
        File parent = outputFile.getParentFile();
        if (parent != null && !parent.isDirectory() && !parent.mkdirs()) {
            throw new IllegalStateException("could not create output directory " + parent);
        }
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        int completed = 0;
        int failed = 0;
        try (PrintWriter output = new PrintWriter(new BufferedWriter(new FileWriter(outputFile)))) {
            output.println("{\"schema\":\"fable-native-helper-decompiles/0.1\",\"helpers\":[");
            for (int index = 0; index < targets.size(); index++) {
                String[] row = targets.get(index);
                Address address = toAddr(Long.parseLong(row[0].replace("0x", ""), 16));
                Function function = getFunctionAt(address);
                if (function == null) function = getFunctionContaining(address);
                String text = null;
                String error = null;
                if (function == null) {
                    error = "function missing";
                } else {
                    DecompileResults result = decompiler.decompileFunction(function, 90, monitor);
                    if (result.decompileCompleted()) text = result.getDecompiledFunction().getC();
                    else error = result.getErrorMessage();
                }
                if (error == null) completed++; else failed++;
                output.print("{\"targetAddress\":" + json("0x" + address.toString().toUpperCase()) +
                    ",\"currentName\":" + json(function == null ? null : function.getName(true)) +
                    ",\"category\":" + json(row[1]) + ",\"calls\":" + row[2] +
                    ",\"scripts\":" + row[3] + ",\"roles\":" + json(row[4]) +
                    ",\"helperNames\":" + json(row[5]) +
                    ",\"status\":" + json(error == null ? "decompiled" : "failed") +
                    ",\"error\":" + json(error) + ",\"directCalls\":" + directCallsJson(function) +
                    ",\"decompile\":" + json(text) + "}");
                output.println(index + 1 == targets.size() ? "" : ",");
            }
            output.println("]}");
        } finally {
            decompiler.dispose();
        }
        println("ExportNativeHelperTargets: helpers=" + targets.size() + " completed=" + completed +
            " failed=" + failed + " -> " + args[1]);
    }
}
