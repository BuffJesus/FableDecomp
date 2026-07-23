import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

public class DisasmRange extends GhidraScript {
  @Override public void run() throws Exception {
    var listing = currentProgram.getListing();
    var fm = currentProgram.getFunctionManager();
    String[] args = getScriptArgs();
    Address start = toAddr(Long.parseLong(args[0].replace("0x",""),16));
    Address end   = toAddr(Long.parseLong(args[1].replace("0x",""),16));
    InstructionIterator ii = listing.getInstructions(start, true);
    while (ii.hasNext()){
      Instruction ins = ii.next();
      if (ins.getAddress().compareTo(end)>0) break;
      String tgt="";
      for (Reference r: ins.getReferencesFrom()){
        Function tf=fm.getFunctionAt(r.getToAddress());
        if(tf!=null) tgt=" -> "+tf.getName();
        else {
          Data d = listing.getDataAt(r.getToAddress());
          if(d!=null && d.hasStringValue()) tgt=" -> \""+d.getValue()+"\"";
        }
      }
      println(ins.getAddress()+"  "+ins.toString()+tgt);
    }
  }
}
