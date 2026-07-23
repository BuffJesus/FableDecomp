// Dump every function called by the target, with its signature, return type,
// calling convention, and parameter storage - to find a poisoned prototype
// that breaks the caller's decompilation ("Attribute size is not present").
// Arg: hex address of the caller.
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.data.DataType;
import java.util.Set;
import java.util.TreeSet;

public class DumpCallees extends GhidraScript {
  @Override public void run() throws Exception {
    Address ad = toAddr(Long.parseLong(getScriptArgs()[0].replace("0x", ""), 16));
    Function f = getFunctionAt(ad);
    if (f == null) { println("no function at " + ad); return; }
    Set<Function> callees = f.getCalledFunctions(monitor);
    println("DumpCallees: " + f.getName() + " calls " + callees.size() + " functions");
    Set<String> lines = new TreeSet<>();
    for (Function c : callees) {
      DataType ret = c.getReturnType();
      StringBuilder sb = new StringBuilder();
      sb.append(c.getEntryPoint()).append("  ").append(c.getName())
        .append("  ret=").append(ret != null ? ret.getName() + "(" + ret.getLength() + ")" : "null")
        .append("  cc=").append(c.getCallingConventionName());
      for (Parameter p : c.getParameters()) {
        DataType pt = p.getDataType();
        sb.append("  [").append(pt != null ? pt.getName() + "(" + pt.getLength() + ")" : "null")
          .append(" ").append(p.getVariableStorage()).append("]");
      }
      // Flag suspicious: zero/negative-length datatypes anywhere.
      boolean bad = (ret != null && ret.getLength() <= 0 && !"void".equals(ret.getName()));
      for (Parameter p : c.getParameters()) {
        DataType pt = p.getDataType();
        if (pt != null && pt.getLength() <= 0) bad = true;
      }
      lines.add((bad ? "SUSPECT " : "        ") + sb);
    }
    for (String s : lines) println(s);
  }
}
