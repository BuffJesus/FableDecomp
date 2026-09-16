"""TeddyGirl's twelve owned health scopes, including a far destructor join."""
import hashlib,json,re
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData,ROOT
from tools.script_recovery.native_call_setup_ir import read_call_window
DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_TeddyGirl.lua'
SOURCE='''function TeddyGirlControlledHealthPositive(resources, control)
    local actor = resources:NewThingFromResource(control)
    local ok, positive = xpcall(function()
        return resources:ThingHealth(actor) > 0.0
    end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(actor) end)
    if not ok then error(positive, 0) end
    if not closed then error(err, 0) end
    return positive
end
'''

def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('teddy_girl_health_witness.json').read_text())
    raw=data.bytes_at(w['mainAddress'],w['mainSize'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['mainSha256']:raise ValueError('TeddyGirl native Main changed')
    if data.bytes_at(w['thresholdAddress'],4)!=bytes.fromhex(w['thresholdHex']):raise ValueError('TeddyGirl health threshold changed')
    for row in w['healthSites']:
        setup=read_call_window(data,w['mainAddress'],w['mainSize'],row['getter'],argument_count=1)
        if setup is None or setup.ecx!=('stack',20) or list(setup.stack_arguments[0])!=row['output']:raise ValueError('TeddyGirl health resource/output changed')
    return w

def lower(source,data=None):
    w=prove(data)
    if hashlib.sha256(source.encode()).hexdigest()!=w['draftSha256']:raise ValueError('TeddyGirl draft changed')
    pattern=r'(?m)^(?P<i> *)fVar18 = quest:GetHealth\((?:r1|nil --\[\[missing\]\])\)\n(?P=i)fVar3 = _DAT_0122dedc\n(?P=i)if fVar3 < fVar18 then'
    matches=list(re.finditer(pattern,source));order=[1,0,2,3,4,5,6,9,10,8,7,11]
    if len(matches)!=12:raise ValueError('TeddyGirl health consumers changed')
    correspondence=[]
    for match,index in zip(matches,order):
        key=re.search(r'pcVar19 = "([^"]+)"',source[match.end():])[1]
        if key!=w['healthSites'][index]['speech']:raise ValueError('TeddyGirl health branch role changed')
        correspondence.append({'nativeIndex':index,'speech':key})
    for match in reversed(matches):source=source[:match.start()]+match['i']+'if TeddyGirlControlledHealthPositive(resources, teddy_control) then'+source[match.end():]
    return SOURCE+'\n'+source,dict(w,sourceCorrespondence=correspondence,
        limits=['Caller must own resource stack20. This operand pass alone does not recover complete Main or retained Bully lifetime.'])
