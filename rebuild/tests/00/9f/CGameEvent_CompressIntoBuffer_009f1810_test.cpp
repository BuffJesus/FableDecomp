#include <cstdio>
#include <cstring>

struct Event { long type; unsigned char player, data[32], endPos, valid, replacement; };
static long Compress(const Event& event, unsigned char* out)
{
    unsigned char* start=out; unsigned short type=static_cast<unsigned short>(event.type);
    if(event.replacement) type|=0x8000; std::memcpy(out,&type,2); out+=2;
    *out++=event.player; *out++=event.endPos; std::memcpy(out,event.data,event.endPos); out+=event.endPos;
    return static_cast<long>(out-start);
}
int main()
{
    Event event; std::memset(&event,0,sizeof(event)); unsigned char out[64];
    event.type=0x1234; event.player=7; if(Compress(event,out)!=4) return 1;
    if(out[0]!=0x34||out[1]!=0x12||out[2]!=7||out[3]!=0) return 2;
    event.replacement=1; event.endPos=7; for(int i=0;i<7;++i)event.data[i]=static_cast<unsigned char>(0xA0+i);
    if(Compress(event,out)!=11||out[1]!=0x92||out[3]!=7) return 3;
    for(int i=0;i<7;++i)if(out[4+i]!=static_cast<unsigned char>(0xA0+i))return 4;
    std::puts("GAME_EVENT_COMPRESS_PASS");return 0;
}