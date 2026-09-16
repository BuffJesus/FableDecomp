"""Native hit conversation uses one returned id and retained Victim listener."""
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

SOURCE='''function BullyHitConversation(resources, me, retainedVictim)
    local conversation = resources:NewConversation(me, false, false)
    resources:AddConversationPerson(conversation, retainedVictim)
    resources:AddConversationLine(conversation, "TEXT_QST_048_BULLY_SCRMSG_GET_OFF", me, retainedVictim, false)
    resources:AddConversationLine(conversation, "TEXT_QST_048_VICTIM_REVENGE", me, retainedVictim, false)
end
'''

def lower(source,data=None):
    data=data or RData();recover(data)
    if [data.string_at(p) for p in (0x12d9a38,0x12d9a1c)]!=['TEXT_QST_048_BULLY_SCRMSG_GET_OFF','TEXT_QST_048_VICTIM_REVENGE']:
        raise ValueError('Bully hit conversation literal changed')
    old='''            ppVar11 = quest:AddNewConversation(nil --[[missing]], 0xff, false)
            quest:AddPersonToConversation(0, nil --[[missing]])
            ppVar20 = ppVar11
            quest:AddLineToConversation(ppVar11, "TEXT_QST_048_BULLY_SCRMSG_GET_OFF", nil --[[missing]], nil --[[missing]], false)
            quest:AddLineToConversation(ppVar11, "TEXT_QST_048_VICTIM_REVENGE", nil --[[missing]], nil --[[missing]], false)'''
    if source.count(old)!=1:raise ValueError('Bully hit conversation source correspondence changed')
    return SOURCE+'\n'+source.replace(old,'            BullyHitConversation(resources, me, r1)'),{
        'newConversation':0xdbc4c3,'addPerson':0xdbc4d6,'lines':[0xdbc505,0xdbc540],
        'stringScopes':[[0xdbc4ea,0xdbc512],[0xdbc525,0xdbc54d]],
        'semantics':'NewConversation(bound self,false,false), returned id including -1 passed unchanged; both lines use bound self and retained Victim Thing, false flag.',
        'limits':'Native effect of conversation API failures not invented; no native conversation destruction occurs here.'}
