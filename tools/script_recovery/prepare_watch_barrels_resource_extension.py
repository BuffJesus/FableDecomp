"""Stage WatchBarrels methods over the Theresa proposal; never apply the patch."""
import difflib
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT

METHODS = ('NewBarrelWatchSnapshot', 'RewardRemainingBarrel', 'SpawnBarrelBeetle')


def prepare(base=ROOT/'work/barrel_thug_resource_integration', output=ROOT/'work/watch_barrels_resource_integration'):
    base, output = Path(base), Path(output)
    parent = json.loads((base/'proposal.json').read_text())
    digest = lambda body: hashlib.sha256(body).hexdigest()
    raw = (base/'LuaRetailResources.h').read_bytes()
    if digest(raw) != parent['candidateSha256']: raise ValueError('WatchBarrels parent changed')
    original = Path(parent['source']).read_bytes()
    if digest(original) != parent['sourceSha256']: raise ValueError('WatchBarrels runtime source changed')
    artifacts = {}
    additional = parent['additionalSources']
    for name, record in additional.items():
        body = (base/name).read_bytes()
        if digest(body) != record['candidateSha256'] or digest(Path(record['source']).read_bytes()) != record['sourceSha256']:
            raise ValueError('WatchBarrels inherited source changed: '+name)
        artifacts[name] = body
    for name, expected in parent['helpers'].items():
        body = (base/name).read_bytes()
        if digest(body) != expected: raise ValueError('WatchBarrels inherited helper changed: '+name)
        artifacts[name] = body
    methods = Path(__file__).with_name('retail_watch_barrels_rewards.inc').read_text()
    methods += """
    std::shared_ptr<RetailBarrelWatchSnapshot> NewBarrelWatchSnapshot() {
        auto& entry=Add(Kind::BarrelWatchSnapshot);
        entry.barrelSnapshot=std::make_shared<RetailBarrelWatchSnapshot>(m_game);
        entry.live=true;
        return entry.barrelSnapshot;
    }
"""
    source = raw.decode()
    edits = [
        ('#pragma once', '#pragma once\n#include "retail_barrel_watch_snapshot.h"'),
        ('TheresaGuardVector, TheresaPresented }', 'TheresaGuardVector, TheresaPresented, BarrelWatchSnapshot }'),
        ('        std::shared_ptr<RetailTheresaGuardVector> guardVector;', '        std::shared_ptr<RetailTheresaGuardVector> guardVector;\n        std::shared_ptr<RetailBarrelWatchSnapshot> barrelSnapshot;'),
        ('        case Kind::TheresaGuardVector:', '        case Kind::BarrelWatchSnapshot: try { e.barrelSnapshot->Close(); } catch(...) {} break;\n        case Kind::TheresaGuardVector:'),
        ('    auto guards=lua.new_usertype<RetailTheresaGuardVector>', '    auto snapshot=lua.new_usertype<RetailBarrelWatchSnapshot>("RetailBarrelWatchSnapshot",sol::no_constructor);\n    snapshot["Refresh"]=&RetailBarrelWatchSnapshot::Refresh;\n    snapshot["Count"]=&RetailBarrelWatchSnapshot::Count;\n    snapshot["Close"]=&RetailBarrelWatchSnapshot::Close;\n    auto guards=lua.new_usertype<RetailTheresaGuardVector>'),
    ]
    for before, after in edits:
        if source.count(before)!=1: raise ValueError('WatchBarrels owner anchor changed: '+before)
        source=source.replace(before,after)
    artifacts['retail_barrel_watch_snapshot.h']=Path(__file__).with_name('retail_barrel_watch_snapshot.h').read_bytes()
    anchor = '    unsigned NewThingFromResource(unsigned id) {'
    if source.count(anchor)!=1: raise ValueError('WatchBarrels method insertion anchor changed')
    for name in METHODS:
        if name+'(' in source or 'type["'+name+'"]' in source: raise ValueError('WatchBarrels method already exists: '+name)
    source = source.replace(anchor,methods+'\n'+anchor)
    anchor = '    type["NewResource"]'
    if source.count(anchor)!=1: raise ValueError('WatchBarrels registration anchor changed')
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
    (output/'watch-barrels-resource-integration.patch').write_bytes(patch.encode())
    report = dict(status='proposal-only-not-applied',source=parent['source'],sourceSha256=digest(original),
        candidateSha256=digest(artifacts['LuaRetailResources.h']),parentCandidateSha256=parent['candidateSha256'],
        methods=METHODS,additionalSources=additional,
        helpers={name:digest(body) for name,body in artifacts.items() if name!='LuaRetailResources.h' and name not in additional},
        remaining='Compiled adapter behavior, full native dispatcher, merged owner/state/scheduler, DLL and gameplay validation')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__': print(json.dumps(prepare(),indent=2))
