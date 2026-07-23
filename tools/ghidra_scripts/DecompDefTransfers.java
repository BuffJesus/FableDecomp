// Batch-decompile the def Transfer methods listed in a targets TSV
// (<hexAddr>\t<className>) and write all pseudo-C to one file with per-function
// headers, so a field-layout parser can walk it. Each CxxxDef::Transfer streams
// its persisted fields through a CPersistContext; the offsets it touches ARE the
// def payload layout (same technique that mapped CChestDef by hand).
// Args: <targetsTsv> <outCFile>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.io.PrintWriter;
import java.nio.file.*;
import java.util.*;

public class DecompDefTransfers extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    String targetsPath = args[0];
    String outPath = args[1];

    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);

    List<String> lines = Files.readAllLines(Paths.get(targetsPath));
    int ok = 0, fail = 0, missing = 0;
    try (PrintWriter pw = new PrintWriter(outPath)) {
      for (String line : lines) {
        String s = line.trim();
        if (s.isEmpty() || s.startsWith("#")) continue;
        String[] cols = s.split("\t");
        if (cols.length < 2) continue;
        Address ad = toAddr(Long.parseLong(cols[0].replace("0x", ""), 16));
        String name = cols[1];
        Function f = getFunctionAt(ad);
        if (f == null) f = getFunctionContaining(ad);
        if (f == null) {
          pw.println("//=== MISSING " + name + " @ " + ad + " ===");
          missing++;
          continue;
        }
        DecompileResults res = di.decompileFunction(f, 90, monitor);
        pw.println("//=== " + name + " @ " + ad + " ===");
        if (res != null && res.decompileCompleted()
            && res.getDecompiledFunction() != null) {
          pw.println(res.getDecompiledFunction().getC());
          ok++;
        } else {
          pw.println("// <decompile failed: "
              + (res != null ? res.getErrorMessage() : "null") + ">");
          fail++;
        }
        pw.println();
        if ((ok + fail) % 25 == 0) {
          println("DecompDefTransfers: " + (ok + fail) + "/" + lines.size());
        }
      }
    }
    di.dispose();
    println("DecompDefTransfers: ok=" + ok + " fail=" + fail + " missing="
        + missing + " -> " + outPath);
  }
}
