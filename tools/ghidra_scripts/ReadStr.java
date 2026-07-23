import ghidra.app.script.GhidraScript;
public class ReadStr extends GhidraScript {
  @Override public void run() throws Exception {
    for (String a : getScriptArgs()) {
      long addr = Long.parseLong(a.replace("0x",""),16);
      StringBuilder sb=new StringBuilder();
      byte[] b=new byte[64];
      currentProgram.getMemory().getBytes(toAddr(addr),b);
      for (byte x: b){ if(x==0) break; sb.append((x>=32&&x<127)?(char)x:'.'); }
      println("STR "+a+" = \""+sb+"\"");
    }
  }
}
