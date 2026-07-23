// Scan .rdata + .data for 4-byte big-endian words that look like code pointers
// into the guest code range. Output distinct targets (candidate vtable/callback
// method entries). Python filters against registered config/pdata starts.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;
import java.util.*;
public class FindAllCodePointers extends GhidraScript {
    static final long LO=0x82170000L, HI=0x832CA03CL;  // .text range
    int readWord(long a){ try{byte[] b=new byte[4];currentProgram.getMemory().getBytes(toAddr(a),b);
        return ((b[0]&0xff)<<24)|((b[1]&0xff)<<16)|((b[2]&0xff)<<8)|(b[3]&0xff);}catch(Exception e){return 0;} }
    @Override public void run() throws Exception {
        TreeMap<Long,Integer> hits=new TreeMap<>();
        for (MemoryBlock blk: currentProgram.getMemory().getBlocks()){
            if(!blk.isInitialized()) continue;
            String nm=blk.getName();
            if(!nm.equals(".rdata") && !nm.equals(".data")) continue;
            long s=blk.getStart().getOffset(), e=blk.getEnd().getOffset();
            byte[] buf=new byte[(int)Math.min(e-s+1,1<<24)];
            long pos=s;
            while(pos<=e){
                int chunk=(int)Math.min(buf.length,e-pos+1);
                blk.getBytes(toAddr(pos),buf,0,chunk);
                for(int i=0;i+4<=chunk;i+=4){
                    long w=((buf[i]&0xffL)<<24)|((buf[i+1]&0xffL)<<16)|((buf[i+2]&0xffL)<<8)|(buf[i+3]&0xffL);
                    if(w>=LO && w<HI && (w&3)==0) hits.merge(w,1,Integer::sum);
                }
                pos+=chunk;
            }
        }
        // validate as function-start: prev word is terminator, or is prologue
        int kept=0;
        StringBuilder sb=new StringBuilder();
        for(Map.Entry<Long,Integer> en: hits.entrySet()){
            long t=en.getKey(); int wt=readWord(t);
            if(wt==0) continue;
            long p=t-4; int wp=readWord(p);
            while(wp==0 && p>t-0x10){ p-=4; wp=readWord(p); }
            boolean prevTerm=(wp==0x4E800020)||(wp==0x4E800420)||((wp>>>26)==18);
            boolean prologue=(wt==0x7D8802A6)||((wt&0xFFFF0000)==0x94210000)
                ||((wt&0xFC1F0000)==0x90010000)||((wt&0xFC1F0000)==0xF8010000);
            if(prevTerm||prologue){ kept++; sb.append(String.format("ENTRY %08X%n",t)); }
        }
        print(sb.toString());
        println("=== distinct code-ptrs="+hits.size()+" validated-entries="+kept+" ===");
    }
}
