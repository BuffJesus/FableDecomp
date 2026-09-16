// Typed variant of ExportScriptTranslationUnit: before decompiling, apply (in memory, read-only
// session) the native prototypes FSE proved for the engine — fixed helper functions by address,
// and per-call-site overrides for the game-script-interface (GSI) vtable slots and CScriptThing
// vtable slots, found by tracking register provenance from `this` (ECX at entry):
//   this+4 (entity) / this+0x40 (quest) -> GSI ; [GSI] -> GSI vtable ; call [vt + slot]
//   lea reg,[this+8] -> the entity's own CScriptThing ; [thing] -> thing vtable ; call [vt + slot]
// With prototypes the decompiler recovers every argument instead of printing `(**(code**)(..))()`.
// Usage: -postScript ExportTypedTranslationUnit.java <lo-hex> <hi-hex> <output-json> <define-list|-> <spec-json>
//@category FableTLC
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressRangeIterator;
import ghidra.program.model.data.*;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.*;
import ghidra.program.model.pcode.HighFunctionDBUtil;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;
import java.io.*;
import java.nio.file.Files;
import java.util.*;

public class ExportTypedTranslationUnit extends GhidraScript {
    private String json(String value) {
        if (value == null) return "null";
        return "\"" + value.replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t") + "\"";
    }
    private String hex(Address a) { return "0x" + a.toString().toUpperCase(); }
    private String stringAt(Address a) {
        Data d = getDataAt(a);
        if (d == null) d = getDataContaining(a);
        if (d == null) return null;
        Object v = d.getValue();
        if (v instanceof String) return (String) v;
        DataType t = d.getDataType();
        if (t != null && t.getName().toLowerCase().contains("string")) return d.getDefaultValueRepresentation();
        return null;
    }

    // ---------- typing ----------
    private DataTypeManager dtm;
    private Map<String, DataType> byValue = new HashMap<>();
    private Map<String, FunctionDefinitionDataType> slotDefs = new HashMap<>();
    private Map<String, FunctionDefinitionDataType> thingDefs = new HashMap<>();
    private int overrides = 0, helpersTyped = 0;

    private DataType type(String name) {
        switch (name) {
            case "void": return VoidDataType.dataType;
            case "bool": return BooleanDataType.dataType;
            case "int": return IntegerDataType.dataType;
            case "uint": return UnsignedIntegerDataType.dataType;
            case "float": return FloatDataType.dataType;
            case "double": return DoubleDataType.dataType;
            case "ulonglong": return UnsignedLongLongDataType.dataType;
            case "void *": return new PointerDataType(VoidDataType.dataType);
        }
        if (name.endsWith(" *")) return new PointerDataType(type(name.substring(0, name.length() - 2)));
        DataType bv = byValue.get(name);
        return bv != null ? bv : IntegerDataType.dataType;
    }

    private FunctionDefinitionDataType definition(String name, JsonObject spec, String cc) {
        FunctionDefinitionDataType def = new FunctionDefinitionDataType(name);
        def.setReturnType(type(spec.get("ret").getAsString()));
        List<ParameterDefinition> params = new ArrayList<>();
        // Ghidra's __thiscall model declares `this` explicitly as the first parameter (bound to ECX).
        if (cc.equals("__thiscall")) params.add(new ParameterDefinitionImpl("this", new PointerDataType(VoidDataType.dataType), null));
        for (JsonElement p : spec.getAsJsonArray("params")) {
            JsonObject po = p.getAsJsonObject();
            params.add(new ParameterDefinitionImpl(po.get("name").getAsString(), type(po.get("type").getAsString()), null));
        }
        def.setArguments(params.toArray(new ParameterDefinition[0]));
        try { def.setCallingConvention(cc); } catch (Exception e) { println("cc " + cc + " rejected: " + e); }
        return def;
    }

