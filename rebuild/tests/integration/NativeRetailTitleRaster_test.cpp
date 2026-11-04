// Native renderer verification. Decoder/container/OS/upload are boundaries.
// The canonical synthetic controls execute too; their sources remain unchanged.
#define main CanonicalRasterControls
#include "NativeIndexedRaster_test.cpp"
#undef main
#include "../../integration/fable_bank_reader.h"
#include "../../integration/fable_texture_decode.h"

int main(int argc,char** argv)
{
    if(argc!=10)return 20;
    char* controls[8];controls[0]=argv[0];
    for(unsigned n=0;n<7;++n)controls[n+1]=argv[n+3];
    int controlResult=CanonicalRasterControls(8,controls);
    if(controlResult)return controlResult;
    fable_bank::Bank bank;
    if(!fable_bank::Open(&bank,argv[1]))return 21;
    unsigned size=0,infoLen=0;const unsigned char* info=0;
    const unsigned char* payload=fable_bank::FindEntry(&bank,"GBANK_FRONT_END_PC","FRONTEND_TITLE_01_SPRITE",&size,&info,&infoLen);
    if(!payload||infoLen!=34||size!=72474)return 22;
    unsigned char rgba[256*128*4];unsigned width=0,height=0;
    if(FableDecodeBankTexture(info,payload,size,rgba,sizeof(rgba),&width,&height)||width!=256||height!=128)return 23;
    fable_bank::Close(&bank);
    typedef void* (__stdcall* Create)(unsigned);
    void* lib=LoadLibraryA("d3d9.dll");if(!lib)return 24;
    Create create=(Create)GetProcAddress(lib,"Direct3DCreate9");if(!create)return 25;
    void* api=create(32);if(!api)return 26;
    void* window=CreateWindowExA(0,"STATIC","Recovered renderer real Fable title asset",0,0,0,288,160,0,0,0,0);if(!window)return 27;
    Present pp={288,160,22,1,0,0,1,window,1,0,0,0,0,0x80000000};void* device=0;
    CHECK("AssetCreateDevice",((long(__stdcall*)(void*,unsigned,unsigned,void*,unsigned,Present*,void**))slot(api,16))(api,0,1,window,0x20,&pp,&device));
    unsigned long storage[sizeof(CRenderManagerCore)/4];memset(storage,0,sizeof(storage));
    CRenderManagerCore* manager=(CRenderManagerCore*)storage;manager->PD3DDevice=(IDirect3DDevice9*)device;
    unsigned long displayStorage[sizeof(CDisplayManager)/4];memset(displayStorage,0,sizeof(displayStorage));
    CDisplayManager* display=(CDisplayManager*)displayStorage;
    *(CRenderManagerCore**)((unsigned char*)displayStorage+8)=manager;display->D3DDevice=(IDirect3DDevice9*)device;
    CRenderStateManager::CStateInfo states[5];
    const unsigned params[]={7,22,137,27,8};const unsigned values[]={0,1,0,0,3};
    for(unsigned i=0;i<5;++i){states[i].CurrentState=~values[i];states[i].DesiredState=values[i];states[i].StateParam=params[i];states[i].StateType=1;states[i].DirtyListFlag=true;}
    CRenderStateManagerTestAccess::setup(manager,device,states);
    manager->SetVertexFormat(0x144);
    void* texture=0;
    CHECK("AssetCreateTexture",((long(__stdcall*)(void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,void**,void*))slot(device,23))(device,width,height,1,0,21,1,&texture,0));
    Locked texLock;CHECK("AssetTextureLock",((long(__stdcall*)(void*,unsigned,Locked*,void*,unsigned))slot(texture,19))(texture,0,&texLock,0,0));
    for(unsigned ty=0;ty<height;++ty)for(unsigned tx=0;tx<width;++tx){unsigned char* p=rgba+(ty*width+tx)*4;*(unsigned*)((char*)texLock.bits+ty*texLock.pitch+tx*4)=((unsigned)p[3]<<24)|((unsigned)p[0]<<16)|((unsigned)p[1]<<8)|p[2];}
    CHECK("AssetTextureUnlock",((long(__stdcall*)(void*,unsigned))slot(texture,20))(texture,0));
    unsigned textureStorage[2]={(unsigned)texture,0};CTexture* engineTexture=(CTexture*)textureStorage;
    C2DExtentsI dims=engineTexture->GetDimensions(0);
    if(dims.X!=256||dims.Y!=128||engineTexture->GetLevelByteLength(0)!=131072)return 28;
    CRenderStateManager::CStateInfo textureStates[10];
    const unsigned textureParams[]={8,1,2,4,5,6,5,1,2,1};
    const unsigned textureValues[]={3,2,2,2,2,1,1,3,3,1};
    for(unsigned ts=0;ts<10;++ts){textureStates[ts].CurrentState=~textureValues[ts];textureStates[ts].DesiredState=textureValues[ts];textureStates[ts].StateParam=textureParams[ts];textureStates[ts].StateType=ts==0?1:ts>=5&&ts<=8?3:2;textureStates[ts].TextureStage=ts==9?1:0;}
    // Both render-state and texture-state lists must be realized together.
    CRenderStateManager::CStateInfo mergedStates[15];
    memcpy(mergedStates,states,sizeof(states));memcpy(mergedStates+5,textureStates,sizeof(textureStates));
    CRenderStateManagerTestAccess::queue(manager,mergedStates,15);
    CHECK("AssetClear",((long(__stdcall*)(void*,unsigned,void*,unsigned,unsigned,float,unsigned))slot(device,43))(device,0,0,1,0xff102030,1.0f,0));
    if(!display->BeginRender())return 29;
    struct Vertex {float x,y,z,rhw;unsigned colour;float u,v;};
    Vertex vertices[]={{16,16,0,1,0xffffffff,0,0},{272,16,0,1,0xffffffff,1,0},{16,144,0,1,0xffffffff,0,1},{272,144,0,1,0xffffffff,1,1}};
    unsigned short indices[]={0,1,2,2,1,3};const CTexture* textures[]={engineTexture};
    manager->DrawAsIndexedTris(textures,1,vertices,sizeof(Vertex),4,indices,2);
    display->EndRender();
    if(manager->Rendering||display->CanBeginRender||!CRenderStateManagerTestAccess::empty(manager)||manager->CurrentlySelectedTextures[0]!=(IDirect3DBaseTexture9*)texture)return 30;
    void* target=0;void* readback=0;
    CHECK("AssetGetRenderTarget",((long(__stdcall*)(void*,unsigned,void**))slot(device,38))(device,0,&target));
    CHECK("AssetCreateReadback",((long(__stdcall*)(void*,unsigned,unsigned,unsigned,unsigned,void**,void*))slot(device,36))(device,288,160,22,2,&readback,0));
    CHECK("AssetReadback",((long(__stdcall*)(void*,void*,void*))slot(device,32))(device,target,readback));
    Locked lock;CHECK("AssetLockReadback",((long(__stdcall*)(void*,Locked*,void*,unsigned))slot(readback,13))(readback,&lock,0,0x10));
    FILE* out=fopen(argv[2],"wb");if(!out)return 31;fprintf(out,"P6\n288 160\n255\n");
    unsigned mismatches=0,assetPixels=0,backgroundPixels=0;
    for(unsigned y=0;y<160;++y)for(unsigned x=0;x<288;++x){
        unsigned pixel=(*(unsigned*)((char*)lock.bits+y*lock.pitch+x*4))&0xffffff;
        unsigned expected=0x102030;
        if(x>=16&&x<272&&y>=16&&y<144){const unsigned char* p=rgba+((y-16)*256+x-16)*4;expected=((unsigned)p[0]<<16)|((unsigned)p[1]<<8)|p[2];++assetPixels;}else ++backgroundPixels;
        if(pixel!=expected){if(mismatches<4)printf("ASSET_MISMATCH x=%u y=%u got=%06x expected=%06x\n",x,y,pixel,expected);++mismatches;}
        unsigned char rgb[]={(unsigned char)(pixel>>16),(unsigned char)(pixel>>8),(unsigned char)pixel};fwrite(rgb,1,3,out);
    }
    fclose(out);CHECK("AssetUnlockReadback",((long(__stdcall*)(void*))slot(readback,14))(readback));
    release(readback);release(target);manager->AttachTextureToStage(0,0);release(texture);release(device);release(api);DestroyWindow(window);
    if(mismatches)return failure("AssetPixelValidation",mismatches);
    printf("NATIVE_REAL_ASSET_PASS title_id=3 asset_pixels=%u background_pixels=%u total_pixels=46080 texture_level_bytes=131072\n",assetPixels,backgroundPixels);return 0;
}
