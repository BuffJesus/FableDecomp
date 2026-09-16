"""Compose the Villager text/list methods over Barrel; never modify runtime files."""
import difflib
import hashlib
import json
import re
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT


def prepare(base=ROOT/'work/barrel_resource_integration',output=ROOT/'work/villager_resource_integration'):
    base,output=Path(base),Path(output)
    parent=json.loads((base/'proposal.json').read_text());raw=(base/'LuaRetailResources.h').read_bytes()
    if hashlib.sha256(raw).hexdigest()!=parent['candidateSha256']:raise ValueError('Villager parent proposal changed')
    target=Path(parent['source']);original=target.read_bytes()
    if hashlib.sha256(original).hexdigest()!=parent['sourceSha256']:raise ValueError('Villager runtime target changed')
    source=raw.decode();folder=Path(__file__).parent
    bully=(folder/'bully_runoff_methods.inc').read_text()
    # Share the already-reviewed Text construction/destruction semantics without unrelated Bully methods.
    text_methods=[]
    for pattern in (r'unsigned NewText\(\) \{.*?\n\}',r'void DestroyText\(unsigned id\) \{[^\n]+\}'):
        matches=re.findall(pattern,bully,re.S)
        if len(matches)!=1:raise ValueError('Owned Text methods changed')
        text_methods.append(matches[0])
    fragments=[folder/name for name in ('retail_villager_text_actions.inc','retail_villager_speech.inc','retail_villager_speech_list_actions.inc','retail_villager_ambient.inc','retail_villager_messages.inc')]
    methods=['NewText','DestroyText','AddVillagerAmbientText','StartVillagerTalkConversation',
             'AssignVillagerSuffix','AddVillagerTalkLine','SpeakVillagerAttacked','AssignVillagerSpeechText',
             'ShouldVillagerStartAmbientConversation','SetVillagerAmbientTimer','StartVillagerAmbientConversation',
             'WasVillagerHit','WasVillagerTalkedTo','SetVillagerHeroAllies','InitializeVillager']
    if any('type["'+name+'"]' in source for name in methods):raise ValueError('Villager method already registered')
    additions='\n'.join(text_methods+[p.read_text() for p in fragments])
    registration='''
    auto lists=lua.new_usertype<RetailVillagerSpeechLists>("RetailVillagerSpeechLists",
        sol::constructors<RetailVillagerSpeechLists()>());
    lists["Append"]=&RetailVillagerSpeechLists::Append;
    lists["Count"]=&RetailVillagerSpeechLists::Count;
    lists["Close"]=&RetailVillagerSpeechLists::Close;
'''
    edits=[('#pragma once','#pragma once\n#include "retail_villager_speech_lists.h"'),
           ('    unsigned NewThingFromResource(unsigned id) {',additions+'\n    unsigned NewThingFromResource(unsigned id) {'),
           ('enum class Kind { Resource, ActorMap, Movie, Thing };','enum class Kind { Resource, ActorMap, Movie, Thing, Text };'),
           ('        CScriptThing thing{};','        CScriptThing thing{};\n        CCharString text{};'),
           ('        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;',
            '        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;\n        case Kind::Text: CCharString_Destroy(&e.text); break;'),
           ('inline void RegisterRetailResources(sol::state& lua) {','inline void RegisterRetailResources(sol::state& lua) {'+registration),
           ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;',
            '    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join('    type["'+name+'"] = &LuaRetailResources::'+name+';' for name in methods))]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Villager integration anchor changed: '+old)
        source=source.replace(old,new)
    output.mkdir(parents=True,exist_ok=True);helpers={}
    for path in [p for p in base.glob('*.h') if p.name!='LuaRetailResources.h']+[folder/'retail_villager_speech_lists.h']:
        body=path.read_bytes();(output/path.name).write_bytes(body);helpers[path.name]=hashlib.sha256(body).hexdigest()
    candidate=source.encode();(output/'LuaRetailResources.h').write_bytes(candidate)
    additional={}
    state_path=target.with_name('LuaQuestState.h');state_original=state_path.read_bytes();state=state_original.decode()
    state_edits=[('#include "FableAPI.h"','#include "FableAPI.h"\n#include "retail_villager_speech_lists.h"'),
        ('    CGameScriptInterfaceBase* GetGameInterface()',
         '    std::shared_ptr<RetailVillagerSpeechLists> GetVillagerSpeechLists() { return m_villagerSpeechLists.Get(); }\n    CGameScriptInterfaceBase* GetGameInterface()'),
        ('private:','private:\n    RetailVillagerSpeechListOwner m_villagerSpeechLists;')]
    for old,new in state_edits:
        if state.count(old)!=1:raise ValueError('Villager quest-state anchor changed')
        state=state.replace(old,new)
    manager_path=target.with_name('LuaManager.cpp');manager_original=manager_path.read_bytes();manager=manager_original.decode()
    anchor='    RegisterRetailResources(lua);'
    if manager.count(anchor)!=1:raise ValueError('Villager quest binding anchor changed')
    manager=manager.replace(anchor,anchor+'\n    questState_type["GetVillagerSpeechLists"] = &LuaQuestState::GetVillagerSpeechLists;')
    for path,old,new in ((state_path,state_original,state),(manager_path,manager_original,manager)):
        (output/path.name).write_bytes(new.encode())
        additional[path.name]={'source':str(path),'sourceSha256':hashlib.sha256(old).hexdigest(),
                               'candidateSha256':hashlib.sha256(new.encode()).hexdigest()}
    patch=''.join(difflib.unified_diff(original.decode().splitlines(True),source.splitlines(True),
                                     fromfile='a/FableScriptExtender/LuaRetailResources.h',tofile='b/FableScriptExtender/LuaRetailResources.h'))
    for name in helpers:
        patch+=''.join(difflib.unified_diff([],(output/name).read_text().splitlines(True),fromfile='/dev/null',tofile='b/FableScriptExtender/'+name))
    for path,old,new in ((state_path,state_original,state),(manager_path,manager_original,manager)):
        patch+=''.join(difflib.unified_diff(old.decode().splitlines(True),new.splitlines(True),
                                          fromfile='a/FableScriptExtender/'+path.name,tofile='b/FableScriptExtender/'+path.name))
    (output/'villager-resource-integration.patch').write_text(patch)
    report=dict(status='proposal-only-not-applied',source=str(target),sourceSha256=hashlib.sha256(original).hexdigest(),
                candidateSha256=hashlib.sha256(candidate).hexdigest(),parentCandidateSha256=parent['candidateSha256'],
                methods=methods,helpers=helpers,additionalSources=additional,
                remaining='Quest Init append integration, complete host teardown ordering/scheduler validation, full Villager Main, DLL and gameplay validation.')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
