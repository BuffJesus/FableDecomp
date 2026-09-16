"""Reproduce disabled phase candidate and guarded, unapplied draft correspondence."""
import hashlib
import json
import re
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.bully_initial_phases import recover

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Bully.lua'

def correspondence(source=None):
    source=DRAFT.read_text() if source is None else source
    _,w=recover()
    if hashlib.sha256(source.encode()).hexdigest()!=w['draftTextSha256']:
        raise ValueError('Bully draft source correspondence changed')
    pattern=r'(?m)^(?P<i> *)fVar18 = quest:GetHealth\((?P<actor>r1|nil --\[\[missing\]\])\)\n(?P=i)fVar2 = _DAT_0122dedc\n(?P=i)if (?P<comparison>fVar2 < fVar18|fVar18 <= fVar2) then'
    matches=list(re.finditer(pattern,source))
    if len(matches)!=9:raise ValueError('Bully health consumer correspondence changed')
    result=[]
    # Ghidra emits the presented-item arm before the earlier native talk arm.
    # Match speech/branch roles, never assume decompiler text follows addresses.
    source_order=(3,2,0,1,4,5,6,7,8)
    for index,match in enumerate(matches):
        site=w['healthSites'][source_order[index]]
        inverse=match['comparison']=='fVar18 <= fVar2'
        if inverse!=(index==2):raise ValueError('Bully inverse health branch correspondence changed')
        result.append({'kind':'health','nativeStart':site['start'],'physicalHealthIndex':source_order[index], 'old':match[0],
            'new':match['i']+'if '+('not ' if inverse else '')+'BullyControlledHealthAboveThreshold(resources, bully_control) then',
            'unorderedPolicy':'NaN is not positive; inverse branch must use not-positive, not <=.'})
    start=source.index('    me:GetHomePos()\n')
    end=source.index('    alive = not quest:IsActiveThreadTerminating()\n    if not alive then goto LAB_00dbcceb end\n    r1 =',start)
    result.append({'kind':'home','nativeStart':w['home']['start'],'old':source[start:end],
        'new':'    if not BullyReturnHomePhase(quest, me, resources, bully_control) then goto LAB_00dbcceb end\n'})
    return result

def generate():
    source,w=recover();edits=correspondence();out=ROOT/'work/bully_converter';out.mkdir(exist_ok=True)
    (out/'INITIAL_PHASES.lua').write_text(source)
    report={'enabled':False,'gameplayComplete':False,'status':'initial phases only; actor draft edits guarded but unapplied',
        'native':w,'edits':edits,'integrationGates':[
            'Map all native resource acquisitions and distinct late-phase locals before applying actor-wide rewrites.',
            'Retained Victim, temporary Thing, movie/pause/map lifetimes and remaining missing operands still require recovery.',
            'Use actual native temporary Things for distance/health, not me or cached Victim.',
            'Initial outer resource scope closes on callback return/errors; compiled host validation and engine cancellation integration remain separate.',
            'Existing LuaRetailResources needs reviewed ThingIsDistanceFromPositionOver and MoveToPosition extensions; no host installation performed.']}
    (out/'INITIAL_CANDIDATE_REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return report

if __name__=='__main__':print(generate()['status'])
