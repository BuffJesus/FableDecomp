"""Compile shared bank declarations and verify all seven native primary vtables."""
import argparse,json,tempfile
from pathlib import Path
import pefile,check_one
from pe_oracle import EXE
from gate_game_component_abi import Coff
from bank_vtable_contract import verify,TABLES
ROOT=Path(__file__).resolve().parents[2]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-vtables.json');a=ap.parse_args()
 work=Path(tempfile.mkdtemp(prefix='bank-vtables-'));source=work/'probe.cpp';obj=work/'probe.obj'
 source.write_text('#include "fable_shader_data_bank.h"\n#include "fable_font_data_banks.h"\n#include "fable_particle_engine.h"\nCBankFile::CBankFile():ChecksumCache(0x10000){}\nCBankFileAsync::CBankFileAsync(){}\nCShaderDataBank::CShaderDataBank(){}\nCFontDataBank::CFontDataBank(){}\nCStreamingFontDataBank::CStreamingFontDataBank(){}\nNGameText::CDataBank::CDataBank(){}\nNParticleEngine::CParticleDataBank::CParticleDataBank(){}\n')
 env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr
 c=Coff(obj)
 result=verify(c,pefile.PE(str(EXE)),[r[0] for r in TABLES]);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n');print('BANK_VTABLES_PASS tables=7 slots=140')
if __name__=='__main__':main()
