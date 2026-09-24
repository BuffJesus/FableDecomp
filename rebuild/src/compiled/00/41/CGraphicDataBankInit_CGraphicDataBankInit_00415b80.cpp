#include "fable_ui_manager_construction.h"
FableUiGraphicBankInit* __fastcall FableUiConstructGraphicBankInit(FableUiGraphicBankInit* init,void*)
{
    init->NonAlphaFormat=init->AlphaFormat=init->InterpolatedAlphaFormat=init->BooleanAlphaFormat=-1;
    init->UncompressedNonAlphaFormat=init->UncompressedAlphaFormat=init->SignedFormat=-1;
    init->GenerateMipmaps=false; init->MaxGraphicWidth=init->MaxGraphicHeight=~0u;
    init->AllowDither=true; init->AllowCompressedTextures=false; init->AllowDXT1ForBooleanAlpha=true;
    return init;
}
