"""Bounded native graphic frame initializer proof with explicit helper boundaries."""
import hashlib,json,re,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
fixture=R/'rebuild/tests/integration/NativeGraphicInfoFrameInitialise_test.py'
B=R/'work/codex-native-startup/native-graphic-info-frame-initialise-20261010'
counts=[]
for start in range(0,256,16):
 r=subprocess.run([sys.executable,str(fixture),str(start)],capture_output=True,text=True,timeout=45)
 print(r.stdout,flush=True);assert not r.returncode,(start,r.returncode,r.stdout,r.stderr)
 line=next(x for x in r.stdout.splitlines() if x.startswith('INITIALISE_FRAME_BATCH_PASS'))
 _,batch,cases,snaps,faults=line.split();assert int(batch)==start and int(cases)==3072
 counts.append((int(cases),int(snaps),int(faults)))
r=subprocess.run([sys.executable,str(fixture),'supplement'],capture_output=True,text=True,timeout=45)
print(r.stdout,flush=True);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
line=next(x for x in r.stdout.splitlines() if x.startswith('INITIALISE_FRAME_SUPPLEMENT_PASS'))
_,extra_cases,extra_snapshots,groups=line.split(' ',3)
supplement=dict(cases=int(extra_cases),snapshots=int(extra_snapshots),groups=json.loads(groups))
assert supplement['cases']==5120 and supplement['snapshots']==20480
r=subprocess.run([sys.executable,str(fixture),'expanded'],capture_output=True,text=True,timeout=45)
print(r.stdout,flush=True);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
line=next(x for x in r.stdout.splitlines() if x.startswith('INITIALISE_FRAME_EXPANDED_PASS'))
_,ec,es,nh,uc,groups=line.split(' ',5)
expanded=dict(cases=int(ec),snapshots=int(es),null_heap_delete_calls=int(nh),uninitialise_info_clear_calls=int(uc),groups=json.loads(groups))
assert (expanded['cases'],expanded['snapshots'],expanded['null_heap_delete_calls'],expanded['uninitialise_info_clear_calls'])==(5120,25088,1024,512)
required=R/'rebuild/tests/integration/NativeGraphicInfoFrameInitialiseRequired.cpp';obj=B/'required.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(required)],V.env(),B);assert not r.returncode,r.stdout+r.stderr
symbol='?InitialiseFrame@CGraphicInfoFrame@@AAEXAAVCGraphicFrame@@AAVCManagedTexture@@C@Z'
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(B/'required.exe'),'/include:'+symbol,str(obj)]
r=subprocess.run(cmd,env=V.env(),capture_output=True,text=True);diagnostic=r.stdout+r.stderr
assert r.returncode and any(symbol in l and re.search(r'\bLNK(?:2001|2019)\b',l) for l in diagnostic.splitlines()),diagnostic
r=subprocess.run(cmd+[str(B/'root.obj')],env=V.env(),capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
source=R/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_InitialiseFrame_009fe550.cpp'
# Conservative transitive quoted-include closure; keep cyclic headers bounded.
seen={source.resolve(),required.resolve()};pending=list(seen);headers=[]
while pending:
 path=pending.pop()
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(encoding='utf-8-sig'),re.M):
  candidates=(path.parent/name,R/'rebuild/include'/name)
  dependency=next((p.resolve() for p in candidates if p.is_file()),None)
  assert dependency is not None,(path,name)
  if dependency not in seen:
   seen.add(dependency);headers.append(dependency);pending.append(dependency)
headers.sort()
# Imported proof helpers used in child fixtures, including their current local transitive imports.
helpers=[R/'tools'/name for name in ('artifact_layout.py','decomp_pipeline/check_one.py','decomp_pipeline/gate_game_component_abi.py','decomp_pipeline/pe_oracle.py','decomp_pipeline/variable_allocator_parity.py','decomp_pipeline/near_match.py','decomp_pipeline/verify_and_land.py','decomp_pipeline/crawl/rowtrim.py','decomp_pipeline/crawl/purity.py')]
files=[source,fixture,Path(__file__),required]+headers+helpers
report=dict(status='PASS_ROOT_GLUE',cases=sum(c[0] for c in counts),snapshots=sum(c[1] for c in counts),null_allocation_fault_cases=sum(c[2] for c in counts),arena_bytes=16384,omission_checks=1,expected_missing_symbol=symbol,omission_diagnostic=diagnostic,source_sha256={str(f.relative_to(R)):hashlib.sha256(f.read_bytes()).hexdigest() for f in files},compiled_object_sha256=hashlib.sha256((B/'root.obj').read_bytes()).hexdigest(),boundary='Complete exact208B/76instructions/sevenresolvedrelocations original/COFF/independent root algorithm, full16KiB arena callback/final snapshots. All256mip/flagbytes,rawcounts0/1/2/FFFFFFFF,two placements/fills,12 callbackmodes, cachedFrameSlot/liveowner/incomingsource-read-aftercleanup,constructor/delete/managed callbacks. Nullallocation4096 cases explicitly matches original unmappedread9FE596,not safe return. Constructor,Uninitialise,managedassignment/Release,virtual/counted/heapdelete,allocator are disclosed functional interfaces with deterministic mutation effects; their behavior not recovered/countable here and no realheap/GPU claim. All inline ownership operations of root executed. Canonical private declaration intact;fastcall fixtureABI ECXthis/unusedEDX+3originalstackarguments links exactprivateC-mangledsignedchar symbol, executable only linkchecked nevercalledwithnullvalues.')
report['main_cases']=report['cases'];report['main_snapshots']=report['snapshots'];report['supplement']=supplement
report['header_dependency_mode']='conservative_recursive_quoted_include_closure'
report['header_dependency_count']=len(headers)
report['proof_helper_count']=len(helpers)
report['cases']+=supplement['cases'];report['snapshots']+=supplement['snapshots']
report['boundary']+=' Focused5120case supplement covers incomingFrameNULL,oldPTexture.InfoNULL oncount1destruction, validincomingFrameargument==cachedtargetFrameSlot oldcounts2/FFFFFFFF (noDelete),managedalias+NULLincoming. COFFrelativecalladdends explicitly0checked beforeoriginaltargetpatching. Linker/include requests actualprivatesymbol alongsidefastcallcaller.'
report['accepted_baseline_cases']=report['cases'];report['accepted_baseline_snapshots']=report['snapshots']
report['expanded_supplement']=expanded
report['cases']+=expanded['cases'];report['snapshots']+=expanded['snapshots']
assert report['null_allocation_fault_cases']==4096
report['boundary']+=' Additional independent ownership partitions: oldFrame count1 with Info counts0/2/FFFFFFFF; Uninitialise clears Info; counted-delete clears Info then genuine live reload passes NULL to disclosed heap-delete interface (not null-safe shortcut). Each group all256 flags/mips, two placements and fills. No real heap validity claim.'
(R/'rebuild/compile-gate/native-graphic-info-frame-initialise.json').write_text(json.dumps(report,indent=2)+'\n')
print('INITIALISE_FRAME_BEHAVIOR_GATE_PASS',report['cases'],report['snapshots'],report['null_allocation_fault_cases'])
