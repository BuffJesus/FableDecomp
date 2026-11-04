"""Validate pointer fill-insert and its complete native growth/fill chain."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from vertex_declaration_fill_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
INSERT='?_M_fill_insert@?$vector@U_D3DVERTEXELEMENT9@@V?$allocator@U_D3DVERTEXELEMENT9@@@_STL@@@_STL@@QAEXPAU_D3DVERTEXELEMENT9@@IABU3@@Z'
COPY='??$__uninitialized_copy@PAU_D3DVERTEXELEMENT9@@PAU1@@_STL@@YIPAU_D3DVERTEXELEMENT9@@PAU1@00ABU__false_type@0@@Z'
BACKWARD='?CopyVertexDeclarationsBackward@_STL@@YIPAU_D3DVERTEXELEMENT9@@PAU2@00ABU__false_type@1@@Z'
FILL='?FillVertexDeclarations@_STL@@YIXPAU_D3DVERTEXELEMENT9@@0ABU2@@Z'
CONSTRUCT='?ConstructVertexDeclarations@_STL@@YIPAU_D3DVERTEXELEMENT9@@PAU2@IABU2@@Z'
OVERFLOW='?_M_insert_overflow@?$vector@U_D3DVERTEXELEMENT9@@V?$allocator@U_D3DVERTEXELEMENT9@@@_STL@@@_STL@@QAEXPAU_D3DVERTEXELEMENT9@@ABU3@ABU__false_type@2@I_N@Z'
SPECS=[(0xab4260,INSERT,280),(0xab3f00,COPY,44),(0xab3e60,BACKWARD,43),(0xab3cd0,FILL,37),(0xab3f30,CONSTRUCT,40),(0xab3ff0,OVERFLOW,259)]
TARGETS={symbol:entry for entry,symbol,length in SPECS};TARGETS.update({'_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6})
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/ab/CVertexDeclaration_FillInsert_00ab4260.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/ab/CVertexDeclaration_FillInsert_00ab4260_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-declaration-fill.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-declaration-fill-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);f=Coff(test);assert all(symbol not in f.symbols for entry,symbol,length in SPECS)
 candidate={};native={};records=[];md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
 with pefile.PE(str(EXE)) as pe:
  for entry,symbol,length in SPECS:
   sec,off=c.symbols[symbol];assert off==0;built,relocs=c.sections[sec];original=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
   for site,(name,kind) in sorted(relocs.items()):
    assert kind==20 and built[site:site+4]==b'\0'*4;target=TARGETS[name];struct.pack_into('<I',patched,site,(target-entry-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
   score=near_match.compare(built,original,list(relocs),[])
   if entry==0xab4260:
    assert patched==original
    calls=[i.operands[0].imm for i in md.disasm(original,entry) if i.mnemonic=='call' and i.operands[0].type==capstone.x86_const.X86_OP_IMM];assert calls==[r['target'] for r in checked];grade='RELOCATION_MATCH'
   else:grade='RELOCATION_MATCH' if patched==original else 'CHAIN_ONLY'
   candidate[entry]=patched;native[entry]=original;records.append(dict(address='%08x'%entry,grade=grade,built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked))
 comparative=compare(candidate,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'DECLARATION_FILL_BEHAVIOR_PASS scenarios=2736' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparative,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('DECLARATION_FILL_PASS compiled=2736 comparative='+str(comparative))
if __name__=='__main__':main()
