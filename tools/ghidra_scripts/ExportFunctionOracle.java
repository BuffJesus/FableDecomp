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
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;

public class ExportFunctionOracle extends GhidraScript {
  @Override public void run() throws Exception {
    String[] args = getScriptArgs();
    if (args.length < 2) {
      throw new IllegalArgumentException(
          "usage: <output.tsv> <hex-address> [hex-address ...] | <output.tsv> @<address-file>");
    }
    File output = new File(args[0]);
    List<String> addressTexts = new ArrayList<>();
    if (args.length == 2 && args[1].startsWith("@")) {
      File addressFile = new File(args[1].substring(1));
      for (String line : Files.readAllLines(addressFile.toPath(), StandardCharsets.UTF_8)) {
        String text = line.trim();
        if (!text.isEmpty() && !text.startsWith("#")) {
          addressTexts.add(text);
        }
      }
    }
    else {
      for (int index = 1; index < args.length; ++index) {
        addressTexts.add(args[index]);
      }
    }
    if (addressTexts.isEmpty()) {
      throw new IllegalArgumentException("address list is empty");
    }

    Memory memory = currentProgram.getMemory();
    try (BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
        new FileOutputStream(output), StandardCharsets.UTF_8))) {
      writer.write("address\tname\tlength\tbytes\n");
      for (String addressText : addressTexts) {
        String text = addressText.replace("0x", "");
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
