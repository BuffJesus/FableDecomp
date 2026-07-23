// Scan .text for functions whose address is TAKEN IN CODE via `lis rX,HI` + `addi/ori rX,rX,LO`
// (address-taken-only leaf functions codegen misses — not reachable by flow, not in data vtables).
// Emits validated function-start candidates so Python can diff vs the generated emit set.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;
import java.util.*;

public class FindCodeAddrTaken extends GhidraScript {
    static final long LO = 0x82170000L, HI = 0x832CA03CL;  // .text range
    int rd(long a) {
        try { byte[] b = new byte[4]; currentProgram.getMemory().getBytes(toAddr(a), b);
            return ((b[0]&0xff)<<24)|((b[1]&0xff)<<16)|((b[2]&0xff)<<8)|(b[3]&0xff); }
        catch (Exception e) { return 0; }
    }
    boolean validStart(long t) {
        int wt = rd(t);
        if (wt == 0) return false;
        // REQUIRE the previous (non-zero) word to be a function terminator: blr / bctr / b-form /
        // bclr. This is the reliable "start of a new function" signal and avoids mid-function
        // false positives (a lis/addi-computed address that lands inside an existing function).
        long p = t-4; int wp = rd(p); int guard=0;
        while (wp==0 && guard++<4) { p-=4; wp=rd(p); }
        boolean prevTerm = (wp==0x4E800020)||(wp==0x4E800420)||((wp>>>26)==18)||((wp&0xFC0007FE)==0x4C000020);
        return prevTerm;
    }
    @Override public void run() throws Exception {
        // hi16 per GPR most recently set by lis
        long[] hi = new long[32];
        boolean[] have = new boolean[32];
        TreeSet<Long> hits = new TreeSet<>();
        for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
            if (!blk.isInitialized() || !blk.isExecute()) continue;
            long s = blk.getStart().getOffset(), e = blk.getEnd().getOffset();
            if (e < LO || s >= HI) continue;
            s = Math.max(s, LO); e = Math.min(e, HI);
            for (long a = s; a + 4 <= e; a += 4) {
                int ins = rd(a);
                int op = ins >>> 26;
                int rD = (ins>>>21)&31, rA = (ins>>>16)&31;
                int uimm = ins & 0xFFFF;
                if (op == 15 && rA == 0) {              // lis rD, HI
                    hi[rD] = ((long)uimm) << 16; have[rD] = true;
                } else if ((op == 14 || op == 24) && have[rA]) {  // addi/ori rD, rA, LO -> full addr
                    long addr = (op==14) ? hi[rA] + (short)uimm : hi[rA] | uimm;
                    // USAGE-AWARE: only accept if the computed address is STORED as a callback
                    // (`stw rD, off(rY)`) within a few instrs, or CALLED (`mtctr rD; bctrl`). Those
                    // are real function pointers; a jump-TABLE base is instead used with lwzx+bctr
                    // (no store) so it's rejected -> avoids splitting functions on switch cases.
                    boolean isFnPtr = false;
                    if (addr>=LO && addr<HI && (addr&3)==0 && validStart(addr)) {
                        for (int k=1;k<=5;k++){
                            int nx = rd(a + 4L*k);
                            int nop = nx>>>26, nS=(nx>>>21)&31, nA=(nx>>>16)&31;
                            if (nop==36 && nS==rD) { isFnPtr=true; break; }        // stw rD,off(rA)
                            if ((nx&0xFC1FFFFF)==(0x7C0903A6|(rD<<21))) {          // mtctr rD
                                // look one further for bctrl (0x4E800421)
                                for(int m=1;m<=2;m++){ if(rd(a+4L*(k+m))==0x4E800421){isFnPtr=true;} }
                                break;
                            }
                            if (nop==31 && nS==rD) break;   // rD overwritten
                            if ((nx>>>26)==18) break;       // branch -> stop
                        }
                    }
                    if (isFnPtr) hits.add(addr);
                    have[rD] = false;
                } else if (op == 15) {
                    // another lis to a different reg handled above; nothing
                } else {
                    // any write to a tracked reg invalidates it (approx: opcodes with rD in 21..25)
                    if (op==14||op==24||op==25||op==32||op==34||op==36||op==38||op==40) have[rD]=false;
                }
            }
        }
        StringBuilder sb = new StringBuilder();
        for (long t : hits) sb.append(String.format("CODEADDR %08X%n", t));
        print(sb.toString());
        println("=== code-address-taken candidates=" + hits.size() + " ===");
    }
}
