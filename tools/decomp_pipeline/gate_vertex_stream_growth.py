"""Validate stream-vector growth with its copy and destruction helpers."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from vertex_stream_growth_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
INSERT='?_M_insert_overflow@?$vector@VCStreamInfo@CVertexShaderInput@@V?$allocator@VCStreamInfo@CVertexShaderInput@@@_STL@@@_STL@@QAEXPAVCStreamInfo@CVertexShaderInput@@ABV34@ABU__false_type@2@I_N@Z'
COPY='??$__uninitialized_copy@PAVCStreamInfo@CVertexShaderInput@@PAV12@@_STL@@YIPAVCStreamInfo@CVertexShaderInput@@PAV12@00ABU__false_type@0@@Z'
FILL='??$__uninitialized_fill_n@PAVCStreamInfo@CVertexShaderInput@@IV12@@_STL@@YIPAVCStreamInfo@CVertexShaderInput@@PAV12@IABV12@ABU__false_type@0@@Z'
DESTROY='??$__destroy_aux@PAVCStreamInfo@CVertexShaderInput@@@_STL@@YIXPAVCStreamInfo@CVertexShaderInput@@0ABU__false_type@0@@Z'
SPECS=[(0xab4540,INSERT,242),(0xab4450,COPY,61),(0xab4490,FILL,57),(0xa5e900,DESTROY,55)]
TARGETS={symbol:entry for entry,symbol,length in SPECS};TARGETS.update({'_malloc':0xbfea0e,'_free':0xbfea14,'??1CBaseClass@@UAE@XZ':0x99a2e0,'??_7CVertexStreamFormat@@6B@':0x129d578,'??0?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAE@ABV01@@Z':0xa5e0a0})
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/ab/CVertexShaderInput_StreamVectorGrowth_00ab4540.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/ab/CVertexShaderInput_StreamVectorGrowth_00ab4540_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-stream-growth.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-stream-growth-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);f=Coff(test);assert INSERT not in f.symbols
 candidate={};native={};records=[];md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
 with pefile.PE(str(EXE)) as pe:
  for entry,symbol,length in SPECS:
   sec,off=c.symbols[symbol];assert off==0;built,relocs=c.sections[sec];original=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
   native_abs=[]
   for site,(name,kind) in sorted(relocs.items()):
    assert built[site:site+4]==bytes(4);target=TARGETS[name]
    if kind==6:assert struct.unpack_from('<I',original,site)[0]==target;native_abs.append(site)
    else:assert kind==20 and original[site-1]==0xe8 and entry+site+4+struct.unpack_from('<i',original,site)[0]==target
    struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
   score=near_match.compare(built,original,list(relocs),native_abs);assert patched==original;grade='RELOCATION_MATCH'
   candidate[entry]=patched;native[entry]=original;records.append(dict(address='%08x'%entry,grade=grade,built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked))
 comparative=compare(candidate,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'VERTEX_STREAM_GROWTH_BEHAVIOR_PASS scenarios=870' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparative,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('VERTEX_STREAM_GROWTH_PASS compiled=870 comparative='+str(comparative))
if __name__=='__main__':main()
