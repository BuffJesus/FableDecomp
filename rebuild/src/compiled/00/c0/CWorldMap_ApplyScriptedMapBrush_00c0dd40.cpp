// CWorldMap::ApplyScriptedMapBrush  retail 0x00c0dd40
struct CScriptedMapBrush;

// Helper: two stack args, callee-cleaned (stdcall), returns a pointer.
void* __stdcall ResolveMapBrush(CScriptedMapBrush* a, CScriptedMapBrush* b);

struct CWorldMap {
    void ApplyScriptedMapBrush(CScriptedMapBrush* brush);
    void ConsumeMapBrush(void* p); // thiscall, 1 arg
};

void CWorldMap::ApplyScriptedMapBrush(CScriptedMapBrush* brush)
{
    void* r = ResolveMapBrush(brush, brush);
    this->ConsumeMapBrush(r);
}