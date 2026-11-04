"""Validate stream-format building and the type-size switch table."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from vertex_stream_building_oracle import compare,SIZES
ROOT=Path(__file__).resolve().parents[2]
SIZE='?GetDataTypeSize@CVertexStreamFormat@@SIKW4EVertexStreamDataType@@@Z'
CTOR='??0CVertexStreamFormat@@QAE@XZ'
ADD='?AddDataElement@CVertexStreamFormat@@QAEXKW4EVertexStreamDataType@@@Z'
TOTAL='?CalculateVertexSize@CVertexStreamFormat@@QBEKXZ'
OVERFLOW='?_M_insert_overflow@?$vector@VCDataElement@CVertexStreamFormat@@V?$allocator@VCDataElement@CVertexStreamFormat@@@_STL@@@_STL@@QAEXPAVCDataElement@CVertexStreamFormat@@ABV34@ABU__false_type@2@I_N@Z'
SPECS=[(0xab3740,SIZE,108),(0xab37f0,CTOR,29),(0xab3810,ADD,83),(0xab37b0,TOTAL,58),(0xab4150,OVERFLOW,259)]
TARGETS={s:a for a,s,n in SPECS};TARGETS.update({'??0CBaseClass@@QAE@XZ':0x99a2d0,'??_7CVertexStreamFormat@@6B@':0x129d578,'_malloc':0xbfea0e,'_free':0xbfea14})

def validate(obj):
 c=Coff(obj);candidate={};native={};records=[];md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
 with pefile.PE(str(EXE)) as pe:
  for entry,symbol,length in SPECS:
   sec,off=c.symbols[symbol];assert off==0;b,rel=c.sections[sec];n=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(b);checked=[]
   for site,(name,kind) in rel.items():
    add=int.from_bytes(b[site:site+4],'little')
    if name in TARGETS:target=TARGETS[name];assert add==0
    else:
     assert symbol==SIZE and kind==6;s,o=c.symbols[name];assert s==sec;target=entry+o
    struct.pack_into('<I',patched,site,(target+add-(entry+site+4 if kind==20 else 0))&0xffffffff);checked.append(dict(offset=site,symbol=name,kind=kind,target=target+add))
   if symbol==SIZE:
    assert len(b)==98 and len(rel)==8 and b[58:60]==b'\x8b\xff' and n[51:52]==b'\x90'
    assert struct.unpack_from('<I',patched,8)[0]==entry+84 and struct.unpack_from('<I',patched,15)[0]==entry+60 and struct.unpack_from('<I',n,8)[0]==entry+52
    assert list(b[84:])==[0,1,2,3,0,0,1,0,4,0,5,1,0,0]
    assert set(rel)=={8,15,60,64,68,72,76,80}
    for code,code_size,table,indexed in [(patched,58,60,True),(n,51,52,False)]:
     ins=list(md.disasm(bytes(code[:code_size]),entry));assert sum(i.size for i in ins)==code_size and ins[-1].mnemonic=='ret';starts={i.address for i in ins}
     for i in range(14):
      address=struct.unpack_from('<I',code,table+4*(code[84+i] if indexed else i))[0];assert address in starts
      instruction=next(md.disasm(bytes(code[address-entry:]),address));assert instruction.mnemonic=='mov' and instruction.operands[0].reg==capstone.x86.X86_REG_EAX and instruction.operands[1].imm==SIZES[i]
    score=near_match.compare(b[:58],n[:51],list(rel),[8]);assert score.near_match;grade='NEAR_MATCH'
   else:
    assert patched==n,(symbol,len(b),len(n));score=near_match.compare(b,n,list(rel),[12] if symbol==CTOR else []);grade='RELOCATION_MATCH'
   candidate[entry]=patched;native[entry]=n;records.append(dict(address='%08x'%entry,grade=grade,built_bytes=len(b),retail_bytes=len(n),similarity=score.to_dict(),relocations=checked))
 return candidate,native,records

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/ab/CVertexStreamFormat_Building_00ab37f0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/ab/CVertexStreamFormat_Building_00ab37f0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-stream-building.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='vertex-stream-building-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 assert all(symbol not in Coff(test).symbols for symbol in [SIZE,CTOR,ADD,TOTAL])
 b,n,records=validate(obj);comparisons=compare(b,n)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(test),str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'VERTEX_STREAM_BUILDING_BEHAVIOR_PASS' in r.stdout,r.stdout+r.stderr
 report=dict(address='00ab3740',grade='NEAR_MATCH',entries=records,behavior=r.stdout.strip(),comparative_scenarios=comparisons,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest())
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('VERTEX_STREAM_BUILDING_PASS comparative='+str(comparisons)+' '+r.stdout.strip())
if __name__=='__main__':main()