    private void loadSpec(File specFile) throws Exception {
        JsonObject spec = JsonParser.parseString(Files.readString(specFile.toPath())).getAsJsonObject();
        dtm = currentProgram.getDataTypeManager();
        for (Map.Entry<String, JsonElement> e : spec.getAsJsonObject("byValue").entrySet()) {
            StructureDataType st = new StructureDataType(e.getKey() + "_bv", e.getValue().getAsInt());
            byValue.put(e.getKey(), st);
        }
        for (Map.Entry<String, JsonElement> e : spec.getAsJsonObject("slots").entrySet()) {
            JsonObject so = e.getValue().getAsJsonObject();
            slotDefs.put(e.getKey(), definition("GSI_" + so.get("name").getAsString(), so, "__thiscall"));
        }
        for (Map.Entry<String, JsonElement> e : spec.getAsJsonObject("thingSlots").entrySet()) {
            JsonObject so = e.getValue().getAsJsonObject();
            thingDefs.put(e.getKey(), definition("Thing_" + so.get("name").getAsString(), so, "__thiscall"));
        }
        for (Map.Entry<String, JsonElement> e : spec.getAsJsonObject("helpers").entrySet()) {
            JsonObject ho = e.getValue().getAsJsonObject();
            Address a = toAddr(Long.parseLong(e.getKey().substring(2), 16));
            Function f = getFunctionAt(a);
            if (f == null) {
                Function containing = getFunctionContaining(a);
                if (containing != null) { println("HELPER " + a + " " + ho.get("name").getAsString() + " is inside " + containing.getName() + "; skipped"); continue; }
                if (getInstructionAt(a) == null) disassemble(a);
                f = createFunction(a, ho.get("name").getAsString());
                if (f == null) { println("HELPER " + a + " could not be defined"); continue; }
            }
            String cc = ho.get("cc").getAsString();
            FunctionDefinitionDataType def = definition(ho.get("name").getAsString(), ho, cc);
            try {
                List<ParameterImpl> params = new ArrayList<>();
                for (ParameterDefinition pd : def.getArguments())
                    params.add(new ParameterImpl(pd.getName(), pd.getDataType(), currentProgram));
                f.updateFunction(cc, new ReturnParameterImpl(def.getReturnType(), currentProgram), params,
                    Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.USER_DEFINED);
                helpersTyped++;
            } catch (Exception ex) { println("HELPER " + a + " signature rejected: " + ex); }
        }
        println("Typing spec: " + slotDefs.size() + " GSI slots, " + thingDefs.size() + " thing slots, " + helpersTyped + " helpers typed");
    }

    // register/stack provenance tags
    private static final String THIS = "THIS", GSI = "GSI", GSIVT = "GSIVT", PARENT = "PARENT", ME = "ME", MEVT = "MEVT";

    private String regName(Object o) { return (o instanceof Register) ? ((Register) o).getName() : null; }

