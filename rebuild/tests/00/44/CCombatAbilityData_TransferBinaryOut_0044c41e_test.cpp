#include <cstdio>

struct CBinaryStream {
    int intCalls; int floatCalls; int lastInt; float lastFloat;
    void PutInt(int v){ intCalls++; lastInt=v; }
    void PutFloat(float v){ floatCalls++; lastFloat=v; }
};
struct CCombatAbilityData { int field_0; int int4; float f8; float fc; int int10; };

static void TransferBinaryOut(CCombatAbilityData* self, CBinaryStream* stream)
{
    stream->PutInt(self->int4);
    stream->PutFloat(*(volatile float*)&self->f8);
    stream->PutFloat(*(volatile float*)&self->fc);
    stream->PutInt(self->int10);
}

int main(){
    CCombatAbilityData d; d.field_0=1; d.int4=42; d.f8=1.5f; d.fc=2.5f; d.int10=99;
    CBinaryStream s; s.intCalls=0; s.floatCalls=0; s.lastInt=0; s.lastFloat=0.0f;
    TransferBinaryOut(&d,&s);
    if(s.intCalls==2 && s.floatCalls==2 && s.lastInt==99 && s.lastFloat==2.5f){
        std::printf("CCombatAbilityData_0044c41e_TEST PASS\n"); return 0;
    }
    std::printf("FAIL i=%d f=%d li=%d lf=%f\n", s.intCalls, s.floatCalls, s.lastInt, s.lastFloat);
    return 1;
}