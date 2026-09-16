"""Stage Theresa methods over the Villager proposal, including scope-owned vectors."""
import difflib
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.native_new_oakvale_conditions import verify as verify_condition


def prepare(base=ROOT/'work/villager_resource_integration', output=ROOT/'work/theresa_resource_integration'):
    base, output = Path(base), Path(output)
    parent = json.loads((base/'proposal.json').read_text())
    raw = (base/'LuaRetailResources.h').read_bytes()
    digest = lambda b: hashlib.sha256(b).hexdigest()
    if digest(raw) != parent['candidateSha256']: raise ValueError('Theresa parent proposal changed')
    target = Path(parent['source']); original = target.read_bytes()
    if digest(original) != parent['sourceSha256']: raise ValueError('Theresa runtime target changed')
    folder = Path(__file__).parent
    source = raw.decode()
    methods = ['SpeakTheresa','TryAcquireTheresaHero','DoesTheresaHeroHaveChocolates','TakeTheresaChocolatesAndUpdateObjective',
               'ClearTheresaInformation','IsTheresaNearHero','PlayTheresaSkip','ShowTheresaChocolateQuestion','NewTheresaGuardVector',
               'NewPresentedItemOutput','PollPresentedItem','PresentedItemMatches','DestroyPresentedItemOutput','NewTheresaDepartureTrigger','IsTheresaHeroNearTrigger','TheresaTalkOffersChocolates','InitializeTheresaActor']
    factory = '''
    std::shared_ptr<RetailTheresaGuardVector> NewTheresaGuardVector() {
        auto& entry=Add(Kind::TheresaGuardVector);
        entry.guardVector=std::make_shared<RetailTheresaGuardVector>(m_game);
        entry.live=true;
        return entry.guardVector;
    }
'''
    registration = '''
    auto guards=lua.new_usertype<RetailTheresaGuardVector>("RetailTheresaGuardVector",sol::no_constructor);
    guards["RemoveLivingGuards"]=&RetailTheresaGuardVector::RemoveLivingGuards;
    guards["Close"]=&RetailTheresaGuardVector::Close;
'''
    edits = [('#pragma once','#pragma once\n#include "retail_theresa_guard_vector.h"'),
             ('            if (!table || !table->GetScriptThing) throw std::runtime_error("Retail resource Thing getter is unavailable");\n            table->GetScriptThing(expert, &e.thing);',
              '            try {\n                if (!table || !table->GetScriptThing) throw std::runtime_error("Retail resource Thing getter is unavailable");\n                table->GetScriptThing(expert, &e.thing);\n            } catch (...) { Destroy(e); throw; }'),
             ('    unsigned NewThingFromResource(unsigned id) {',
              (folder/'retail_theresa_actions.inc').read_text()+(folder/'retail_theresa_presented.inc').read_text()+
              (folder/'retail_theresa_trigger.inc').read_text()+(folder/'retail_theresa_talk_offer.inc').read_text()+
              (folder/'retail_theresa_init.inc').read_text()+factory+'\n    unsigned NewThingFromResource(unsigned id) {'),
             ('enum class Kind { Resource, ActorMap, Movie, Thing, Text };',
              'enum class Kind { Resource, ActorMap, Movie, Thing, Text, TheresaGuardVector, TheresaPresented };'),
             ('        CCharString text{};','        CCharString text{};\n        CScriptThing* presentedActor = nullptr;\n        std::shared_ptr<RetailTheresaGuardVector> guardVector;'),
             ('        case Kind::Text: CCharString_Destroy(&e.text); break;',
              '        case Kind::Text: CCharString_Destroy(&e.text); break;\n        case Kind::TheresaPresented: CCharString_Destroy(&e.text); break;\n        case Kind::TheresaGuardVector: try { e.guardVector->Close(); } catch(...) {} break;'),
             ('inline void RegisterRetailResources(sol::state& lua) {',
              'inline void RegisterRetailResources(sol::state& lua) {'+registration),
             ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;',
              '    type["DestroyThing"] = &LuaRetailResources::DestroyThing;\n'+'\n'.join(
                  '    type["'+name+'"] = &LuaRetailResources::'+name+';' for name in methods))]
    for old, new in edits:
        if source.count(old) != 1: raise ValueError('Theresa integration anchor changed: '+old)
        source = source.replace(old, new)
    artifacts = {}
    for name, expected in parent['helpers'].items():
        body = (base/name).read_bytes()
        if digest(body) != expected: raise ValueError('Theresa parent helper changed: '+name)
        artifacts[name] = body
    additional = parent.get('additionalSources', {})
    for name, record in additional.items():
        body = (base/name).read_bytes()
        if digest(body) != record['candidateSha256'] or digest(Path(record['source']).read_bytes()) != record['sourceSha256']:
            raise ValueError('Theresa additional source changed: '+name)
        artifacts[name] = body
    verify_condition('NOVI_Theresa')
    manager = artifacts['LuaManager.cpp'].decode()
    include = '#include "LuaRetailCondition.h"'
    binding = '    questState_type["RegisterBoundAliveCondition"]'
    if manager.count(include)!=1 or manager.count(binding)!=1 or 'RegisterBoundConsciousCondition' in manager:
        raise ValueError('Theresa conscious-condition integration anchors changed')
    newline='\r\n' if '\r\n' in manager else '\n'
    registration=(folder/'conscious_condition_registration.inc').read_text().replace('\n',newline)
    manager=manager.replace(include,include+newline+'#include "LuaRetailConsciousCondition.h"')
    manager=manager.replace(binding,registration+newline+binding)
    artifacts['LuaManager.cpp']=manager.encode()
    additional={name:dict(record) for name,record in additional.items()}
    additional['LuaManager.cpp']['candidateSha256']=digest(artifacts['LuaManager.cpp'])
    artifacts['LuaRetailConsciousCondition.h']=(folder/'retail_conscious_condition.h').read_bytes()
    vector = (folder/'retail_theresa_guard_vector.h').read_bytes()
    artifacts['retail_theresa_guard_vector.h'] = vector
    artifacts['LuaRetailResources.h'] = source.encode()
    output.mkdir(parents=True, exist_ok=True)
    for name, body in artifacts.items(): (output/name).write_bytes(body)
    patch = ''.join(difflib.unified_diff(original.decode().splitlines(True), source.splitlines(True),
                    fromfile='a/FableScriptExtender/LuaRetailResources.h', tofile='b/FableScriptExtender/LuaRetailResources.h'))
    for name, body in artifacts.items():
        if name == 'LuaRetailResources.h': continue
        before = Path(additional[name]['source']).read_text().splitlines(True) if name in additional else []
        patch += ''.join(difflib.unified_diff(before,body.decode().splitlines(True),
                         fromfile='a/FableScriptExtender/'+name if before else '/dev/null',tofile='b/FableScriptExtender/'+name))
    (output/'theresa-resource-integration.patch').write_bytes(patch.encode())
    report = dict(status='proposal-only-not-applied',source=str(target),sourceSha256=digest(original),
                  candidateSha256=digest(source.encode()),parentCandidateSha256=parent['candidateSha256'],methods=methods,
                  helpers={name:digest(body) for name,body in artifacts.items() if name!='LuaRetailResources.h' and name not in additional},
                  additionalSources=additional,
                  remaining='Merged scope runtime behavior, full Theresa Main, state/scheduler, DLL and gameplay validation')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__ == '__main__': print(json.dumps(prepare(),indent=2))
