"""Fresh VC71 parity gate: CRenderManager2D Draw3DScaledText + Draw2DText (inlined Draw3DScaledText at z 0, scale 1)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CWideString@@QAE@ABV0@@Z':0x99b720,
 '?AddText@CRenderManager2D@@AAEXABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z':0x9db700,
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw3DScaledText_009dbff0.cpp'
ROOTS=[
 dict(name='scaled_text',source=F,symbol='?Draw3DScaledText@CRenderManager2D@@QAE_NABVC3DVector@@MVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z',address=0x9dbff0,size=49,instructions=17,relocations=2),
 dict(name='text',source=F,symbol='?Draw2DText@CRenderManager2D@@QAE_NABVC2DVector@@VCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z',address=0x9dd8f0,size=109,instructions=34,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-text.json',[],
  'CRenderManager2D Draw3DScaledText + Draw2DText (inlined Draw3DScaledText at z 0, scale 1)')
 print('NATIVE_RENDER2D_TEXT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
