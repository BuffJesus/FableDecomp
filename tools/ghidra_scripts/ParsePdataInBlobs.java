// Parse XEX .pdata (IMAGE_CE_RUNTIME_FUNCTION, 8 bytes each) and output every
// function whose BeginAddress falls inside one of the over-merged blob ranges.
// size = FunctionLength*4, FunctionLength = (Data>>8)&0x3FFFFF (Data read big-endian).
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;

public class ParsePdataInBlobs extends GhidraScript {
    static final long[][] BLOBS = {
        {0x82d86ae4L, 0x82d86ae4L+0x0fe184L}, {0x82b9f038L, 0x82b9f038L+0x010054L},
        {0x822ea8a8L, 0x822ea8a8L+0x010000L}, {0x82fba028L, 0x82fba028L+0x010000L},
        {0x8284c28cL, 0x8284c28cL+0x00e42cL}, {0x828e9fe4L, 0x828e9fe4L+0x00d614L},
        {0x82f06cfcL, 0x82f06cfcL+0x00cd6cL}, {0x8222c3d0L, 0x8222c3d0L+0x00c300L},
        {0x82eedfd8L, 0x82eedfd8L+0x00ad68L}, {0x82c06244L, 0x82c06244L+0x008facL},
        {0x8218db04L, 0x8218db04L+0x008ec4L}, {0x82c00118L, 0x82c00118L+0x0060d8L},
        {0x82c14d58L, 0x82c14d58L+0x004d00L}, {0x82f2ff68L, 0x82f2ff68L+0x003eb8L},
        {0x822050b4L, 0x822050b4L+0x002f6cL},
    };
    int blobIdx(long v){ for(int i=0;i<BLOBS.length;i++){ if(v>=BLOBS[i][0]&&v<BLOBS[i][1]) return i;} return -1; }
    int be32(byte[] b,int o){ return ((b[o]&0xff)<<24)|((b[o+1]&0xff)<<16)|((b[o+2]&0xff)<<8)|(b[o+3]&0xff); }
    @Override public void run() throws Exception {
        MemoryBlock pd = currentProgram.getMemory().getBlock(".pdata");
        long start=pd.getStart().getOffset(), end=pd.getEnd().getOffset();
        int n=(int)(end-start+1);
        byte[] buf=new byte[n];
        pd.getBytes(pd.getStart(), buf);
        int total=0, inblob=0;
        for(int off=0; off+8<=n; off+=8){
            long begin = be32(buf,off) & 0xFFFFFFFFL;
            int data = be32(buf,off+4);
            int funcLen = (data>>>8) & 0x3FFFFF;
            long size = ((long)funcLen)*4;
            if(begin==0 && data==0) continue;
            total++;
            int bi = blobIdx(begin);
            if(bi>=0){
                inblob++;
                println(String.format("PD 0x%08X 0x%X blob=%d", begin, size, bi));
            }
        }
        println("=== total pdata entries="+total+"  in-blob="+inblob+" ===");
    }
}
