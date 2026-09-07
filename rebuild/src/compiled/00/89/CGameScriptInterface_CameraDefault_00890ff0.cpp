class CGameCameraManager
{
public:
    void ResetCamera();
    void ReleaseScriptedControl();
};

struct CameraTarget
{
    unsigned char unused00[0x91];
    unsigned char flags91;
};
struct CameraProvider
{
    virtual void unused00();
    virtual void unused04();
    virtual void unused08();
    virtual void unused0C();
    virtual void unused10();
    virtual void unused14();
    virtual void unused18();
    virtual void unused1C();
    virtual void unused20();
    virtual void GetCameraManager(CGameCameraManager** manager);
};
struct WorldOverlay
{
    unsigned char unused00[0x34];
    CameraProvider* cameraProvider;
};
struct GlobalOverlay
{
    unsigned char unused00[0x1C];
    void* cameraOwner;
};

extern GlobalOverlay* gCameraGlobal;
extern void* __fastcall GetCameraContext(void* cameraOwner);
extern CameraTarget* __fastcall GetCameraTarget(void* cameraContext);

class CGameScriptInterface
{
public:
    virtual void unused00();
    virtual void unused04();
    virtual void unused08();
    virtual void unused0C();
    virtual void unused10();
    virtual void unused14();
    virtual void unused18();
    virtual void ReleaseOneCameraLayer();
    WorldOverlay* world;
    unsigned char padding08[0x40];
    long cameraLayerCount;
    void CameraDefault();
};

void CGameScriptInterface::CameraDefault()
{
    void* context = GetCameraContext(gCameraGlobal->cameraOwner);
    CameraTarget* target = GetCameraTarget(context);
    if (target != 0 && (target->flags91 & 1) == 0)
    {
        CGameCameraManager* cameraManager;
        world->cameraProvider->GetCameraManager(&cameraManager);
        if (cameraLayerCount >= 0)
        {
            cameraManager->ResetCamera();
            do
            {
                ReleaseOneCameraLayer();
                --cameraLayerCount;
            }
            while (cameraLayerCount != -1);
            cameraLayerCount = -1;
            return;
        }
        cameraManager->ReleaseScriptedControl();
        cameraLayerCount = -1;
    }
}