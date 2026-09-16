"""Stage BarrelThug methods over the Theresa proposal; never apply the patch."""
import difflib
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT

METHODS = ('InitializeBarrelThugActor', 'ResetResource', 'SpeakBarrelThug',
           'FollowBarrelThugHero', 'PlaceBarrelThugAtStart',
           'NewBarrelThugRemarkConversation', 'AddBarrelThugRemark')


def prepare(base=ROOT/'work/theresa_resource_integration', output=ROOT/'work/barrel_thug_resource_integration'):
    base, output = Path(base), Path(output)
    parent = json.loads((base/'proposal.json').read_text())
    digest = lambda body: hashlib.sha256(body).hexdigest()
    raw = (base/'LuaRetailResources.h').read_bytes()
    if digest(raw) != parent['candidateSha256']: raise ValueError('BarrelThug parent changed')
    original = Path(parent['source']).read_bytes()
    if digest(original) != parent['sourceSha256']: raise ValueError('BarrelThug runtime source changed')
    artifacts = {}
    additional = parent['additionalSources']
    for name, record in additional.items():
        body = (base/name).read_bytes()
        if digest(body) != record['candidateSha256'] or digest(Path(record['source']).read_bytes()) != record['sourceSha256']:
            raise ValueError('BarrelThug inherited source changed: '+name)
        artifacts[name] = body
    for name, expected in parent['helpers'].items():
        body = (base/name).read_bytes()
        if digest(body) != expected: raise ValueError('BarrelThug inherited helper changed: '+name)
        artifacts[name] = body
    methods = Path(__file__).with_name('retail_barrel_thug_actions.inc').read_text()
    source = raw.decode()
    anchor = '    unsigned NewThingFromResource(unsigned id) {'
    if source.count(anchor)!=1: raise ValueError('BarrelThug method insertion anchor changed')
    for name in METHODS:
        if name+'(' in source or 'type["'+name+'"]' in source: raise ValueError('BarrelThug method already exists: '+name)
    source = source.replace(anchor,methods+'\n'+anchor)
    anchor = '    type["NewResource"]'
    if source.count(anchor)!=1: raise ValueError('BarrelThug registration anchor changed')
    bindings = ''.join('    type["'+name+'"] = &LuaRetailResources::'+name+';\n' for name in METHODS)
    source = source.replace(anchor,bindings+anchor)
    artifacts['LuaRetailResources.h'] = source.encode()
    patch = ''
    for name, body in artifacts.items():
        before = original if name=='LuaRetailResources.h' else (Path(additional[name]['source']).read_bytes() if name in additional else b'')
        patch += ''.join(difflib.unified_diff(before.decode().splitlines(True),body.decode().splitlines(True),
            fromfile='a/FableScriptExtender/'+name if before else '/dev/null',tofile='b/FableScriptExtender/'+name))
    output.mkdir(parents=True,exist_ok=True)
    for name,body in artifacts.items(): (output/name).write_bytes(body)
    (output/'barrel-thug-resource-integration.patch').write_bytes(patch.encode())
    report = dict(status='proposal-only-not-applied',source=parent['source'],sourceSha256=digest(original),
        candidateSha256=digest(artifacts['LuaRetailResources.h']),parentCandidateSha256=parent['candidateSha256'],
        methods=METHODS,additionalSources=additional,
        helpers={name:digest(body) for name,body in artifacts.items() if name!='LuaRetailResources.h' and name not in additional},
        remaining='Compiled adapter behavior, full native dispatcher, merged owner/state/scheduler, DLL and gameplay validation')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__': print(json.dumps(prepare(),indent=2))
