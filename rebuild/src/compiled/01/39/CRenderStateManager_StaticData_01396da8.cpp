#include "fable_render_state_manager.h"
#include "fable_render_state_defaults.h"

// SDK enum values, checked against the retail lookup entries read by setup.
unsigned long FableRenderDefaultValue_01396dd0 = 3; // D3DTADDRESS_CLAMP
unsigned long FableRenderDefaultValue_01396ddc = 2; // D3DCMP_LESS
unsigned long FableRenderDefaultValue_01396de8 = 5; // D3DCMP_GREATER
unsigned long FableRenderDefaultValue_01396e04 = 4; // D3DCMP_LESSEQUAL
unsigned long FableRenderDefaultValue_01396e14 = 8; // D3DCMP_ALWAYS
unsigned long FableRenderDefaultValue_01396e34 = 8; // D3DCMP_ALWAYS
unsigned long FableRenderDefaultValue_01396e58 = 1; // D3DTEXF_POINT
unsigned long FableRenderDefaultValue_01396e5c = 2; // D3DTEXF_LINEAR
unsigned long FableRenderDefaultValue_01396e70 = 1; // D3DTOP_DISABLE
unsigned long FableRenderDefaultValue_01396e74 = 2; // D3DTOP_SELECTARG1
unsigned long FableRenderDefaultValue_01396e7c = 4; // D3DTOP_MODULATE
unsigned long FableRenderDefaultValue_01396ed0 = 1; // D3DBLENDOP_ADD
unsigned long FableRenderDefaultValue_01396ee8 = 1; // D3DTA_CURRENT
unsigned long FableRenderDefaultValue_01396eec = 0; // D3DTA_DIFFUSE
unsigned long FableRenderDefaultValue_01396ef0 = 2; // D3DTA_TEXTURE
unsigned long FableRenderDefaultValue_01396f38 = 1; // D3DSTENCILOP_KEEP
unsigned long FableRenderDefaultValue_01396f40 = 3; // D3DSTENCILOP_REPLACE
unsigned long FableRenderDefaultValue_01396f58 = 0; // D3DFOG_NONE
unsigned long FableRenderDefaultValue_01396f68 = 1; // D3DBLEND_ZERO
unsigned long FableRenderDefaultValue_01396f6c = 2; // D3DBLEND_ONE
unsigned long FableRenderDefaultValue_01396f78 = 5; // D3DBLEND_SRCALPHA
unsigned long FableRenderDefaultValue_01396f7c = 6; // D3DBLEND_INVSRCALPHA
unsigned long FableRenderDefaultValue_01396fa0 = 2; // D3DSHADE_GOURAUD
unsigned long FableRenderDefaultValue_01396fa4 = 3; // D3DFILL_SOLID
unsigned long FableRenderDefaultValue_01396fb0 = 3; // D3DCULL_CCW
unsigned long FableRenderDefaultValue_01396fbc = 0; // D3DVBF_DISABLE
unsigned long FableRenderDefaultValue_01396fcc = 0; // D3DTTFF_DISABLE
unsigned long FableRenderDefaultValue_01396ff4 = 0; // D3DMCS_MATERIAL

unsigned long CRenderStateManager::D3DTextureMipMapModeTable[3] = {1,2,0}; // POINT, LINEAR, NONE
D3DRENDERSTATETYPE FableRenderWrapStateTypes_01396da8[8] = {D3DRS_WRAP0,D3DRS_WRAP1,D3DRS_WRAP2,D3DRS_WRAP3,D3DRS_WRAP4,D3DRS_WRAP5,D3DRS_WRAP6,D3DRS_WRAP7};
// Zero-initialized virtual tail of the retail .data section.
CRenderStateManager* FableCurrentRenderStateManager_013cb508 = 0;
