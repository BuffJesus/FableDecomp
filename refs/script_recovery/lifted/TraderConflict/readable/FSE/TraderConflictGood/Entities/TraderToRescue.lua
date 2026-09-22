-- Readable native conversion: TraderToRescue. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("TraderConflictGood.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local barIndex

-- TraderToRescue.Main (retail 0x00dfe0f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult, predicateResult14, scratchValue12, predicateResult18, predicateResult19
    local predicateResult23, scratchValue13, isRegionLoaded, dist, getHeroTargetedThing
    local scratchValue23, getDataString, scratchValue25, banditHostageKeeper, teleporterMarker
    local conversationId, scratchValue29, scratchValue30, scratchValue31, scratchValue32
    local scratchValue33, timerId, timerId2, scratchValue34, line, timerId3, scratchValue36
    local timerId4, movie3, timerId5
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("IntroDone") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    scratchValue32 = resources:NewResource()
    resources:PrepareResource(scratchValue32)
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e005f9 end
    end
    if not quest:IsActiveThreadTerminating() then
        timerId4 = quest:RegisterTimer()
        timerId3 = quest:RegisterTimer()
        banditHostageKeeper = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        isRegionLoaded = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId4)
                resources:ReleaseResource(scratchValue32)
                return
            end
            if me:IsTalkedToByHero() then goto LAB_00dfe32c end
            getDataString = me:GetDataString()
            if getDataString ~= nil and getDataString == "TRADERB" then
                if quest:GetStateBool("OpenedCage") then goto LAB_00dfe32c end
            end
            scratchValue36 = me:MsgExpressionPerformedTo()
            if scratchValue36 == nil then goto LAB_00dfe4df end
            if scratchValue36 == nil then
                goto LAB_00dfe4df
            else
                if scratchValue36 ~= "EXPRESSION_FOLLOW" then goto LAB_00dfe4df end
            end
            goto LAB_00dfe32c
            goto FLOW_past_lab_00dfe4df
            ::LAB_00dfe4df::
            predicateResult3 = false
            ::FLOW_past_lab_00dfe4df::
            goto FLOW_past_lab_00dfe32c
            ::LAB_00dfe32c::
            predicateResult3 = true
            ::FLOW_past_lab_00dfe32c::
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scratchValue32 = resources:ScriptThing(scratchValue33)
                if quest:GetHealth(scratchValue32) <= 0.0 then goto LAB_00dfebc5 end
                me:Speak(hero, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_INTRO", 0, false, true, false)
                scratchValue13 = me:IsPerformingScriptTask()
                goto LAB_00dfeb57
            end
            local sequence1 = quest:GetTimer(timerId3) == 0 or (isRegionLoaded == 0 and not (banditHostageKeeper ~= nil and banditHostageKeeper:IsAlive()))
            if sequence1 then
                dist = 15.0
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    local scratchValue2 = isRegionLoaded == 0 and not (banditHostageKeeper ~= nil and banditHostageKeeper:IsAlive())
                    if scratchValue2 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_KEEPERISDEAD", me, hero, false)
                        isRegionLoaded = 1
                    elseif math.random(0, 32767) % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_OVERHERE", me, hero, false)
                    end
                    quest:SetTimer(timerId4, 10)
                    quest:SetTimer(timerId4, quest:GetTimer(timerId4) + 5)
                end
            end
            if quest:GetTimer(timerId4) ~= 0 then goto LAB_00dfe63d end
            getHeroTargetedThing = quest:GetHeroTargetedThing()
            if not (getHeroTargetedThing ~= nil and getHeroTargetedThing:IsEqualTo(me)) then goto LAB_00dfe63d end
            predicateResult4 = true
            if not quest:IsPlayerHoldingLockTargetButton() then goto LAB_00dfe63d end
            goto FLOW_past_lab_00dfe63d
            ::LAB_00dfe63d::
            predicateResult4 = false
            ::FLOW_past_lab_00dfe63d::
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                local sequence = IsPlayerThreateningEntity(me) == 0 or math.random(0, 32767) % 3 ~= 0
                if sequence then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATENWEAPON", me, hero, false)
                end
                quest:SetTimer(timerId4, 20)
                quest:SetTimer(quest:GetTimer(timerId5) + 5, dist)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00dfe8b5
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dfe8b5 end
                end
                predicateResult5 = false
            end
            goto FLOW_past_lab_00dfe8b5
            ::LAB_00dfe8b5::
            predicateResult5 = true
            ::FLOW_past_lab_00dfe8b5::
            if not predicateResult5 then quest:NewScriptFrame(me); predicateResult2 = quest:IsActiveThreadTerminating(); goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
            quest:SetTimer(timerId4, 10)
            quest:NewScriptFrame(me)
            predicateResult2 = quest:IsActiveThreadTerminating()
            ::continue_1::
        until false
    end
    goto LAB_00e005f9
    ::LAB_00dfeb57::
    if scratchValue13 then
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            goto LAB_00e005d5
        end
        scratchValue13 = me:IsPerformingScriptTask()
        goto LAB_00dfeb57
    end
    if not quest:IsActiveThreadTerminating() then goto LAB_00dfebc5 end
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    goto FLOW_past_lab_00dfebc5
    ::LAB_00dfebc5::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    if not quest:IsActiveThreadTerminating() then
        quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
        quest:ActivateQuest("Q_TraderConflictGood_Extras")
        quest:SetIsPushableByHero(me, true)
        quest:EntityFollowThing(me, hero, nil --[[missing]], nil --[[missing]])
        quest:SetEntityAsRegionFollowing(hero, me, true)
        quest:EntitySetOpinionReactionsEnabled(me, false)
        quest:EntitySetDeedReactionsEnabled(me, false)
        quest:EntitySetCombatEnabled(me, false)
        quest:EntitySetInFaction(me, "FACTION_TRADERS")
        quest:DisplayQuestInfo(true)
        if me:GetDataString() ~= "TRADERA" then
            if me:GetDataString() == "TRADERB" then
                goto LAB_00dfee05
            end
            goto FLOW_past_lab_00dfee05
            ::LAB_00dfee05::
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            scratchValue25 = "HUD_QUEST_ICON_TRADER_HAT_02"
            goto LAB_00dfee1b
            ::FLOW_past_lab_00dfee05::
            if me:GetDataString() == "TRADERC" then goto LAB_00dfee05 end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            scratchValue25 = "HUD_QUEST_ICON_TRADER"
            goto LAB_00dfee1b
        end
        goto FLOW_past_lab_00dfee1b
        ::LAB_00dfee1b::
        barIndex = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, scratchValue25, 1.0)
        ::FLOW_past_lab_00dfee1b::
        resources:PrepareResource(scratchValue32)
        while not quest:IsEntityFollowingHero(me) do
            if not quest:NewScriptFrame(me) then goto LAB_00e005d5 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + 1)
            scratchValue = 10
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 20)
            timerId2 = quest:RegisterTimer()
            while not quest:IsActiveThreadTerminating() do
                isRegionLoaded = not quest:IsRegionLoaded("BanditCampEntrance")
                while isRegionLoaded do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        scratchValue34 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                        line = scratchValue34 .. tostring(scratchValue)
                        if not quest:TextEntryExists(line) then
                            line = scratchValue34 .. tostring(10)
                        end
                        -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                        resources:PrepareResource(scratchValue23)
                        while not me:AcquireControl(4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        local movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if quest:GetHealth(resources:ScriptThing(timerId2)) > 0.0 then
                            me:Speak(hero, line, 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00e005b1
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00e005b1
                            end
                        end
                        resources:PrepareResource(scratchValue32)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    end
                    local timeRemaining = quest:GetTimer(timerId4) == 0 and IsPlayerThreateningEntity(me) ~= 0
                    local scratchValue3 = timeRemaining and quest:IsPlayerHoldingLockTargetButton()
                    if scratchValue3 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                        quest:SetTimer(timerId4, 20)
                    end
                    if me:MsgIsHitBy("") then goto LAB_00dff3b5 end
                    if me:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00dff3b5 end
                    goto LAB_00dff3f2
                    goto FLOW_past_lab_00dff3f2
                    ::LAB_00dff3f2::
                    predicateResult = false
                    ::FLOW_past_lab_00dff3f2::
                    goto FLOW_past_lab_00dff3b5
                    ::LAB_00dff3b5::
                    predicateResult = true
                    if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff3f2 end
                    ::FLOW_past_lab_00dff3b5::
                    if predicateResult then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        local health = quest:GetHealth(me) <= 5.0 or quest:GetTimer(scratchValue32) ~= 0
                        if health then
                            goto LAB_00dff54a
                        else
                            if me:MsgIsHitByHero() then goto LAB_00dff54a end
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff54a end
                            end
                            predicateResult14 = true
                        end
                        goto FLOW_past_lab_00dff54a
                        ::LAB_00dff54a::
                        predicateResult14 = false
                        ::FLOW_past_lab_00dff54a::
                        if predicateResult14 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            if me:GetDataString() ~= "TRADERA" then
                                if me:GetDataString() ~= "TRADERB" then
                                    scratchValue = scratchValue32
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    quest:PlaySoundOnThing(me, "SND_MM_TRADER_C_SCREAM_0" .. tostring(scratchValue32))
                                    scratchValue12 = scratchValue == 4
                                else
                                    scratchValue = scratchValue32
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    quest:PlaySoundOnThing(me, "SND_MM_TRADER_B_SCREAM_0" .. tostring(scratchValue32))
                                    scratchValue12 = scratchValue == 3
                                end
                            else
                                scratchValue = scratchValue32
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                quest:PlaySoundOnThing(me, "SND_MM_TRADER_A_SCREAM_0" .. tostring(scratchValue32))
                                scratchValue12 = scratchValue == 3
                            end
                            if scratchValue12 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                scratchValue = 0
                            end
                            -- TODO(native): xStack_148 = (CCharString)((int)CVar13 + 1);
                            quest:SetTimer(4, scratchValue)
                        else
                            if quest:GetHealth(me) <= 5.0 then
                                goto LAB_00dff887
                            else
                                if not me:MsgIsHitByHero() then
                                    if not (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)) then
                                        goto LAB_00dff887
                                    end
                                end
                                predicateResult18 = true
                            end
                            goto FLOW_past_lab_00dff887
                            ::LAB_00dff887::
                            predicateResult18 = false
                            ::FLOW_past_lab_00dff887::
                            if predicateResult18 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                conversationId = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId, hero)
                                quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
                            end
                        end
                    end
                    if me:MsgIsHitByHero() then goto LAB_00dffa46 end
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dffa46 end
                    end
                    predicateResult19 = false
                    goto FLOW_past_lab_00dffa46
                    ::LAB_00dffa46::
                    predicateResult19 = true
                    ::FLOW_past_lab_00dffa46::
                    local sequence3 = predicateResult19 and quest:IsActiveThreadTerminating()
                    if sequence3 then goto LAB_00e005b1 end
                    if not me:MsgExpressionPerformedTo() then isRegionLoaded = not quest:IsRegionLoaded("BanditCampEntrance"); scratchValue = 10; goto continue_3 end
                    if line ~= nil and line == "EXPRESSION_WAIT" then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                    end
                    isRegionLoaded = not quest:IsRegionLoaded("BanditCampEntrance")
                    scratchValue = 10
                    ::continue_3::
                end
                if quest:IsActiveThreadTerminating() then break end
                teleporterMarker = quest:GetThingWithScriptName("TeleporterMarker")
                isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance")
                while isRegionLoaded do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        scratchValue34 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                        line = scratchValue34 .. tostring(scratchValue)
                        if not quest:TextEntryExists(line) then
                            line = scratchValue34 .. tostring(10)
                        end
                        -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                        resources:PrepareResource(scratchValue23)
                        while not me:AcquireControl(4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        movie3 = resources:ScriptThing(scratchValue32)
                        if quest:GetHealth(movie3) > 0.0 then
                            me:Speak(hero, line, 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00e005ac
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00e005ac
                            end
                        end
                        resources:PrepareResource(scratchValue32)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                    end
                    local timeRemaining2 = quest:GetTimer(timerId4) == 0 and IsPlayerThreateningEntity(me) ~= 0
                    local scratchValue6 = timeRemaining2 and quest:IsPlayerHoldingLockTargetButton()
                    if scratchValue6 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                        quest:SetTimer(timerId4, 20)
                    end
                    if me:MsgIsHitByHero() then goto LAB_00e0006b end
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e0006b end
                    end
                    predicateResult23 = false
                    goto FLOW_past_lab_00e0006b
                    ::LAB_00e0006b::
                    predicateResult23 = true
                    ::FLOW_past_lab_00e0006b::
                    if predicateResult23 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
                    end
                    if me:MsgExpressionPerformedTo() then
                        if timerId3 ~= nil and timerId3 == "EXPRESSION_WAIT" then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                        end
                    end
                    if not quest:IsDistanceBetweenThingsUnder(me, scratchValue30, 20.0) then isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance"); scratchValue = 10; goto continue_5 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                    quest:SetStateInt("TradersReachedTeleporter", quest:GetStateInt("TradersReachedTeleporter") + 1)
                    quest:EntityStopFollowing(me)
                    quest:SetEntityAsRegionFollowing(hero, me, false)
                    quest:EntitySetAsScared(me, false)
                    if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        quest:SetStateBool("OutroStart", true)
                        quest:SetStateBool("MissionSucceeded", true)
                        quest:NewScriptFrame(me)
                        helpers.helper_DFDED0(quest, me, "CS_TRADERCON_GOOD_OUTRO")
                        quest:SetStateBool("OutroDone", true)
                        goto LAB_00e00595
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                    resources:PrepareResource(scratchValue31)
                    scratchValue13 = me:AcquireControl(4)
                    goto LAB_00e0035a
                    isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance")
                    scratchValue = 10
                    ::continue_5::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                quest:NewScriptFrame(me)
            end
            goto LAB_00e005b1
        end
    end
    ::FLOW_past_lab_00dfebc5::
    goto LAB_00e005d5
    ::LAB_00e0035a::
    if not scratchValue13 then
        if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
        scratchValue13 = me:AcquireControl(4)
        goto LAB_00e0035a
    end
    if not quest:IsActiveThreadTerminating() then
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_FREED_10", me, hero, false)
        while quest:IsDistanceBetweenThingsOver(me, scratchValue29, 2.0) do
            if quest:GetStateBool("OutroStart") then break end
            if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                me:MoveToThing(teleporterMarker, 1.0, ENTITY_MOVE_RUN)
            end
        end
        if not quest:IsActiveThreadTerminating() then
            if not quest:GetStateBool("OutroStart") then
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveQuestInfoElement(barIndex)
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                resources:PrepareResource(banditHostageKeeper)
                while not quest:GetStateBool("OutroDone") do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00e00595 end
            end
        end
    end
    goto FLOW_past_lab_00e00595
    ::LAB_00e00595::
    quest:RemoveThing(me, false, true)
    ::FLOW_past_lab_00e00595::
    ::LAB_00e005ac::
    ::LAB_00e005b1::
    quest:DeregisterTimer(timerId2)
    quest:DeregisterTimer(timerId)
    ::LAB_00e005d5::
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId4)
    ::LAB_00e005f9::
    resources:ReleaseResource(scratchValue32)
end

-- TraderToRescue.Init (retail 0x00dfb1b0)
function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsScared(me, true)
    quest:EntitySetInFaction(me, "FACTION_NEUTRAL")
    quest:EntitySetAsAllowedToFollowHero(me, true)
    barIndex = 0
end

-- TraderToRescue.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TraderToRescue.OnPredicateFail (retail 0x00dfb100)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED", true)
    end
end

