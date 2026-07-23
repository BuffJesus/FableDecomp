import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
public class DisasmFuncs extends GhidraScript {
  @Override public void run() throws Exception {
    for (String a : getScriptArgs()) {
      Address ad = toAddr(Long.parseLong(a.replace("0x",""),16));
      Function f = getFunctionContaining(ad);
      Address end = f!=null? f.getBody().getMaxAddress(): ad.add(0x400);
      println("==================== "+a+" ("+(f!=null?f.getName():"?")+") ====================");
      Listing l = currentProgram.getListing();
      InstructionIterator it = l.getInstructions(ad, true);
      int n=0;
      while(it.hasNext() && n<900){
        Instruction ins=it.next();
        if(ins.getAddress().compareTo(end)>0) break;
        println(ins.getAddress()+"  "+ins.toString());
        n++;
      }
    }
  }
}
