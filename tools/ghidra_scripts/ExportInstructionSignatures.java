// Export address-independent instruction-shape signatures for every function.
//
// The signature preserves mnemonics, registers, operand kinds, and small
// constants/field offsets while replacing internal flow and data addresses.
// This complements BSim for tiny functions whose p-code is too generic.
//
// Args: <out-tsv>
// Row: address, name, default_name, body_size, instruction_count,
//      strict_signature, loose_signature
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;

public class ExportInstructionSignatures extends GhidraScript {
    private boolean isDefaultName(String name) {
        String lower = name.toLowerCase();
        return lower.startsWith("fun_") || lower.startsWith("sub_") ||
               lower.startsWith("thunk_") || lower.startsWith("unknown_");
    }

    private String clean(String value) {
        return value.replace('\t', ' ').replace('\r', ' ').replace('\n', ' ');
    }

    private String hash16(String value) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] bytes = digest.digest(value.getBytes(StandardCharsets.UTF_8));
        StringBuilder result = new StringBuilder();
        for (byte valueByte : bytes) {
            result.append(String.format("%02x", valueByte));
        }
        return result.substring(0, 16);
    }

    private String scalarToken(Scalar scalar, boolean loose) {
        long value = scalar.getSignedValue();
        if (loose && (value > 0x1000 || value < -0x1000)) {
            return "IMM";
        }
        return "S" + scalar.bitLength() + ":" + Long.toHexString(value);
    }

    private String operandToken(Instruction instruction, int operand, boolean loose) {
        boolean flow = false;
        boolean data = false;
        for (Reference reference : instruction.getOperandReferences(operand)) {
            if (reference.getReferenceType().isFlow()) flow = true;
            if (reference.getReferenceType().isData()) data = true;
        }
        if (flow) return "FLOW";
        if (data) return "DATA";

        StringBuilder token = new StringBuilder();
        token.append(Integer.toHexString(instruction.getOperandType(operand))).append(':');
        Object[] objects = instruction.getOpObjects(operand);
        for (Object object : objects) {
            if (object instanceof Register) {
                token.append("R:").append(((Register) object).getName().toLowerCase());
            } else if (object instanceof Scalar) {
                token.append(scalarToken((Scalar) object, loose));
            } else if (object instanceof Address) {
                token.append("ADDR");
            } else {
                token.append(object.getClass().getSimpleName());
            }
            token.append(',');
        }
        return token.toString();
    }

    private String canonical(Function function, boolean loose) {
        StringBuilder result = new StringBuilder();
        InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            result.append(instruction.getMnemonicString().toLowerCase()).append('|');
            for (int operand = 0; operand < instruction.getNumOperands(); operand++) {
                result.append(operandToken(instruction, operand, loose)).append('|');
            }
            result.append(';');
        }
        return result.toString();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            throw new IllegalArgumentException("Usage: ExportInstructionSignatures.java <out-tsv>");
        }

        String outPath = args[0];
        FunctionManager functions = currentProgram.getFunctionManager();
        int exported = 0;
        try (PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(outPath)))) {
            out.println("#address\tname\tdefault_name\tbody_size\tinstruction_count\tstrict_signature\tloose_signature");
            for (Function function : functions.getFunctions(true)) {
                monitor.checkCancelled();
                int instructionCount = 0;
                InstructionIterator iterator = currentProgram.getListing().getInstructions(function.getBody(), true);
                while (iterator.hasNext()) {
                    iterator.next();
                    instructionCount++;
                }
                String strict = canonical(function, false);
                String loose = canonical(function, true);
                out.println(
                    function.getEntryPoint() + "\t" + clean(function.getName(true)) + "\t" +
                    (isDefaultName(function.getName()) ? "1" : "0") + "\t" +
                    function.getBody().getNumAddresses() + "\t" + instructionCount + "\t" +
                    hash16(strict) + "\t" + hash16(loose)
                );
                exported++;
                if ((exported % 5000) == 0) {
                    println("ExportInstructionSignatures: " + exported + " functions");
                }
            }
        }
        println("ExportInstructionSignatures: wrote " + exported + " rows to " + outPath);
    }
}
