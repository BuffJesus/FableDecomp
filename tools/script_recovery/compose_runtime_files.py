"""Hash-check and compose reviewed source edits over an unapplied runtime stage."""
import difflib
import hashlib
import json
from pathlib import Path


def compose(base,out,edit,patch_name,extra_sources=None):
    base,out=Path(base),Path(out);parent=json.loads((base/'proposal.json').read_text())
    digest=lambda body:hashlib.sha256(body).hexdigest()
    records=dict(parent['additionalSources'])
    records['LuaRetailResources.h']=dict(source=parent['source'],sourceSha256=parent['sourceSha256'],candidateSha256=parent['candidateSha256'])
    for name,path in (extra_sources or {}).items():
        if name in records or name in parent['helpers']:raise ValueError('Additional source already staged: '+name)
        path=Path(path);sha=digest(path.read_bytes())
        records[name]=dict(source=str(path),sourceSha256=sha,candidateSha256=sha)
    originals={};bodies={}
    for name,record in records.items():
        before=Path(record['source']).read_bytes();body=before if name in (extra_sources or {}) else (base/name).read_bytes()
        if digest(before)!=record['sourceSha256'] or digest(body)!=record['candidateSha256']:raise ValueError('Runtime composition input changed: '+name)
        originals[name]=before;bodies[name]=body
    for name,expected in parent['helpers'].items():
        body=(base/name).read_bytes()
        if digest(body)!=expected:raise ValueError('Runtime composition helper changed: '+name)
        originals[name]=b'';bodies[name]=body
    evidence=edit(bodies)
    out.mkdir(parents=True,exist_ok=True);additional={};helpers={};patch=''
    for name,body in bodies.items():
        before=originals.get(name,b'');(out/name).write_bytes(body)
        patch+=''.join(difflib.unified_diff(before.decode().splitlines(True),body.decode().splitlines(True),
            fromfile='a/FableScriptExtender/'+name if before else '/dev/null',tofile='b/FableScriptExtender/'+name))
        if name=='LuaRetailResources.h':continue
        if name in records:additional[name]=dict(records[name],candidateSha256=digest(body))
        else:helpers[name]=digest(body)
    report=dict(parent,status='proposal-only-not-applied',parentCandidateSha256=parent['candidateSha256'],
        candidateSha256=digest(bodies['LuaRetailResources.h']),additionalSources=additional,helpers=helpers,
        composition=evidence)
    (out/patch_name).write_bytes(patch.encode());(out/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report
