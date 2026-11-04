"""Validate typed bank policies, entry flags and the streaming update pass."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from bank_async_cleanup_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SPECS=[{'file': 'CBankFileAsync_Close_009d58a0.cpp', 'address': '009d58a0', 'symbol': '?Close@CBankFileAsync@@UAEXXZ', 'bytes': 36, 'native_bytes': 36, 'relocations': [[24, ['?Close@CThreadedFile@@QAEXXZ', 20]], [32, ['?Close@CBankFile@@UAEXXZ', 20]]], 'grade': {'retail_instructions': 12, 'built_instructions': 12, 'instruction_delta': 0.0, 'opcode_similarity': 1.0, 'complete_decode': True, 'near_match': True}}, {'file': 'CBankFileAsync_ProcessDisposalList_009d6730.cpp', 'address': '009d6730', 'symbol': '?ProcessDisposalList@CBankFileAsync@@QAEXXZ', 'bytes': 164, 'native_bytes': 164, 'relocations': [[23, ['??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z', 20]], [71, ['?Abort@CBankFileAsyncData@@QAE_NXZ', 20]], [124, ['??3@YAXPAX@Z', 20]], [139, ['_free', 20]], [153, ['??1CCriticalSectionLock@@QAE@XZ', 20]]], 'grade': {'retail_instructions': 63, 'built_instructions': 63, 'instruction_delta': 0.0, 'opcode_similarity': 1.0, 'complete_decode': True, 'near_match': True}}]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/BankAsyncCleanup_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-async-cleanup.json');a=ap.parse_args()
 specs=SPECS
 w=Path(tempfile.mkdtemp(prefix='bank-async-cleanup-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 for i,s in enumerate(specs):
  source=a.source_dir/s['file'] if a.source_dir and i<2 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in specs};targets.update({'?Close@CThreadedFile@@QAEXXZ':0x98ec40,'?Close@CBankFile@@UAEXXZ':0x9ce360,'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,'??1CCriticalSectionLock@@QAE@XZ':0xa629a0,'?Abort@CBankFileAsyncData@@QAE_NXZ':0x9d6350,'??3@YAXPAX@Z':0xbfe9bc,'_free':0xbfea14});candidate={};native={}
 with pefile.PE(str(EXE)) as pe:
  for s,c in zip(specs,coffs):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0 and len(b)==s['bytes'];patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
 comparisons=compare(candidate,native)
 test=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(test);assert all(s['symbol'] not in c.symbols for s in specs);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(2):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'BANK_ASYNC_CLEANUP_BEHAVIOR_PASS scenarios=1224' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=[dict(s,grade='RELOCATION_MATCH') for s in SPECS],comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=2,boundaries=['critical-section lock','async read Abort','counted-pointer delete callback','operator delete/free','threaded/base Close'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('BANK_ASYNC_CLEANUP_PASS compiled=1224 comparative='+str(comparisons)+' providers=2')
if __name__=='__main__':main()
