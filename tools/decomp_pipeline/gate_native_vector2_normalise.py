"""Fresh VC71 parity gate: C2DVector::Normalise (0x00A14540) through the inline GFFastOneOverSqrt (FableWin name; GFInitMaths table), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?g_FableInvSqrtTable_013CE558@@3PAKA':0x13ce558,
 '__real@3f000000':0x122f59c,
 '__real@40400000':0x122ded4,
}
ROOTS=[
 dict(name='vector2_normalise',source=R/'rebuild/src/compiled/00/a1/C2DVector_Normalise_00a14540.cpp',symbol='?Normalise@C2DVector@@QAEAAV1@XZ',address=0xa14540,size=125,instructions=41,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-vector2-normalise.json',[],
  'C2DVector::Normalise (0x00A14540) through the inline GFFastOneOverSqrt (FableWin name; GFInitMaths table), exact after r')
 print('NATIVE_VECTOR2_NORMALISE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
