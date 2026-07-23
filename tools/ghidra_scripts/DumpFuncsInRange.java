// Dump function entry points + sizes in an address range from the analyzed program.
// Usage: analyzeHeadless <proj> Fable2_TU1 -process default_tu1.xex -noanalysis \
//        -scriptPath tools/ghidra_label -postScript DumpFuncsInRange.java 0x82205000 0x82209000
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class DumpFuncsInRange extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        long lo = Long.parseLong(a[0].replace("0x",""),16);
        long hi = Long.parseLong(a[1].replace("0x",""),16);
        Address alo = toAddr(lo), ahi = toAddr(hi);
        println("=== functions in ["+a[0]+","+a[1]+") ===");
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(alo, true);
        while (it.hasNext()) {
            Function f = it.next();
            Address ep = f.getEntryPoint();
            if (ep.getOffset() >= hi) break;
            long sz = f.getBody().getNumAddresses();
            println(String.format("0x%08X size=0x%X %s", ep.getOffset(), sz, f.getName()));
        }
        // explicitly report the containing function of the failing target
        Address t = toAddr(0x82208020L);
        Function cf = getFunctionContaining(t);
        println("--- 0x82208020 containing function: " +
            (cf==null? "NONE (not in any function)" :
             String.format("%s @ 0x%08X size=0x%X", cf.getName(), cf.getEntryPoint().getOffset(), cf.getBody().getNumAddresses())));
        Function ef = getFunctionAt(t);
        println("--- 0x82208020 is a function ENTRY: " + (ef!=null));
    }
}
