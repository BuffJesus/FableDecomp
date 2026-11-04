"""Validate nested stream-element vector copy construction and assignment."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from vertex_stream_elements_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
ASSIGN='??4?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z'
CTOR='??0?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAE@ABV01@@Z'
COPY='?CopyVertexStreamElements@@YIPAVCDataElement@CVertexStreamFormat@@PBV12@0PAV12@ABU__false_type@_STL@@@Z'
CONSTRUCT='??$__uninitialized_copy@PBVCDataElement@CVertexStreamFormat@@PAV12@@_STL@@YIPAVCDataElement@CVertexStreamFormat@@PBV12@0PAV12@ABU__false_type@0@@Z'
ALLOC='?_M_allocate_and_copy@?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAEPAVCDataElement@CVertexStreamFormat@@IPBV34@0@Z'
SPECS=[(0xa5e0a0,CTOR,118),(0xa5e4d0,ASSIGN,198),(0xa5df30,COPY,38),(0xa5dde0,CONSTRUCT,44),(0xa5dee0,ALLOC,78)]
TARGETS={symbol:entry for entry,symbol,length in SPECS};TARGETS.update({'_malloc':0xbfea0e,'_free':0xbfea14})
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CVertexStreamFormat_ElementVectors_00a5e0a0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CVertexStreamFormat_ElementVectors_00a5e0a0_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-stream-elements.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-stream-elements-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
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
   if entry==0xa5e4d0:
    assert score.near_match and len(built)==186
    calls=[ins.operands[0].imm for ins in md.disasm(original,entry) if ins.mnemonic=='call' and ins.operands[0].type==capstone.x86_const.X86_OP_IMM]
    assert calls==[item['target'] for item in checked];grade='NEAR_MATCH'
   elif entry==0xa5df30:
    # VC7.1 specializes this static helper: destination in EAX, no stack args.
    # Its address is used only to compare each complete chain, not as an ABI claim.
    assert not relocs and len(built)==37 and built[-1:]==b'\xc3' and original[-3:]==b'\xc2\x08\x00';grade='PRIVATE_ABI_VARIANT'
   else:assert patched==original;grade='RELOCATION_MATCH' if relocs else 'MATCH'
   candidate[entry]=patched;native[entry]=original;records.append(dict(address='%08x'%entry,grade=grade,built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked))
 comparative=compare(candidate,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'VERTEX_STREAM_ELEMENTS_BEHAVIOR_PASS scenarios=1254' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparative,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('VERTEX_STREAM_ELEMENTS_PASS compiled=1254 comparative='+str(comparative))
if __name__=='__main__':main()
