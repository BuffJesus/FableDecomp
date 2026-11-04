-- Readable native conversion: ChickenMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    CHK_LowPrize = 4008,  -- 50.0
    CHK_MidPrize = 4012,  -- 150.0
    CHK_HighPrize = 4016,  -- 250.0
}

local helpers = require("V_ChickenKicking.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local ghostChat_, haveTalked_, heroHasPlayed_, kickingChickens

-- ChickenMaster.Main (retail 0x00e64fb0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local native_call_target
    local scratchValue, scratchValue3, isActiveThreadTerminating, predicateResult, scratchValue10
    local scratchValue11, ctr_CVar, questionAnswer, questionAnswer2, scratchValue17, sequence1
    local sequence, sequence3, scratchValue19, getActiveQuestName, getActiveQuestName2
    local scratchValue20, position, scratchValue21, scratchValue22, scratchValue23, mkCkOrg, this_00
    local scratchValue25, movie2, movie3, movie4, movie5, scratchValue26, scratchValue27
    local scriptThing, scriptThing2
    scratchValue3 = 0
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:AssignResource(resources:MemberResource("seh_ChickenMaster"), resource)
    quest:EntitySetAppearanceMorphSeed(me, 4)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false, true)
    position = me:GetPos()
    local vec_188 = {x = position.x, y = position.y, z = position.z}
    local timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 0)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    scratchValue10 = 0
    if quest:GetStateBool("KnowGhostHasGone") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId4)
            resources:ReleaseResource(resource)
            return
        end
        scratchValue19 = quest:GetThingWithScriptName("MK_CK_ORG")
        quest:EntityTeleportToThing(me, scratchValue19, false)
    end
    while not quest:GetStateBool("KnowGhostHasGone") do
        if not quest:NewScriptFrame(me) then goto LAB_00e68aeb end
        scratchValue19 = quest:GetHero()
        if (quest:IsDistanceBetweenThingsUnder(scratchValue19, me, 5.5) and (me ~= nil and me:IsDistanceFromPositionUnder(vec_188, 6.0))) and quest:GetTimer(timerId4) < 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
            scratchValue19 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, scratchValue19, false)
            local conversationId = quest:AddNewConversation(me, false, false)
            scratchValue19 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, scratchValue19)
            if not ghostChat_ then
                if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
                scratchValue19 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_PRECHAT_10", me, scratchValue19, false)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
                scratchValue19 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_POSTCHAT", me, scratchValue19, false)
            end
            quest:SetTimer(timerId4, 8)
            while (0 < quest:GetTimer(timerId4) and (scratchValue10 == 0)) and not quest:GetStateBool("TalkedTo") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(timerId4)
                    resources:ReleaseResource(resource)
                    return
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId4)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:SetStateBool("TalkedTo", true)
                end
                ctr_CVar = scratchValue3 % 4294967296
                scratchValue = ctr_CVar | 1
                scratchValue3 = scratchValue
                if not (me:MsgIsHitByHero()) then
                    scratchValue = ctr_CVar | 3
                    scratchValue3 = scratchValue
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue = ctr_CVar | 7
                        scratchValue3 = scratchValue
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e654e0 end
                    end
                    scratchValue11 = 0
                    goto FLOW_past_lab_00e654e0
                end
                ::LAB_00e654e0::
                scratchValue11 = 1
                ::FLOW_past_lab_00e654e0::
                if (scratchValue & 4) ~= 0 then
                    scratchValue = scratchValue & 0xfffffffb
                    scratchValue3 = scratchValue
                end
                if (scratchValue & 2) ~= 0 then
                    scratchValue = scratchValue & 0xfffffffd
                    scratchValue3 = scratchValue
                end
                if (scratchValue & 1) ~= 0 then
                    scratchValue3 = scratchValue & 0xfffffffe
                end
                if scratchValue11 ~= 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId4)
                        resources:ReleaseResource(resource)
                        return
                    end
                    scratchValue10 = 1
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
            quest:SetTimer(timerId4, 15)
        end
        ctr_CVar = scratchValue3
        scratchValue25 = scratchValue3 | 8
        if not (me:MsgIsHitByHero()) then
            scratchValue25 = ctr_CVar | 24
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue25 = ctr_CVar | 56
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e65617 end
            end
            scratchValue11 = 0
            if scratchValue10 ~= 0 then goto LAB_00e65617 end
            goto FLOW_past_lab_00e65617
        end
        ::LAB_00e65617::
        scratchValue11 = 1
        ::FLOW_past_lab_00e65617::
        if (scratchValue25 & 32) ~= 0 then
            scratchValue25 = scratchValue25 & 0xffffffdf
        end
        if (scratchValue25 & 16) ~= 0 then
            scratchValue25 = scratchValue25 & 0xffffffef
        end
        if (scratchValue25 & 8) ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffff7
        end
        if scratchValue11 ~= 0 then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then goto LAB_00e68aeb end
            scratchValue10 = isActiveThreadTerminating and 1 or 0
            movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            helpers.helper_E68B20(quest, me, "CS_CHICKING_HITGUYTOP")
            quest:SetStateBool("RanOff", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
        end
        scratchValue3 = scratchValue25 | 64
        sequence1 = me:IsTalkedToByHero()
        if not sequence1 then
            scratchValue11 = 0
            sequence1 = quest:GetStateBool("TalkedTo")
        end
        if sequence1 then
            scratchValue11 = 1
        end
        scratchValue3 = scratchValue25 & 0xffffffbf
        if scratchValue11 == 0 then goto continue_3 end
        if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
        quest:SetStateBool("TalkedTo", false)
        getActiveQuestName = quest:GetActiveQuestName()
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_CHICKEN_KICKING", getActiveQuestName, false)
        getActiveQuestName2 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_01", "", "")
        if not ghostChat_ then
            if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
            ghostChat_ = true
            movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue20 = quest:GetThingWithScriptName("GhostFisherman")
            isActiveThreadTerminating = scratchValue20 ~= nil and scratchValue20:IsAlive()
            scratchValue20 = nil
            predicateResult = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                if not (predicateResult) then
                    goto FLOW_past_lab_00e66150
                end
                ::LAB_00e66150::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto LAB_00e68aeb
                ::FLOW_past_lab_00e66150::
                scriptThing2 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scriptThing2) then
                    scratchValue20 = quest:GetHero()
                    if not me:Speak(scratchValue20, "TEXT_QST_B17_MASTER_INITIAL_MEETING", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId4)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId4)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e66150 end
                end
            else
                if predicateResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    goto LAB_00e68aeb
                end
                helpers.helper_E68B20(quest, me, "CS_CHICKING_INITIALWALK1")
                quest:SetStateBool("KnowGhostHasGone", true)
                local getActiveQuestName3 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(getActiveQuestName3, "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02", "", "")
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
            movie5 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue20 = quest:GetThingWithScriptName("GhostFisherman")
            isActiveThreadTerminating = scratchValue20 ~= nil and scratchValue20:IsAlive()
            scratchValue20 = nil
            predicateResult = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                if not (predicateResult) then
                    goto FLOW_past_lab_00e661e4
                end
                ::LAB_00e661e4::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                goto LAB_00e68aeb
                ::FLOW_past_lab_00e661e4::
                scriptThing = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scriptThing) then
                    scratchValue20 = quest:GetHero()
                    if not me:Speak(scratchValue20, "TEXT_QST_B17_MASTER_RETURN_MEETING", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId4)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId4)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e661e4 end
                end
            else
                if predicateResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00e68aeb
                end
                helpers.helper_E68B20(quest, me, "CS_CHICKING_INITIALWALK2")
                quest:SetStateBool("KnowGhostHasGone", true)
                local getActiveQuestName4 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(getActiveQuestName4, "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02", "", "")
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie5)
        end
        ::continue_3::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
    if not quest:NewScriptFrame(me) then goto LAB_00e68aeb end
    mkCkOrg = quest:GetThingWithScriptName("MK_CK_ORG")
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    while not isActiveThreadTerminating do
        scratchValue19 = quest:GetHero()
        if (quest:IsDistanceBetweenThingsUnder(scratchValue19, me, 5.5) and quest:IsDistanceBetweenThingsUnder(mkCkOrg, me, 6.0)) and quest:GetTimer(timerId4) < 1 then
            if quest:IsActiveThreadTerminating() then break end
            scratchValue19 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, scratchValue19, false)
            local conversationId2 = quest:AddNewConversation(me, false, false)
            scratchValue19 = quest:GetHero()
            quest:AddPersonToConversation(conversationId2, scratchValue19)
            scratchValue19 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_B17_MASTER_GAME_ASIDE", me, scratchValue19, false)
            quest:SetTimer(timerId4, 8)
            while (0 < quest:GetTimer(timerId4) and (scratchValue10 == 0)) and not quest:GetStateBool("TalkedTo") do
                if not quest:NewScriptFrame(me) then goto LAB_00e68aeb end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
                    quest:SetStateBool("TalkedTo", true)
                end
                ctr_CVar = scratchValue3
                scratchValue3 = scratchValue3 | 128
                if not (me:MsgIsHitByHero()) then
                    scratchValue = ctr_CVar | 384
                    scratchValue3 = scratchValue
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue = ctr_CVar | 896
                        scratchValue3 = scratchValue
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e661fd end
                    end
                    scratchValue11 = 0
                    goto FLOW_past_lab_00e661fd
                end
                ::LAB_00e661fd::
                scratchValue11 = 1
                ::FLOW_past_lab_00e661fd::
                if (scratchValue & 512) ~= 0 then
                    scratchValue = scratchValue & 0xfffffdff
                    scratchValue3 = scratchValue
                end
                if (scratchValue & 256) ~= 0 then
                    scratchValue = scratchValue & 0xfffffeff
                    scratchValue3 = scratchValue
                end
                if (scratchValue & 128) ~= 0 then
                    scratchValue3 = scratchValue & 0xffffff7f
                end
                if scratchValue11 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68aeb end
                    scratchValue10 = 1
                end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:SetTimer(timerId4, 15)
        end
        ctr_CVar = scratchValue3
        scratchValue25 = scratchValue3 | 1024
        if not (me:MsgIsHitByHero()) then
            scratchValue25 = ctr_CVar | 3072
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue25 = ctr_CVar | 0x1c00
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e6634f end
            end
            sequence = scratchValue10 ~= 0
            if not sequence then
                scratchValue11 = 0
                sequence = quest:GetStateBool("SpectatorsUnderAttack")
            end
            if sequence then goto LAB_00e6634f end
            goto FLOW_past_lab_00e6634f
        end
        ::LAB_00e6634f::
        scratchValue11 = 1
        ::FLOW_past_lab_00e6634f::
        if (scratchValue25 & 4096) ~= 0 then
            scratchValue25 = scratchValue25 & 0xffffefff
        end
        if (scratchValue25 & 2048) ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffff7ff
        end
        if (scratchValue25 & 1024) ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffbff
        end
        if scratchValue11 ~= 0 then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then break end
            quest:SetStateBool("SpectatorsUnderAttack", false)
            scratchValue10 = isActiveThreadTerminating and 1 or 0
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            helpers.helper_E68B20(quest, me, "CS_CHICKING_HITGUYBOTTOM")
            quest:SetStateBool("RanOff", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        scratchValue3 = scratchValue25 | 0x2000
        sequence3 = me:IsTalkedToByHero()
        if not sequence3 then
            scratchValue11 = 0
            sequence3 = quest:GetStateBool("TalkedTo")
        end
        if sequence3 then
            scratchValue11 = 1
        end
        scratchValue3 = scratchValue25 & 0xffffdfff
        if scratchValue11 ~= 0 then
            if quest:IsActiveThreadTerminating() then break end
            quest:SetStateBool("TalkedTo", false)
            if haveTalked_ then
                if quest:IsActiveThreadTerminating() then break end
                movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scriptThing = resources:ScriptThing(resource)
                if quest:GetHealth(scriptThing) <= 0.0 then
                    goto LAB_00e665da
                else
                    scratchValue19 = quest:GetHero()
                    if not me:Speak(scratchValue19, "TEXT_QST_B17_MASTER_GREETING_RETURN", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00e667c9
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e667c9 end
                    end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e665da end
                end
                goto FLOW_past_lab_00e667c9
                ::LAB_00e665da::
                quest:GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e667c9 end
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if not quest:IsActiveThreadTerminating() then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if not isActiveThreadTerminating then
                            if quest:GetHeroGold() < 50 then
                                if not quest:IsActiveThreadTerminating() then
                                    local scratchValue29 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scratchValue29) then
                                        scratchValue20 = quest:GetHero()
                                        if not me:Speak(scratchValue20, "TEXT_QST_B17_MASTER_NO_MONEY", GROUP_SELECT_FIRST, false, true, false) then
                                            goto LAB_00e667c9
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e667c9 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e667c9 end
                                    end
                                    goto LAB_00e66ceb
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                if not heroHasPlayed_ then
                                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e667c9 end
                                    local scratchValue31 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scratchValue31) then
                                        scratchValue19 = quest:GetHero()
                                        if not me:Speak(scratchValue19, "TEXT_QST_B17_MASTER_START_KICKING", GROUP_SELECT_FIRST, false, true, false) then
                                            goto LAB_00e667c9
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e667c9 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e667c9 end
                                    end
                                    heroHasPlayed_ = true
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                                    scriptThing2 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scriptThing2) then
                                        scratchValue19 = quest:GetHero()
                                        if not me:Speak(scratchValue19, "TEXT_QST_B17_MASTER_START_KICKING_RETURN", GROUP_SELECT_FIRST, false, true, false) then
                                            goto FLOW_past_lab_00e667c9
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e667c9 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                                    end
                                end
                                quest:FadeScreenOut(0.5, 0.5)
                                if quest:GetStateBool("KnowGhostHasGone") and not quest:GetStateBool("SpectatorsCreated") then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                                    position = quest:GetThingWithScriptName("Spectator1"):GetPos()
                                    quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "Spectator", false)
                                    position = quest:GetThingWithScriptName("Spectator2"):GetPos()
                                    quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", position, "Spectator", false)
                                    getActiveQuestName = "Spectator"
                                    position = quest:GetThingWithScriptName("Spectator3"):GetPos()
                                    quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, getActiveQuestName, false)
                                    quest:SetStateBool("SpectatorsCreated", true)
                                end
                                quest:ConfiscateAllHeroWeapons()
                                quest:SetHeroWillAsUsable(false)
                                quest:SetTeleportingAsActive(false)
                                quest:GiveHeroGold(-50)
                                kickingChickens = true
                                goto LAB_00e66cef
                            end
                            goto FLOW_past_lab_00e667c9
                        end
                    elseif not isActiveThreadTerminating then
                        local scratchValue28 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue28) then
                            scratchValue20 = quest:GetHero()
                            if not me:Speak(scratchValue20, "TEXT_QST_B17_MASTER_DONT_KICK", GROUP_SELECT_FIRST, false, true, false) then
                                goto FLOW_past_lab_00e667c9
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e667c9 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e667c9 end
                        end
                        goto LAB_00e66ceb
                    end
                    goto FLOW_past_lab_00e66ceb
                    ::LAB_00e66ceb::
                    kickingChickens = false
                    ::LAB_00e66cef::
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie4
                    goto LAB_00e6762e
                    ::FLOW_past_lab_00e66ceb::
                    goto LAB_00e667c9
                end
                goto FLOW_past_lab_00e667c9
                ::LAB_00e667c9::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                break
                ::FLOW_past_lab_00e667c9::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                break
            end
            if quest:IsActiveThreadTerminating() then break end
            movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            haveTalked_ = true
            if not me:IsPerformingScriptTask() or not quest:IsDistanceBetweenThingsOver(mkCkOrg, me, 6.0) then
                if not quest:IsActiveThreadTerminating() then
                    quest:FadeScreenOut(0.5, 0.5)
                    if quest:GetStateBool("KnowGhostHasGone") and not quest:GetStateBool("SpectatorsCreated") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e68a59 end
                        goto FLOW_hoist_lab_00e68a59_1
                    end
                    goto FLOW_hoist_lab_00e68a59_2
                end
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                local scratchValue30 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue30) then
                    scratchValue20 = quest:GetHero()
                    if not me:Speak(scratchValue20, "TEXT_QST_B17_MASTER_QUICK_WALK", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00e68a59
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00e68a59 end
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e6762e end
                end
                quest:FadeScreenOut(0.5, 0.5)
                scratchValue20 = quest:GetThingWithScriptName("MK_CK_ORG")
                quest:EntityTeleportToThing(me, scratchValue20, false)
                if not quest:GetStateBool("KnowGhostHasGone") or quest:GetStateBool("SpectatorsCreated") then
                    goto LAB_00e6743e
                else
                    if not quest:IsActiveThreadTerminating() then
                        position = quest:GetThingWithScriptName("Spectator1"):GetPos()
                        quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "Spectator", false)
                        position = quest:GetThingWithScriptName("Spectator2"):GetPos()
                        quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", position, "Spectator", false)
                        position = quest:GetThingWithScriptName("Spectator3"):GetPos()
                        quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "Spectator", false)
                        quest:SetStateBool("SpectatorsCreated", true)
                        goto LAB_00e6743e
                    end
                    goto LAB_00e68a8f
                end
                goto FLOW_hoist_lab_00e68a8f_1
            end
            goto FLOW_past_lab_00e68a8f
            ::LAB_00e68a8f::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            ::FLOW_hoist_lab_00e68a8f_1::
            goto FLOW_past_lab_00e68a73
            ::FLOW_past_lab_00e68a8f::
            goto FLOW_past_lab_00e68a59
            ::LAB_00e68a59::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            break
            ::FLOW_hoist_lab_00e68a59_1::
            position = quest:GetThingWithScriptName("Spectator1"):GetPos()
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "Spectator", false)
            position = quest:GetThingWithScriptName("Spectator2"):GetPos()
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", position, "Spectator", false)
            position = quest:GetThingWithScriptName("Spectator3"):GetPos()
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "Spectator", false)
            quest:SetStateBool("SpectatorsCreated", true)
            ::FLOW_hoist_lab_00e68a59_2::
            scratchValue19 = quest:GetThingWithScriptName("MK_CK_ORG")
            quest:EntityTeleportToThing(me, scratchValue19, false)
            goto LAB_00e6743e
            ::FLOW_past_lab_00e68a59::
            goto FLOW_past_lab_00e6743e
            ::LAB_00e6743e::
            helpers.helper_E68B20(quest, me, "CS_CHICKING_INTRO")
            quest:FadeScreenIn()
            quest:GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer2 < 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00e68a73 end
                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e68a8f end
            if questionAnswer2 == 1 then
                do
                    if 49 < quest:GetHeroGold() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e68a8f end
                        helpers.helper_E68B20(quest, me, "CS_CHICKING_START")
                        heroHasPlayed_ = true
                        quest:ConfiscateAllHeroWeapons()
                        quest:SetHeroWillAsUsable(false)
                        quest:SetTeleportingAsActive(false)
                        quest:GiveHeroGold(-50)
                        kickingChickens = true
                        goto LAB_00e67619
                    end
                    if not quest:IsActiveThreadTerminating() then scratchValue21 = "CS_CHICKING_NOCASH"; goto LAB_00e6760c end
                end
                goto LAB_00e68a73
            else
                scratchValue21 = "CS_CHICKING_NOKICK"
                goto LAB_00e6760c
            end
            goto FLOW_past_lab_00e6760c
            ::LAB_00e6760c::
            helpers.helper_E68B20(quest, me, scratchValue21)
            goto LAB_00e67619
            ::FLOW_past_lab_00e6760c::
            goto FLOW_past_lab_00e67619
            ::LAB_00e67619::
            quest:PauseAllNonScriptedEntities(false)
            this_00 = movie3
            goto LAB_00e6762e
            ::FLOW_past_lab_00e67619::
            goto FLOW_past_lab_00e68a73
            ::LAB_00e68a73::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            ::FLOW_past_lab_00e68a73::
            goto FLOW_hoist_lab_00e6762e_2
            ::FLOW_past_lab_00e6743e::
            goto FLOW_past_lab_00e6762e
            ::LAB_00e6762e::
            resources:DestroyMovie(this_00)
            if not kickingChickens then goto LAB_00e68a20 end
            if not quest:IsActiveThreadTerminating() then
                local infoCounter3 = quest:AddQuestInfoCounter("HUD_ICON_CHICKEN_BROWN", 5, 1.0)
                local infoCounter = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                local infoCounter2 = infoCounter
                local infoCounter4 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("MaxChickenKickingScore"), -1)
                quest:DisplayQuestInfo(true)
                quest:Pause(1.0)
                native_call_target = quest:GetThingWithScriptName("CK_HeroStart")
                local native_call_target_token = resources:TrackThingOutput(native_call_target)
                scratchValue20 = quest:GetHero()
                quest:EntityTeleportToThing(scratchValue20, native_call_target, false)
                resources:RetireThingOutput(native_call_target_token)
                native_call_target = nil
                scratchValue20 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, scratchValue20, false)
                scratchValue17 = 0
                ctr_CVar = 0
                scratchValue26 = 0
                repeat
                    if not quest:NewScriptFrame(me) then goto LAB_00e68ae2 end
                    position = quest:GetThingWithScriptName("CK_ChickenStart"):GetPos()
                    local kickingChicken01 = quest:CreateCreature("CREATURE_KICKING_CHICKEN_01", position, "KickedChicken", false)
                    quest:SetPlayerCreatureOnlyTarget(kickingChicken01)
                    quest:SetIsPushableByHero(kickingChicken01, false)
                    quest:UpdateQuestInfoCounter(infoCounter4, ctr_CVar, -1)
                    quest:UpdateQuestInfoCounter(infoCounter3, scratchValue17, -1)
                    quest:FadeScreenIn()
                    if not quest:GetStateBool("InfoDisplayed") then
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_10")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_20")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_30")
                                    while not quest:MsgIsGameInfoClickedPast() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                                    end
                                    if not quest:IsActiveThreadTerminating() then quest:SetStateBool("InfoDisplayed", true); goto FLOW_past_lab_00e68ace end
                                end
                            end
                        end
                    else
                        goto FLOW_past_lab_00e68ace
                    end
                    ::LAB_00e68ace::
                    goto LAB_00e68ae2
                    ::FLOW_past_lab_00e68ace::
                    quest:SetStateBool("ChickenLanded", false)
                    while not quest:GetStateBool("ChickenLanded") do
                        if not quest:NewScriptFrame(me) then goto LAB_00e68ace end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ace end
                    movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    scratchValue27 = "CS_CHICKING_LANDED"
                    local distanceBand = quest:GetStateInt("DistanceBand")
                    if distanceBand == 4 then
                        if not quest:IsActiveThreadTerminating() then resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_TOO_FAR_NEW"); goto LAB_00e6804a end
                        goto FLOW_hoist_lab_00e6804a_2
                    end
                    if distanceBand == 0 then
                        if not quest:IsActiveThreadTerminating() then resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_TOO_WEAK_NEW"); goto LAB_00e6805d end
                    else
                        goto FLOW_past_lab_00e68ab9
                    end
                    ::LAB_00e68ab9::
                    quest:PauseAllNonScriptedEntities(false)
                    ::LAB_00e68ac5::
                    resources:DestroyMovie(movie5)
                    goto LAB_00e68ace
                    ::FLOW_past_lab_00e68ab9::
                    if distanceBand ~= -1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                        local finalSector = quest:GetStateInt("FinalSector")
                        if finalSector == 1 then
                            if not quest:IsActiveThreadTerminating() then
                                local distanceBand2 = quest:GetStateInt("DistanceBand")
                                if distanceBand2 == 3 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_FAR_LEFT_NEW")
                                    ctr_CVar = ctr_CVar + 100
                                    scratchValue26 = ctr_CVar
                                elseif distanceBand2 == 2 then
                                    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00e6804a_2 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_CENTRE_LEFT_NEW")
                                    ctr_CVar = ctr_CVar + 50
                                    scratchValue26 = ctr_CVar
                                elseif distanceBand2 == 1 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_FRONT_LEFT_NEW")
                                    ctr_CVar = ctr_CVar + 25
                                    scratchValue26 = ctr_CVar
                                end
                                goto LAB_00e6805d
                            end
                        elseif finalSector == 2 then
                            if not quest:IsActiveThreadTerminating() then
                                local distanceBand3 = quest:GetStateInt("DistanceBand")
                                if distanceBand3 == 3 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_FAR_CENTRE_NEW")
                                    ctr_CVar = ctr_CVar + 10
                                    scratchValue26 = ctr_CVar
                                elseif distanceBand3 == 2 then
                                    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00e6804a_2 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_CENTRE_CENTRE_NEW")
                                    ctr_CVar = ctr_CVar + 25
                                    scratchValue26 = ctr_CVar
                                elseif distanceBand3 == 1 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_NEAR_CENTRE_NEW")
                                    ctr_CVar = ctr_CVar + 10
                                    scratchValue26 = ctr_CVar
                                end
                                goto LAB_00e6805d
                            end
                        else
                            if finalSector ~= 3 then
                                if finalSector == 0 then
                                    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00e6804a_2 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_OFF_COURSE")
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_NOWHERE")
                                end
                                goto LAB_00e6804a
                            end
                            goto FLOW_hoist_lab_00e6804a_1
                        end
                        goto FLOW_hoist_lab_00e6804a_2
                    end
                    goto FLOW_past_lab_00e6804a
                    ::LAB_00e6804a::
                    scratchValue22 = "CS_CHICKING_MISSED"
                    goto LAB_00e68054
                    ::FLOW_hoist_lab_00e6804a_1::
                    if not quest:IsActiveThreadTerminating() then
                        local distanceBand4 = quest:GetStateInt("DistanceBand")
                        if distanceBand4 == 3 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                            resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_FAR_RIGHT_NEW")
                            ctr_CVar = ctr_CVar + 100
                            scratchValue26 = ctr_CVar
                        elseif distanceBand4 == 2 then
                            if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00e6804a_2 end
                            resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_CENTRE_RIGHT_NEW")
                            ctr_CVar = ctr_CVar + 50
                            scratchValue26 = ctr_CVar
                        elseif distanceBand4 == 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e68ab9 end
                            resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_NEAR_RIGHT_NEW")
                            ctr_CVar = ctr_CVar + 25
                            scratchValue26 = ctr_CVar
                        end
                        goto LAB_00e6805d
                    end
                    ::FLOW_hoist_lab_00e6804a_2::
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00e68ac5
                    ::FLOW_past_lab_00e6804a::
                    if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00e6804a_2 end
                    scratchValue22 = "CS_CHICKING_FOWL"
                    ::LAB_00e68054::
                    scratchValue27 = scratchValue22
                    ::LAB_00e6805d::
                    helpers.helper_E68B20(quest, me, scratchValue27)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    scratchValue20 = quest:GetThingWithScriptName("KickedChicken")
                    quest:RemoveThing(scratchValue20, false, true)
                    scratchValue17 = scratchValue17 + 1
                    quest:ResetPlayerCreatureOnlyTarget()
                until not (scratchValue17 < 5)
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveQuestInfoElement(infoCounter4)
                    quest:RemoveQuestInfoElement(infoCounter2)
                    quest:RemoveQuestInfoElement(infoCounter3)
                    quest:ReturnAllConfiscatedItemsToHero()
                    quest:SetHeroWillAsUsable(true)
                    quest:SetTeleportingAsActive(true)
                    local native_wide_arguments = resources:NewWideTextArguments()
                    native_wide_arguments:AppendInteger((ctr_CVar + 2147483648) % 4294967296 - 2147483648)
                    native_wide_arguments:FormatGlobalText(0x143e908, "TEXT_QST_B17_SCORE")
                    scratchValue27 = "CS_CHICKING_END"
                    local scratchValue14 = string.unpack("<f", string.pack("<f", (scratchValue26 + 2147483648) % 4294967296 - 2147483648))
                    if not (((scratchValue26 + 2147483648) % 4294967296 - 2147483648) >= quest:ReadGlobalGameDataFloat(SCRIPT_DEF.CHK_HighPrize)) then
                        if not (scratchValue14 >= quest:ReadGlobalGameDataFloat(SCRIPT_DEF.CHK_MidPrize)) then
                            if not (scratchValue14 >= quest:ReadGlobalGameDataFloat(SCRIPT_DEF.CHK_LowPrize)) then
                                if not quest:IsActiveThreadTerminating() then
                                    resources:SetString(resources:MemberStringMap("csargs"), "$ANIM", "CHICK_KICK_LOW")
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LOOP", "CHICK_KICK_LOW_LOOP")
                                    if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar then
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:SetMasterGameState("MaxChickenKickingScore", (ctr_CVar + 2147483648) % 4294967296 - 2147483648)
                                            resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN_HIGH")
                                            goto LAB_00e6894f
                                        end
                                    elseif not quest:IsActiveThreadTerminating() then
                                        resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN")
                                        goto LAB_00e6894f
                                    end
                                    goto FLOW_past_lab_00e6894f
                                    ::LAB_00e6894f::
                                    goto LAB_00e68954
                                    ::FLOW_past_lab_00e6894f::
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                resources:SetString(resources:MemberStringMap("csargs"), "$ANIM", "CHICK_KICK_MID")
                                resources:SetString(resources:MemberStringMap("csargs"), "$LOOP", "CHICK_KICK_MID_LOOP")
                                if (quest:GetStateInt("PrizesWon") & 1) == 0 then
                                    if not quest:IsActiveThreadTerminating() then
                                        resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_LOW_PRIZE_FIRST")
                                        quest:SetStateInt("PrizesWon", quest:GetStateInt("PrizesWon") | 1)
                                        scratchValue27 = "CS_CHICKING_LOWPRIZE"
                                        goto LAB_00e687e4
                                    end
                                elseif not quest:IsActiveThreadTerminating() then
                                    if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar then
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:SetMasterGameState("MaxChickenKickingScore", (ctr_CVar + 2147483648) % 4294967296 - 2147483648)
                                            resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_LOW_PRIZE_AGAIN_HIGH")
                                            goto LAB_00e687e4
                                        end
                                    elseif not quest:IsActiveThreadTerminating() then
                                        resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_LOW_PRIZE_AGAIN")
                                        goto LAB_00e687e4
                                    end
                                end
                                goto FLOW_past_lab_00e687e4
                                ::LAB_00e687e4::
                                scratchValue23 = "50"
                                resources:SetString(resources:MemberStringMap("csargs"), "$PRIZE", scratchValue23)
                                goto LAB_00e68954
                                ::FLOW_past_lab_00e687e4::
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            resources:SetString(resources:MemberStringMap("csargs"), "$ANIM", "CHICK_KICK_HIGH")
                            resources:SetString(resources:MemberStringMap("csargs"), "$LOOP", "CHICK_KICK_HIGH_LOOP")
                            if (quest:GetStateInt("PrizesWon") & 2) == 0 then
                                if not quest:IsActiveThreadTerminating() then
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_MIDDLE_PRIZE_FIRST")
                                    quest:SetStateInt("PrizesWon", quest:GetStateInt("PrizesWon") | 2)
                                    scratchValue27 = "CS_CHICKING_MIDPRIZE"
                                    goto LAB_00e685d7
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar then
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:SetMasterGameState("MaxChickenKickingScore", (ctr_CVar + 2147483648) % 4294967296 - 2147483648)
                                        resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_MIDDLE_PRIZE_AGAIN_HIGH")
                                        goto LAB_00e685d7
                                    end
                                elseif not quest:IsActiveThreadTerminating() then
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_MIDDLE_PRIZE_AGAIN")
                                    goto LAB_00e685d7
                                end
                            end
                            goto FLOW_past_lab_00e685d7
                            ::LAB_00e685d7::
                            scratchValue23 = "100"
                            resources:SetString(resources:MemberStringMap("csargs"), "$PRIZE", scratchValue23)
                            goto LAB_00e68954
                            ::FLOW_past_lab_00e685d7::
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        resources:SetString(resources:MemberStringMap("csargs"), "$ANIM", "CHICK_KICK_HIGH")
                        resources:SetString(resources:MemberStringMap("csargs"), "$LOOP", "CHICK_KICK_HIGH_LOOP")
                        if (quest:GetStateInt("PrizesWon") & 4) == 0 then
                            if not quest:IsActiveThreadTerminating() then
                                quest:SetMasterGameState("MaxChickenKickingScore", (ctr_CVar + 2147483648) % 4294967296 - 2147483648)
                                quest:SetStateInt("PrizesWon", quest:GetStateInt("PrizesWon") | 4)
                                scratchValue27 = "CS_CHICKING_TOPPRIZE"
                                goto LAB_00e68954
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar then
                                if not quest:IsActiveThreadTerminating() then
                                    quest:SetMasterGameState("MaxChickenKickingScore", (ctr_CVar + 2147483648) % 4294967296 - 2147483648)
                                    resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_TOP_PRIZE_AGAIN_HIGH")
                                    goto LAB_00e68395
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                resources:SetString(resources:MemberStringMap("csargs"), "$LINE", "TEXT_QST_B17_MASTER_TOP_PRIZE_AGAIN")
                                goto LAB_00e68395
                            end
                            goto FLOW_past_lab_00e68395
                            ::LAB_00e68395::
                            scratchValue23 = "200"
                            resources:SetString(resources:MemberStringMap("csargs"), "$PRIZE", scratchValue23)
                            goto LAB_00e68954
                            ::FLOW_past_lab_00e68395::
                        end
                    end
                    goto FLOW_past_lab_00e68954
                    ::LAB_00e68954::
                    if ((ctr_CVar + 2147483648) % 4294967296 - 2147483648) == quest:GetMasterGameState("MaxChickenKickingScore") then
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e68954 end
                        resources:UpdateOnlineScore(2904, scratchValue14)
                    end
                    movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    helpers.helper_E68B20(quest, me, scratchValue27)
                    native_wide_arguments:ClearGlobalText(0x143e908)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    native_wide_arguments:Close()
                    goto LAB_00e68a20
                    ::FLOW_past_lab_00e68954::
                    native_wide_arguments:Close()
                end
                ::LAB_00e68ae2::
            end
            ::FLOW_hoist_lab_00e6762e_2::
            break
            ::FLOW_past_lab_00e6762e::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            break
        end
        ::LAB_00e68a20::
        quest:NewScriptFrame(me)
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    end
    ::LAB_00e68aeb::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId4)
    resources:ReleaseResource(resource)
end

-- ChickenMaster.Init (retail 0x00e63560)
function Init(quest, me)
    heroHasPlayed_ = false
    haveTalked_ = false
    kickingChickens = false
    ghostChat_ = false
end

-- ChickenMaster.OnPersist (retail 0x00e641b0)
function OnPersist(quest, me, context)
    local heroHasPlayed = heroHasPlayed_ or false
    heroHasPlayed_ = quest:PersistTransferBool(context, "HeroHasPlayed", heroHasPlayed)
    local haveTalked = haveTalked_ or false
    haveTalked_ = quest:PersistTransferBool(context, "HaveTalked", haveTalked)
    local ghostChat = ghostChat_ or false
    ghostChat_ = quest:PersistTransferBool(context, "GhostChat", ghostChat)
end

-- ChickenMaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

