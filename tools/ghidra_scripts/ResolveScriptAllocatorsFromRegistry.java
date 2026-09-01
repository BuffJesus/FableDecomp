// Recover script allocator operands from CScriptManager_RegisterAllScripts.
// Args: <catalogTsv> <outputTsv>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import java.io.*;
import java.util.*;

public class ResolveScriptAllocatorsFromRegistry extends GhidraScript {
  private static String quotedString(Listing listing, Instruction ins) {
    for (Reference ref : ins.getReferencesFrom()) {
      Data data = listing.getDataAt(ref.getToAddress());
      if (data != null && data.hasStringValue()) return (String)data.getValue();
    }
    return null;
  }

  private static Address immediateAddress(Instruction ins) {
    Object[] objects = ins.getOpObjects(1);
    for (Object object : objects) {
      if (object instanceof Address) return (Address)object;
      if (object instanceof Scalar) {
        long value = ((Scalar)object).getUnsignedValue();
        return ins.getAddress().getAddressSpace().getAddress(value);
      }
    }
    return null;
  }

  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length != 2) throw new IllegalArgumentException("usage: <catalogTsv> <outputTsv>");

    LinkedHashSet<String> names = new LinkedHashSet<>();
    try (BufferedReader reader = new BufferedReader(new FileReader(args[0]))) {
      String line;
      boolean header = true;
      while ((line = reader.readLine()) != null) {
        if (header) { header = false; continue; }
        String[] fields = line.split("\\t", -1);
        if (fields.length > 0 && !fields[0].isEmpty()) names.add(fields[0]);
      }
    }

    Function registry = getGlobalFunctions("CScriptManager_RegisterAllScripts").get(0);
    Listing listing = currentProgram.getListing();
    Map<String, String[]> found = new LinkedHashMap<>();
    InstructionIterator all = listing.getInstructions(registry.getBody(), true);
    while (all.hasNext()) {
      Instruction stringUse = all.next();
      String name = quotedString(listing, stringUse);
      if (!names.contains(name) || found.containsKey(name)) continue;

      Instruction cursor = stringUse;
      for (int count = 0; count < 32; ++count) {
        cursor = cursor.getNext();
        if (cursor == null || !registry.getBody().contains(cursor.getAddress())) break;
        String text = cursor.toString();
        if (text.startsWith("CALL") && text.contains("0x00cb5c90")) break;
        // Most registrations store the allocator at ESP+20, but a shorter
        // LadyGrey block uses ESP+1c. The allocator is the first executable
        // immediate stored into the pending registration record.
        if (!text.startsWith("MOV dword ptr [ESP + ")) continue;
        Address allocator = immediateAddress(cursor);
        if (allocator == null || !currentProgram.getMemory().getExecuteSet().contains(allocator)) continue;
        found.put(name, new String[] { allocator.toString(), stringUse.getAddress().toString(),
                                      cursor.getAddress().toString() });
        break;
      }
    }

    try (PrintWriter out = new PrintWriter(args[1])) {
      out.println("quest_name\tallocator_address\tname_xref\tallocator_operand\tmethod");
      for (String name : names) {
        String[] row = found.get(name);
        if (row != null) out.println(name + "\t" + row[0] + "\t" + row[1] + "\t" + row[2] +
                                     "\tregistry-exact-string-to-stack-allocator");
      }
    }
    println("ResolveScriptAllocatorsFromRegistry: catalog=" + names.size() + " resolved=" + found.size());
  }
}
