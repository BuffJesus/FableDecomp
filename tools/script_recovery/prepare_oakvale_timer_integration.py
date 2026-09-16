"""Compose the opt-in timer owner with all current common resource changes."""
import difflib
import hashlib
import json
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.oakvale_timer_host_prepare import prepare as prepare_host


def prepare(base=ROOT/'work/quest_markers_resource_integration',
            out=ROOT/'work/oakvale_timer_integration'):
    base, out = Path(base), Path(out)
    parent = json.loads((base/'proposal.json').read_text())
    digest = lambda body: hashlib.sha256(body).hexdigest()
    runtime = Path(parent['source']).parent
    records = dict(parent['additionalSources'])
    records['LuaRetailResources.h'] = {
        'source': parent['source'], 'sourceSha256': parent['sourceSha256'],
        'candidateSha256': parent['candidateSha256']}
    inherited = {}
    for name, record in records.items():
        body = (base/name).read_bytes()
        if digest(body) != record['candidateSha256']:
            raise ValueError('Timer integration parent changed: '+name)
        if digest(Path(record['source']).read_bytes()) != record['sourceSha256']:
            raise ValueError('Timer integration runtime changed: '+name)
        inherited[name] = body
    for name, expected in parent['helpers'].items():
        body = (base/name).read_bytes()
        if digest(body) != expected:
            raise ValueError('Timer integration helper changed: '+name)
        inherited[name] = body

    out, report = prepare_host(runtime=runtime, base=base, out=out)
    modified = {'dllmain.cpp', 'LuaQuestHost.h', 'LuaQuestHost.cpp',
                'LuaQuestState.h', 'LuaQuestState.cpp'}
    # Preserve inherited implementation files as well as headers.
    for name, body in inherited.items():
        if name not in modified:
            (out/name).write_bytes(body)
    names = set(inherited) | modified | {'retail_oakvale_timers.h', 'oakvale_timer_host_policy.h'}
    helpers = {}
    additional = {}
    patch = ''
    for name in sorted(names):
        source = runtime/name
        before = source.read_bytes() if source.exists() else b''
        body = (out/name).read_bytes()
        patch += ''.join(difflib.unified_diff(
            before.decode().splitlines(True), body.decode().splitlines(True),
            fromfile='a/FableScriptExtender/'+name if source.exists() else '/dev/null',
            tofile='b/FableScriptExtender/'+name))
        if name == 'LuaRetailResources.h':
            continue
        if source.exists():
            additional[name] = dict(source=str(source), sourceSha256=digest(before),
                                    candidateSha256=digest(body))
        else:
            helpers[name] = digest(body)
    (out/'oakvale-timer-integration.patch').write_bytes(patch.encode())
    report.update(status='proposal-only-not-applied', source=parent['source'],
                  sourceSha256=parent['sourceSha256'],
                  candidateSha256=digest((out/'LuaRetailResources.h').read_bytes()),
                  parentCandidateSha256=parent['candidateSha256'],
                  additionalSources=additional, helpers=helpers,
                  methods=[], nativeLifetime='NewOakValeIntro (explicit opt-in only)',
                  remaining='Full DLL, scheduler teardown, restore order and live validation pending.')
    report['inputs'].update({str(base/name): digest(body) for name, body in inherited.items()})
    (out/'proposal.json').write_text(json.dumps(report, indent=2)+'\n')
    return out, report


if __name__ == '__main__':
    print(prepare()[0])
