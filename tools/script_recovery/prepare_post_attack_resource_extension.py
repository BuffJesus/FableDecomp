"""Stage the post-attack adapter over the existing verified resource proposal."""
import difflib
import hashlib
import json
import re
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT


def prepare(base=ROOT/'work/watch_barrels_resource_integration', output=ROOT/'work/post_attack_resource_integration', *,
            methods=('TryAcquirePostAttackHero','SetInitialOakvaleObjective'),
            fragments=('retail_post_attack_actions.inc','retail_oakvale_objective.inc'),
            patch_name='post-attack-resource-integration.patch', method_source=None, transform=None):
    base, output = Path(base), Path(output)
    parent = json.loads((base/'proposal.json').read_text())
    digest = lambda body: hashlib.sha256(body).hexdigest()
    original = Path(parent['source']).read_bytes()
    raw = (base/'LuaRetailResources.h').read_bytes()
    if digest(original) != parent['sourceSha256'] or digest(raw) != parent['candidateSha256']:
        raise ValueError('Post-attack parent or runtime source changed')
    artifacts = {}
    for name, record in parent['additionalSources'].items():
        body = (base/name).read_bytes()
        if digest(body) != record['candidateSha256'] or digest(Path(record['source']).read_bytes()) != record['sourceSha256']:
            raise ValueError('Post-attack inherited source changed: '+name)
        artifacts[name] = body
    for name, expected in parent['helpers'].items():
        body = (base/name).read_bytes()
        if digest(body) != expected: raise ValueError('Post-attack inherited helper changed: '+name)
        artifacts[name] = body
    source = raw.decode()
    if any(re.search(r'(?m)^[ \t]*(?:bool|void|unsigned|int|float)\s+'+re.escape(method)+r'\s*\(',source) or 'type["'+method+'"]' in source
           for method in methods): raise ValueError('Oakvale adapter already present')
    anchor = '    unsigned NewThingFromResource(unsigned id) {'
    binding = '    type["NewResource"]'
    if source.count(anchor) != 1 or source.count(binding) != 1:
        raise ValueError('Post-attack insertion anchors changed')
    if method_source is None:
        method_source = '\n'.join(Path(__file__).with_name(name).read_text() for name in fragments)
    source = source.replace(anchor, method_source+'\n'+anchor)
    source = source.replace(binding, ''.join('    type["'+method+'"] = &LuaRetailResources::'+method+';\n'
                                            for method in methods)+binding)
    if transform is not None:
        source = transform(source)
    artifacts['LuaRetailResources.h'] = source.encode()
    patch = ''
    for name, body in artifacts.items():
        before = original if name == 'LuaRetailResources.h' else (
            Path(parent['additionalSources'][name]['source']).read_bytes() if name in parent['additionalSources'] else b'')
        patch += ''.join(difflib.unified_diff(before.decode().splitlines(True), body.decode().splitlines(True),
            fromfile='a/FableScriptExtender/'+name if before else '/dev/null', tofile='b/FableScriptExtender/'+name))
    output.mkdir(parents=True, exist_ok=True)
    for name, body in artifacts.items(): (output/name).write_bytes(body)
    (output/patch_name).write_bytes(patch.encode())
    report = dict(status='proposal-only-not-applied', source=parent['source'], sourceSha256=digest(original),
        candidateSha256=digest(artifacts['LuaRetailResources.h']), parentCandidateSha256=parent['candidateSha256'],
        methods=list(methods), additionalSources=parent['additionalSources'], helpers=parent['helpers'],
        remaining='Merged quest owner, state, scheduling, full DLL and live gameplay validation pending.')
    (output/'proposal.json').write_text(json.dumps(report, indent=2)+'\n')
    return report


if __name__ == '__main__': print(json.dumps(prepare(), indent=2))
