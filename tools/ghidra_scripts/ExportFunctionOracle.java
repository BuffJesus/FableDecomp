//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;

public class ExportFunctionOracle extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length < 2) {
      throw new IllegalArgumentException("usage: <output.tsv> <hex-address> [hex-address ...]");
    }
    File output = new File(args[0]);
    Memory memory = currentProgram.getMemory();
    try (BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
        new FileOutputStream(output), StandardCharsets.UTF_8))) {
      writer.write("address\tname\tlength\tbytes\n");
      for (int index = 1; index < args.length; ++index) {
        String text = args[index].replace("0x", "");
        Address address = toAddr(Long.parseLong(text, 16));
        Function function = getFunctionAt(address);
        if (function == null) {
          println("NO FUNC at " + address);
          continue;
        }
        long lengthLong = function.getBody().getNumAddresses();
        if (lengthLong > Integer.MAX_VALUE) {
          throw new IllegalStateException("function is too large: " + address);
        }
        byte[] bytes = new byte[(int) lengthLong];
        memory.getBytes(function.getEntryPoint(), bytes);
        StringBuilder hex = new StringBuilder(bytes.length * 2);
        for (byte value : bytes) {
          hex.append(String.format("%02x", value & 0xff));
        }
        String name = function.getName(true).replace('\t', ' ').replace('\n', ' ');
        writer.write(text.toLowerCase());
        writer.write('\t');
        writer.write(name);
        writer.write('\t');
        writer.write(Integer.toString(bytes.length));
        writer.write('\t');
        writer.write(hex.toString());
        writer.write('\n');
      }
    }
    println("WROTE oracle " + output.getAbsolutePath());
  }
}
