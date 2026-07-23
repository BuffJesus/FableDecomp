// LabelApply — self-contained headless labeler (apply step).
//
// Reads a TSV of function labels and applies them to the Ghidra program:
//   <hex-address> \t <new_name> \t <optional plate comment>
// Lines starting with '#' or blank are ignored. Address may be 0x-prefixed.
// If no function exists at the address, one is created (disassembling first).
//
// Usage (headless):
//   analyzeHeadless <proj> Fable2_TU1 -process default_tu1.xex -noanalysis \
//     -scriptPath <dir> -postScript LabelApply.java <tsv-path>
// If no arg is given, defaults to D:\Documents\Fable2RE\ghidra_out\labels_in.tsv
//@category Fable2
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class LabelApply extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String path = (args.length > 0) ? args[0]
            : "D:\\Documents\\Fable2RE\\ghidra_out\\labels_in.tsv";
        File f = new File(path);
        if (!f.exists()) { println("LabelApply: no input file " + path); return; }

        int applied = 0, renamed = 0, commented = 0, created = 0, failed = 0;
        for (String line : Files.readAllLines(f.toPath())) {
            String s = line.trim();
            if (s.isEmpty() || s.startsWith("#")) continue;
            String[] cols = line.split("\t", -1);
            if (cols.length < 2) continue;
            String addrStr = cols[0].trim().replaceFirst("^0x", "");
            String name = cols[1].trim();
            String comment = (cols.length > 2) ? cols[2].trim() : "";

            long addrL;
            try { addrL = Long.parseLong(addrStr, 16); }
            catch (NumberFormatException e) { println("bad addr: " + cols[0]); failed++; continue; }
            Address a = toAddr(addrL);

            Function fn = getFunctionAt(a);
            if (fn == null) {
                if (getInstructionAt(a) == null)
                    new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
                CreateFunctionCmd cmd = new CreateFunctionCmd(a);
                cmd.applyTo(currentProgram, monitor);
                fn = getFunctionAt(a);
                if (fn != null) created++;
            }
            if (fn == null) { println("no function at " + a + " (" + name + ")"); failed++; continue; }

            if (!name.isEmpty() && !name.equals(fn.getName())) {
                fn.setName(name, SourceType.USER_DEFINED);
                renamed++;
            }
            if (!comment.isEmpty()) {
                fn.setComment(comment);
                commented++;
            }
            applied++;
        }
        println(String.format(
            "LabelApply: %d rows applied (%d renamed, %d commented, %d created, %d failed)",
            applied, renamed, commented, created, failed));
    }
}
