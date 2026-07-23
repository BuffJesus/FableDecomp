import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

public class CallTargets extends GhidraScript {
  @Override public void run() throws Exception {
    var fm = currentProgram.getFunctionManager();
    var listing = currentProgram.getListing();
    for (String a : getScriptArgs()) {
      Address ep = toAddr(Long.parseLong(a.replace("0x",""),16));
      Function f = fm.getFunctionContaining(ep);
      if (f==null){ println("no func "+a); continue; }
      println("==== CALLS IN "+f.getName()+" @ "+f.getEntryPoint()+" ====");
      InstructionIterator ii = listing.getInstructions(f.getBody(), true);
      while (ii.hasNext()) {
        Instruction ins = ii.next();
        if (ins.getMnemonicString().equals("CALL")) {
          Reference[] rs = ins.getReferencesFrom();
          String tgt="?";
          for (Reference r: rs){
            Function tf = fm.getFunctionAt(r.getToAddress());
            tgt = (tf!=null?tf.getName()+" @ "+r.getToAddress():r.getToAddress().toString());
          }
          println("  "+ins.getAddress()+"  CALL "+tgt);
        }
      }
    }
  }
}
