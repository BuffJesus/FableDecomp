#include "engine/CCodeSectionManager.h"
void FABLE_FASTCALL StaticLoadXMVCode() {
 if (g_FableXMVCodeReferences_013CA7E4 == 0)
  CTBaseSingleton<CCodeSectionManager>::Get().LoadSection(g_FableXMVCodeHandle_013CA7E0);
 ++g_FableXMVCodeReferences_013CA7E4;
}