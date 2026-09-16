"""Compose the remaining emitted quest API bindings into the staged runtime."""
import difflib
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT

OUTPUT=ROOT/'work/oakvale_quest_integration'


def prepare(base=ROOT/'work/oakvale_entity_integration',out=OUTPUT):
    base,out=Path(base),Path(out);parent=json.loads((base/'proposal.json').read_text())
    digest=lambda body:hashlib.sha256(body).hexdigest()
    records=dict(parent['additionalSources']);records['LuaRetailResources.h']=dict(source=parent['source'],sourceSha256=parent['sourceSha256'],candidateSha256=parent['candidateSha256'])
    bodies={};originals={}
    for name,record in records.items():
        before=Path(record['source']).read_bytes();body=(base/name).read_bytes()
        if digest(before)!=record['sourceSha256'] or digest(body)!=record['candidateSha256']:raise ValueError('Quest composition input changed: '+name)
        originals[name]=before;bodies[name]=body
    for name,expected in parent['helpers'].items():
        body=(base/name).read_bytes()
        if digest(body)!=expected:raise ValueError('Quest composition helper changed: '+name)
        bodies[name]=body;originals[name]=b''
    declarations=('    void SetStateFloat(const std::string& key,float value);\n'
                  '    float GetStateFloat(const std::string& key);\n'
                  '    int StartConversationWithHero(CScriptThing*,bool,bool);\n'
                  '    void AddConversationLineToHero(int,const std::string&,CScriptThing*,bool);\n'
                  '    void FaceThingByScriptName(CScriptThing*,const std::string&,bool);\n')
    methods=('SetStateFloat','GetStateFloat','StartConversationWithHero','AddConversationLineToHero','FaceThingByScriptName')
    header=bodies['LuaQuestState.h'].decode();anchor='    void AddLineToConversation('
    if header.count(anchor)!=1 or any(method in header for method in methods):raise ValueError('Quest header insertion changed')
    bodies['LuaQuestState.h']=header.replace(anchor,declarations+anchor).encode()
    manager=bodies['LuaManager.cpp'].decode();anchor='    questState_type["AddLineToConversation"] ='
    if manager.count(anchor)!=1:raise ValueError('Quest registration insertion changed')
    bindings=''.join('    questState_type["'+m+'"] = &LuaQuestState::'+m+';\n' for m in methods)
    bodies['LuaManager.cpp']=manager.replace(anchor,bindings+anchor).encode()
    source=Path(__file__).parent
    adapters={name:(source/name).read_text() for name in ('book_trader_conversation_adapter.inc','oakvale_quest_adapter.inc')}
    bodies['LuaQuestState.cpp']+=('\n'+'\n'.join(adapters.values())+'\n').encode()
    out.mkdir(parents=True,exist_ok=True);patch='';additional={};helpers={}
    for name,body in bodies.items():
        (out/name).write_bytes(body);before=originals[name]
        patch+=''.join(difflib.unified_diff(before.decode().splitlines(True),body.decode().splitlines(True),
            fromfile='a/FableScriptExtender/'+name if before else '/dev/null',tofile='b/FableScriptExtender/'+name))
        if name=='LuaRetailResources.h':continue
        if name in records:additional[name]=dict(records[name],candidateSha256=digest(body))
        else:helpers[name]=digest(body)
    (out/'oakvale-quest-integration.patch').write_bytes(patch.encode())
    report=dict(parent,status='proposal-only-not-applied',parentCandidateSha256=parent['candidateSha256'],
                additionalSources=additional,helpers=helpers,questMethods=list(methods),
                questAdapterInputs={name:digest(body.encode()) for name,body in adapters.items()},
                remaining='Full updated DLL, quest adapter behavior, scheduler/state/restore and live gameplay validation required.')
    (out/'proposal.json').write_text(json.dumps(report,indent=2)+'\n');return report


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
