"""Guard's dynamic alert/chase/recheck phase; pending host capabilities explicit."""
from tools.script_recovery.guard_health import prove
from tools.script_recovery.signed_int32_lua import SOURCE as SIGNED_INT32

SOURCE=SIGNED_INT32+'''function GuardApproach(quest, me, resources, control)
    local function hasUnpunishedDeeds()
        local badDeeds = quest:GetStateInt("BadDeedsPerformed")
        local punishedDeeds = quest:GetStateInt("GuardsDealtWithBadDeeds")
        local unpunishedDeeds = wrapSignedInt32(badDeeds - punishedDeeds)
        return unpunishedDeeds > 0
    end
    if not hasUnpunishedDeeds() then return "skip" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    local count = quest:GetStateInt("BadDeedsPerformed")
    if count >= 3 then count = 3 end
    local range = resources:ReadGuardAlertRange(count)
    if not resources:IsDistanceBetweenThingsUnder(me, quest:GetHero(), range) then return "skip" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return "cancel" end
    end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    range = resources:ReadGuardLectureRange()
    if not resources:IsDistanceBetweenThingsUnder(me, quest:GetHero(), range) then
        if quest:IsActiveThreadTerminating() then return "cancel" end
        resources:GuardFaceHero(me, true, false)
        local conversation = resources:NewConversation(me, false, false)
        resources:AddRawConversationPerson(conversation, quest:GetHero())
        resources:GuardAddHeroConversationLine(conversation, "TEXT_QST_048_GUARD_COME_HERE", me)
        resources:FollowThing(control, quest:GetHero(), 1.0, true)
    end
    resources:SetRawCutsceneBehaviour(me, 1)
    range = resources:ReadGuardLectureRange()
    while not resources:IsDistanceBetweenThingsUnder(me, quest:GetHero(), range) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return "cancel" end
        range = resources:ReadGuardLectureRange()
    end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    resources:SetRawCutsceneBehaviour(me, 2)
    if not hasUnpunishedDeeds() then return "claimed" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    return "lecture"
end
'''

def recover(data=None):
    from tools.script_recovery.lift_native_lua import RData
    data=data or RData();w=prove(data)
    # Exact key comes from the native constructor, not an inferred display name.
    key=data.string_at(0x12d8650)
    return SOURCE.replace('TEXT_QST_048_GUARD_COME_HERE',key),{
        'nativeMainSha256':w['mainSha256'],'start':0xdac7a0,'end':0xdaca19,
        'alertTable':0x13ac844,'lectureRange':0x13ac840,'conversationKey':key,
        'signedDifference':'32-bit subtraction, then signed >0; no lower index clamp',
        'pendingCapabilities':['ReadGuardAlertRange','ReadGuardLectureRange','FollowThing',
            'GuardFaceHero','AddRawConversationPerson','GuardAddHeroConversationLine','SetRawCutsceneBehaviour'],
        'limits':['Cancellation preserves native current cutscene state; caller owns control cleanup.',
                  'Negative alert indexes retain native pointer arithmetic semantics; integration must not invent a lower clamp.']}
