"""Compose native-checked BarrelThug phases into disabled converter entry points."""
import hashlib
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_new_oakvale_conditions import verify as condition

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelThug.lua'
DRAFT_SHA='087b08108e3a34be2b758d49ef770a963794a1c01977d6a59086cf3686a7f3f1'
MODULES=('init','intro_prepare','intro','talk_body','conversation','timed_remarks','hit','main_body')


def generate(*,draft_path=DRAFT):
    if hashlib.sha256(Path(draft_path).read_bytes()).hexdigest()!=DRAFT_SHA:
        raise ValueError('BarrelThug converter draft changed')
    data=RData()
    if hashlib.sha256(data.bytes_at(0xdb6c60,0x1091)).hexdigest()!='eafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09':
        raise ValueError('BarrelThug native Main changed')
    if hashlib.sha256(data.bytes_at(0xdb6bf0,65)).hexdigest()!='d2dc4a56b4f8265bafa812550a582da8e8b2b0e989bee90c0e8bf32f2b45d8ba':
        raise ValueError('BarrelThug native Init changed')
    entry=condition('NOVI_BarrelThug')
    if not entry or entry['method']!='RegisterBoundConsciousCondition':raise ValueError('BarrelThug condition changed')
    folder=Path(__file__).parent
    parts=[];hashes={}
    for name in MODULES:
        path=folder/f'barrel_thug_{name}.lua';body=path.read_text()
        hashes[path.name]=hashlib.sha256(path.read_bytes()).hexdigest()
        if body.count('\nreturn ')!=1:raise ValueError('BarrelThug module export changed')
        parts.append(body.rsplit('\nreturn ',1)[0])
    state="""-- Disabled native-backed BarrelThug phase composition. Runtime proposal remains unapplied.
local entityFields = {}
local entityState = {}
for _, kind in ipairs({"Bool", "Int"}) do
    entityState["GetState" .. kind] = function(_, name) return entityFields[name] end
    entityState["SetState" .. kind] = function(_, name, value) entityFields[name] = value end
end
"""
    glue="""
function Init(quest, me)
    quest:WithRetailResources(function(resources)
        initializeBarrelThug(quest, me, resources, entityState)
    end)
end

function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:WithRetailResources(function(resources)
        runBarrelThugMainAfterCondition(quest, me, resources, entityState)
    end)
end
"""
    source=state+'\n'.join(parts)+glue
    LuaRuntime().execute('assert(load(...))',source)
    report=dict(enabled=False,status='disabled; native phase composition',entryCondition=entry,draftSha256=DRAFT_SHA,
        candidateSha256=hashlib.sha256(source.encode()).hexdigest(),modules=hashes,
        pending=['Dispatcher comparison abstracts separately tested phases; no single unabstracted whole-engine run.',
                 'Seven adapters compile and pass36 real-Lua policies in unapplied full-class proposal.',
                 'Pre-return lookup ownership on exceptions, merged state/persistence/scheduler/DLL/gameplay validation.'])
    return source,report
