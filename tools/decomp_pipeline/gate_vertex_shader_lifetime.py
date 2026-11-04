"""Validate vertex shader handles, data ownership and compiler deleting destructors."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from vertex_shader_lifetime_oracle import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(CTOR,'??0CVertexShader@@QAE@XZ',16),
       (COUNT,'?CountReferences@CVertexShader@@EAEKXZ',7),
       (DATA_CTOR,'??0CVertexShaderData@@AAE@PAUIDirect3DVertexDeclaration9@@PAUIDirect3DVertexShader9@@PBU_D3DVERTEXELEMENT9@@ABVCCharString@@3@Z',48),
       (DATA_DTOR,'??1CVertexShaderData@@EAE@XZ',43),
       (SCALAR,'??_GCVertexShader@@UAEPAXI@Z',55),
       (DATA_SCALAR,'??_GCVertexShaderData@@EAEPAXI@Z',64)]
def vtable_proof(coff,pe):
 tables=[('CVertexShader',0x129b3d0,['??_ECVertexShader@@UAEPAXI@Z','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','?CountReferences@CVertexShader@@EAEKXZ'],[SCALAR,0x987cf0,COUNT]),
         ('CShaderBase',0x129a030,['??_ECShaderBase@@MAEPAXI@Z','?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ','__purecall'],[0x987d10,0x987cf0,0xbfea08]),
         ('CVertexShaderData',0x129da50,['??_ECVertexShaderData@@EAEPAXI@Z'],[DATA_SCALAR])]
 raw=coff.data;ptr,n=struct.unpack_from('<II',raw,8);aliases={};i=0
 while i<n:
  at=ptr+18*i;storage,aux=struct.unpack_from('<BB',raw,at+16)
  if storage==105:
   assert aux==1;target,kind=struct.unpack_from('<II',raw,at+18);assert kind==2
   aliases[coff.indices[i]]=coff.indices[target]
  i+=1+aux
 proof=[]
 for owner,address,names,targets in tables:
  coff.table('??_7'+owner+'@@6B@',names)
  actual=list(struct.unpack('<'+'I'*len(targets),pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,len(targets)*4)));assert actual==targets
  vector=names[0];scalar=vector.replace('??_E','??_G');assert aliases[vector]==scalar and scalar in coff.symbols
  proof.append(dict(owner=owner,address=address,slots=names,targets=targets,deleting_destructor_alias=scalar))
 return proof

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a6/CVertexShader_Lifetime_00a62fb0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a6/CVertexShader_Lifetime_00a62fb0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-shader-lifetime.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='vertex-shader-lifetime-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[];codes={};natives={}
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length,(hex(entry),len(built))
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   value=target if kind==6 else (target-entry-site-4)&0xffffffff;assert int.from_bytes(native[site:site+4],'little')==value
   struct.pack_into('<I',patched,site,value);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
  assert patched==native,hex(entry);codes[entry]=patched;natives[entry]=native
  proof.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH' if relocs else 'MATCH',bytes=length,relocations=checked))
 cases=0
 for case in scenarios():
  assert execute(codes,*case)==execute(natives,*case),case;cases+=1
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'VERTEX_SHADER_LIFETIME_BEHAVIOR_PASS scenarios=5632' in r.stdout,r.stdout+r.stderr
 result=dict(vtables=vtable_proof(coff,pe),entries=proof,behavior=r.stdout.strip(),comparative_scenarios=cases,source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('VERTEX_SHADER_LIFETIME_PASS compiled=5632 comparative='+str(cases))
if __name__=='__main__':main()

