"""Validate creation of vertex shader COM interfaces and their counted data owner."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from gate_vertex_shader_lifetime import vtable_proof
from pe_oracle import EXE
from vertex_shader_create_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='??0CVertexShader@@AAE@ABV?$CArrayVector@K@@PBU_D3DVERTEXELEMENT9@@ABVCCharString@@2@Z'
TARGETS={'??_7CVertexShader@@6B@':0x129b3d0,'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'??2@YAPAXI@Z':0xbfea1a,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CVertexShaderData@@6B@':0x129da50}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a6/CVertexShader_Lifetime_00a62fb0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a6/CVertexShader_Create_00a63020_test.cpp');ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-shader-create.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-shader-create-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);f=Coff(test);assert SYMBOL not in f.symbols;sec,off=c.symbols[SYMBOL];assert off==0;built,relocs=c.sections[sec];patched=bytearray(built);checked=[]
 with pefile.PE(str(EXE)) as pe:native=pe.get_data(0xa63020-pe.OPTIONAL_HEADER.ImageBase,167);tables=vtable_proof(c,pe)
 for site,(name,kind) in sorted(relocs.items()):
  assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name];struct.pack_into('<I',patched,site,target if kind==6 else (target-0xa63020-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
 md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;absolute=[];native_targets=[];com_slots=[]
 for i in md.disasm(native,0xa63020):
  if i.mnemonic=='mov' and i.operands[-1].type==capstone.x86_const.X86_OP_IMM and i.operands[-1].imm in (0x129b3d0,0x129da50):absolute.append(i.address-0xa63020+i.imm_offset);native_targets.append(i.operands[-1].imm)
  elif i.mnemonic=='call':
   if i.operands[0].type==capstone.x86_const.X86_OP_IMM:native_targets.append(i.operands[0].imm)
   else:com_slots.append(i.operands[0].mem.disp)
 assert native_targets==[r['target'] for r in checked] and com_slots==[0x16c,0x158]
 score=near_match.compare(built,native,list(relocs),absolute);assert score.near_match and len(built)==168
 comparative=compare(patched,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'VERTEX_SHADER_CREATE_BEHAVIOR_PASS scenarios=1728' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(behavior=r.stdout.strip(),comparative_scenarios=comparative,similarity=score.to_dict(),relocations=checked,vtables=tables,com_slots=com_slots,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('VERTEX_SHADER_CREATE_PASS compiled=1728 comparative='+str(comparative)+' similarity='+str(score.opcode_similarity))
if __name__=='__main__':main()
