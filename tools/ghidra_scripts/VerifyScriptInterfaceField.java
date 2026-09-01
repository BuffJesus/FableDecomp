// Verify allocator storage of the CGameScriptInterface* argument (EDX) at object +0x40.
// Args: <conversion-queue-tsv> <output-tsv>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.HashSet;
import java.util.Set;

public class VerifyScriptInterfaceField extends GhidraScript {
  private Address stackDerivedInterfaceStore(Function function) {
    Set<String> stackRegisters = new HashSet<>();
    InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
    while (instructions.hasNext()) {
      Instruction instruction = instructions.next();
      String text = instruction.toString().replace(" ", "").toUpperCase();
      if (!text.startsWith("MOV")) continue;
      String operands = text.substring(3);
      int comma = operands.indexOf(',');
      if (comma < 0) continue;
      String destination = operands.substring(0, comma);
      String source = operands.substring(comma + 1);
      if (destination.matches("E(?:AX|BX|CX|DX|SI|DI|BP|SP)")) {
        if (source.startsWith("DWORDPTR[ESP+")) stackRegisters.add(destination);
        else if (source.matches("E(?:AX|BX|CX|DX|SI|DI|BP|SP)") && stackRegisters.contains(source))
          stackRegisters.add(destination);
        else stackRegisters.remove(destination);
      }
      if (destination.contains("+0X40]") && stackRegisters.contains(source))
        return instruction.getAddress();
    }
    return null;
  }

  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length != 2) throw new IllegalArgumentException("usage: <queue-tsv> <output-tsv>");
    int total = 0, verified = 0;
    try (BufferedReader input = new BufferedReader(new FileReader(args[0]));
         PrintWriter output = new PrintWriter(args[1])) {
      String header = input.readLine();
      String[] columns = header.split("\\t", -1);
      int nameColumn = -1, allocatorColumn = -1;
      for (int i = 0; i < columns.length; ++i) {
        if (columns[i].equals("name")) nameColumn = i;
        if (columns[i].equals("allocatorAddress")) allocatorColumn = i;
      }
      if (nameColumn < 0 || allocatorColumn < 0) throw new IllegalArgumentException("queue columns missing");
      output.println("script\tallocator_address\tinterface_store\tmethod\tverified");
      String line;
      while ((line = input.readLine()) != null) {
        String[] fields = line.split("\\t", -1);
        if (fields[allocatorColumn].isEmpty()) continue;
        ++total;
        Address address = toAddr(Long.parseLong(fields[allocatorColumn].replace("0x", ""), 16));
        Function allocator = getFunctionAt(address);
        Address store = null;
        String method = "";
        if (allocator != null) {
          Set<String> interfaceRegisters = new HashSet<>();
          interfaceRegisters.add("EDX");
          InstructionIterator instructions = currentProgram.getListing().getInstructions(allocator.getBody(), true);
          while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            String text = instruction.toString().replace(" ", "").toUpperCase();
            if (text.startsWith("MOV")) {
              String operands = text.substring(3);
              int comma = operands.indexOf(',');
              if (comma > 0) {
                String destination = operands.substring(0, comma);
                String source = operands.substring(comma + 1);
                if (destination.matches("E(?:AX|BX|CX|DX|SI|DI|BP|SP)") &&
                    source.matches("E(?:AX|BX|CX|DX|SI|DI|BP|SP)")) {
                  if (interfaceRegisters.contains(source)) interfaceRegisters.add(destination);
                  else interfaceRegisters.remove(destination);
                }
                if (destination.contains("+0X40]") && interfaceRegisters.contains(source)) {
                  store = instruction.getAddress();
                  method = "allocator-register-flow";
                  break;
                }
              }
            }
            if (instruction.getFlowType().isCall()) {
              boolean pushedInterface = false;
              Instruction previous = instruction.getPrevious();
              for (int back = 0; previous != null && back < 8; ++back, previous = previous.getPrevious()) {
                String prior = previous.toString().replace(" ", "").toUpperCase();
                if (prior.startsWith("PUSH") && interfaceRegisters.contains(prior.substring(4)))
                  pushedInterface = true;
              }
              if (pushedInterface) {
                for (Address target : instruction.getFlows()) {
                  Function called = getFunctionAt(target);
                  if (called == null) continue;
                  Address candidate = stackDerivedInterfaceStore(called);
                  if (candidate != null) {
                    store = candidate;
                    method = "constructor-stack-forwarding";
                    break;
                  }
                }
                if (store != null) break;
              }
            }
          }
        }
        if (store != null) ++verified;
        output.println(fields[nameColumn] + "\t" + address + "\t" +
                       (store == null ? "" : store.toString()) + "\t" + method + "\t" + (store != null));
      }
    }
    println("VerifyScriptInterfaceField: total=" + total + " verified=" + verified);
  }
}
