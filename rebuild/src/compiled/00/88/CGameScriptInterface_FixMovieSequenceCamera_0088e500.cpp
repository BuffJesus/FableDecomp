struct CGameScriptInterface;
extern unsigned char g_movieSeqCameraFixed;

void __fastcall CGameScriptInterface_FixMovieSequenceCamera(CGameScriptInterface* self, void* edx, unsigned char enabled)
{
    (void)self;
    (void)edx;
    g_movieSeqCameraFixed = (unsigned char)(enabled == 0);
}