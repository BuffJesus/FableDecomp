"""Shared build-evidence plumbing; behavioral models remain in the fixtures."""
import hashlib,json,subprocess
from pathlib import Path
import check_one as V
from near_match import compare,retail_relocations

def finish(root,source,obj,symbol,targets,entry,native,compiled,rel,cases,snapshots,name,fixture):
    build=obj.parent;test=build/'required.cpp';test_obj=build/'required.obj'
    aliases=['#pragma comment(linker,"/alternatename:_RequiredRoot='+symbol+'")']
    aliases+=['#pragma comment(linker,"/alternatename:'+target+'=_UnusedBoundary")' for target in targets]
    test.write_text('#include <stdlib.h>\nextern "C" void UnusedBoundary(){abort();}\nextern "C" void RequiredRoot();\nint main(){RequiredRoot();return 0;}\n'+'\n'.join(aliases)+'\n')
    q=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test_obj),str(test)],V.env(),build)
    assert not q.returncode,q.stdout+q.stderr
    command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),str(test_obj)]
    q=subprocess.run(command,env=V.env(),capture_output=True,text=True)
    assert q.returncode and ('LNK2001' in q.stdout or 'LNK2019' in q.stdout),q.stdout+q.stderr
    q=subprocess.run(command+[str(obj)],env=V.env(),capture_output=True,text=True)
    assert not q.returncode,q.stdout+q.stderr
    score=compare(compiled,native,tuple(rel),retail_relocations('%08x'%entry,len(native)))
    assert score.near_match
    files=[source,fixture,Path(__file__)]+[root/'rebuild/include'/f for f in ('fable_graphic_frame.h','fable_graphic_frame_data.h','fable_counted_pointer.h','fable_surface_types.h','fable_extents_i.h')]
    report=dict(cases=cases,snapshots=snapshots,arena_bytes=16384,built_bytes=len(compiled),retail_bytes=len(native),relocations=len(rel),parity=score.to_dict(),omission_checks=1,
        source_sha256={str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},original_sha256=hashlib.sha256(native).hexdigest(),
        boundary='Root-only native/compiler/independent-model full arena differential proof. Allocation, data initializer or counted deletion/texture dimensions remain explicit functional boundaries; no real heap/GPU claim.')
    destination=root/'rebuild/compile-gate'/name
    destination.write_text(json.dumps(report,indent=2)+'\n')
