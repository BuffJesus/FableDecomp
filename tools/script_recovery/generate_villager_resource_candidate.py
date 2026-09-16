"""Compose the disabled Villager Main; retain explicit host and validation limits."""
import hashlib
import json
import re
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_new_oakvale_conditions import verify as entry_condition
from tools.script_recovery.native_villager_control_resource import verify as control
from tools.script_recovery.native_villager_movie_scope import verify as movie
from tools.script_recovery.native_villager_health_temporaries import verify as health
from tools.script_recovery.native_villager_talk_key import verify as suffix
from tools.script_recovery.native_villager_conversation_key import verify as key
from tools.script_recovery.native_villager_init import verify as init

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Villager.lua'
RAW_SHA='040f8075780f42d44a9d6e8fa9d62241689cb27fad33ccd83c04c8051f40e7ea'


def generate(output=ROOT/'work/villager_candidate'):
    raw=DRAFT.read_bytes()
    if hashlib.sha256(raw).hexdigest()!=RAW_SHA:raise ValueError('Villager draft changed')
    source=raw.decode().replace('\r\n','\n');data=RData();evidence={name:verify(data) for name,verify in
        (('control',control),('movie',movie),('health',health),('suffix',suffix),('key',key),('init',init))}
    entry=entry_condition('NOVI_Villager',data)
    if not entry or entry['method']!='RegisterBoundConsciousCondition':raise ValueError('Villager entry condition changed')
    prefix=source.split('function Main(quest, me)')[0]
    prefix,count=re.subn(r'local __native_vectors = \{.*?^\}\n','',prefix,flags=re.S|re.M)
    if count!=1:raise ValueError('Villager vector declaration changed')
    prefix,count=re.subn(r'function Init\(quest, me\).*?^end', '''function Init(quest, me)
    __native_entity_state:SetStateBool("HeroDidHitMe", false)
    quest:WithRetailResources(function(resources)
        resources:InitializeVillager(me)
    end)
end''',prefix,flags=re.S|re.M)
    if count!=1:raise ValueError('Villager Init correspondence changed')
    folder=Path(__file__).parent;parts=[];inputs={}
    for name in ('villager_attacked_body.lua','villager_talk_body.lua','villager_ambient_selection.lua','villager_main_body.lua'):
        path=folder/name;body=path.read_text();inputs[name]=hashlib.sha256(path.read_bytes()).hexdigest()
        if name!='villager_main_body.lua':body=re.sub(r'\nreturn \w+\s*$','\n',body)
        parts.append(body)
    index='function GetVillagerSpeechIndex'+source.split('function GetVillagerSpeechIndex',1)[1]
    result='-- DISABLED resource candidate; requires unapplied Villager runtime/quest-owner proposal.\n'+prefix+'\n'.join(parts)+'\n'+index
    LuaRuntime().execute('assert(load(...))',result)
    report=dict(status='disabled-composed-candidate',sourceSha256=RAW_SHA,
        candidateSha256=hashlib.sha256(result.encode()).hexdigest(),inputs=inputs,entryCondition=entry,evidence=evidence,
        remaining=['Whole-engine composition beyond separately verified native phase boundaries',
                   'Quest helper/state/scheduler integration','DLL and gameplay validation'])
    output=Path(output);output.mkdir(parents=True,exist_ok=True)
    (output/'NOVI_Villager.resource_candidate.lua').write_text(result)
    (output/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return result,report


if __name__=='__main__':print(generate()[1]['status'])
