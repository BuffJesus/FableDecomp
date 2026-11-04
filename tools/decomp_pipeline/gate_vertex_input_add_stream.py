"""Validate AddStream ownership and post-copy declaration generation."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,near_match,check_one
from pe_oracle import EXE
from gate_game_component_abi import Coff
from vertex_input_add_stream_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?AddStream@CVertexShaderInput@@QAEXKABVCVertexStreamFormat@@_N@Z'
COPY='??0?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAE@ABV01@@Z'
OVERFLOW='?_M_insert_overflow@?$vector@VCStreamInfo@CVertexShaderInput@@V?$allocator@VCStreamInfo@CVertexShaderInput@@@_STL@@@_STL@@QAEXPAVCStreamInfo@CVertexShaderInput@@ABV34@ABU__false_type@2@I_N@Z'
TARGETS={'??_7CVertexStreamFormat@@6B@':0x129d578,COPY:0xa5e0a0,OVERFLOW:0xab4540,'_free':0xbfea14,'??1CBaseClass@@UAE@XZ':0x99a2e0,'?GenerateD3DDeclaration@CVertexShaderInput@@AAEXXZ':0xab3870}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/ab/CVertexShaderInput_AddStream_00ab3b40.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/ab/CVertexShaderInput_AddStream_00ab3b40_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-input-add-stream.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-input-add-stream-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);assert SYMBOL not in Coff(test).symbols;sec,off=c.symbols[SYMBOL];assert off==0;b,rel=c.sections[sec];assert len(b)==160;patched=bytearray(b);checked=[]
 with pefile.PE(str(EXE)) as pe:n=pe.get_data(0xab3b40-pe.OPTIONAL_HEADER.ImageBase,160)
 assert set(rel)=={31,36,74,79,107,121,133,148}
 for site,(symbol,kind) in rel.items():
  target=TARGETS[symbol];assert b[site:site+4]==bytes(4)
  if site in [31,74]:assert kind==6 and struct.unpack_from('<I',n,site)[0]==target
  else:assert kind==20 and n[site-1]==0xe8 and 0xab3b40+site+4+struct.unpack_from('<i',n,site)[0]==target
  struct.pack_into('<I',patched,site,(target-(0xab3b40+site+4 if kind==20 else 0))&0xffffffff);checked.append(dict(offset=site,symbol=symbol,kind=kind,target=target))
 # Only the address of the unused STLport trait tag differs. It remains a
 # real compiler-allocated temporary in both bodies; it is never dereferenced.
 assert [(i,x,y) for i,(x,y) in enumerate(zip(patched,n)) if x!=y]==[(96,40,48)]
 assert b[93:97]==b'\x8d\x54\x24\x28' and n[93:97]==b'\x8d\x54\x24\x30'
 score=near_match.compare(b,n,list(rel),[31,74]);assert score.near_match
 comparative=compare(patched,n)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(test),str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'VERTEX_INPUT_ADD_STREAM_BEHAVIOR_PASS scenarios=552' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(behavior=r.stdout.strip(),comparative_scenarios=comparative,grade='NEAR_MATCH',similarity=score.to_dict(),relocations=checked,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('VERTEX_INPUT_ADD_STREAM_PASS compiled=552 comparative='+str(comparative))
if __name__=='__main__':main()
