"""Restore Bully's two termination results and the lost post-subdual retry edge."""
import hashlib
from tools.script_recovery.bully_initial_phases import recover

OLD_JOIN='''    -- TODO(native): joined_r0x00dbc8b9:
    if cVar4 ~= 0 then goto LAB_00dbc8eb end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00dbcce2 end
    cVar4 = me:AcquireControl(4)
    -- TODO(native): goto joined_r0x00dbc8b9;
'''
NEW_JOIN='''    ::acquireBullyAfterSubdued::
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00dbcce2 end
        cVar4 = me:AcquireControl(4)
    end
'''

def recover_flow(source):
    _,w=recover()
    if hashlib.sha256(source.encode()).hexdigest()!=w['draftTextSha256']:
        raise ValueError('Bully control-flow source changed')
    edits=[('    cVar4 = extraout_AL_06\n    while cVar4 == 0 do\n','    while alive do\n'),
           ('        cVar4 = extraout_AL_57\n',''),(OLD_JOIN,NEW_JOIN),
           ('                    -- TODO(native): goto joined_r0x00dbc8b9;\n','                    goto acquireBullyAfterSubdued\n')]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Bully native loop source correspondence changed')
        source=source.replace(old,new)
    return source,{'terminationResults':[0xdbb57b,0xdbc779],
        'interactionBackedge':{'branch':0xdbc780,'target':0xdbb588},
        'subdualRetry':{'first':0xdbc8b4,'branch':0xdbc8b9,'retry':0xdbc8e4,'backedge':0xdbc8e9,'target':0xdbc8c0},
        'limits':'Only these native loop edges recovered; operand/resource/movie lowering remains separate.'}

RETRY_PHASE='''function BullyReacquireAfterSubdual(quest, resources, selfControl, me)
    resources:PrepareResource(selfControl)
    while not resources:TryAcquire(selfControl, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end
'''
