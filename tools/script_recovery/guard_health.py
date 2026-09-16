"""Guard's nineteen owned getter/health/comparison scopes, pinned to retail bytes."""
import hashlib
import json
import re
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData, ROOT
from tools.script_recovery.native_call_setup_ir import read_call_window

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Guard.lua'
SOURCE='''function GuardControlledHealthPositive(resources, control)
    local actor = resources:NewThingFromResource(control)
    local health = resources:ThingHealth(actor)
    local positive = health > 0.0
    resources:DestroyThing(actor)
    return positive
end
'''

def prove(data=None):
    data=data or RData()
    w=json.loads(Path(__file__).with_name('guard_health_witness.json').read_text())
    raw=data.bytes_at(w['mainAddress'],w['mainSize'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['mainSha256']:
        raise ValueError('Guard native Main changed')
    if data.bytes_at(w['thresholdAddress'],4).hex()!=w['thresholdHex']:
        raise ValueError('Guard health threshold changed')
    for row in w['healthSites']:
        setup=read_call_window(data,w['mainAddress'],w['mainSize'],row['getter'],argument_count=1)
        if setup is None or setup.ecx!=('stack',16) or list(setup.stack_arguments[0])!=row['output']:
            raise ValueError('Guard health resource/output correspondence changed')
    return w

def lower(source,data=None):
    w=prove(data)
    if hashlib.sha256(source.encode()).hexdigest()!=w['draftSha256']:
        raise ValueError('Guard draft changed')
    pattern=r'(?m)^(?P<i> *)fVar20 = quest:GetHealth\(nil --\[\[missing\]\]\)\n(?P=i)fVar4 = 0.0\n(?P<extra>(?:(?P=i)[^\n]*\n)*?)(?P=i)if fVar4 < fVar20 then'
    matches=list(re.finditer(pattern,source))
    if len(matches)!=19:raise ValueError('Guard health consumers changed')
    # Ghidra presents repeat lecture first, then first lecture. Pin speech roles
    # as well as whole draft: equal crime names belong to separate native paths.
    native_order=[11,12,13,14,15,16,17,0,1,2,3,4,5,7,8,9,10,6,18]
    correspondence=[]
    for match,index in zip(matches,native_order):
        key=re.search(r'pcVar12 = "([^"]+)"',source[match.end():])[1]
        if key!=w['healthSites'][index]['speech']:raise ValueError('Guard health speech role changed')
        correspondence.append({'nativeIndex':index,'speech':key,'start':w['healthSites'][index]['start']})
    for match in reversed(matches):
        source=source[:match.start()]+match['extra']+match['i']+'if GuardControlledHealthPositive(resources, guard_control) then'+source[match.end():]
    return SOURCE+'\n'+source,dict(w,sourceCorrespondence=correspondence,
        limits=['Caller must retain native resource stack16; this operand phase does not recover full Main control flow.'])
