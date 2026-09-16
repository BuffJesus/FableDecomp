"""Native talk/hit dispatcher operands; CString scopes delegated atomically."""
from tools.script_recovery.guard_health import prove

SOURCE='''function GuardTalk(quest, me, resources, control)
    if not me:IsTalkedToByHero() then return true end
    if quest:IsActiveThreadTerminating() then return false end
    resources:GuardFaceHero(me, false, false)
    local conversation = resources:NewConversation(me, false, false)
    resources:AddRawConversationPerson(conversation, quest:GetHero())
    local bad = quest:GetStateInt("BadDeedsPerformed")
    local key
    if bad == 0 and quest:GetStateInt("GoodDeedsPerformed") > 0 then
        key = "TEXT_QST_048_GUARD_ON_TALK_GOOD"
    elseif bad > 0 then
        key = "TEXT_QST_048_GUARD_ON_TALK_BAD"
    else
        key = "TEXT_QST_048_GUARD_ON_TALK_NEUTRAL"
    end
    if quest:IsActiveThreadTerminating() then return false end
    resources:GuardAddHeroConversationLine(conversation, key, me)
    while quest:IsConversationActive(conversation) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    if quest:IsActiveThreadTerminating() then return false end
    resources:PrepareResource(control)
    return true
end

function GuardAcquire(quest, me, resources, control)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function GuardHit(quest, me, resources, control)
    if not resources:IsHitByHeroExceptAbility(me, 14) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    if not GuardAcquire(quest, me, resources, control) then return false end
    local movie = resources:StartMovie("")
    resources:Pause(true)
    local active = true
    if GuardControlledHealthPositive(resources, control) then
        resources:Speak(control, quest:GetHero(), "TEXT_QST_048_GUARD_ON_HIT", 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then active = false; break end
        end
        if active then active = not quest:IsActiveThreadTerminating() end
    end
    if active then resources:PrepareResource(control) end
    resources:Pause(false)
    resources:DestroyMovie(movie)
    return active
end
'''

def recover(data=None):
    from tools.script_recovery.lift_native_lua import RData
    data=data or RData();w=prove(data)
    for at,key in ((0x125d1c8,'SCRIPT_NAME_HERO'),(0x12d843c,'TEXT_QST_048_GUARD_ON_TALK_GOOD'),
                   (0x12d841c,'TEXT_QST_048_GUARD_ON_TALK_BAD'),(0x12d83f8,'TEXT_QST_048_GUARD_ON_TALK_NEUTRAL'),
                   (0x12d83dc,'TEXT_QST_048_GUARD_ON_HIT')):
        if data.string_at(at)!=key:raise ValueError('Guard interaction key changed')
    return SOURCE,{'nativeMainSha256':w['mainSha256'],'talkRange':[0xdad894,0xdada6c],
        'hitPredicateRange':[0xdada6c,0xdadb38],'hitRange':[0xdadb38,0xdadd0f],
        'excludedAbility':14,'talkWaitId':'Original returned conversation ID survives all polls; no zero substitution.',
        'hitMoviePause':True,'prepareBeforeHitMoviePauseFalse':True,
        'limits':['Host conversation line must construct CString before fetching Hero; generic wrapper argument evaluation does not prove this order.']}