    private void overrideCalls(Function f) {
        Map<String, String> tags = new HashMap<>();
        tags.put("ECX", THIS);
        InstructionIterator ins = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ins.hasNext()) {
            Instruction i = ins.next();
            String mn = i.getMnemonicString().toUpperCase();
            int n = i.getNumOperands();
            if (mn.equals("CALL")) {
                if (n == 1) {
                    Object[] ops = i.getOpObjects(0);
                    String base = ops.length >= 1 ? regName(ops[0]) : null;
                    long disp = 0;
                    for (Object o : ops) if (o instanceof Scalar) disp = ((Scalar) o).getSignedValue();
                    if (base != null && i.getOperandType(0) != ghidra.program.model.lang.OperandType.REGISTER) {
                        String tag = tags.get(base);
                        String key = "0x" + Long.toHexString(disp);
                        FunctionDefinitionDataType def = null;
                        if (GSIVT.equals(tag)) def = slotDefs.get(key);
                        else if (MEVT.equals(tag)) def = thingDefs.get(key);
                        if (def != null) {
                            try { HighFunctionDBUtil.writeOverride(f, i.getAddress(), def); overrides++; }
                            catch (Exception ex) { println("override failed at " + i.getAddress() + ": " + ex); }
                        }
                    }
                }
                tags.remove("EAX"); tags.remove("ECX"); tags.remove("EDX");
                continue;
            }
            if (n == 2 && (mn.equals("MOV") || mn.equals("LEA"))) {
                Object[] dst = i.getOpObjects(0), src = i.getOpObjects(1);
                boolean dstReg = i.getOperandType(0) == ghidra.program.model.lang.OperandType.REGISTER;
                boolean srcReg = i.getOperandType(1) == ghidra.program.model.lang.OperandType.REGISTER;
                String d = dst.length >= 1 ? regName(dst[0]) : null;
                String s = src.length >= 1 ? regName(src[0]) : null;
                long disp = 0; boolean hasDisp = false;
                for (Object o : src) if (o instanceof Scalar) { disp = ((Scalar) o).getSignedValue(); hasDisp = true; }
                if (dstReg && d != null) {
                    String tag = null;
                    if (mn.equals("MOV") && srcReg && s != null) tag = tags.get(s);
                    else if (mn.equals("MOV") && s != null) {
                        String st = tags.get(s);
                        if (s.equals("ESP") || s.equals("EBP")) tag = tags.get("STK" + s + ":" + disp);
                        else if (THIS.equals(st) && !hasDisp) tag = null;
                        else if (THIS.equals(st) && (disp == 4 || disp == 0x40)) tag = GSI;
                        else if (THIS.equals(st) && disp == 0x14) tag = PARENT;
                        else if (PARENT.equals(st) && disp == 0x40) tag = GSI;
                        else if (GSI.equals(st) && !hasDisp) tag = GSIVT;
                        else if (ME.equals(st) && !hasDisp) tag = MEVT;
                    } else if (mn.equals("LEA") && s != null && THIS.equals(tags.get(s)) && disp == 8) tag = ME;
                    if (tag != null) tags.put(d, tag); else tags.remove(d);
                } else if (mn.equals("MOV") && d != null && (d.equals("ESP") || d.equals("EBP")) && srcReg && s != null) {
                    long ddisp = 0;
                    for (Object o : dst) if (o instanceof Scalar) ddisp = ((Scalar) o).getSignedValue();
                    String tag = tags.get(s);
                    if (tag != null) tags.put("STK" + d + ":" + ddisp, tag); else tags.remove("STK" + d + ":" + ddisp);
                }
                continue;
            }
            // any other instruction writing a register invalidates it (conservative)
            for (int k = 0; k < n; k++) {
                if (i.getOperandType(k) == ghidra.program.model.lang.OperandType.REGISTER && k == 0
                        && !mn.equals("PUSH") && !mn.equals("CMP") && !mn.equals("TEST")) {
                    Object[] ops = i.getOpObjects(k);
                    if (ops.length >= 1 && regName(ops[0]) != null) tags.remove(regName(ops[0]));
                }
            }
            if (mn.equals("POP") && n == 1) { Object[] ops = i.getOpObjects(0); if (ops.length >= 1 && regName(ops[0]) != null) tags.remove(regName(ops[0])); }
        }
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 5) throw new IllegalArgumentException("expected lo hi output-json define-list|- spec-json");
        java.util.LinkedHashSet<Long> extra = new java.util.LinkedHashSet<>();
        if (!args[3].equals("-")) {
            for (String line : Files.readAllLines(new File(args[3]).toPath())) {
                String t = line.trim();
                if (t.isEmpty() || t.startsWith("#")) continue;
                long a = Long.parseLong(t.split("\\s+")[0].replace("0x", ""), 16);
                Address addr = toAddr(a);
                if (getFunctionAt(addr) == null) {
                    if (getInstructionAt(addr) == null) disassemble(addr);
                    Function created = createFunction(addr, null);
                    println("DEFINE " + addr + " -> " + (created == null ? "FAILED" : created.getName()));
                }
                extra.add(a);
            }
        }
        loadSpec(new File(args[4]));
        long lo = Long.parseLong(args[0].replace("0x", ""), 16);
        long hi = Long.parseLong(args[1].replace("0x", ""), 16);
        File outputFile = new File(args[2]);
        var fm = currentProgram.getFunctionManager();
        var rm = currentProgram.getReferenceManager();
        List<Function> selected = new ArrayList<>();
        FunctionIterator it = fm.getFunctions(toAddr(lo), true);
        while (it.hasNext()) {
            Function f = it.next();
            if (f.getEntryPoint().getOffset() >= hi) break;
            selected.add(f);
        }
        for (long a : extra) {
            Function f = getFunctionAt(toAddr(a));
            if (f != null && !selected.contains(f)) selected.add(f);
        }
        for (Function f : selected) {
            try { f.setCallingConvention("__thiscall"); } catch (Exception e) { }
            overrideCalls(f);
        }
        println("Call-site overrides written: " + overrides);
        DecompInterface decompiler = new DecompInterface();
        decompiler.setSimplificationStyle("decompile");
        decompiler.openProgram(currentProgram);
        List<String> rows = new ArrayList<>();
        int count = 0;
        for (Function f : selected) {
            count++;
            List<String> calls = new ArrayList<>();
            List<String> strings = new ArrayList<>();
            List<String> immediates = new ArrayList<>();
            List<String> bodyRanges = new ArrayList<>();
            AddressRangeIterator bodyRangeIterator = f.getBody().getAddressRanges();
            while (bodyRangeIterator.hasNext()) {
                AddressRange range = bodyRangeIterator.next();
                bodyRanges.add("{\"start\":" + json(hex(range.getMinAddress())) +
                    ",\"endExclusive\":" + json(hex(range.getMaxAddress().add(1))) +
                    ",\"addressCount\":" + range.getLength() + "}");
            }
            InstructionIterator ins = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ins.hasNext()) {
                Instruction i = ins.next();
                if (i.getFlowType().isCall()) {
                    for (Address target : i.getFlows()) {
                        Function called = getFunctionAt(target);
                        if (called == null) called = getFunctionContaining(target);
                        calls.add("{\"site\":" + json(hex(i.getAddress())) + ",\"target\":" + json(hex(target)) +
                            ",\"currentName\":" + json(called == null ? null : called.getName(true)) + "}");
                    }
                }
                for (Reference r : i.getReferencesFrom()) {
                    Address to = r.getToAddress();
                    if (r.getReferenceType().isData() || r.getReferenceType() == RefType.DATA) {
                        String s = stringAt(to);
                        if (s != null) {
                            strings.add("{\"site\":" + json(hex(i.getAddress())) + ",\"address\":" + json(hex(to)) + ",\"value\":" + json(s) + "}");
                        } else {
                            Function pointee = getFunctionAt(to);
                            immediates.add("{\"site\":" + json(hex(i.getAddress())) + ",\"address\":" + json(hex(to)) +
                                ",\"function\":" + json(pointee == null ? null : pointee.getName(true)) + "}");
                        }
                    }
                }
            }
            List<String> pointedFrom = new ArrayList<>();
            List<String> callers = new ArrayList<>();
            ReferenceIterator refs = rm.getReferencesTo(f.getEntryPoint());
            while (refs.hasNext()) {
                Reference r = refs.next();
                Address from = r.getFromAddress();
                Function cf = fm.getFunctionContaining(from);
                if (r.getReferenceType().isCall()) {
                    callers.add("{\"site\":" + json(hex(from)) + ",\"function\":" + json(cf == null ? null : cf.getName(true)) +
                        ",\"functionAddress\":" + json(cf == null ? null : hex(cf.getEntryPoint())) + "}");
                } else {
                    pointedFrom.add("{\"site\":" + json(hex(from)) + ",\"inFunction\":" + json(cf == null ? null : hex(cf.getEntryPoint())) + "}");
                }
            }
            String body;
            DecompileResults res = decompiler.decompileFunction(f, 120, monitor);
            if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null)
                body = res.getDecompiledFunction().getC();
            else body = null;
            long bodyExtent = f.getBody().getMaxAddress().subtract(f.getBody().getMinAddress()) + 1;
            rows.add("{\"address\":" + json(hex(f.getEntryPoint())) +
                ",\"size\":" + f.getBody().getNumAddresses() +
                ",\"bodyAddressCount\":" + f.getBody().getNumAddresses() +
                ",\"bodyMin\":" + json(hex(f.getBody().getMinAddress())) +
                ",\"bodyMaxInclusive\":" + json(hex(f.getBody().getMaxAddress())) +
                ",\"bodyEndExclusive\":" + json(hex(f.getBody().getMaxAddress().add(1))) +
                ",\"bodyExtent\":" + bodyExtent +
                ",\"bodyRanges\":[" + String.join(",", bodyRanges) + "]" +
                ",\"currentName\":" + json(f.getName(true)) +
                ",\"calls\":[" + String.join(",", calls) + "]" +
                ",\"strings\":[" + String.join(",", strings) + "]" +
                ",\"immediates\":[" + String.join(",", immediates) + "]" +
                ",\"callers\":[" + String.join(",", callers) + "]" +
                ",\"pointedFrom\":[" + String.join(",", pointedFrom) + "]" +
                ",\"decompile\":" + json(body) + "}");
            println("TU " + hex(f.getEntryPoint()) + " " + f.getName(true));
        }
        try (PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(outputFile)))) {
            w.println("{\"schema\":\"fable-script-translation-unit/0.3-typed\",\"program\":" + json(currentProgram.getName()) +
                ",\"range\":[" + json(args[0]) + "," + json(args[1]) + "],\"functionCount\":" + count +
                ",\"typing\":{\"overrides\":" + overrides + ",\"helpers\":" + helpersTyped + "}" +
                ",\"functions\":[" + String.join(",\n", rows) + "]}");
        }
        println("ExportTypedTranslationUnit: " + count + " functions, " + overrides + " overrides -> " + outputFile);
    }
}
