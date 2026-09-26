-- Readable native conversion: KG_Chief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local initialChatted

-- KG_Chief.Main (retail 0x00e19d80)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, thing, movie3, resource2
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        resources:ReleaseResource(resource2)
    end
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetAsToAddToStatChangesWhenHit(me, false)
    me:SetFriendsWithEverythingFlag(true)
    resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e1a6c6 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e1a6c6 end
    quest:EntityAttachToScript(me, quest:GetActiveQuestName())
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame(me) then goto LAB_00e1a6c6 end
        if me:IsTalkedToByHero() then
            if not quest:GetStateBool("WhiteBalverineAlive") then
                local resource = resources:NewResource()
                resources:TryAcquire(resource, hero, 4)
                local actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "HERO", resource)
                resources:SetActor(actorMap, "CHIEF", resource2)
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_WBK_CHIEF4", actorMap, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:SetStateBool("MissionSucceeded", true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource)
            else
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if not initialChatted then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    initialChatted = false
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                        me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource2)
                            return
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                        me:Speak(hero, "TEXT_QST_074_CHIEF_BALVERINE_GONE_SECOND", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource2)
                            return
                        end
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
            end
        end
        if me:MsgIsHitByHero() then
            goto LAB_00e1a451
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e1a451 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e1a451
        ::LAB_00e1a451::
        predicateResult = true
        ::FLOW_past_lab_00e1a451::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then goto LAB_00e1a6c6 end
            if not quest:NewScriptFrame(me) then goto LAB_00e1a6c6 end
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, thing, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e1a6c6 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e1a6c6 end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                me:Speak(hero, "TEXT_QST_074_CHIEF_BEEN_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource2)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource2)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            if not quest:NewScriptFrame(me) then goto LAB_00e1a6c6 end
            me:SetFriendsWithEverythingFlag(thing)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:ClearThingHasInformation(me)
    end
    ::LAB_00e1a6c6::
    resources:ReleaseResource(resource2)
end

-- KG_Chief.Init (retail 0x00e19390)
function Init(quest, me)
    initialChatted = false
    quest:SetThingHasInformation(me, false, true, false)
end

-- KG_Chief.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- KG_Chief.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

