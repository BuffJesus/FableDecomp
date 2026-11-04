"""Validate CMemoryDataOutputStream's SetPosition, GetPosition, Close, destructor and scalar deleting destructor."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from memory_output_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
def S(file,address,symbol,size):return dict(file=file,address=address,symbol=symbol,native_bytes=size)
SPECS=[
 S('CMemoryDataOutputStream_SetPosition_00994b10.cpp','00994b10','?SetPosition@CMemoryDataOutputStream@@UAEXK@Z',46),
 S('CMemoryDataOutputStream_GetPosition_009945b0.cpp','009945b0','?GetPosition@CMemoryDataOutputStream@@UBEKXZ',4),
 S('CMemoryDataOutputStream_Close_009945c0.cpp','009945c0','?Close@CMemoryDataOutputStream@@UAEXXZ',1),
 S('CMemoryDataOutputStream_Destructor_00996b30.cpp','00996b30','??1CMemoryDataOutputStream@@UAE@XZ',47)]
EXTERNAL={'__TI2?AVCBBBFileException@@':0x1369380,'__TI1?AVCBBBFileException@@':0x1369380,'__CxxThrowException@8':0xbfeb84,'??_7CMemoryDataOutputStream@@6B@':0x129a788,'??_7CDataStream@@6B@':0x129a69c,
 '_free':0xbfea14,'??1CCharString@@QAE@XZ':0x99eae0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc}
# The destructor's unit emits the scalar deleting destructor (with the vtable) and expands the destructor in it.
COMDAT=[dict(file='CMemoryDataOutputStream_Destructor_00996b30.cpp',address='009973c0',symbol='??_GCMemoryDataOutputStream@@UAEPAXI@Z',native_bytes=68)]
# Landed providers the fixture runs with (not omission-checked here).
SUPPORT=['00/99/CMemoryDataOutputStream_CMemoryDataOutputStream_00996af0.cpp']
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/MemoryOutput_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/memory-output.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='memory-output-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS+COMDAT};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  owners=[coffs[[x['file'] for x in SPECS].index(s['file'])] for s in COMDAT]
  for s,c in zip(SPECS+COMDAT,coffs+owners):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0 and len(b)==s['native_bytes'],(s['file'],len(b));patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
   entries.append(dict(s,bytes=len(b),relocations=sorted([pos,list(v)] for pos,v in rel.items()),grade='RELOCATION_MATCH' if rel else 'MATCH'))
 comparisons=compare(candidate,native)
 test=compile(a.test,'test.obj');tc=Coff(test);assert all(s['symbol'] not in tc.symbols for s in SPECS);objects.append(test)
 support=[compile(ROOT/'rebuild/src/compiled'/p,'support%d.obj'%i) for i,p in enumerate(SUPPORT)]
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]+[str(o) for o in support]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'MEMORY_OUTPUT_BEHAVIOR_PASS scenarios=48' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),
  boundaries=[],covered=[c['symbol']+' from '+c['file'] for c in COMDAT],deferred=['_STL::__copy 0x009B1620 (exact; reached only from the deferred CParentDefClassBase::RemoveSubDef)'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('MEMORY_OUTPUT_PASS compiled=48 comparative=%d providers=%d'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
