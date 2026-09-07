// List instructions whose scalar operands equal any requested hex value.
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.scalar.Scalar;
import java.util.*;

public class FindScalarOperand extends GhidraScript {
  @Override public void run() throws Exception {
    Set<Long> wanted = new HashSet<>();
    for (String arg : getScriptArgs())
      wanted.add(Long.parseUnsignedLong(arg.replace("0x", ""), 16));
    Set<String> emitted = new HashSet<>();
    var listing = currentProgram.getListing();
    var fm = currentProgram.getFunctionManager();
    var it = listing.getInstructions(true);
    while (it.hasNext() && !monitor.isCancelled()) {
      var ins = it.next();
      String rendered = ins.toString().toLowerCase();
      for (long value : wanted) {
        String needle = "0x" + Long.toHexString(value);
        if (rendered.contains(needle)) {
          Function f = fm.getFunctionContaining(ins.getAddress());
          String key = ins.getAddress().toString() + ":text:" + value;
          if (emitted.add(key))
            println(String.format("0x%X at %s in %s @ %s: %s", value,
              ins.getAddress(), f != null ? f.getName() : "?",
              f != null ? f.getEntryPoint() : "?", ins));
        }
      }
      for (int op = 0; op < ins.getNumOperands(); ++op) {
        for (Object obj : ins.getOpObjects(op)) {
          if (!(obj instanceof Scalar)) continue;
          long value = ((Scalar)obj).getUnsignedValue();
          if (!wanted.contains(value)) continue;
          Function f = fm.getFunctionContaining(ins.getAddress());
          String key = ins.getAddress().toString() + ":" + value;
          if (emitted.add(key))
            println(String.format("0x%X at %s in %s @ %s: %s", value,
              ins.getAddress(), f != null ? f.getName() : "?",
              f != null ? f.getEntryPoint() : "?", ins));
        }
      }
    }
  }
}
