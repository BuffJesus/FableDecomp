// Scan the ENTIRE program data-type manager for composites/arrays/typedefs whose
// length is <= 0. A zero-length type anywhere can poison the decompiler with
// "Attribute size is not present" when it streams that type in. No args.
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;
import java.util.Iterator;

public class ScanZeroTypes extends GhidraScript {
  @Override public void run() throws Exception {
    DataTypeManager dtm = currentProgram.getDataTypeManager();
    Iterator<DataType> it = dtm.getAllDataTypes();
    int total = 0, bad = 0;
    while (it.hasNext()) {
      DataType dt = it.next();
      total++;
      int len;
      try { len = dt.getLength(); } catch (Exception e) { len = -99; }
      boolean interesting = (dt instanceof Composite) || (dt instanceof Array)
          || (dt instanceof TypeDef);
      if (interesting && len <= 0) {
        bad++;
        println("ZEROTYPE len=" + len + "  " + dt.getPathName()
            + "  (" + dt.getClass().getSimpleName() + ")");
        if (dt instanceof Composite) {
          Composite c = (Composite) dt;
          for (DataTypeComponent comp : c.getComponents()) {
            DataType ct = comp.getDataType();
            int cl; try { cl = ct.getLength(); } catch (Exception e) { cl = -99; }
            if (cl <= 0) println("    field " + comp.getFieldName() + " : "
                + ct.getPathName() + " len=" + cl);
          }
        }
      }
      if (total % 20000 == 0) println("ScanZeroTypes progress: " + total + " types, " + bad + " bad");
    }
    println("ScanZeroTypes: scanned " + total + " types, " + bad + " zero-length");
  }
}
