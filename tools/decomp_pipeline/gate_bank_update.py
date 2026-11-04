"""Validate typed bank policies, entry flags and the streaming update pass."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from bank_update_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SPECS=[{'address': '009cba70', 'file': 'CBankFile_AdjustUpdateEntries_009cba70.cpp', 'owner': 'CBankFile', 'method': 'AdjustUpdateEntries', 'symbol': '?AdjustUpdateEntries@CBankFile@@MAEKXZ', 'bytes': 3, 'native_bytes': 3, 'relocations': []}, {'address': '009cbc40', 'file': 'CBankFile_IsDiskFile_009cbc40.cpp', 'owner': 'CBankFile', 'method': 'IsDiskFile', 'symbol': '?IsDiskFile@CBankFile@@MAE_NKK@Z', 'bytes': 5, 'native_bytes': 5, 'relocations': []}, {'address': '00a63340', 'file': 'CFontDataBank_GetDataFormatVersion_00a63340.cpp', 'owner': 'CFontDataBank', 'method': 'GetDataFormatVersion', 'symbol': '?GetDataFormatVersion@CFontDataBank@@MAEKXZ', 'bytes': 6, 'native_bytes': 6, 'relocations': []}, {'address': '00a63ac0', 'file': 'CFontDataBank_IsDiskFile_00a63ac0.cpp', 'owner': 'CFontDataBank', 'method': 'IsDiskFile', 'symbol': '?IsDiskFile@CFontDataBank@@MAE_NKK@Z', 'bytes': 5, 'native_bytes': 5, 'relocations': []}, {'address': '00a5ee90', 'file': 'CStreamingFontDataBank_GetDataFormatVersion_00a5ee90.cpp', 'owner': 'CStreamingFontDataBank', 'method': 'GetDataFormatVersion', 'symbol': '?GetDataFormatVersion@CStreamingFontDataBank@@MAEKXZ', 'bytes': 6, 'native_bytes': 6, 'relocations': []}, {'address': '00a5f9c0', 'file': 'CStreamingFontDataBank_IsDiskFile_00a5f9c0.cpp', 'owner': 'CStreamingFontDataBank', 'method': 'IsDiskFile', 'symbol': '?IsDiskFile@CStreamingFontDataBank@@MAE_NKK@Z', 'bytes': 5, 'native_bytes': 5, 'relocations': []}, {'address': '009cc190', 'file': 'CBankFile_EntryExists_009cc190.cpp', 'owner': 'CBankFile', 'method': 'EntryExists', 'symbol': '?EntryExists@CBankFile@@QBE_NK@Z', 'bytes': 57, 'native_bytes': 57, 'relocations': [[26, ['??ACPackedUIntArray@@QBEKK@Z', 20]]]}, {'address': '009cc300', 'file': 'CBankFile_EntrySetUpdate_009cc300.cpp', 'owner': 'CBankFile', 'method': 'EntrySetUpdate', 'symbol': '?EntrySetUpdate@CBankFile@@QAE_NK_N@Z', 'bytes': 75, 'native_bytes': 75, 'relocations': []}, {'address': '00a5eea0', 'file': 'CStreamingFontDataBank_AdjustUpdateEntries_00a5eea0.cpp', 'owner': 'CStreamingFontDataBank', 'method': 'AdjustUpdateEntries', 'symbol': '?AdjustUpdateEntries@CStreamingFontDataBank@@MAEKXZ', 'bytes': 57, 'native_bytes': 57, 'relocations': [[20, ['?EntryExists@CBankFile@@QBE_NK@Z', 20]], [34, ['?EntrySetUpdate@CBankFile@@QAE_NK_N@Z', 20]]]}]

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/BankUpdate_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-update.json');a=ap.parse_args()
 specs=SPECS+[dict(address='00a629f0',file='CPackedUIntArray_Get_00a629f0.cpp',symbol='??ACPackedUIntArray@@QBEKK@Z',bytes=101)]
 w=Path(tempfile.mkdtemp(prefix='bank-update-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 for i,s in enumerate(specs):
  source=a.source_dir/s['file'] if a.source_dir and i<9 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in specs};targets['?g_PackedUIntMasks_013979E8@@3PAKA']=0x13979e8;candidate={};native={}
 with pefile.PE(str(EXE)) as pe:
  for s,c in zip(specs,coffs):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0 and len(b)==s['bytes'];patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
 comparisons=compare(candidate,native)
 test=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(test);assert all(s['symbol'] not in c.symbols for s in specs);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(10):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'BANK_UPDATE_BEHAVIOR_PASS scenarios=468' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=[dict(s,grade='RELOCATION_MATCH') for s in SPECS],comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=10,boundaries=[])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('BANK_UPDATE_PASS compiled=468 comparative='+str(comparisons)+' providers=10')
if __name__=='__main__':main()
