// DumpStructLayouts — dump all composite (struct/class) data types with member
// offsets/types from a PDB-symbolized program, for generating typed headers
// (fable_types.h) that back injection + decomp.
//
// Arg: <out-tsv>. Emits one line per member:
//   <StructName> \t <structSize> \t <memberOffset> \t <memberType> \t <memberName>
// plus a header line per struct: @STRUCT <name> <size> <numComponents>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class DumpStructLayouts extends GhidraScript {
  @Override public void run() throws Exception {
    String out = getScriptArgs().length > 0 ? getScriptArgs()[0]
        : "D:\\Documents\\FableTLC\\ghidra_out\\struct_layouts.tsv";
    DataTypeManager dtm = currentProgram.getDataTypeManager();
    PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)));
    Iterator<Composite> it = dtm.getAllComposites();
    int nStruct = 0; long nMember = 0;
    while (it.hasNext()) {
      Composite c = it.next();
      if (!(c instanceof Structure)) continue;   // structs/classes only, skip unions
      Structure s = (Structure) c;
      DataTypeComponent[] comps = s.getDefinedComponents();
      w.println("@STRUCT\t" + s.getName() + "\t" + s.getLength() + "\t" + comps.length);
      for (DataTypeComponent m : comps) {
        DataType mt = m.getDataType();
        String mn = m.getFieldName();
        w.println(s.getName() + "\t" + s.getLength() + "\t" + m.getOffset()
            + "\t" + (mt != null ? mt.getName() : "?") + "\t" + (mn != null ? mn : ""));
        nMember++;
      }
      nStruct++;
    }
    w.close();
    println("DumpStructLayouts: " + nStruct + " structs, " + nMember + " members -> " + out);
  }
}
