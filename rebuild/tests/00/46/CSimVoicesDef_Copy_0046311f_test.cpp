#include <cstdio>

// Behaviour model of CSimVoicesDef::Copy:
//   1) base Copy(src) copies the base region
//   2) sub-object at +0x3c gets its own Copy(src+0x3c)

struct SubVoices {
    int val;
    void Copy(SubVoices const* s){ val = s->val + 1000; }
};

struct CSimVoicesDef {
    char base_region[0x3c]; // base occupies [0,0x3c)
    SubVoices sub;          // sub at +0x3c

    void baseCopy(CSimVoicesDef const* src){
        for(int i=0;i<0x3c;i++) base_region[i]=src->base_region[i];
    }
    void Copy(CSimVoicesDef const* src){
        baseCopy(src);
        SubVoices const* srcSub =
            (SubVoices const*)((char const*)src + 0x3c);
        this->sub.Copy(srcSub);
    }
};

int main(){
    CSimVoicesDef a, b;
    for(int i=0;i<0x3c;i++){ a.base_region[i]=(char)(i+1); b.base_region[i]=0; }
    a.sub.val = 5;
    b.sub.val = 0;

    b.Copy(&a);

    bool baseOk = true;
    for(int i=0;i<0x3c;i++) if(b.base_region[i]!=(char)(i+1)) baseOk=false;
    bool subOk = (b.sub.val == 1005);

    if(baseOk && subOk) printf("COPY_PARITY_OK\n");
    else printf("FAIL base=%d sub=%d\n", (int)baseOk, b.sub.val);
    return 0;
}