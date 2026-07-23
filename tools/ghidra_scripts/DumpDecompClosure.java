// DumpDecompClosure — export decompilation for a set of seed functions PLUS
// their direct callees (1 level), so unnamed decode/skin helpers are captured
// for offline analysis. Same record format as DumpNamingBatch.
//
// Args: <out-path> <hexaddr> [<hexaddr> ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class DumpDecompClosure extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    String out = a[0];
    var fm = currentProgram.getFunctionManager();
    // Build the closure: seeds + their callees.
    LinkedHashSet<Function> set = new LinkedHashSet<>();
    List<Function> seeds = new ArrayList<>();
    for (int i = 1; i < a.length; i++) {
      Address ad = toAddr(Long.parseLong(a[i].replace("0x",""),16));
      Function f = fm.getFunctionContaining(ad);
      if (f != null) { seeds.add(f); set.add(f); }
    }
    for (Function s : seeds) {
      for (Function ce : s.getCalledFunctions(monitor)) set.add(ce);
    }
    println("DumpDecompClosure: " + seeds.size() + " seeds -> " + set.size() + " functions");

    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);
    var rm = currentProgram.getReferenceManager();
    PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)));
    for (Function f : set) {
      // named callers
      List<String> callers = new ArrayList<>(); Set<String> cs = new HashSet<>();
      ReferenceIterator it = rm.getReferencesTo(f.getEntryPoint());
      while (it.hasNext()) {
        Reference r = it.next(); if (!r.getReferenceType().isCall()) continue;
        Function cf = fm.getFunctionContaining(r.getFromAddress());
        if (cf != null && cs.add(cf.getName())) callers.add(cf.getName());
      }
      List<String> callees = new ArrayList<>(); Set<String> es = new HashSet<>();
      for (Function ce : f.getCalledFunctions(monitor)) if (es.add(ce.getName())) callees.add(ce.getName());
      w.println("@@@FUNC " + f.getEntryPoint() + " " + f.getBody().getNumAddresses() + " " + callers.size());
      w.println("@@NAME " + f.getName());
      w.println("@@CALLERS " + String.join(", ", callers));
      w.println("@@CALLEES " + String.join(", ", callees));
      w.println("@@DECOMP");
      DecompileResults res = di.decompileFunction(f, 90, monitor);
      if (res != null && res.decompileCompleted() && res.getDecompiledFunction()!=null)
        w.println(res.getDecompiledFunction().getC());
      else w.println("<decompile failed>");
      w.println("@@ENDFUNC");
      w.flush();
    }
    w.close(); di.dispose();
    println("DumpDecompClosure: wrote " + set.size() + " records to " + out);
  }
}
