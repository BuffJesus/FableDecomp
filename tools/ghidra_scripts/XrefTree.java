// Recursively walk callers (references-to) of the given seed addresses up to a
// depth limit, printing the caller function name+address at each level. Used to
// trace the LZO decompressor up to the bank/resource/mesh load path.
// Args: <maxDepth> <seedHex1> [seedHex2 ...]
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.*;
import ghidra.program.model.listing.*;
import java.util.*;

public class XrefTree extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    int maxDepth = Integer.parseInt(a[0]);
    Set<Long> visited = new HashSet<>();
    // BFS frontier of function entry points
    Deque<long[]> q = new ArrayDeque<>(); // {entry, depth}
    for (int i = 1; i < a.length; i++) {
      long seed = Long.parseLong(a[i].replace("0x",""), 16);
      Address sa = toAddr(seed);
      Function sf = getFunctionContaining(sa);
      long entry = sf != null ? sf.getEntryPoint().getOffset() : seed;
      println("SEED " + Long.toHexString(seed) + " -> func " + (sf!=null?sf.getName()+"@"+Long.toHexString(entry):"?"));
      if (visited.add(entry)) q.add(new long[]{entry,0});
    }
    while (!q.isEmpty()) {
      long[] cur = q.poll();
      long entry = cur[0]; int depth = (int)cur[1];
      if (depth >= maxDepth) continue;
      Address ea = toAddr(entry);
      Function ef = getFunctionAt(ea);
      String ename = ef!=null?ef.getName():"?";
      ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(ea);
      Set<Long> callerEntries = new LinkedHashSet<>();
      while (it.hasNext()) {
        Reference r = it.next();
        if (!r.getReferenceType().isCall() && !r.getReferenceType().isJump() && !r.getReferenceType().isData()) continue;
        Function cf = getFunctionContaining(r.getFromAddress());
        if (cf == null) { println("  d"+depth+" "+ename+" <- (no func) "+r.getFromAddress()+" type="+r.getReferenceType()); continue; }
        long ce = cf.getEntryPoint().getOffset();
        if (callerEntries.add(ce)) {
          println("  d"+depth+" "+ename+"@"+Long.toHexString(entry)+" <- "+cf.getName()+"@"+Long.toHexString(ce)+" ("+r.getFromAddress()+" "+r.getReferenceType()+")");
          if (visited.add(ce)) q.add(new long[]{ce, depth+1});
        }
      }
    }
    println("XrefTree: visited "+visited.size()+" functions");
  }
}
