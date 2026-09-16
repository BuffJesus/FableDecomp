"""Bully proximity, ruined-Teddy conversation and departure target lifetime."""
from tools.script_recovery.teddy_girl_health import prove
SOURCE='''function TeddyGirlDeparture(quest, resources, me, control, bully)
    if not quest:GetStateBool("SpokeAboutFindingTeddy") then return true end
    if not resources:IsDistanceUnderThing(me, bully, 10.0) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    local conversation = resources:NewConversation(me, false, false)
    resources:AddRawConversationPerson(conversation, quest:GetHero())
    resources:TeddyGirlAddHeroLine(conversation, me, "TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED")
    quest:SetMasterGameState("TeddySolution", "C")
    if not TeddyGirlAcquire(quest, resources, me, control) then return false end
    local target = resources:NewThingFromScriptName("NOVI_AffairWife")
    local ok, result = xpcall(function()
        resources:TeddyGirlMoveToThing(control, target, 3.0, 1, nil, false, false, true)
        while resources:IsActorPositionOnScreen(me) do
            if resources:IsDistanceOver(me, quest:GetHero(), 20.0) then break end
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        resources:RemoveRawThing(me, false, true)
        return true
    end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(target) end)
    if not ok then error(result, 0) end
    if not closed then error(err, 0) end
    return result
end
'''

def recover(data=None):
    import hashlib
    from tools.script_recovery.lift_native_lua import RData
    data=data or RData();w=prove(data)
    if data.bytes_at(0x7e7300,15)!=bytes.fromhex('8b490885c974058b01ff6014c21c00'):raise ValueError('TeddyGirl movement wrapper changed')
    if data.bytes_at(0x1260f0c+0x694,4)!=bytes.fromhex('00736e00'):raise ValueError('TeddyGirl screen predicate dispatch changed')
    for address,digest in ((0xcbe2ff,'9d320829e96a4ce4eff39dc46e20f3f1e0a8ef2099ba02663064501ad10d6c57'),(0xcbe3ea,'c43ee5f60921209b191dfde5463e4fcfd719be760f517b6630e7dc177c84b76a')):
        if hashlib.sha256(data.bytes_at(address,114)).hexdigest()!=digest:raise ValueError('TeddyGirl native distance helper changed')
    for at,value in ((0x12d885c,'TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED'),(0x12d82e8,'NOVI_AffairWife'),(0x12d8858,'C')):
        if data.string_at(at)!=value:raise ValueError('TeddyGirl departure literal changed')
    return SOURCE,{'nativeMainSha256':w['mainSha256'],'start':0xdafa9b,'end':0xdafc99,
        'targetOffset':156,'retainedBullyOffset':40,'moveArguments':[3.0,1,None,False,False,True],
        'limits':['Distance and screen APIs remain raw host boundaries.','Whole owner cleanup and callback-error target construction are separate gates.']}
