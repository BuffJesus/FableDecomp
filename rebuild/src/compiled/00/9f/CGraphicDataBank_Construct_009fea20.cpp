#include "fable_ui_bank_runtime.h"
#include <string.h>
FableUiGraphicsBank* __fastcall FableUiConstructGraphicsBank(void* storage,void*)
{
    FableUiGraphicsBankRuntimeView* bank=static_cast<FableUiGraphicsBankRuntimeView*>(storage);
    FableUiConstructBankFileBase(bank,0);
    FableUiConstructResourceBank(&bank->ResourceBank,0);
    bank->ResourceBank.Vtable=FableUiGraphicResourceBankVtable;
    bank->Vtable=&FableUiGraphicsBankRuntimeVtable;
    bank->GraphicList.Begin=bank->GraphicList.End=bank->GraphicList.Capacity=0;
    bank->GraphicFrameBuffer.Begin=bank->GraphicFrameBuffer.End=bank->GraphicFrameBuffer.Capacity=0;
    bank->Initialised=false; bank->StateBlock.__vftable=FableUiGraphicStateVtable;
    // BankFormats and CompressedBankFormats: fourteen CPixelFormat indices.
    int empty=-1;
    for(unsigned f=0;f<14;++f) memcpy(bank->StateBlock.State+f*4,&empty,4);
    bank->TextureManager.Data=0; bank->TextureManager.Info=0;
    for(unsigned i=0;i<11;++i) { bank->PixelFormats[i]=-1; memset(&bank->BlankTextures[i],0,sizeof(CTexture)); }
    void* object=FableUiAllocateGraphicsBank(0x5D4);
    FableUiResetTextureManager(&bank->TextureManager,0,object ? FableUiConstructTextureManager(object,0) : 0);
    return reinterpret_cast<FableUiGraphicsBank*>(bank);
}
