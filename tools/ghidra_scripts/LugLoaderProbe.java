// Probe the .lug audio loader/playback path.
// Finds strings LHAudioBankSampleTable / LHAudioWaveData / LHFileSegmentBankInfo,
// prints their xrefs, and decompiles every function that references them (plus
// callers up one level). Output: one C file.
// Args: <outCFile>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.io.PrintWriter;
import java.util.*;

public class LugLoaderProbe extends GhidraScript {
  @Override public void run() throws Exception {
    String out = getScriptArgs()[0];
    String[] needles = {"LHAudioBankSampleTable","LHAudioWaveData",
        "LHFileSegmentBankInfo","LHAudioBankCriteiaInfo","adpcm","XBOX_ADPCM"};
    LinkedHashMap<Long,Function> targets = new LinkedHashMap<>();
    DataIterator it = currentProgram.getListing().getDefinedData(true);
    while (it.hasNext() && !monitor.isCancelled()) {
      Data d = it.next();
      Object v = d.getValue();
      if (!(v instanceof String)) continue;
      String s = (String) v;
      boolean m=false; for (String n: needles) if (s.contains(n)) m=true;
      if (!m) continue;
      Reference[] refs = getReferencesTo(d.getAddress());
      println("STR "+d.getAddress()+" = \""+s+"\" refs="+refs.length);
      for (Reference r: refs) {
        Function f = getFunctionContaining(r.getFromAddress());
        println("    <- "+r.getFromAddress()+(f!=null?" in "+f.getName()+" @"+f.getEntryPoint():" (nofunc)"));
        if (f!=null) {
          targets.put(f.getEntryPoint().getOffset(), f);
          // one level of callers
          Reference[] crefs = getReferencesTo(f.getEntryPoint());
          for (Reference cr: crefs) {
            Function cf = getFunctionContaining(cr.getFromAddress());
            if (cf!=null) targets.put(cf.getEntryPoint().getOffset(), cf);
          }
        }
      }
    }
    println("=== decompiling "+targets.size()+" functions ===");
    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);
    try (PrintWriter pw = new PrintWriter(out)) {
      for (Function f: targets.values()) {
        pw.println("//=== "+f.getName()+" @ "+f.getEntryPoint()+" ===");
        DecompileResults res = di.decompileFunction(f, 120, monitor);
        if (res!=null && res.decompileCompleted() && res.getDecompiledFunction()!=null)
          pw.println(res.getDecompiledFunction().getC());
        else
          pw.println("// <decompile failed: "+(res!=null?res.getErrorMessage():"null")+">");
        pw.println();
      }
    }
    di.dispose();
    println("wrote "+out);
  }
}
