// Probe the .lug record-consumer callback + audio decode path.
// Dumps the vtable at 012b4d3c (record consumer), decompiles the slot-0 fn,
// and searches for ADPCM/decode strings + functions reading the 652-byte record's
// format fields. Also decompiles FUN_00c0e9e0 (lookup insert) for record layout.
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

public class LugPlaybackProbe extends GhidraScript {
  DecompInterface di;
  PrintWriter pw;
  Set<Long> done = new HashSet<>();

  void dump(long addr) throws Exception {
    if (!done.add(addr)) return;
    Address a = toAddr(addr);
    Function f = getFunctionContaining(a);
    if (f==null) { pw.println("//=== no function at "+a+" ==="); return; }
    pw.println("//=== "+f.getName()+" @ "+f.getEntryPoint()+" ===");
    DecompileResults res = di.decompileFunction(f, 120, monitor);
    if (res!=null && res.decompileCompleted() && res.getDecompiledFunction()!=null)
      pw.println(res.getDecompiledFunction().getC());
    else pw.println("// <decompile failed>");
    pw.println();
  }

  @Override public void run() throws Exception {
    String out = getScriptArgs()[0];
    di = new DecompInterface();
    di.setSimplificationStyle("decompile");
    DecompileOptions opts = new DecompileOptions();
    opts.setMaxPayloadMBytes(256);
    di.setOptions(opts);
    di.openProgram(currentProgram);

    // read the vtable pointer at 012b4d3c (first slot = record consumer)
    long[] vt = {0x012b4d3cL, 0x012b4d34L, 0x012b4d40L};
    pw = new PrintWriter(out);
    for (long v: vt) {
      try {
        long slot0 = getInt(toAddr(v)) & 0xffffffffL;
        pw.println("// vtable "+Long.toHexString(v)+" slot0 -> "+Long.toHexString(slot0));
        dump(slot0);
      } catch (Exception e) { pw.println("// vt "+Long.toHexString(v)+" err "+e); }
    }
    // FUN_00c0e9e0 = lookup insert (record layout hint)
    dump(0x00c0e9e0L);

    // Search all strings for adpcm/decode/sound-buffer + list their functions
    String[] needles = {"adpcm","ADPCM","DirectSound","IDirectSoundBuffer","CreateSoundBuffer",
        "0069","SoundBuffer","voice","Voice","codec","decode","WaveFormat","WAVEFORMAT",
        "block align","BlockAlign","samples per","nBlockAlign"};
    DataIterator it = currentProgram.getListing().getDefinedData(true);
    int shown=0;
    while (it.hasNext() && !monitor.isCancelled() && shown<120) {
      Data d = it.next();
      Object val = d.getValue();
      if (!(val instanceof String)) continue;
      String s = (String) val;
      boolean m=false; for (String n: needles) if (s.contains(n)) m=true;
      if (!m) continue;
      shown++;
      Reference[] refs = getReferencesTo(d.getAddress());
      StringBuilder sb=new StringBuilder();
      for (Reference r: refs) {
        Function f=getFunctionContaining(r.getFromAddress());
        sb.append("  <-").append(r.getFromAddress()).append(f!=null?"/"+f.getName():"");
      }
      println("STR "+d.getAddress()+" = \""+(s.length()>50?s.substring(0,50):s)+"\" refs="+refs.length+sb);
    }
    pw.close();
    di.dispose();
    println("wrote "+out+" (strings shown="+shown+")");
  }
}
