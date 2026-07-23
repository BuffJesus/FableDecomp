// Bar #1 coverage sweep (docs/FULL_DECOMP.md): decompile EVERY function and
// record whether it produces clean pseudo-C. Output TSV columns:
//   address, name, named(0/1), size, thunk(0/1), ok(0/1), error
// Arg: output TSV path. Progress is printed every 1000 functions so the
// driver log shows liveness (the sweep takes hours on ~49k functions).
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.PrintWriter;
import java.io.FileWriter;

public class DecompCoverage extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    String outPath = args.length > 0 ? args[0]
        : "D:/Documents/FableTLC/ghidra_out/coverage.tsv";

    DecompInterface di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    di.openProgram(currentProgram);

    int total = 0, ok = 0, failed = 0, thunks = 0;
    PrintWriter out = new PrintWriter(new FileWriter(outPath));
    out.println("address\tname\tnamed\tsize\tthunk\tok\terror");

    FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
    while (it.hasNext() && !monitor.isCancelled()) {
      Function f = it.next();
      total++;
      String name = f.getName();
      boolean named = !(name.startsWith("FUN_") || name.startsWith("thunk_FUN_"));
      long size = f.getBody().getNumAddresses();
      boolean thunk = f.isThunk();
      if (thunk) thunks++;

      boolean good = false;
      String error = "";
      try {
        DecompileResults res = di.decompileFunction(f, 60, monitor);
        if (res != null && res.decompileCompleted()
            && res.getDecompiledFunction() != null
            && res.getDecompiledFunction().getC() != null) {
          good = true;
        } else {
          error = res != null ? String.valueOf(res.getErrorMessage()) : "null result";
        }
      } catch (Exception e) {
        error = e.getClass().getSimpleName() + ": " + e.getMessage();
      }
      if (good) ok++; else failed++;

      out.println(f.getEntryPoint() + "\t" + name + "\t" + (named ? 1 : 0) + "\t"
          + size + "\t" + (thunk ? 1 : 0) + "\t" + (good ? 1 : 0) + "\t"
          + error.replace('\t', ' ').replace('\n', ' '));

      if (total % 1000 == 0) {
        println("DecompCoverage progress: " + total + " done, " + ok + " ok, "
            + failed + " failed");
        out.flush();
      }
    }

    out.flush();
    out.close();
    di.dispose();
    println("DecompCoverage: total=" + total + " ok=" + ok + " failed=" + failed
        + " thunks=" + thunks + " -> " + outPath);
  }
}
