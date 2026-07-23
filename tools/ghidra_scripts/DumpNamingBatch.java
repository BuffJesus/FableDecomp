// DumpNamingBatch — export a naming corpus for still-unnamed (FUN_*) functions.
// For each default-named non-thunk function, emit a record with relational
// evidence (named callers, callees, string refs) + decompiled C, so an offline
// agent can propose an evidence-based name without holding the Ghidra lock.
//
// Ranks candidates by caller count (most-referenced = most valuable/nameable),
// takes the top N, writes a single delimited corpus file.
//
// Args: <out-path> <topN>
// Record format:
//   @@@FUNC <hexaddr> <size> <nCallers>
//   @@CALLERS <comma-list of named callers>
//   @@CALLEES <comma-list of callee names>
//   @@STRINGS <tab-list of referenced string literals>
//   @@DECOMP
//   <raw C>
//   @@ENDFUNC
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class DumpNamingBatch extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    String outPath = args.length > 0 ? args[0] : "D:\\Documents\\FableTLC\\ghidra_out\\naming_corpus.txt";
    int topN = args.length > 1 ? Integer.parseInt(args[1]) : 800;

    var fm = currentProgram.getFunctionManager();
    var rm = currentProgram.getReferenceManager();
    var listing = currentProgram.getListing();

    // Collect default-named, non-thunk candidates with caller counts.
    class Cand { Function f; int callers; List<String> namedCallers; }
    List<Cand> cands = new ArrayList<>();
    for (Function f : fm.getFunctions(true)) {
      String nm = f.getName();
      if (!nm.startsWith("FUN_")) continue;
      if (f.isThunk()) continue;
      Cand c = new Cand(); c.f = f; c.namedCallers = new ArrayList<>();
      Set<String> seen = new HashSet<>();
      int callers = 0;
      ReferenceIterator it = rm.getReferencesTo(f.getEntryPoint());
      while (it.hasNext()) {
        Reference r = it.next();
        if (!r.getReferenceType().isCall()) continue;
        callers++;
        Function cf = fm.getFunctionContaining(r.getFromAddress());
        if (cf != null) {
          String cn = cf.getName();
          if (!cn.startsWith("FUN_") && seen.add(cn)) c.namedCallers.add(cn);
        }
      }
      c.callers = callers;
      cands.add(c);
    }
    cands.sort((a,b) -> b.callers - a.callers);
    println("DumpNamingBatch: " + cands.size() + " FUN_* candidates; exporting top " + topN);

    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);

    PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(outPath)));
    int n = Math.min(topN, cands.size());
    for (int i = 0; i < n; i++) {
      Cand c = cands.get(i);
      Function f = c.f;
      // callees
      List<String> callees = new ArrayList<>();
      Set<String> cseen = new HashSet<>();
      for (Function ce : f.getCalledFunctions(monitor)) {
        String cn = ce.getName();
        if (cseen.add(cn)) callees.add(cn);
      }
      // string refs: walk instructions, collect referenced string data
      List<String> strs = new ArrayList<>();
      Set<String> sseen = new HashSet<>();
      AddressSetView body = f.getBody();
      InstructionIterator ii = listing.getInstructions(body, true);
      while (ii.hasNext()) {
        Instruction ins = ii.next();
        for (Reference r : ins.getReferencesFrom()) {
          if (!r.getReferenceType().isData()) continue;
          Data d = listing.getDataAt(r.getToAddress());
          if (d == null || !d.isDefined()) continue;
          DataType dt = d.getBaseDataType();
          if (dt instanceof AbstractStringDataType || (dt != null && dt.getName().toLowerCase().contains("string"))) {
            Object val = d.getValue();
            if (val != null) {
              String s = val.toString().replace("\n"," ").replace("\r"," ").replace("\t"," ");
              if (s.length() > 1 && sseen.add(s)) strs.add(s.length() > 120 ? s.substring(0,120) : s);
            }
          }
        }
      }

      w.println("@@@FUNC " + f.getEntryPoint() + " " + f.getBody().getNumAddresses() + " " + c.callers);
      w.println("@@CALLERS " + String.join(", ", c.namedCallers));
      w.println("@@CALLEES " + String.join(", ", callees));
      w.println("@@STRINGS " + String.join("\t", strs));
      w.println("@@DECOMP");
      DecompileResults res = di.decompileFunction(f, 60, monitor);
      if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null) {
        w.println(res.getDecompiledFunction().getC());
      } else {
        w.println("<decompile failed: " + (res != null ? res.getErrorMessage() : "null") + ">");
      }
      w.println("@@ENDFUNC");
      if ((i % 50) == 0) { println("  ..." + i + "/" + n); w.flush(); }
    }
    w.close();
    di.dispose();
    println("DumpNamingBatch: wrote " + n + " records to " + outPath);
  }
}
