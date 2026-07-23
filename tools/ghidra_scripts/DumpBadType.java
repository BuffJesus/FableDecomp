// Walk every data type referenced by a function's variables (params + locals),
// recursing through pointers/structs/arrays, and flag any component whose length
// is <= 0 - the likely source of a decompiler "Attribute size is not present".
// Arg: hex address of the function.
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Variable;
import ghidra.program.model.data.*;
import java.util.HashSet;
import java.util.Set;

public class DumpBadType extends GhidraScript {
  Set<String> seen = new HashSet<>();
  int bad = 0, visited = 0;

  void walk(DataType dt, int depth, String via) {
    if (dt == null || depth > 8) return;
    String key = dt.getPathName() + "@" + System.identityHashCode(dt);
    if (!seen.add(key)) return;
    visited++;
    int len = dt.getLength();
    if (len <= 0 && !(dt instanceof VoidDataType) && !(dt instanceof FunctionDefinition)) {
      println("BADTYPE len=" + len + "  " + dt.getPathName()
          + "  (" + dt.getClass().getSimpleName() + ")  via " + via);
      bad++;
    }
    if (dt instanceof Pointer) walk(((Pointer)dt).getDataType(), depth+1, via+"->*");
    else if (dt instanceof Array) walk(((Array)dt).getDataType(), depth+1, via+"[]");
    else if (dt instanceof TypeDef) walk(((TypeDef)dt).getDataType(), depth+1, via+"~td");
    else if (dt instanceof Composite) {
      Composite c = (Composite)dt;
      for (DataTypeComponent comp : c.getDefinedComponents()) {
        walk(comp.getDataType(), depth+1, dt.getName()+"."+comp.getFieldName());
      }
    }
  }

  @Override public void run() throws Exception {
    Address ad = toAddr(Long.parseLong(getScriptArgs()[0].replace("0x", ""), 16));
    Function f = getFunctionAt(ad);
    if (f == null) { println("no function at " + ad); return; }
    walk(f.getReturnType(), 0, "return");
    for (Variable v : f.getAllVariables()) {
      walk(v.getDataType(), 0, "var " + v.getName());
    }
    println("DumpBadType: visited " + visited + " types, " + bad + " bad in " + f.getName());
  }
}
