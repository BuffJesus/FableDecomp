"""Validate native shader-resource creation and its linked STLport growth chain."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from shader_resource_create_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
CREATE='?CreateResource@CShaderRenderManager@@QAE?AV?$CCountedPointer@VCShaderResource@@@@XZ'
DELETE='?DeleteData@?$CCountedPointer@VCShaderResource@@@@CIXPAX@Z'
OVERFLOW='?_M_insert_overflow@?$vector@V?$CCountedPointer@VCShaderResource@@@@V?$allocator@V?$CCountedPointer@VCShaderResource@@@@@_STL@@@_STL@@QAEXPAV?$CCountedPointer@VCShaderResource@@@@ABV3@ABU__false_type@2@I_N@Z'
COPY='??$__uninitialized_copy@PAV?$CCountedPointer@VCShaderResource@@@@PAV1@@_STL@@YIPAV?$CCountedPointer@VCShaderResource@@@@PAV1@00ABU__false_type@0@@Z'
FILL='??$__uninitialized_fill_n@PAV?$CCountedPointer@VCShaderResource@@@@IV1@@_STL@@YIPAV?$CCountedPointer@VCShaderResource@@@@PAV1@IABV1@ABU__false_type@0@@Z'
DESTROY='??$__destroy_aux@PAV?$CCountedPointer@VCShaderResource@@@@@_STL@@YIXPAV?$CCountedPointer@VCShaderResource@@@@0ABU__false_type@0@@Z'
SPECS=[(0x98b860,CREATE,205),(0x98ba10,DELETE,11)]
LINKED=[(0x98dc30,OVERFLOW,218),(0x98bff0,COPY,50),(0x98c030,FILL,46),(0x98cee0,DESTROY,76)]
TARGETS={'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'??0CShaderResource@@AAE@XZ':0xa5d870,'?Initialise@CShaderResource@@AAEXAAVCShaderRenderManager@@@Z':0xa5d4d0,DELETE:0x98ba10,OVERFLOW:0x98dc30,COPY:0x98bff0,FILL:0x98c030,DESTROY:0x98cee0,'_malloc':0xbfea0e,'_free':0xbfea14}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/98/CShaderRenderManager_CreateResource_0098b860.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/98/CShaderRenderManager_CreateResource_0098b860_test.cpp');ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-resource-create.json');a=ap.parse_args()
 work=Path(tempfile.mkdtemp(prefix='shader-resource-create-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=work/'candidate.obj';test=work/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);fixture=Coff(test);assert CREATE not in fixture.symbols
 pe=pefile.PE(str(EXE));candidate={};native={};records=[];md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
 for entry,symbol,length in SPECS+LINKED:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0
  original=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name];value=target if kind==6 else (target-entry-site-4)&0xffffffff
   struct.pack_into('<I',patched,site,value);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
  retail_sites=set(near_match.retail_relocations('%08x'%entry,length))
  if entry==0x98b860:
   assert original[63:66]==bytes.fromhex('c74004') and int.from_bytes(original[66:70],'little')==0x98ba10
   retail_sites.add(66)
  score=near_match.compare(built,original,list(relocs),retail_sites)
  if entry==0x98b860:
   assert score.near_match and len(built)==211
   absolute=retail_sites;sites=set(absolute)
   for ins in md.disasm(original,entry):
    if ins.mnemonic=='call' and ins.operands[0].type==capstone.x86_const.X86_OP_IMM:sites.add(ins.address-entry+ins.imm_offset)
   native_targets=[int.from_bytes(original[s:s+4],'little') if s in absolute else entry+s+4+int.from_bytes(original[s:s+4],'little',signed=True) for s in sorted(sites)]
   assert native_targets==[r['target'] for r in checked]
   grade='NEAR_MATCH'
  else:
   assert patched==original,hex(entry);grade='RELOCATION_MATCH' if relocs else 'MATCH'
  candidate[entry]=patched;native[entry]=original;records.append(dict(address='%08x'%entry,grade=grade,built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked))
 # The previously recovered 10-byte Initialize boundary is executed directly.
 candidate[0xa5d4d0]=native[0xa5d4d0]=pe.get_data(0xa5d4d0-pe.OPTIONAL_HEADER.ImageBase,10)
 comparative=compare(candidate,native)
 exe=work/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'SHADER_RESOURCE_CREATE_BEHAVIOR_PASS scenarios=289' in r.stdout,r.stdout+r.stderr
 result=dict(entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparative,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest())
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n');print('SHADER_RESOURCE_CREATE_PASS compiled=289 comparative='+str(comparative)+' similarity='+str(records[0]['similarity']['opcode_similarity']))
if __name__=='__main__':main()
