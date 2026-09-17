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
    // CScriptThing copy constructor (retail 0x4ABE90): `sub esp,0xc; mov ecx,esp; push src; call ctor` builds a
    // by-value CScriptThing argument in the outgoing area. FSE's typedefs spell those params `CScriptThing *`.
    private static final long THING_COPY_CTOR = 0x4ABE90L;
    private int byValueSites = 0;
    private Map<String, FunctionDefinitionDataType> byValueDefs = new HashMap<>();
    // per call site: entry-relative stack slots loaded with `lea reg, [esp/ebp + X]` into ECX/EDX or pushed
    // (Ghidra's stack-variable naming drifts after callee-cleaned vtable calls; the lowering restores them)
    private Map<Long, String> siteStackOperands = new HashMap<>();

    /** A copy of def whose first `count` CScriptThing* parameters become 12-byte by-value structs. */
    private FunctionDefinitionDataType withByValueThings(FunctionDefinitionDataType def, int count) {
        String key = def.getName() + "#bv" + count;
        FunctionDefinitionDataType cached = byValueDefs.get(key);
        if (cached != null) return cached;
        FunctionDefinitionDataType copy = new FunctionDefinitionDataType(def.getName() + "_bv" + count);
        copy.setReturnType(def.getReturnType());
        ParameterDefinition[] src = def.getArguments();
        List<ParameterDefinition> params = new ArrayList<>();
        int left = count;
        for (ParameterDefinition pd : src) {
            DataType t = pd.getDataType();
            boolean thingPtr = t instanceof Pointer && ((Pointer) t).getDataType() == byValue.get("CScriptThing");
            if (left > 0 && thingPtr && !pd.getName().equals("this")) { params.add(new ParameterDefinitionImpl(pd.getName(), byValue.get("CScriptThing"), null)); left--; }
            else params.add(pd);
        }
        if (left != 0) return null;   // more by-value constructions than CScriptThing* params: leave the site alone
        copy.setArguments(params.toArray(new ParameterDefinition[0]));
        try { copy.setCallingConvention(def.getCallingConventionName()); } catch (Exception e) { }
        byValueDefs.put(key, copy);
        return copy;
    }

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
            case "float10": return Float10DataType.dataType;
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
            if (ho.get("cc").getAsString().equals("__cdecl")) cdeclTargets.add(a.getOffset());
            Function f = getFunctionAt(a);
            if (f == null) {
                Function containing = getFunctionContaining(a);
                if (getInstructionAt(a) == null) disassemble(a);
                // FSE's proven entry points may sit inside a larger Ghidra function (fall-through code
                // shared by several script helpers). Splitting it here is in-memory only.
                f = createFunction(a, ho.get("name").getAsString());
                if (f == null) { println("HELPER " + a + " could not be defined" + (containing != null ? " (inside " + containing.getName() + ")" : "")); continue; }
                if (containing != null) println("HELPER " + a + " " + ho.get("name").getAsString() + " split out of " + containing.getName());
            }
            String cc = ho.get("cc").getAsString();
            FunctionDefinitionDataType def = definition(ho.get("name").getAsString(), ho, cc);
            try {
                // a parameter with an explicit `storage` register (the x87 operand of __ftol2 in ST0) needs
                // custom storage for the whole signature; the result then stays in EAX by hand
                boolean custom = false;
                for (JsonElement pe : ho.getAsJsonArray("params")) if (pe.getAsJsonObject().has("storage")) custom = true;
                List<ParameterImpl> params = new ArrayList<>();
                int k = 0;
                for (ParameterDefinition pd : def.getArguments()) {
                    JsonObject po = cc.equals("__thiscall") && k == 0 ? null : ho.getAsJsonArray("params").get(cc.equals("__thiscall") ? k - 1 : k).getAsJsonObject();
                    if (custom && po != null && po.has("storage"))
                        params.add(new ParameterImpl(pd.getName(), pd.getDataType(), new VariableStorage(currentProgram, currentProgram.getRegister(po.get("storage").getAsString())), currentProgram));
                    else if (custom)
                        params.add(new ParameterImpl(pd.getName(), pd.getDataType(), new VariableStorage(currentProgram, currentProgram.getRegister("ECX")), currentProgram));
                    else
                        params.add(new ParameterImpl(pd.getName(), pd.getDataType(), currentProgram));
                    k++;
                }
                ReturnParameterImpl ret = custom && def.getReturnType().getLength() > 0
                    ? new ReturnParameterImpl(def.getReturnType(), new VariableStorage(currentProgram, currentProgram.getRegister("EAX")), currentProgram)
                    : new ReturnParameterImpl(def.getReturnType(), currentProgram);
                f.updateFunction(cc, ret, params,
                    custom ? Function.FunctionUpdateType.CUSTOM_STORAGE : Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.USER_DEFINED);
                helpersTyped++;
            } catch (Exception ex) { println("HELPER " + a + " signature rejected: " + ex); }
        }
        cdeclTargets.add(0xBFEA1AL); cdeclTargets.add(0xBFE9BCL); cdeclTargets.add(0xBFEA0EL); cdeclTargets.add(0xBFEA14L);
        println("Typing spec: " + slotDefs.size() + " GSI slots, " + thingDefs.size() + " thing slots, " + helpersTyped + " helpers typed");
    }

    // register/stack provenance tags
    private static final String THIS = "THIS", GSI = "GSI", GSIVT = "GSIVT", PARENT = "PARENT", ME = "ME", MEVT = "MEVT";
    private static final String MEMPTR = "MEMPTR", MEMPTRVT = "MEMPTRVT";
    private int dataSites = 0, paramThings = 0;

    private Map<Long, Long> retPurgeCache = new HashMap<>();
    /** Immediate of the first RET reached by a linear scan from the callee entry (0 for a plain ret, -1 if none within 4 KB). */
    private long retPurge(long target) {
        if (retPurgeCache.containsKey(target)) return retPurgeCache.get(target);
        long result = -1;
        Instruction x = getInstructionAt(toAddr(target));
        for (int n = 0; x != null && n < 2000; n++) {
            if (x.getMnemonicString().equalsIgnoreCase("RET")) {
                result = 0;
                if (x.getNumOperands() == 1 && x.getOpObjects(0).length == 1 && x.getOpObjects(0)[0] instanceof Scalar)
                    result = ((Scalar) x.getOpObjects(0)[0]).getUnsignedValue();
                break;
            }
            if (x.getMnemonicString().equalsIgnoreCase("JMP") && x.getNumOperands() == 1 && x.getOpObjects(0).length == 1) {
                if (!(x.getOpObjects(0)[0] instanceof Address)) break;                 // indirect jump: unknown
                x = getInstructionAt((Address) x.getOpObjects(0)[0]);                  // tail-call thunk: the target's ret decides
                continue;
            }
            if (x.getMnemonicString().equalsIgnoreCase("INT3")) break;                  // padding: the body ended without a ret
            x = x.getNext();
        }
        retPurgeCache.put(target, result);
        return result;
    }

    private boolean isRegOperand(Instruction i, int k) {
        int t = i.getOperandType(k);
        // the type is a flag set (a register written by LEA carries REGISTER|ADDRESS); memory operands are DYNAMIC
        return ghidra.program.model.lang.OperandType.isRegister(t) && !ghidra.program.model.lang.OperandType.isDynamic(t)
            && i.getOpObjects(k).length == 1 && i.getOpObjects(k)[0] instanceof Register;
    }

    private String regName(Object o) { return (o instanceof Register) ? ((Register) o).getName() : null; }

    private java.util.Set<Long> cdeclTargets = new java.util.HashSet<>();

    private String stackKey(String base, long disp, long espDelta) {
        // ESP-relative displacements move with pushes; key slots by their frame offset instead.
        return base.equals("ESP") ? ("STK:" + (disp - espDelta)) : ("BP:" + disp);   // entry-relative: esp_now = esp_entry - espDelta
    }

    private Map<Long, Long> sitePurge = new HashMap<>();   // call site -> exact purge (-1 = unknown), from the linear pass

    /** Flow-sensitive stack depth per instruction (CFG worklist over PUSH/POP/SUB/ADD ESP/CALL purge/RET/JMP/Jcc). */
    private Map<Long, Long> flowDepths(Function f) {
        Map<Long, Long> depthAt = new HashMap<>();
        java.util.ArrayDeque<Object[]> work = new java.util.ArrayDeque<>();
        work.add(new Object[]{f.getEntryPoint(), 0L});
        int guard = 0;
        while (!work.isEmpty() && guard++ < 200000) {
            Object[] item = work.poll();
            Address a = (Address) item[0];
            long depth = (Long) item[1], before = depth;
            boolean pushing = false;
            Instruction x = getInstructionAt(a);
            while (x != null && f.getBody().contains(x.getAddress())) {
                long off = x.getAddress().getOffset();
                if (depthAt.containsKey(off)) break;
                depthAt.put(off, depth);
                String mn = x.getMnemonicString().toUpperCase();
                int n = x.getNumOperands();
                if (mn.equals("PUSH")) { if (!pushing) { before = depth; pushing = true; } depth += 4; }
                else if (mn.equals("POP")) depth -= 4;
                else if ((mn.equals("SUB") || mn.equals("ADD")) && n == 2 && x.getOpObjects(0).length >= 1 && "ESP".equals(regName(x.getOpObjects(0)[0])) && x.getOpObjects(1).length >= 1 && x.getOpObjects(1)[0] instanceof Scalar) {
                    long v = ((Scalar) x.getOpObjects(1)[0]).getSignedValue();
                    depth += mn.equals("SUB") ? v : -v;
                    if (mn.equals("ADD")) pushing = false;
                }
                else if (mn.equals("LEA") && n == 2 && isRegOperand(x, 0) && "ESP".equals(regName(x.getOpObjects(0)[0]))) {
                    long d2 = 0; boolean esp = false;
                    for (Object o : x.getOpObjects(1)) { if (o instanceof Scalar) d2 = ((Scalar) o).getSignedValue(); if (o instanceof Register && "ESP".equals(((Register) o).getName())) esp = true; }
                    if (esp) depth -= d2;
                }
                else if (mn.equals("MOV") && n == 2 && isRegOperand(x, 0) && "ESP".equals(regName(x.getOpObjects(0)[0]))) { depth = 0; pushing = false; }
                else if (mn.equals("CALL")) {
                    long purge = sitePurge.getOrDefault(off, -1L);
                    if (purge == -2) depth -= 4;                                   // thing copy constructor: its own push
                    else if (purge >= 0) depth -= purge;
                    else if (pushing) depth = before;
                    if (purge != -2) pushing = false;
                }
                else if (mn.equals("RET")) break;
                else if (mn.startsWith("J")) {
                    // every resolved destination (direct target, or the switch-table cases Ghidra recovered)
                    for (Address to : x.getFlows()) if (f.getBody().contains(to)) work.add(new Object[]{to, depth});
                    if (mn.equals("JMP")) break;
                }
                x = x.getNext();
            }
        }
        return depthAt;
    }

    private void overrideCalls(Function f) { overrideCalls(f, null); }

    private void overrideCalls(Function f, Map<Long, Long> depthAt) {
        Map<String, String> tags = new HashMap<>();
        tags.put("ECX", THIS);
        // CScriptThing parameters (typed from the PDB / ego_r signature) live at entry-relative stack slots
        for (Parameter prm : f.getParameters()) {
            DataType t = prm.getDataType();
            boolean thingPtr = t instanceof Pointer && ((Pointer) t).getDataType().getName().startsWith("CScriptThing");
            if (thingPtr && prm.isStackVariable()) { tags.put("STK:" + prm.getStackOffset(), ME); paramThings++; println("thing param " + prm.getName() + " @STK:" + prm.getStackOffset() + " in " + f.getName()); }
            else if (thingPtr) println("param " + prm.getName() + " of " + f.getName() + " is a thing but not stack-resident: " + prm.getVariableStorage());
        }
        long espDelta = 0, espBeforePushes = 0, argStart = -1;
        boolean pushing = false;
        int byValuePending = 0;
        Map<String, Long> regStack = new HashMap<>();      // register -> entry-relative stack slot it points at
        Map<String, Long> regValue = new HashMap<>();      // register -> entry-relative stack slot it was loaded from (`mov reg,[esp+X]`)
        List<Long> pushedValue = new ArrayList<>();        // per push: the slot whose value was pushed, or null
        java.util.Set<String> written = new java.util.HashSet<>();   // registers assigned so far (a callee-saved push before any write is a save)
        // callee-saved registers restored by the epilogue(s): VC7.1 pushes them lazily anywhere in the body
        java.util.Set<String> savedRegs = new java.util.HashSet<>(), savedSeen = new java.util.HashSet<>();
        {
            InstructionIterator scan = currentProgram.getListing().getInstructions(f.getBody(), true);
            java.util.List<String> run = new ArrayList<>();
            while (scan.hasNext()) {
                Instruction x = scan.next();
                String xm = x.getMnemonicString().toUpperCase();
                if (xm.equals("POP") && x.getNumOperands() == 1 && x.getOpObjects(0).length >= 1 && regName(x.getOpObjects(0)[0]) != null) { run.add(regName(x.getOpObjects(0)[0])); continue; }
                if (xm.equals("RET")) { savedRegs.addAll(run); run.clear(); continue; }
                if (xm.equals("ADD") && x.getNumOperands() == 2 && x.getOpObjects(0).length >= 1 && "ESP".equals(regName(x.getOpObjects(0)[0]))) continue;
                run.clear();
            }
        }
        List<Long> pushedStack = new ArrayList<>();        // pushed stack-slot addresses since the last call (push order)
        Instruction prev = null;
        InstructionIterator ins = currentProgram.getListing().getInstructions(f.getBody(), true);
        long bodyDelta = 0;   // depth before a (possibly mid-function) epilogue: restored after its RET
        // depth at the source of every forward jump: a block entered only by a jump (the fallthrough after
        // an unconditional JMP or a RET) starts at the depth its jumper had, not at the previous block's
        Map<Long, Long> jumpDepth = new HashMap<>();
        // register state at the source of every forward jump: a block entered by a jump sees the jumper's
        // registers, not those of the unrelated block that happens to precede it in memory
        Map<Long, Object[]> jumpState = new HashMap<>();
        boolean blockBoundary = false;
        while (ins.hasNext()) {
            Instruction i = ins.next();
            String mn = i.getMnemonicString().toUpperCase();
            int n = i.getNumOperands();
            if (depthAt != null) {
                Long flow = depthAt.get(i.getAddress().getOffset());
                if (flow != null && flow != espDelta) { espDelta = flow; if (!pushing) { argStart = -1; } }
            }
            if (blockBoundary) {
                Long known = jumpDepth.get(i.getAddress().getOffset());
                if (known != null) { espDelta = known; pushing = false; argStart = -1; }
                Object[] st = jumpState.get(i.getAddress().getOffset());
                if (st != null) {
                    tags.clear(); tags.putAll((Map<String, String>) st[0]);
                    regStack.clear(); regStack.putAll((Map<String, Long>) st[1]);
                    regValue.clear(); regValue.putAll((Map<String, Long>) st[2]);
                }
                blockBoundary = false;
            }
            if (mn.startsWith("J") && n == 1 && i.getOpObjects(0).length == 1 && i.getOpObjects(0)[0] instanceof Address) {
                long to = ((Address) i.getOpObjects(0)[0]).getOffset();
                if (to > i.getAddress().getOffset()) {
                    jumpDepth.putIfAbsent(to, espDelta);
                    if (!jumpState.containsKey(to)) jumpState.put(to, new Object[]{new HashMap<>(tags), new HashMap<>(regStack), new HashMap<>(regValue)});
                }
                if (mn.equals("JMP")) { if (pushing) espDelta = espBeforePushes; pushing = false; argStart = -1; blockBoundary = true; }   // pending pushes travel with the jump, not into the fallthrough
                continue;
            }
            if (mn.equals("RET")) { espDelta = bodyDelta; pushing = false; argStart = -1; blockBoundary = true; continue; }
            boolean epilogueOp = mn.equals("POP") || ((mn.equals("ADD")) && n == 2 && i.getOpObjects(0).length >= 1 && "ESP".equals(regName(i.getOpObjects(0)[0])));
            if (!epilogueOp) bodyDelta = espDelta;
            if (mn.equals("PUSH")) {
                Object[] pops = i.getOpObjects(0);
                String pr = pops.length >= 1 && isRegOperand(i, 0) ? regName(pops[0]) : null;
                boolean prologueSave = pr != null && savedRegs.contains(pr) && !savedSeen.contains(pr) && !written.contains(pr);
                if (prologueSave) { savedSeen.add(pr); espDelta += 4; prev = i; continue; }   // callee-saved register, not an argument
                if (!pushing) { espBeforePushes = espDelta; pushing = true; pushedStack.clear(); pushedValue.clear(); }
                pushedStack.add(pr != null && regStack.containsKey(pr) ? regStack.get(pr) : null);
                // `push dword ptr [esp+X]` / `push reg` loaded from a slot: the slot's value travels
                Long pv = pr != null && regValue.containsKey(pr) ? regValue.get(pr) : null;
                if (pr == null && pops.length >= 1 && "ESP".equals(regName(pops[0]))) {
                    long pd = 0; for (Object o : pops) if (o instanceof Scalar) pd = ((Scalar) o).getSignedValue();
                    pv = pd - espDelta;
                }
                pushedValue.add(pv);
                espDelta += 4;
                prev = i;
                continue;
            }
            if (mn.equals("POP") && n == 1) {
                espDelta -= 4;
                Object[] ops = i.getOpObjects(0);
                String r = ops.length >= 1 ? regName(ops[0]) : null;
                // callee-saved registers are only popped in (possibly mid-function) epilogues; the
                // linear scan continues into blocks where they still hold their tagged values
                if (r != null && !r.equals("ESI") && !r.equals("EDI") && !r.equals("EBX") && !r.equals("EBP")) tags.remove(r);
                continue;
            }
            if ((mn.equals("SUB") || mn.equals("ADD")) && n == 2) {
                Object[] d0 = i.getOpObjects(0), s1 = i.getOpObjects(1);
                if (d0.length >= 1 && "ESP".equals(regName(d0[0])) && s1.length >= 1 && s1[0] instanceof Scalar) {
                    long v = ((Scalar) s1[0]).getSignedValue();
                    // `sub esp, N` immediately followed by `mov ecx, esp` reserves a by-value argument slot
                    Instruction nx = i.getNext();
                    boolean reserve = mn.equals("SUB") && nx != null && nx.getMnemonicString().equalsIgnoreCase("MOV")
                        && nx.getNumOperands() == 2 && nx.getOpObjects(1).length == 1 && "ESP".equals(regName(nx.getOpObjects(1)[0]));
                    if (reserve && argStart < 0) argStart = pushing ? espBeforePushes : espDelta;   // the argument area began with any pushes just before
                    espDelta += mn.equals("SUB") ? v : -v;
                    prev = i;
                    continue;
                }
            }
            if (mn.equals("CALL")) {
                long target = -1;
                if (n == 1 && i.getOpObjects(0).length == 1 && i.getOpObjects(0)[0] instanceof Address)
                    target = ((Address) i.getOpObjects(0)[0]).getOffset();
                long purge = -1;   // bytes the callee pops (exact when known; -1 = fall back to the push heuristic)
                boolean siteReturnsThing = false;
                if (target >= 0 && cdeclTargets.contains(target)) purge = 0;          // the caller's `add esp, N` follows
                if (target >= 0 && !cdeclTargets.contains(target)) {
                    // the callee's own `ret N` is the ground truth (Ghidra's purge size can be 0 when two bodies were merged)
                    long ret = retPurge(target);
                    if (ret >= 0) purge = ret;
                    else {
                        Function callee = getFunctionAt(toAddr(target));
                        if (callee != null && callee.getStackPurgeSize() >= 0 && callee.getStackPurgeSize() <= 0x40) purge = callee.getStackPurgeSize();
                    }
                }
                if (n == 1) {
                    Object[] ops = i.getOpObjects(0);
                    String base = ops.length >= 1 ? regName(ops[0]) : null;
                    long disp = 0;
                    for (Object o : ops) if (o instanceof Scalar) disp = ((Scalar) o).getSignedValue();
                    if (base != null && !isRegOperand(i, 0)) {
                        String tag = tags.get(base);
                        String key = "0x" + Long.toHexString(disp);
                        FunctionDefinitionDataType def = null;
                        if (GSIVT.equals(tag)) def = slotDefs.get(key);
                        else if (MEVT.equals(tag)) def = thingDefs.get(key);
                        // a vtable fetched through an untagged pointer loaded from an object member: the Data
                        // pointer of a CScriptThing (same slot layout); only the large thing-only slot numbers
                        else if (MEMPTRVT.equals(tag) && disp >= 0x40 && thingDefs.containsKey(key)) { def = thingDefs.get(key); dataSites++; }
                        if (def != null && byValuePending > 0) {
                            FunctionDefinitionDataType bv = withByValueThings(def, byValuePending);
                            if (bv != null) { def = bv; byValueSites++; }
                            else println("by-value mismatch at " + i.getAddress() + ": " + byValuePending + " constructions for " + def.getName());
                        }
                        if (def != null) {
                            try { HighFunctionDBUtil.writeOverride(f, i.getAddress(), def); overrides++; }
                            catch (Exception ex) { println("override failed at " + i.getAddress() + ": " + ex); }
                            // __thiscall slot: every parameter after `this` is on the stack (by-value CScriptThing = 12 bytes)
                            long bytes = 0;
                            ParameterDefinition[] pds = def.getArguments();
                            for (int q = 1; q < pds.length; q++) {
                                DataType pt = pds[q].getDataType();
                                bytes += (pt instanceof Pointer || pt.getLength() <= 0) ? 4 : ((pt.getLength() + 3) / 4) * 4;
                            }
                            purge = bytes;
                            DataType rt = def.getReturnType();
                            siteReturnsThing = (rt instanceof Pointer && ((Pointer) rt).getDataType().getName().startsWith("CScriptThing"))
                                || rt.getName().startsWith("CScriptThing");
                        }
                    }
                }
                {
                    StringBuilder sb = new StringBuilder();
                    sb.append("\"depth\":").append(espDelta).append(",");
                    if (regValue.containsKey("ECX")) sb.append("\"ecxValue\":").append(regValue.get("ECX")).append(",");
                    if (regValue.containsKey("EDX")) sb.append("\"edxValue\":").append(regValue.get("EDX")).append(",");
                    if (!pushedValue.isEmpty() && pushedValue.stream().anyMatch(v -> v != null)) {
                        sb.append("\"pushedValue\":[");
                        for (int q = 0; q < pushedValue.size(); q++) { if (q > 0) sb.append(","); sb.append(pushedValue.get(q) == null ? "null" : String.valueOf(pushedValue.get(q))); }
                        sb.append("],");
                    }
                    if (regStack.containsKey("ECX")) sb.append("\"ecxStack\":").append(regStack.get("ECX")).append(",");
                    if (regStack.containsKey("EDX")) sb.append("\"edxStack\":").append(regStack.get("EDX")).append(",");
                    if (!pushedStack.isEmpty()) {
                        sb.append("\"pushedStack\":[");
                        for (int q = 0; q < pushedStack.size(); q++) { if (q > 0) sb.append(","); sb.append(pushedStack.get(q) == null ? "null" : String.valueOf(pushedStack.get(q))); }
                        sb.append("],");
                    }
                    if (sb.length() > 0) siteStackOperands.put(i.getAddress().getOffset(), sb.toString());
                    pushedStack.clear(); pushedValue.clear();
                }
                regStack.remove("EAX"); regStack.remove("ECX"); regStack.remove("EDX");
                regValue.remove("EAX"); regValue.remove("ECX"); regValue.remove("EDX");
                tags.remove("EAX"); tags.remove("ECX"); tags.remove("EDX");
                if (siteReturnsThing) tags.put("EAX", ME);   // a CScriptThing result: its vtable calls resolve through thingDefs
                sitePurge.put(i.getAddress().getOffset(), target == THING_COPY_CTOR ? -2L : purge);
                if (target == THING_COPY_CTOR) {
                    // the copy constructor only pops its own `push src` (ret 4); the by-value slot and any earlier
                    // argument pushes stay reserved for the real call
                    espDelta -= 4;
                    byValuePending++;
                    prev = i;
                    continue;
                }
                // callee-cleaned conventions pop their arguments; cdecl callers restore ESP themselves
                if (purge >= 0) espDelta -= purge;
                else if (!cdeclTargets.contains(target)) { if (argStart >= 0) espDelta = argStart; else if (pushing) espDelta = espBeforePushes; }
                pushing = false;
                argStart = -1;
                byValuePending = 0;
                prev = i;
                continue;
            }
            // an argument sequence is `push; lea/mov; push; ...; call`: instructions that do not touch ESP keep it open
            if (n == 2 && (mn.equals("MOV") || mn.equals("LEA"))) {
                Object[] dst = i.getOpObjects(0), src = i.getOpObjects(1);
                boolean dstReg = isRegOperand(i, 0);
                boolean srcReg = isRegOperand(i, 1);
                String d = dst.length >= 1 ? regName(dst[0]) : null;
                String sName = src.length >= 1 ? regName(src[0]) : null;
                if (sName == null) for (Object o : src) if (o instanceof Register) { sName = ((Register) o).getName(); break; }   // operand objects are not always base-first
                long disp = 0; boolean hasDisp = false;
                for (Object o : src) if (o instanceof Scalar) { disp = ((Scalar) o).getSignedValue(); hasDisp = true; }
                if (dstReg && d != null) {
                    written.add(d);
                    String tag = null;
                    boolean gsiGlobal = false;
                    for (Object o : src) if (o instanceof Address && ((Address) o).getOffset() == 0x143e8f8L) gsiGlobal = true;
                    if (mn.equals("MOV") && gsiGlobal && sName == null) tag = GSI;   // g_ScriptInterface singleton
                    else if (mn.equals("MOV") && srcReg && sName != null) tag = tags.get(sName);
                    else if (mn.equals("MOV") && sName != null) {
                        String st = tags.get(sName);
                        if (sName.equals("ESP") || sName.equals("EBP")) tag = tags.get(stackKey(sName, disp, espDelta));
                        else if (THIS.equals(st) && !hasDisp) tag = null;
                        else if (THIS.equals(st) && (disp == 4 || disp == 0x40)) tag = GSI;
                        else if (THIS.equals(st) && disp == 0x14) tag = PARENT;
                        else if (PARENT.equals(st) && disp == 0x40) tag = GSI;
                        else if (GSI.equals(st) && !hasDisp) tag = GSIVT;
                        else if (ME.equals(st) && !hasDisp) tag = MEVT;
                        else if (MEMPTR.equals(st) && !hasDisp) tag = MEMPTRVT;
                        else if (st == null && hasDisp && disp >= 4 && !sName.equals("ESP") && !sName.equals("EBP")) tag = MEMPTR;
                    } else if (mn.equals("LEA") && sName != null && THIS.equals(tags.get(sName)) && disp == 8) tag = ME;
                    if (tag != null) tags.put(d, tag); else tags.remove(d);
                    int srcRegs = 0;
                    for (Object o : src) if (o instanceof Register) srcRegs++;
                    if (mn.equals("LEA") && sName != null && (sName.equals("ESP") || sName.equals("EBP")) && srcRegs == 1)
                        regStack.put(d, sName.equals("ESP") ? disp - espDelta : disp);
                    else if (mn.equals("MOV") && srcReg && sName != null && regStack.containsKey(sName)) regStack.put(d, regStack.get(sName));
                    else regStack.remove(d);
                    if (mn.equals("MOV") && !srcReg && sName != null && (sName.equals("ESP") || sName.equals("EBP")) && srcRegs == 1)
                        regValue.put(d, sName.equals("ESP") ? disp - espDelta : disp);
                    else if (mn.equals("MOV") && srcReg && sName != null && regValue.containsKey(sName)) regValue.put(d, regValue.get(sName));
                    else regValue.remove(d);
                    if (d.equals("ESP") && mn.equals("LEA") && "ESP".equals(sName)) espDelta -= disp;   // `lea esp,[esp+N]` (N = 0 is padding)
                    else if (d.equals("ESP")) { espDelta = 0; pushing = false; argStart = -1; }
                } else if (mn.equals("MOV") && d != null && (d.equals("ESP") || d.equals("EBP")) && srcReg && sName != null) {
                    long ddisp = 0;
                    for (Object o : dst) if (o instanceof Scalar) ddisp = ((Scalar) o).getSignedValue();
                    String tag = tags.get(sName);
                    String key = stackKey(d, ddisp, espDelta);
                    if (tag != null) tags.put(key, tag); else tags.remove(key);
                }
                continue;
            }
            for (int k = 0; k < n; k++) {
                if (isRegOperand(i, k) && k == 0
                        && !mn.equals("CMP") && !mn.equals("TEST")) {
                    Object[] ops = i.getOpObjects(k);
                    if (ops.length >= 1 && regName(ops[0]) != null) { tags.remove(regName(ops[0])); regStack.remove(regName(ops[0])); regValue.remove(regName(ops[0])); written.add(regName(ops[0])); }
                }
            }
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
            overrideCalls(f);                           // pass 1: tags, prototypes, purges (linear)
            int o1 = overrides, b1 = byValueSites, d1 = dataSites, p1 = paramThings;
            overrideCalls(f, flowDepths(f));            // pass 2: the same, with flow-sensitive depths for every slot
            overrides = o1; byValueSites = b1; dataSites = d1; paramThings = p1;   // counts are per function, not per pass
        }
        println("Call-site overrides written: " + overrides + " (by-value CScriptThing sites: " + byValueSites + ", Data-pointer thing sites: " + dataSites + ", thing params tagged: " + paramThings + ")");
        DecompInterface decompiler = new DecompInterface();
        decompiler.setSimplificationStyle("decompile");
        decompiler.openProgram(currentProgram);
        List<String> rows = new ArrayList<>();
        int count = 0;
        for (Function f : selected) {
            count++;
            List<String> calls = new ArrayList<>();
            List<String> indirectCalls = new ArrayList<>();   // vtable calls: site, slot displacement, stack operands
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
                if (i.getFlowType().isCall() && i.getFlows().length == 0 && i.getNumOperands() == 1) {
                    long slot = -1;
                    for (Object o : i.getOpObjects(0)) if (o instanceof Scalar) slot = ((Scalar) o).getSignedValue();
                    String ops = siteStackOperands.getOrDefault(i.getAddress().getOffset(), "");
                    indirectCalls.add("{\"site\":" + json(hex(i.getAddress())) + ",\"slot\":" + (slot < 0 ? "null" : json("0x" + Long.toHexString(slot))) + "," + ops + "\"kind\":\"vtable\"}");
                }
                if (i.getFlowType().isCall()) {
                    for (Address target : i.getFlows()) {
                        Function called = getFunctionAt(target);
                        if (called == null) called = getFunctionContaining(target);
                        String ops = siteStackOperands.getOrDefault(i.getAddress().getOffset(), "");
                        calls.add("{\"site\":" + json(hex(i.getAddress())) + ",\"target\":" + json(hex(target)) + "," + ops +
                            "\"currentName\":" + json(called == null ? null : called.getName(true)) + "}");
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
                ",\"calls\":[" + String.join(",", calls) + "]" + ",\"indirectCalls\":[" + String.join(",", indirectCalls) + "]" +
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
