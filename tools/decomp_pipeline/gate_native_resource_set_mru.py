import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-resource-set-mru-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CResource_SetAsMRU_009fc4a0.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/resource_set_mru_candidate_20261009.cpp'
symbol='?SetAsMRU@CResource@@QAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9fc4a0,80,0,{})
(w/'resource_set_mru_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9fc4a0,80)))+'};\n');(w/'resource_set_mru_aliases.h').write_text('#pragma comment(linker,"/alternatename:@SetMRU@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeResourceSetMRU_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=1728 snapshots=3456' in r.stdout
header=ROOT/'rebuild/include/fable_game_text_bank.h'
report=dict(cases=1728,snapshots=3456,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete80B SetAsMRU,original identity verified by public PDB donor B6C460; independent age-guarded inline TouchResource is separate. Independent/native/compiled full16KiB final snapshots,no callbacks. Null/liveList,detached/reciprocal/brokenlinks,detached sentinel alias (linked sentinel excluded: original clears its own tail then dereferences null),tail empty/self/node,age/count/accounting unchanged,two placements/fills;one mandatory source omission.')
(ROOT/'rebuild/compile-gate/native-resource-set-mru.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_RESOURCE_SET_MRU_GATE_PASS')
