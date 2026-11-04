import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-resource-set-lru-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CResource_SetAsLRU_009fc450.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/resource_set_lru_candidate_20261009.cpp'
symbol='?SetAsLRU@CResource@@QAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9fc450,78,0,{})
(w/'resource_set_lru_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9fc450,78)))+'};\n');(w/'resource_set_lru_aliases.h').write_text('#pragma comment(linker,"/alternatename:@SetLRU@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeResourceSetLRU_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=2304 snapshots=4608' in r.stdout
header=ROOT/'rebuild/include/fable_game_text_bank.h'
report=dict(cases=2304,snapshots=4608,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete78B SetAsLRU,original identity verified by donor textureRelease callpacket B7D190->B6C410. Independent/native/compiled full16KiB final snapshots,no callbacks. Null/liveList,detached/reciprocal/brokenlinks,sentinel alias,front empty/self/node,age/count/accounting unchanged,two placements/fills;one mandatory source omission.')
(ROOT/'rebuild/compile-gate/native-resource-set-lru.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_RESOURCE_SET_LRU_GATE_PASS')
