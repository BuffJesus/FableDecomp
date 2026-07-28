// Dump selected enum definitions from the current program's data-type archive.
// Usage: DumpEnums.java <output.tsv> <exact-or-substring> [...]

import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeManager;

import java.io.PrintWriter;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.Set;

public class DumpEnums extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            throw new IllegalArgumentException(
                "usage: <output.tsv> <exact-or-substring> [...]");
        }

        Set<String> needles = new LinkedHashSet<>();
        for (int index = 1; index < args.length; ++index) {
            needles.add(args[index].toLowerCase());
        }

        int enumCount = 0;
        try (PrintWriter output = new PrintWriter(args[0], "UTF-8")) {
            output.println("enum\tlength\tmember\tvalue_hex\tvalue_signed");
            DataTypeManager manager = currentProgram.getDataTypeManager();
            Iterator<DataType> types = manager.getAllDataTypes();
            while (types.hasNext() && !monitor.isCancelled()) {
                DataType type = types.next();
                if (!(type instanceof ghidra.program.model.data.Enum)) {
                    continue;
                }
                String name = type.getName();
                String lowerName = name.toLowerCase();
                boolean selected = false;
                for (String needle : needles) {
                    if (lowerName.contains(needle)) {
                        selected = true;
                        break;
                    }
                }
                if (!selected) {
                    continue;
                }
                ghidra.program.model.data.Enum enumType =
                    (ghidra.program.model.data.Enum) type;
                for (String member : enumType.getNames()) {
                    long value = enumType.getValue(member);
                    output.printf(
                        "%s\t%d\t%s\t0x%x\t%d%n",
                        name,
                        enumType.getLength(),
                        member,
                        value,
                        value);
                }
                ++enumCount;
            }
        }
        println("DumpEnums wrote " + enumCount + " enums -> " + args[0]);
    }
}
