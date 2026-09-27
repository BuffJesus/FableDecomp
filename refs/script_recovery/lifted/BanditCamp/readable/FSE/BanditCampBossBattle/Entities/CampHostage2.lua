-- Readable native conversion: CampHostage2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CampHostage2.Main (retail 0x00d08790)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local hostagesRescued, scratchValue
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    hostagesRescued = quest:GetStateBool("HostagesRescued")
    scratchValue = 0
    while not hostagesRescued and not quest:GetStateBool("HostageKilled") do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        if quest:GetTimer(timerId) >= 1 then
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        elseif not quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        else
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            me:PlayAnimation("CS_DISAPPOINTED", false, false, false, true, true, false, false)
            if scratchValue == 0 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST", me, hero, false)
                scratchValue = 1
            elseif scratchValue == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND", me, hero, false)
                scratchValue = 0
            end
            quest:SetTimer(timerId, 8)
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        end
    end
    if not quest:IsActiveThreadTerminating() then
        resources:PrepareResource(resource)
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- CampHostage2.Init (retail 0x00d08720)
function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_NEUTRALS")
    quest:EntitySetOpinionReactionEnabled(me, 34, false)
end

-- CampHostage2.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CampHostage2.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

