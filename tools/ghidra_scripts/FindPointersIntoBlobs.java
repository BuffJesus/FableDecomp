// Scan all memory for 4-byte big-endian words that point INTO one of the over-merged
// blob ranges (candidate vtable/callback method entries). Output distinct targets.
// Blob ranges hardcoded below (start,end) from Fable2_config.toml (size>0x2000).
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import java.util.*;

public class FindPointersIntoBlobs extends GhidraScript {
    // {start, end} for each blob
    static final long[][] BLOBS = {
        {0x82d86ae4L, 0x82d86ae4L+0x0fe184L},
        {0x82b9f038L, 0x82b9f038L+0x010054L},
        {0x822ea8a8L, 0x822ea8a8L+0x010000L},
        {0x82fba028L, 0x82fba028L+0x010000L},
        {0x8284c28cL, 0x8284c28cL+0x00e42cL},
        {0x828e9fe4L, 0x828e9fe4L+0x00d614L},
        {0x82f06cfcL, 0x82f06cfcL+0x00cd6cL},
        {0x8222c3d0L, 0x8222c3d0L+0x00c300L},
        {0x82eedfd8L, 0x82eedfd8L+0x00ad68L},
        {0x82c06244L, 0x82c06244L+0x008facL},
        {0x8218db04L, 0x8218db04L+0x008ec4L},
        {0x82c00118L, 0x82c00118L+0x0060d8L},
        {0x82c14d58L, 0x82c14d58L+0x004d00L},
        {0x82f2ff68L, 0x82f2ff68L+0x003eb8L},
        {0x822050b4L, 0x822050b4L+0x002f6cL},
    };
    boolean inBlob(long v){
        for(long[] b:BLOBS){ if(v>=b[0] && v<b[1]) return true; } return false;
    }
    boolean isBlobStart(long v){
        for(long[] b:BLOBS){ if(v==b[0]) return true; } return false;
    }
    @Override
    public void run() throws Exception {
        TreeMap<Long,Integer> hits = new TreeMap<>();  // target -> count of pointers found
        for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
            if (!blk.isInitialized()) continue;
            String nm = blk.getName();
            if (!nm.equals(".rdata") && !nm.equals(".data")) continue;  // vtables live here only
            long start = blk.getStart().getOffset();
            long end   = blk.getEnd().getOffset();
            long len = end - start + 1;
            byte[] buf = new byte[(int)Math.min(len, 1<<24)];
            long pos = start;
            while (pos <= end) {
                int chunk = (int)Math.min(buf.length, end-pos+1);
                blk.getBytes(toAddr(pos), buf, 0, chunk);
                for (int i=0; i+4<=chunk; i+=4) {
                    long w = ((buf[i]&0xffL)<<24)|((buf[i+1]&0xffL)<<16)|((buf[i+2]&0xffL)<<8)|(buf[i+3]&0xffL);
                    if (inBlob(w) && !isBlobStart(w)) {
                        hits.merge(w, 1, Integer::sum);
                    }
                }
                pos += chunk;
            }
        }
        println("=== distinct code-pointers into blobs: " + hits.size() + " ===");
        int kept=0;
        for (Map.Entry<Long,Integer> e : hits.entrySet()) {
            long t = e.getKey();
            if ((t & 3) != 0) continue;              // must be 4-aligned
            int wt = readWord(t);
            // skip if target itself is padding/undecodable
            if (wt == 0) continue;
            // previous non-padding word
            long p = t - 4;
            int wp = readWord(p);
            while (wp == 0 && p > t-0x10) { p -= 4; wp = readWord(p); }
            boolean prevTerm = (wp==0x4E800020)            // blr
                || (wp==0x4E800420)                        // bctr
                || ((wp>>>26)==18)                         // b/ba (uncond branch)
                || (wp==0x4E800021)||(wp==0x4E800421);     // blrl/bctrl (rare tail)
            boolean prologue = (wt==0x7D8802A6)            // mflr r12
                || ((wt&0xFFFF0000)==0x94210000)           // stwu r1,-x(r1)
                || ((wt&0xFFFF0000)==0xF8210000)           // std rX save via std r1? (stdu)
                || ((wt&0xFC1F0000)==0x90010000)           // stw rX,d(r1)
                || ((wt&0xFC1F0000)==0xF8010000);          // std rX,d(r1)
            if (prevTerm || prologue) {
                kept++;
                println(String.format("ENTRY 0x%08X x%d prevTerm=%b prologue=%b", t, e.getValue(), prevTerm, prologue));
            }
        }
        println("=== validated function-entry pointers: " + kept + " ===");
    }
    int readWord(long addr) {
        try {
            byte[] b = new byte[4];
            currentProgram.getMemory().getBytes(toAddr(addr), b);
            return ((b[0]&0xff)<<24)|((b[1]&0xff)<<16)|((b[2]&0xff)<<8)|(b[3]&0xff);
        } catch (Exception ex) { return 0; }
    }
}
