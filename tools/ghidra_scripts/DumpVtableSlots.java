// DumpVtableSlots — locate the CGameScriptInterface vtable and dump slot->method.
// Strategy: (1) if a symbol whose name contains the class + "vftable"/"vtable" exists,
// use it; (2) else scan .rdata for the pointer run with maximum overlap against the
// known CGSI method-address set (loaded from a catalog TSV: col0=hexaddr).
//
// Args: <catalog-tsv> <out-tsv>
// Out: slot \t hexaddr \t functionName
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class DumpVtableSlots extends GhidraScript {
  @Override public void run() throws Exception {
    String[] a = getScriptArgs();
    String catalog = a[0];
    String out = a[1];
    var fm = currentProgram.getFunctionManager();
    var mem = currentProgram.getMemory();
    long imageBase = currentProgram.getImageBase().getOffset();

    // Load known CGSI method addresses.
    Set<Long> methodSet = new HashSet<>();
    try (BufferedReader br = new BufferedReader(new FileReader(catalog))) {
      String line;
      while ((line = br.readLine()) != null) {
        String h = line.split("\t")[0].trim();
        if (h.isEmpty()) continue;
        try { methodSet.add(Long.parseLong(h.replace("0x",""),16)); } catch (Exception e) {}
      }
    }
    println("DumpVtableSlots: loaded " + methodSet.size() + " catalog method addresses");

    // 1) Try a symbol.
    Address vtAddr = null;
    var st = currentProgram.getSymbolTable();
    for (SymbolIterator it = st.getAllSymbols(true); it.hasNext();) {
      Symbol s = it.next();
      String n = s.getName();
      if (n.contains("GameScriptInterface") && (n.contains("vftable") || n.contains("vtable"))
          && !s.getAddress().isMemoryAddress()==false) {
        vtAddr = s.getAddress(); println("symbol vtable candidate: " + n + " @ " + vtAddr); break;
      }
    }

    // 2) Scan for the best pointer-run overlapping the method set.
    if (vtAddr == null) {
      println("no vtable symbol; scanning .rdata for the CGSI pointer run...");
      Address best = null; int bestHits = 0;
      for (MemoryBlock b : mem.getBlocks()) {
        if (!b.isInitialized() || b.isExecute()) continue;
        Address start = b.getStart(), end = b.getEnd();
        for (Address p = start; p.compareTo(end) < 0; p = p.add(4)) {
          // count consecutive pointers from p that are in methodSet
          int hits = 0, slot = 0;
          Address q = p;
          while (q.compareTo(end) < 0 && slot < 1000) {
            long ptr;
            try { ptr = mem.getInt(q) & 0xFFFFFFFFL; } catch (Exception e) { break; }
            if (methodSet.contains(ptr)) hits++;
            else if (slot > 4 && hits*3 < slot) break; // bail if run goes cold early
            slot++; q = q.add(4);
            if (slot - hits > 40) break;
          }
          if (hits > bestHits) { bestHits = hits; best = p; }
          if (bestHits > 300) break;
        }
        if (bestHits > 300) break;
      }
      vtAddr = best;
      println("best pointer-run @ " + vtAddr + " with " + bestHits + " method hits");
    }
    if (vtAddr == null) { println("could not locate vtable"); return; }

    // Dump slots until a pointer no longer resolves to a function.
    PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)));
    int slot = 0, miss = 0;
    Address q = vtAddr;
    while (slot < 1100) {
      long ptr = mem.getInt(q) & 0xFFFFFFFFL;
      Address t = toAddr(ptr);
      Function f = fm.getFunctionAt(t);
      if (f == null) { miss++; if (miss > 3) break; }
      else miss = 0;
      w.println(slot + "\t" + String.format("%08x", ptr) + "\t" + (f != null ? f.getName() : "<nonfunc>"));
      slot++; q = q.add(4);
    }
    w.close();
    println("DumpVtableSlots: dumped " + slot + " slots from " + vtAddr + " -> " + out);
  }
}
