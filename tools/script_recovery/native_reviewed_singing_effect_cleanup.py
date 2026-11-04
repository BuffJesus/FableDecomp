"""Recover SingingStones live-effect removal from the retail helper."""
import hashlib
import json
import re
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
KILL='-- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);'
ERASE='-- TODO(native): Std_Vector_Erase_Range(quest:GetStateListRef("EffectList"),quest:GetStateListRef("EffectList"),(quest:GetStateListCount("EffectList") * 0xc));'
KILL_DIAG='unlifted: KillAllThingsInVector(*(void **)(this + 0x40),QUESTLIST_Ref("EffectList"),0);'
ERASE_DIAG='unlifted: Std_Vector_Erase_Range(QUESTLIST_Ref("EffectList"),QUESTLIST_BeginValue("EffectList"),(QUE'
HELPER='''local function RemoveLiveEffects(quest)
    local effects = quest:GetStateListCopy("EffectList")
    for index = 1, #effects do
        local effect = effects[index]
        if effect:IsAlive() then
            quest:RemoveThing(effect, false, true)
        end
    end
end

'''


def restore(source,todo,calls,fn,row,rdata,meta):
    if row is None:
        return source,todo,calls
    def require(ok,reason):
        if not ok:
            raise ValueError('SingingStones effect cleanup: '+reason)
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'source changed')
    require(hashlib.sha256(rdata.bytes_at(start,end-start)).hexdigest()==row['nativeSha256'],'native body changed')
    direct={x['site'].lower():x for x in fn['calls']}
    require(len(row['killSites'])==len(row['eraseSites']) in (1,3) and
            all(direct.get(site,{}).get('target','').lower()=='0x00cbed82' for site in row['killSites']) and
            all(direct.get(site,{}).get('target','').lower()=='0x008ac9d0' for site in row['eraseSites']),
            'native kill/erase sites changed')
    helper=ROOT/meta['helperLua']['path']
    require(hashlib.sha256(helper.read_bytes()).hexdigest()==meta['helperLua']['sha256'] and
            'remove_thing(thing, remove_flag, true)' in helper.read_text(),
            'reviewed native helper changed')
    ir=json.loads((ROOT/'refs/script_recovery/native_helper_operation_ir.json').read_text())
    op=next((x for x in ir['helpers'] if x.get('targetAddress')=='0x00CBED82'),None)
    expected={'kind':'remove-live-things-in-vector','complete':True,'elementDwords':3,
              'isAliveVtableOffset':'0x12c','removeThingVtableOffset':'0x1b0',
              'removeFlagParameter':'param_3','finalFlag':True}
    require(op is not None and expected in op['semanticPatterns'] and
            op['decompileSha256']==meta['helperDecompileSha256'],
            'native helper semantics changed')
    needles={'NoviUnitBindings.h':'quest["GetStateListCopy"]',
             'LuaQuestState.cpp':'RemoveThing_API(m_pGameInterface, pThing, immediate.value_or(false), removeFromWorld.value_or(true))',
             'LuaEntityAPI.cpp':'bool LuaEntityAPI::IsAlive(CScriptThing* pMe)',
             'LuaManager.cpp':'questState_type["RemoveThing"]'}
    for name,evidence in meta['sidecar'].items():
        file=ROOT/evidence['path']
        require(hashlib.sha256(file.read_bytes()).hexdigest()==evidence['sha256'] and needles[name] in file.read_text(),
                'sidecar API changed: '+name)
    count=len(row['killSites'])
    require(source.count(KILL)==count and todo.count(KILL_DIAG)==count,'lifted kill calls changed')
    require(fn['address'].lower() in ('0x00ed2bb0','0x00ed2a50'),'unexpected caller')
    source=re.sub(r'(?m)^([ ]*)'+re.escape(KILL)+r'$',r'\1RemoveLiveEffects(quest)',source)
    todo=[x for x in todo if x!=KILL_DIAG]
    if count==3:
        require(source.count('function WatchForCompleteTune(quest)')==1 and source.count('quest:StateListClear("EffectList")')==3,
                'tune erases changed')
        source=HELPER+source
    else:
        require(source.count(ERASE)==1 and todo.count(ERASE_DIAG)==1,'region erase changed')
        source=re.sub(r'(?m)^([ ]*)'+re.escape(ERASE)+r'$',r'\1quest:StateListClear("EffectList")',source)
        todo=[x for x in todo if x!=ERASE_DIAG]
        require(source.count('function WatchForRegionLeaving(quest)')==1,'region function header changed')
    return source,todo,[*calls,'GetStateListCopy','IsAlive','RemoveThing']
