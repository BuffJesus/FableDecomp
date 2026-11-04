"""Fresh VC71 parity gate: NParticleEngine::CParticleEmitterDatabase ctor (0x00AE8BF0) and OpenBank (0x00AE8980), exact after relocation; members retyped to the Ego_r names (UsingSymbolMap, Emitters, EmitterBank, CurrentHandle, ParFilename, HeaderFilename)"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??0CParticleDataBank@NParticleEngine@@QAE@XZ':0xaf1670,
 '??0CSymbolMap@@QAE@XZ':0x9e5200,
 '??2@YAPAXI@Z':0xbfea1a,
 '?CreateSymbolMap@CBankFile@@QBEXAAVCSymbolMap@@_N@Z':0x9cc530,
 '?Reset@?$CCountedPointer@VCParticleDataBank@NParticleEngine@@@@QAEXPAVCParticleDataBank@NParticleEngine@@@Z':0xae93e0,
}
ROOTS=[
 dict(name='emitter_database_ctor',source=R/'rebuild/src/compiled/00/ae/CParticleEmitterDatabase_Ctor_00ae8bf0.cpp',symbol='??0CParticleEmitterDatabase@NParticleEngine@@QAE@XZ',address=0xae8bf0,size=55,instructions=18,relocations=3),
 dict(name='emitter_database_open_bank',source=R/'rebuild/src/compiled/00/ae/NParticleEngineCParticleEmitterDatabase_OpenBank_00ae8980.cpp',symbol='?OpenBank@CParticleEmitterDatabase@NParticleEngine@@QAEXABVCCharString@@@Z',address=0xae8980,size=72,instructions=29,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-emitter-database.json',[],
  'NParticleEngine::CParticleEmitterDatabase ctor (0x00AE8BF0) and OpenBank (0x00AE8980), exact after relocation; members r')
 print('NATIVE_EMITTER_DATABASE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
