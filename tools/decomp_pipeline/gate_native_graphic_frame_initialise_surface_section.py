"""Exact57B wrapper; allocation/data-section initializer remain boundaries."""
import concurrent.futures,hashlib,json,re,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
fixture=ROOT/"rebuild/tests/integration/NativeGraphicFrameInitialiseSurfaceSection_test.py"
required=ROOT/"rebuild/tests/integration/NativeGraphicFrameInitialiseSurfaceSection_required.cpp"
source=ROOT/"rebuild/src/compiled/00/9f/CGraphicFrame_InitialiseAsSurfaceSection_009fcb60.cpp"
symbol="?InitialiseAsSurfaceSection@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z"
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def batch(start):
    run=subprocess.run([sys.executable,str(fixture),str(start)],capture_output=True,text=True,timeout=300)
    assert run.returncode==0,(start,run.returncode,run.stdout,run.stderr)
    begin=run.stdout.find("{");assert begin>=0,(start,run.stdout)
    report=json.loads(run.stdout[begin:])
    assert report["status"]=="NATIVE_SECTION_WRAPPER_BATCH_PASS"
    assert report["batch_start"]==start
    assert report["cases"]==221184 and report["snapshots"]==663552
    assert report["mandatory_source_omission_checks"]==1
    assert report["zero_coff_relocation_addends"]==3 and report["relocations"]==3
    assert report["source_sha256"]==sha(source)
    assert report["provider"]["symbol"]==symbol and report["provider"]["grade"]=="MATCH"
    assert report["retail_bytes"]==report["compiled_bytes"]==57
    print("surface-section batch",start,"PASS",flush=True)
    return report
starts=list(range(0,256,32))
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
    reports=list(pool.map(batch,starts))
assert [r["batch_start"] for r in reports]==starts
cases=sum(r["cases"] for r in reports);snapshots=sum(r["snapshots"] for r in reports)
assert (cases,snapshots)==(1769472,5308416)
assert len({r["relocation_resolved_body_sha256"] for r in reports})==1
files={source,fixture,required,Path(__file__).resolve()}
pending=[source,required];visited=set()
while pending:
    path=pending.pop()
    if path in visited:continue
    visited.add(path);files.add(path)
    for name in re.findall(r'^\s*#\s*include\s+"([^"\n]+)"',path.read_text(),re.M):
        candidates=(path.parent/name,ROOT/"rebuild/include"/name)
        resolved=next((p.resolve() for p in candidates if p.is_file()),None)
        assert resolved is not None,(path,name)
        pending.append(resolved)
report=dict(status="PASS",cases=cases,snapshots=snapshots,arena_bytes=16384,
    batches=reports,provider=reports[0]["provider"],omission_checks=1,
    executed_batch_omission_checks=8,zero_coff_relocation_addends=3,
    source_sha256={str(p.relative_to(ROOT)):sha(p) for p in sorted(files)},
    boundary="Complete57B/21instruction wrapper; original/model/compiled full16KiB callback/final snapshots. All256 preconstructor flag bytes, preserved27B object sentinels, allocationnull/fresh/owneralias, oldowner overwritten withoutrelease, allocationcallback owner replacement, rawfloat +/-zero/finite/infinity/quiet/signalingNaN/subnormal bits, signedinteger endpoints/rawbool slots/rawreturn, RET1C and savedregisters. Allocation and unresolved9FCA40 data initializer are explicit boundaries. No actualheap/childinitializer/FPconversion/surfaceUV/GPU claim.")
(ROOT/"rebuild/compile-gate/native-graphic-frame-initialise-surface-section.json").write_text(json.dumps(report,indent=2)+"\n")
print("NATIVE_GRAPHIC_FRAME_INITIALISE_SURFACE_SECTION_GATE_PASS",cases,snapshots)
