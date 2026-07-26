struct CSummonerDef;
extern "C" void __fastcall base_copy(CSummonerDef* self, const CSummonerDef* src);
void __fastcall CSummonerDef_Copy(CSummonerDef* self, const CSummonerDef* src)
{
    base_copy(self, src);
}