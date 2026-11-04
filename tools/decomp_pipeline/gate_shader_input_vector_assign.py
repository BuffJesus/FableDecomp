"""Validate typed stream-vector assignment with its exact copy/destruction chain."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from shader_input_vector_assign_oracle import compare
from gate_shader_resource_input import TARGETS as ASSIGNMENTS
ROOT=Path(__file__).resolve().parents[2]
ASSIGN=next(k for k,v in ASSIGNMENTS.items() if v==0xa5e9c0)
COPY='??$__uninitialized_copy@PBVCStreamInfo@CVertexShaderInput@@PAV12@@_STL@@YIPAVCStreamInfo@CVertexShaderInput@@PBV12@0PAV12@ABU__false_type@0@@Z'
DESTROY='??$__destroy_aux@PAVCStreamInfo@CVertexShaderInput@@@_STL@@YIXPAVCStreamInfo@CVertexShaderInput@@0ABU__false_type@0@@Z'
OVERWRITE='??$__copy@PBVCStreamInfo@CVertexShaderInput@@PAV12@H@_STL@@YIPAVCStreamInfo@CVertexShaderInput@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z'
SPECS=[(0xa5e9c0,ASSIGN,347),(0xa5e7f0,COPY,61),(0xa5e830,OVERWRITE,75),(0xa5e900,DESTROY,55)]
TARGETS={ASSIGN:0xa5e9c0,COPY:0xa5e7f0,OVERWRITE:0xa5e830,DESTROY:0xa5e900,'_malloc':0xbfea0e,'_free':0xbfea14,'??1CBaseClass@@UAE@XZ':0x99a2e0,'??_7CVertexStreamFormat@@6B@':0x129d578,'??0?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAE@ABV01@@Z':0xa5e0a0,'??4?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z':0xa5e4d0}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CVertexShaderInput_VectorAssignment_00a5e9c0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CVertexShaderInput_VectorAssignment_00a5e9c0_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-input-vector-assign.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='shader-input-vector-assign-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);f=Coff(test);assert all(symbol not in f.symbols for entry,symbol,length in SPECS)
 candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  for entry,symbol,length in SPECS:
   sec,off=c.symbols[symbol];assert off==0;built,relocs=c.sections[sec];original=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
   for site,(name,kind) in sorted(relocs.items()):
    assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name];struct.pack_into('<I',patched,site,target if kind==6 else (target-entry-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
   assert patched==original,hex(entry);candidate[entry]=patched;native[entry]=original;records.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH',length=length,relocations=checked))
  sec,off=c.symbols['??_7CVertexStreamFormat@@6B@'];table,relocs=c.sections[sec];assert len(table)==4 and relocs=={0:('??_ECVertexStreamFormat@@UAEPAXI@Z',6)};assert int.from_bytes(pe.get_data(0x129d578-pe.OPTIONAL_HEADER.ImageBase,4),'little')==0xa5e670
 comparative=compare(candidate,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'SHADER_INPUT_VECTOR_ASSIGN_BEHAVIOR_PASS scenarios=540' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparative,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('SHADER_INPUT_VECTOR_ASSIGN_PASS compiled=540 comparative='+str(comparative))
if __name__=='__main__':main()
