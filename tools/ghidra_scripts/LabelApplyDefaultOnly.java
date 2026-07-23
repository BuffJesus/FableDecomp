// Apply function labels only when the current function still has a default name.
// TSV format: <hex-address> \t <new_name> \t <optional comment>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.io.File;
import java.nio.file.Files;

public class LabelApplyDefaultOnly extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            println("Usage: LabelApplyDefaultOnly.java <labels.tsv>");
            return;
        }

        File input = new File(args[0]);
        if (!input.exists()) {
            println("LabelApplyDefaultOnly: no input file " + input.getAbsolutePath());
            return;
        }

        int rows = 0, renamed = 0, commented = 0, created = 0, skippedNamed = 0, failed = 0;
        for (String line : Files.readAllLines(input.toPath())) {
            String s = line.trim();
            if (s.isEmpty() || s.startsWith("#")) {
                continue;
            }

            String[] cols = line.split("\t", -1);
            if (cols.length < 2) {
                continue;
            }
            rows++;

            String addrText = cols[0].trim().replaceFirst("^0x", "");
            String newName = cols[1].trim();
            String comment = (cols.length > 2) ? cols[2].trim() : "";

            Address address;
            try {
                address = toAddr(Long.parseLong(addrText, 16));
            }
            catch (NumberFormatException e) {
                failed++;
                continue;
            }

            Function fn = getFunctionAt(address);
            if (fn == null) {
                if (getInstructionAt(address) == null) {
                    new DisassembleCommand(address, null, true).applyTo(currentProgram, monitor);
                }
                CreateFunctionCmd cmd = new CreateFunctionCmd(address);
                cmd.applyTo(currentProgram, monitor);
                fn = getFunctionAt(address);
                if (fn != null) {
                    created++;
                }
            }
            if (fn == null) {
                failed++;
                continue;
            }

            String currentName = fn.getName();
            boolean isDefault = currentName.startsWith("FUN_") ||
                currentName.startsWith("SUB_") ||
                currentName.startsWith("sub_");
            if (!newName.isEmpty() && isDefault) {
                fn.setName(newName, SourceType.USER_DEFINED);
                renamed++;
            }
            else if (!isDefault) {
                skippedNamed++;
            }

            if (!comment.isEmpty()) {
                String existing = fn.getComment();
                if (existing == null || existing.isEmpty()) {
                    fn.setComment(comment);
                }
                else if (!existing.contains(comment)) {
                    fn.setComment(existing + "\n" + comment);
                }
                commented++;
            }
        }

        println(String.format(
            "LabelApplyDefaultOnly: rows=%d renamed=%d commented=%d created=%d skippedNamed=%d failed=%d",
            rows, renamed, commented, created, skippedNamed, failed));
    }
}
