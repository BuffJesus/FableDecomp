// Decompile one (huge) function and dump pseudo-C slices around given addresses,
// plus the helper functions called within each slice window.
// Args: <hexFuncAddr> <outSlicesFile> <outFullCFile> <name> <hexAddr> ...
// (alternating name/addr pairs — cmd.exe batch arg parsing splits on '=',
//  so name=addr syntax silently becomes two args under analyzeHeadless.bat)
// Usage: analyzeHeadless <proj> FableTLC -process Fable.exe -noanalysis \
//   -scriptPath tools/ghidra_scripts -postScript DumpVerbSlices.java \
//   0xCBFB7D ghidra_out/cutscene_verb_slices.md ghidra_out/runcutscenemacro_full.c \
//   UseCamera 0xcc9f39 GamePause 0xcc88d1 ...
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.io.PrintWriter;
import java.util.*;

public class DumpVerbSlices extends GhidraScript {
  private static final int LINES_BEFORE = 12;
  private static final int LINES_AFTER = 90;

  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    Address fa = toAddr(Long.parseLong(args[0].replace("0x", ""), 16));
    String outSlices = args[1];
    String outFull = args[2];
    Function f = getFunctionAt(fa);
    if (f == null) { println("DumpVerbSlices: no function at " + fa); return; }

    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions options = new DecompileOptions();
    options.setMaxPayloadMBytes(512); // RunCutsceneMacro_Func overflows the 50MB default
    di.setOptions(options);
    di.openProgram(currentProgram);
    long t0 = System.currentTimeMillis();
    DecompileResults res = di.decompileFunction(f, 3600, monitor);
    long ms = System.currentTimeMillis() - t0;
    if (res == null || !res.decompileCompleted() || res.getCCodeMarkup() == null) {
      println("DumpVerbSlices: decompile FAILED in " + ms + "ms err="
          + (res != null ? res.getErrorMessage() : "null"));
      di.dispose();
      return;
    }
    println("DumpVerbSlices: decompiled " + f.getName() + " in " + ms + "ms");

    try (PrintWriter pw = new PrintWriter(outFull)) {
      pw.print(res.getDecompiledFunction().getC());
    }

    PrettyPrinter pp = new PrettyPrinter(f, res.getCCodeMarkup(), null);
    List<ClangLine> lines = pp.getLines();
    println("DumpVerbSlices: " + lines.size() + " pseudo-C lines");

    // Per-line minimum address for locating targets.
    Address[] lineAddr = new Address[lines.size()];
    for (int i = 0; i < lines.size(); i++) {
      Address min = null;
      for (ClangToken tok : lines.get(i).getAllTokens()) {
        Address a = tok.getMinAddress();
        if (a != null && (min == null || a.compareTo(min) < 0)) min = a;
      }
      lineAddr[i] = min;
    }

    try (PrintWriter pw = new PrintWriter(outSlices)) {
      pw.println("# " + f.getName() + " @ " + fa + " — verb handler slices");
      pw.println();
      for (int ai = 3; ai + 1 < args.length; ai += 2) {
        String verb = args[ai];
        Address target = toAddr(Long.parseLong(args[ai + 1].replace("0x", ""), 16));
        // Best line: exact token address match, else nearest by distance.
        int best = -1; long bestDist = Long.MAX_VALUE;
        for (int i = 0; i < lines.size(); i++) {
          for (ClangToken tok : lines.get(i).getAllTokens()) {
            Address a = tok.getMinAddress();
            if (a == null) continue;
            long d = Math.abs(a.subtract(target));
            if (d < bestDist) { bestDist = d; best = i; }
            if (d == 0) break;
          }
          if (bestDist == 0) break;
        }
        pw.println("## " + verb + " (xref " + target + ", line "
            + (best >= 0 ? lines.get(best).getLineNumber() : -1)
            + ", dist " + bestDist + ")");
        pw.println();
        if (best < 0) { pw.println("NOT FOUND"); continue; }
        int lo = Math.max(0, best - LINES_BEFORE);
        int hi = Math.min(lines.size() - 1, best + LINES_AFTER);
        TreeMap<String, String> helpers = new TreeMap<>();
        pw.println("```c");
        for (int i = lo; i <= hi; i++) {
          ClangLine ln = lines.get(i);
          StringBuilder sb = new StringBuilder();
          for (ClangToken tok : ln.getAllTokens()) {
            sb.append(tok.getText());
            if (tok instanceof ClangFuncNameToken) {
              Address ca = null;
              for (ClangToken t2 : ln.getAllTokens()) {
                if (t2 == tok) { ca = tok.getMinAddress(); break; }
              }
              helpers.put(tok.getText(), ca != null ? ca.toString() : "?");
            }
          }
          String mark = (i == best) ? ">>>" : "   ";
          pw.printf("%s %6d %s | %s%n", mark, ln.getLineNumber(),
              lineAddr[i] != null ? lineAddr[i].toString() : "        ", sb.toString());
        }
        pw.println("```");
        pw.println();
        pw.println("Helpers called in window:");
        for (Map.Entry<String, String> e : helpers.entrySet()) {
          pw.println("- `" + e.getKey() + "` (callsite " + e.getValue() + ")");
        }
        pw.println();
        println("SLICE " + verb + " line=" + lines.get(best).getLineNumber()
            + " dist=" + bestDist + " helpers=" + helpers.size());
      }
    }
    di.dispose();
    println("DumpVerbSlices: wrote " + outSlices + " and " + outFull);
  }
}
