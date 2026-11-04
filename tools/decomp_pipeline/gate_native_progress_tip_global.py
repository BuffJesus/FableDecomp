"""Actual progress-tip globals, CRT workers and string lifetimes."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off,read_virtual_data
import oracle_native_progress_tip_global as oracle
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-progress-tip-global-'));env=V.env()
paths=['rebuild/src/compiled/01/21/Global_ProgressTipText_0121a3e0.cpp','rebuild/src/compiled/01/21/Global_ProgressTipTitle_0121a400.cpp','rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp','rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp','rebuild/src/compiled/01/3b/WideStringInstanceCount_013bca20.cpp','rebuild/tests/integration/NativeProgressTipGlobal_test.cpp']
objects=[];originals=[]
for i,path in enumerate(paths):
 obj=w/(str(i)+'.obj');options=['/Dfree=TipFree']if i in (2,3) else []
 r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)]+options,env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 if options:
  original=w/(str(i)+'-original.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr
  a,b=Coff(original),Coff(obj)
  for symbol,(sec,off)in a.symbols.items():
   if not symbol.startswith('?'):continue
   other,otheroff=b.symbols[symbol];body,rel=a.sections[sec];otherbody,otherrel=b.sections[other];assert off==otheroff and body==otherbody and {p:(t.replace('_free','_TipFree'),k)for p,(t,k)in rel.items()}==otherrel
 else:original=obj
 originals.append(original)
proof=[];raw=EXE.read_bytes();sections=pe_sections(raw)
for i,glob,init,fini,address in [(0,'GResetProgressTipText',0x121a3e0,0x1228c40,0x13b89d8),(1,'GResetProgressTipTitleText',0x121a400,0x1228c50,0x13b89dc)]:
 symbol='?'+glob+'@@3VCWideString@@A';targets={symbol:address,'_$E2':fini,'_atexit':0x4012bc,'??0CWideString@@QAE@PBG@Z':0x99b6b0,'??1CWideString@@QAE@XZ':0x99b510,'??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c}
 proof.extend([check(originals[i],'_$E1',init,27,5,targets),check(originals[i],'_$E2',fini,10,2,targets)])
 c=Coff(originals[i]);assert coff_data(c,symbol,4)==bytes(4);assert read_virtual_data(raw,address,4)==bytes(4)
 sec,off=c.symbols['_$S3'];assert c.sections[sec][1]=={0:('_$E1',6)}
 sec,off=c.symbols['??_C@_11LOCGONAA@?$AA?$AA@'];assert c.sections[sec][0][off:off+2]==bytes(2);o=va_to_off(sections,0x122d70c);assert raw[o:o+2]==bytes(2)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
reports=[]
for mode in range(4):
 r=subprocess.run([str(exe),str(w/'actual.bin'),str(mode)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
 expected,report=oracle.collect(mode);actual=(w/'actual.bin').read_bytes()
 if actual!=expected:
  (w/'expected.bin').write_bytes(expected);difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)));raise AssertionError((w,mode,difference,difference//4116,difference%4116,len(actual),len(expected)))
 reports.append(report)
report=dict(providers=proof,modes=4,snapshots=sum(r['snapshots']for r in reports),omission_checks=5,source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()for p in paths},boundary='Real CRT initializes both empty literal globals and registers reverse-order destruction. Actual string constructor/destructor and instance counter, empty/unique/shared/nonpositive-ref shutdown. Full4KiB arena and three global words. Free/delete recorded; unexpected string allocation fails. No normalized bytes.')
(ROOT/'rebuild/compile-gate/native-progress-tip-global.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_PROGRESS_TIP_GLOBAL_PASS',report['snapshots'],'snapshots')
