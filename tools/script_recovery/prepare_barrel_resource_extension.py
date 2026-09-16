"""Compose Barrel helpers over the existing reviewed resource proposal; never apply."""
import difflib
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT


def prepare(base=ROOT/'work/wife_resource_integration',output=ROOT/'work/barrel_resource_integration'):
    base,output=Path(base),Path(output)
    parent=json.loads((base/'proposal.json').read_text())
    raw=(base/'LuaRetailResources.h').read_bytes()
    if hashlib.sha256(raw).hexdigest()!=parent['candidateSha256']:raise ValueError('Barrel parent proposal changed')
    target=Path(parent['source']);original=target.read_bytes()
    if hashlib.sha256(original).hexdigest()!=parent['sourceSha256']:raise ValueError('Barrel runtime target changed')
    source=raw.decode();nl='\r\n' if '\r\n' in source else '\n'
    methods=['WithTimer','IsHeroWithinBarrelApproachDistance','SetBarrelManHeroAllies',
             'IsBarrelManFarFromHero','MoveBarrelManToHero','SetBarrelWatchTimer','TeleportBarrelDepartureActors',
             'GetBarrelWatchTimer','IsOwnedThingPositionOnScreen','TeleportActorToOwnedThing',
             'FaceBarrelManTowardsHero','ShouldBarrelManThankHero','AddBarrelConversation','ShouldBarrelOverhear','ResetBarrelWatchTimer']
    if any('type["'+name+'"]' in source for name in methods):raise ValueError('Barrel methods already registered')
    anchor='    unsigned NewThingFromResource(unsigned id) {'
    binding='    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;'
    registration='inline void RegisterRetailResources(sol::state& lua) {'
    for marker in ('#pragma once',anchor,binding,registration):
        if source.count(marker)!=1:raise ValueError('Barrel integration anchor changed')
    fragments=[Path(__file__).with_name(name) for name in
               ('retail_owned_timer_methods.inc','retail_barrel_approach.inc','retail_barrel_movement.inc','retail_barrel_departure.inc','retail_barrel_marker_actions.inc','retail_barrel_return_encounter.inc')]
    fragments.append(Path(__file__).with_name('retail_barrel_conversation.inc'))
    fragments.append(Path(__file__).with_name('retail_barrel_overhear.inc'))
    fragment=''.join(path.read_text() for path in fragments)
    source=source.replace('#pragma once','#pragma once'+nl+'#include "retail_owned_timer.h"')
    source=source.replace(anchor,fragment.replace('\n',nl)+nl+anchor)
    source=source.replace(registration,registration+nl+'    RegisterRetailOwnedTimer(lua);')
    source=source.replace(binding,''.join('    type["'+name+'"] = &LuaRetailResources::'+name+';'+nl for name in methods)+binding)
    output.mkdir(parents=True,exist_ok=True)
    helpers={}
    for path in base.glob('*.h'):
        if path.name=='LuaRetailResources.h':continue
        content=path.read_bytes();(output/path.name).write_bytes(content)
        helpers[path.name]=hashlib.sha256(content).hexdigest()
    timer=Path(__file__).with_name('retail_owned_timer.h').read_bytes()
    (output/'retail_owned_timer.h').write_bytes(timer);helpers['retail_owned_timer.h']=hashlib.sha256(timer).hexdigest()
    (output/'LuaRetailResources.h').write_bytes(source.encode())
    patch=''.join(difflib.unified_diff(original.decode().splitlines(True),source.splitlines(True),
        fromfile='a/FableScriptExtender/LuaRetailResources.h',tofile='b/FableScriptExtender/LuaRetailResources.h'))
    for name in helpers:
        patch+=''.join(difflib.unified_diff([], (output/name).read_text().splitlines(True),fromfile='/dev/null',tofile='b/FableScriptExtender/'+name))
    (output/'barrel-resource-integration.patch').write_text(patch)
    report=dict(status='proposal-only-not-applied',source=str(target),sourceSha256=hashlib.sha256(original).hexdigest(),
        candidateSha256=hashlib.sha256(source.encode()).hexdigest(),parentCandidateSha256=parent['candidateSha256'],
        methods=methods,helpers=helpers,
        checks=['work/barrel_movement_runtime_checks/result.json','work/barrel_approach_runtime_checks/result.json','work/owned_timer_runtime_checks/result.json','work/barrel_departure_runtime_checks/result.json','work/barrel_marker_runtime_checks/result.json','work/barrel_return_encounter_runtime_checks/result.json'],
        remaining='Full DLL/gameplay, remaining Barrel predicates and final readable integration remain.')
    report['checks'].append('work/barrel_conversation_runtime_checks/result.json')
    report['checks'].append('work/barrel_overhear_runtime_checks/result.json')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
