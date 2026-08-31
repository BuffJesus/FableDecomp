// Discover script allocator candidates by walking anchor -> Main -> vtable -> constructor/caller.
// Usage: -postScript DiscoverScriptAllocatorFromAnchor.java <exact-anchor>
//@category FableTLC
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import java.util.LinkedHashSet;
import java.util.Set;

public class DiscoverScriptAllocatorFromAnchor extends GhidraScript {
    private boolean lifecycleVtable(Address address) {
        Memory memory = currentProgram.getMemory();
        try {
            for (int slot = 0; slot < 5; slot++) {
                Address target = toAddr(memory.getInt(address.add(slot * 4L)) & 0xffffffffL);
                if (memory.getBlock(target) == null || !memory.getBlock(target).isExecute()) return false;
            }
            return true;
        } catch (Exception ignored) {
            return false;
        }
    }

    private Set<Function> callers(Function target) {
        Set<Function> result = new LinkedHashSet<>();
        for (Reference reference : getReferencesTo(target.getEntryPoint())) {
            Function caller = getFunctionContaining(reference.getFromAddress());
            if (caller != null && !caller.equals(target)) result.add(caller);
        }
        return result;
    }

    private boolean allocatesObject(Function function) {
        InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
        while (instructions.hasNext()) {
            Instruction instruction = instructions.next();
            if (!instruction.getFlowType().isCall()) continue;
            for (Address target : instruction.getFlows()) {
                Function called = getFunctionAt(target);
                if (called != null && called.getName().toLowerCase().contains("operator_new")) return true;
            }
        }
        return false;
    }

    private void addRows(String anchor, Data item, Function main, Address vtable, Set<String> rows) {
        for (Reference vtableRef : getReferencesTo(vtable)) {
            Function constructor = getFunctionContaining(vtableRef.getFromAddress());
            if (constructor == null) continue;
            Set<Function> candidates = new LinkedHashSet<>();
            if (allocatesObject(constructor)) candidates.add(constructor);
            for (Function caller : callers(constructor)) {
                if (allocatesObject(caller)) candidates.add(caller);
            }
            for (Function allocator : candidates) {
                rows.add("{\"anchor\":\"" + anchor.replace("\"", "\\\"") +
                    "\",\"anchorAddress\":\"0x" + item.getAddress().toString().toUpperCase() +
                    "\",\"mainAddress\":\"0x" + main.getEntryPoint().toString().toUpperCase() +
                    "\",\"vtableAddress\":\"0x" + vtable.toString().toUpperCase() +
                    "\",\"constructorAddress\":\"0x" + constructor.getEntryPoint().toString().toUpperCase() +
                    "\",\"allocatorCandidate\":\"0x" + allocator.getEntryPoint().toString().toUpperCase() + "\"}");
            }
        }
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("expected one exact string anchor");
        String anchor = args[0];
        Set<String> rows = new LinkedHashSet<>();
        DataIterator data = currentProgram.getListing().getDefinedData(true);
        while (data.hasNext() && !monitor.isCancelled()) {
            Data item = data.next();
            if (!anchor.equals(item.getValue())) continue;
            for (Reference stringRef : getReferencesTo(item.getAddress())) {
                Function main = getFunctionContaining(stringRef.getFromAddress());
                if (main == null) {
                    // Script-local strings commonly follow the lifecycle vtable. Bootstrap an
                    // unanalyzed Main from slot 2, but accept it only if its new body owns this xref.
                    for (long distance = 4; distance <= 0x200; distance += 4) {
                        Address vtable = item.getAddress().subtract(distance);
                        if (!lifecycleVtable(vtable)) continue;
                        try {
                            Address mainAddress = toAddr(currentProgram.getMemory().getInt(vtable.add(8)) & 0xffffffffL);
                            Function candidate = getFunctionAt(mainAddress);
                            if (candidate == null) candidate = createFunction(mainAddress, null);
                            if (candidate != null && candidate.getBody().contains(stringRef.getFromAddress())) {
                                addRows(anchor, item, candidate, vtable, rows);
                            }
                        } catch (Exception ignored) {
                            // Continue scanning; candidate discovery must stay conservative.
                        }
                    }
                    continue;
                }
                for (Reference mainRef : getReferencesTo(main.getEntryPoint())) {
                    if (!mainRef.getFromAddress().isMemoryAddress()) continue;
                    Address vtable = mainRef.getFromAddress().subtract(8); // lifecycle slot 2 is Main
                    if (!lifecycleVtable(vtable)) continue;
                    addRows(anchor, item, main, vtable, rows);
                }
            }
        }
        println("DISCOVERY_BEGIN");
        for (String row : rows) println(row);
        println("DISCOVERY_END count=" + rows.size());
    }
}
