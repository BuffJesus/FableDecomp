"""Wire reviewed atomic helpers for two native returned-Thing lifetimes."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData


def recover(owner,source,rdata=None):
    rdata=rdata or RData()
    w=json.loads(Path(__file__).with_name('scythe_returned_things_witness.json').read_text())
    if owner not in w['sources'] or hashlib.sha256(source.encode()).hexdigest()!=w['sources'][owner]:
        raise ValueError('Scythe returned-Thing source correspondence changed')
    for region in w['regions']:
        raw=rdata.bytes_at(int(region['address'],16),region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Scythe returned-Thing native evidence changed')
    for address,value in [('0x12C7D28','MK_OW_SCYTHE3'),('0x12E2354','ScytheNearOracle'),
                          ('0x12E239C','CREATURE_RIVAL_HERO_SCYTHE')]:
        if rdata.string_at(int(address,16))!=value:
            raise ValueError('Scythe returned-Thing name changed')
    if owner=='ScytheMarker':
        old='''    local native_arg_scythe_spawn_position = me:GetPos()
    local native_arg_spawned_scythe = quest:CreateCreature("CREATURE_RIVAL_HERO_SCYTHE", native_arg_scythe_spawn_position, "ScytheNearOracle")
    quest:RemoveThing(me, false, true)'''
        new='''    -- Pending runtime integration: scoped returned-Thing lifetime.
    quest:SpawnScytheAndRemoveMarker(me)'''
        capability='SpawnScytheAndRemoveMarker'
    else:
        old='''            native_arg_scythe_initial_target = quest:GetThingWithScriptName("MK_OW_SCYTHE3")
            quest:EntitySetFacingAngleTowardsThing(me, native_arg_scythe_initial_target, false)'''
        new='''            -- Pending runtime integration: target and key survive facing.
            quest:FaceThingByScriptName(me, "MK_OW_SCYTHE3", false)'''
        capability='FaceThingByScriptName'
    if source.count(old)!=1:raise ValueError('Scythe returned-Thing block correspondence changed')
    return source.replace(old,new),dict(w,status='pending runtime integration',capability=capability)
