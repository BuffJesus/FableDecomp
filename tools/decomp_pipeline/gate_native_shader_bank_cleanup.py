"""Exact typed shader-bank cleanup, including full multi-return native spans."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
import oracle_native_shader_bank_cleanup as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-shader-bank-cleanup-'));env=V.env()
specs=[('00/a6/CShaderDataBank_Cleanup_00a62050.cpp',[('?GetDataFormatVersion@CShaderDataBank@@MAEKXZ',0xa61f90,6),('?CloseShaders@CShaderDataBank@@AAEXXZ',0xa62050,122),('?Close@CShaderDataBank@@UAEXXZ',0xa62110,16),('?NotifyEntryRemoved@CShaderDataBank@@MAEXK@Z',0xa61fc0,63),('?PrepareDataForInclusion@CShaderDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z',0xa62000,32)]),('00/9c/CBankFile_GetEntrySymbol_009cc1d0.cpp',[('?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z',0x9cc1d0,57)])]
targets={'_memmove':0xbfeae6,'?CloseShaders@CShaderDataBank@@AAEXXZ':0xa62050,'?Close@CBankFile@@UAEXXZ':0x9ce360,'??3@YAXPAX@Z':0xbfe9bc,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??_C@_09ONJBBFFA@?$CIUnknown?$CJ?$AA@':0x129a03c}
objects=[];reports=[]
for i,(source,methods) in enumerate(specs):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 from gate_game_component_abi import Coff
 c=Coff(obj)
 for symbol,address,length in methods:
  sec,_=c.symbols[symbol];reports.append(check(obj,symbol,address,length,len(c.sections[sec][1]),targets))
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/tests/integration/NativeShaderBankCleanup_test.cpp')],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in [0,1]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('required provider',omit)
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
 (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,difference,difference//4108,difference%4108))
report.update(providers=reports,omission_checks=2,boundary='Six actual typed providers, including composed Close/CloseShaders. Entry virtual deletion changes vector pointer and size; all deletion orders/full state compared. By-value state ownership cleanup includes null/zero/one/multiple/wrapped reference counts and mutating release callback. Empty/populated bank symbols; real return ABI with recorded string constructors. Only control callback pointer normalized.')
(ROOT/'rebuild/compile-gate/native-shader-bank-cleanup.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip(),report['snapshots'],'snapshots')
