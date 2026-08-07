struct CCharString;
struct Inner { void SetMode(bool, const CCharString&); };
extern Inner* g_cutscene;
struct CSubtitleRenderer { void SetCutsceneActionMode(bool, const CCharString&) const; };
void CSubtitleRenderer::SetCutsceneActionMode(bool a, const CCharString& b) const {
    ((Inner*)((char*)g_cutscene + 0x18))->SetMode(a, b);
}