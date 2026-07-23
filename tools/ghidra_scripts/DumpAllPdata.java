import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;
public class DumpAllPdata extends GhidraScript {
    int be32(byte[] b,int o){ return ((b[o]&0xff)<<24)|((b[o+1]&0xff)<<16)|((b[o+2]&0xff)<<8)|(b[o+3]&0xff); }
    @Override public void run() throws Exception {
        MemoryBlock pd=currentProgram.getMemory().getBlock(".pdata");
        int n=(int)(pd.getEnd().getOffset()-pd.getStart().getOffset()+1);
        byte[] buf=new byte[n]; pd.getBytes(pd.getStart(),buf);
        StringBuilder sb=new StringBuilder();
        for(int off=0;off+8<=n;off+=8){
            long begin=be32(buf,off)&0xFFFFFFFFL; int data=be32(buf,off+4);
            int fl=(data>>>8)&0x3FFFFF; if(begin==0&&data==0) continue;
            sb.append(String.format("%08X %X%n",begin,fl*4));
        }
        print(sb.toString());
    }
}
