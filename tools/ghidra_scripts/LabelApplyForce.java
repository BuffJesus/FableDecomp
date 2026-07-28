// Apply function labels, overriding any existing name (including prior BSim
// names). Use ONLY for high-confidence sources (e.g. RTTI vtable-slot ports)
// that should win over lower-confidence guesses. Preserves the decorated MSVC
// name so DemangleAll can process it. Skips a row only when the current name
// already equals the proposed name.
// TSV format: <hex-address> \t <new_name> [\t comment]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.InvalidInputException;
import ghidra.program.model.symbol.SymbolUtilities;
import java.io.File;
import java.nio.file.Files;

public class LabelApplyForce extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            println("Usage: LabelApplyForce.java <labels.tsv>");
            return;
        }
        File input = new File(args[0]);
        if (!input.exists()) {
            println("LabelApplyForce: no input file " + input.getAbsolutePath());
            return;
        }

        int rows = 0, renamed = 0, created = 0, unchanged = 0;
        int commented = 0, failed = 0;
        for (String line : Files.readAllLines(input.toPath())) {
            String s = line.trim();
            if (s.isEmpty() || s.startsWith("#")) continue;
            String[] cols = line.split("\t", -1);
            if (cols.length < 2) continue;
            rows++;

            String addrText = cols[0].trim().replaceFirst("^0x", "");
            String newName = cols[1].trim();
            String comment = cols.length >= 3 ? cols[2].trim() : "";
            Address address;
            try {
                address = toAddr(Long.parseLong(addrText, 16));
            } catch (NumberFormatException e) {
                failed++;
                continue;
            }

            Function fn = getFunctionAt(address);
            if (fn == null) {
                if (getInstructionAt(address) == null) {
                    new DisassembleCommand(address, null, true).applyTo(currentProgram, monitor);
                }
                new CreateFunctionCmd(address).applyTo(currentProgram, monitor);
                fn = getFunctionAt(address);
                if (fn != null) created++;
            }
            if (fn == null) {
                failed++;
                continue;
            }

            if (fn.getName().equals(newName)) {
                unchanged++;
                try {
                    if (!comment.isEmpty()) {
                        fn.setComment(comment);
                        commented++;
                    }
                } catch (Exception e) {
                    failed++;
                }
                continue;
            }
            try {
                fn.setName(newName, SourceType.USER_DEFINED);
                renamed++;
            } catch (InvalidInputException e) {
                try {
                    fn.setName(SymbolUtilities.replaceInvalidChars(newName, true),
                        SourceType.USER_DEFINED);
                    renamed++;
                } catch (Exception e2) {
                    failed++;
                    continue;
                }
            }
            try {
                if (!comment.isEmpty()) {
                    fn.setComment(comment);
                    commented++;
                }
            } catch (Exception e) {
                failed++;
            }
        }

        println(String.format(
            "LabelApplyForce: rows=%d renamed=%d created=%d unchanged=%d commented=%d failed=%d",
            rows, renamed, created, unchanged, commented, failed));
    }
}
