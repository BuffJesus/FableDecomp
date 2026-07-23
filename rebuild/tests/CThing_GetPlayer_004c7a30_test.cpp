#include <cstdio>
struct CThing { char pad[0x90]; signed char m_playerIndex; };
struct CPlayer { int id; };
struct CPlayerTable { CPlayer* players; CPlayer* __fastcall Lookup(int idx){return &players[idx];} };
struct CManager { char pad[0x30]; CPlayerTable* m_table; };
CManager* g_manager;
CPlayer* __fastcall CThing_GetPlayer(CThing* self){ return g_manager->m_table->Lookup(self->m_playerIndex); }
int main(){
    CPlayer plist[4]; for(int i=0;i<4;++i) plist[i].id=100+i;
    CPlayerTable tbl; tbl.players=plist; CManager mgr; mgr.m_table=&tbl; g_manager=&mgr;
    CThing t; t.m_playerIndex=2; CPlayer* p=CThing_GetPlayer(&t);
    if(p==&plist[2]&&p->id==102){std::printf("CThing_004c7a30_TEST PASS\n");return 0;}
    std::printf("FAIL\n");return 1;
}