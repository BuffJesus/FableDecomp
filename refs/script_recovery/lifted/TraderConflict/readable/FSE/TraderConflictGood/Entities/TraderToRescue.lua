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
    local scratchValue, predicateResult6, predicateResult9, predicateResult, scratchValue12
    local predicateResult14, predicateResult15, predicateResult19, controlAcquired
    local predicateResult28, predicateResult29, predicateResult30, predicateResult31
    local msgIsHitByAnySpecialAbilityFromHero, getStateBool, isRegionLoaded, conversationId, timerId
    local sequence1, getDataString, target, scratchValue27, teleporterMarker, scratchValue31
    local scratchValue32, timerId2, timerId3, scratchValue34, line, timerId4
    local msgExpressionPerformedTo, timerId5, movie3
    msgIsHitByAnySpecialAbilityFromHero = false
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("IntroDone") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        timerId5 = quest:RegisterTimer()
        timerId4 = quest:RegisterTimer()
        local banditHostageKeeper = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        isRegionLoaded = 0
        predicateResult28 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult28 then
                quest:DeregisterTimer(timerId4)
                quest:DeregisterTimer(timerId5)
                resources:ReleaseResource(resource)
                return
            end
            if me:IsTalkedToByHero() then goto LAB_00dfe32c end
            getDataString = me:GetDataString()
            if getDataString ~= nil and getDataString == "TRADERB" then
                if quest:GetStateBool("OpenedCage") then goto LAB_00dfe32c end
            end
            msgExpressionPerformedTo = me:MsgExpressionPerformedTo()
            if msgExpressionPerformedTo == nil then goto LAB_00dfe4df end
            if msgExpressionPerformedTo == nil or msgExpressionPerformedTo ~= "EXPRESSION_FOLLOW" then goto LAB_00dfe4df end
            goto LAB_00dfe32c
            goto FLOW_past_lab_00dfe4df
            ::LAB_00dfe4df::
            predicateResult29 = false
            ::FLOW_past_lab_00dfe4df::
            goto FLOW_past_lab_00dfe32c
            ::LAB_00dfe32c::
            predicateResult29 = true
            ::FLOW_past_lab_00dfe32c::
            scratchValue32 = 0
            if predicateResult29 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00dfebc5 end
                me:Speak(hero, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_INTRO", 0, false, true, false)
                getStateBool = me:IsPerformingScriptTask()
                goto LAB_00dfeb57
            end
            timerId = quest:GetTimer(timerId4)
            sequence1 = timerId == 0
            if not sequence1 then
                timerId = timerId5
                sequence1 = isRegionLoaded == 0
                if sequence1 then
                    timerId = timerId5
                    sequence1 = not (banditHostageKeeper ~= nil and banditHostageKeeper:IsAlive())
                end
            end
            if sequence1 then
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
                    timerId = timerId5
                    quest:SetTimer(timerId5, quest:GetTimer(timerId5) + 5)
                end
            end
            if quest:GetTimer(timerId) ~= 0 then goto LAB_00dfe63d end
            target = quest:GetHeroTargetedThing()
            if not (target ~= nil and target:IsEqualTo(me)) then goto LAB_00dfe63d end
            predicateResult30 = true
            if not quest:IsPlayerHoldingLockTargetButton() then goto LAB_00dfe63d end
            goto FLOW_past_lab_00dfe63d
            ::LAB_00dfe63d::
            predicateResult30 = false
            ::FLOW_past_lab_00dfe63d::
            if predicateResult30 then
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
                quest:SetTimer(timerId5, 20)
                quest:SetTimer(timerId4, quest:GetTimer(timerId4) + 5)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00dfe8b5
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    msgIsHitByAnySpecialAbilityFromHero = true
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dfe8b5 end
                end
                msgIsHitByAnySpecialAbilityFromHero = true
                predicateResult31 = false
            end
            goto FLOW_past_lab_00dfe8b5
            ::LAB_00dfe8b5::
            predicateResult31 = true
            ::FLOW_past_lab_00dfe8b5::
            if not predicateResult31 then quest:NewScriptFrame(me); predicateResult28 = quest:IsActiveThreadTerminating(); goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
            quest:SetTimer(timerId5, 10)
            quest:NewScriptFrame(me)
            predicateResult28 = quest:IsActiveThreadTerminating()
            ::continue_1::
        until false
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00dfeb57::
    if getStateBool then
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            goto LAB_00e005d5
        end
        getStateBool = me:IsPerformingScriptTask()
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
        quest:EntityFollowThing(me, hero, 3.0, true)
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
            scratchValue27 = "HUD_QUEST_ICON_TRADER_HAT_02"
            goto LAB_00dfee1b
            ::FLOW_past_lab_00dfee05::
            if me:GetDataString() == "TRADERC" then goto LAB_00dfee05 end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            scratchValue27 = "HUD_QUEST_ICON_TRADER"
            goto LAB_00dfee1b
        end
        goto FLOW_past_lab_00dfee1b
        ::LAB_00dfee1b::
        barIndex = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, scratchValue27, 1.0)
        ::FLOW_past_lab_00dfee1b::
        resources:PrepareResource(resource)
        while not quest:IsEntityFollowingHero(me) do
            if not quest:NewScriptFrame(me) then goto LAB_00e005d5 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + 1)
            scratchValue = 10
            timerId2 = quest:RegisterTimer()
            quest:SetTimer(timerId2, 20)
            timerId3 = quest:RegisterTimer()
            predicateResult6 = quest:IsActiveThreadTerminating()
            scratchValue31 = 0
            while not predicateResult6 do
                isRegionLoaded = 1 - (quest:IsRegionLoaded("BanditCampEntrance") and 1 or 0)
                while isRegionLoaded ~= 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        scratchValue34 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                        line = scratchValue34 .. tostring(scratchValue)
                        if not quest:TextEntryExists(line) then
                            scratchValue = 10
                            line = scratchValue34 .. tostring(10)
                        end
                        -- TODO(native): xStack_118 = (CCharString)((int)CVar18 + 0xa);
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        local movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                        resources:PrepareResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    end
                    local timeRemaining = quest:GetTimer(timerId5) == 0 and IsPlayerThreateningEntity(me) ~= 0
                    local scratchValue3 = timeRemaining and quest:IsPlayerHoldingLockTargetButton()
                    if scratchValue3 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                        quest:SetTimer(timerId5, 20)
                    end
                    scratchValue32 = scratchValue31 | 64
                    if me:MsgIsHitBy("") then
                        goto LAB_00dff3b5
                    else
                        scratchValue32 = scratchValue31 | 192
                        if me:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00dff3b5 end
                        goto LAB_00dff3f2
                    end
                    goto FLOW_past_lab_00dff3b5
                    ::LAB_00dff3b5::
                    scratchValue32 = scratchValue32 | 256
                    predicateResult9 = true
                    if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff3f2 end
                    ::FLOW_past_lab_00dff3b5::
                    goto FLOW_past_lab_00dff3f2
                    ::LAB_00dff3f2::
                    predicateResult9 = false
                    ::FLOW_past_lab_00dff3f2::
                    if scratchValue32 & 256 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xfffffeff
                    end
                    if scratchValue32 & 64 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xffffffbf
                    end
                    if predicateResult9 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        scratchValue31 = scratchValue32
                        local health = quest:GetHealth(me) <= 5.0 or quest:GetTimer(timerId3) ~= 0
                        if health then
                            goto LAB_00dff54a
                        else
                            scratchValue31 = scratchValue32 | 512
                            if me:MsgIsHitByHero() then goto LAB_00dff54a end
                            scratchValue31 = scratchValue32 | 1536
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue31 = scratchValue32 | 3584
                                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff54a end
                            end
                            scratchValue32 = scratchValue31
                            predicateResult = true
                        end
                        goto FLOW_past_lab_00dff54a
                        ::LAB_00dff54a::
                        scratchValue32 = scratchValue31
                        predicateResult = false
                        ::FLOW_past_lab_00dff54a::
                        if scratchValue32 & 2048 ~= 0 then
                            scratchValue32 = scratchValue32 & 0xfffff7ff
                        end
                        if scratchValue32 & 1024 ~= 0 then
                            scratchValue32 = scratchValue32 & 0xfffffbff
                        end
                        if scratchValue32 & 512 ~= 0 then
                            scratchValue32 = scratchValue32 & 0xfffffdff
                        end
                        if predicateResult then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            if me:GetDataString() ~= "TRADERA" then
                                if me:GetDataString() ~= "TRADERB" then
                                    scratchValue = 1
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    quest:PlaySoundOnThing(me, "SND_MM_TRADER_C_SCREAM_0" .. tostring(1))
                                    scratchValue12 = 1 == 4
                                else
                                    scratchValue = 1
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    quest:PlaySoundOnThing(me, "SND_MM_TRADER_B_SCREAM_0" .. tostring(1))
                                    scratchValue12 = 1 == 3
                                end
                            else
                                scratchValue = 1
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                quest:PlaySoundOnThing(me, "SND_MM_TRADER_A_SCREAM_0" .. tostring(1))
                                scratchValue12 = 1 == 3
                            end
                            if scratchValue12 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                scratchValue = 0
                            end
                            -- TODO(native): xStack_14c = (CCharString)((int)CVar18 + 1);
                            quest:SetTimer(timerId3, 4)
                        else
                            scratchValue31 = scratchValue32
                            if quest:GetHealth(me) <= 5.0 then
                                goto LAB_00dff887
                            else
                                scratchValue31 = scratchValue32 | 4096
                                if not me:MsgIsHitByHero() then
                                    scratchValue31 = scratchValue32 | 0x3000
                                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                        scratchValue31 = scratchValue32 | 0x7000
                                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff880 end
                                    end
                                    goto LAB_00dff887
                                end
                                ::LAB_00dff880::
                                scratchValue32 = scratchValue31
                                predicateResult14 = true
                            end
                            goto FLOW_past_lab_00dff887
                            ::LAB_00dff887::
                            scratchValue32 = scratchValue31
                            predicateResult14 = false
                            ::FLOW_past_lab_00dff887::
                            if scratchValue32 & 0x4000 ~= 0 then
                                scratchValue32 = scratchValue32 & 0xffffbfff
                            end
                            if scratchValue32 & 0x2000 ~= 0 then
                                scratchValue32 = scratchValue32 & 0xffffdfff
                            end
                            if scratchValue32 & 4096 ~= 0 then
                                scratchValue32 = scratchValue32 & 0xffffefff
                            end
                            if predicateResult14 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                conversationId = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId, hero)
                                quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
                            end
                        end
                    end
                    scratchValue31 = scratchValue32 | 0x8000
                    if me:MsgIsHitByHero() then
                        goto LAB_00dffa46
                    else
                        scratchValue31 = scratchValue32 | 0x18000
                        if me:MsgIsHitByAnySpecialAbilityFromHero() then
                            scratchValue31 = scratchValue32 | 0x38000
                            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dffa46 end
                        end
                        scratchValue32 = scratchValue31
                        predicateResult15 = false
                    end
                    goto FLOW_past_lab_00dffa46
                    ::LAB_00dffa46::
                    scratchValue32 = scratchValue31
                    predicateResult15 = true
                    ::FLOW_past_lab_00dffa46::
                    if scratchValue32 & 0x20000 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xfffdffff
                    end
                    if scratchValue32 & 0x10000 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xfffeffff
                    end
                    if scratchValue32 & 0x8000 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xffff7fff
                    end
                    local sequence3 = predicateResult15 and quest:IsActiveThreadTerminating()
                    if sequence3 then goto LAB_00e005b1 end
                    msgExpressionPerformedTo = me:MsgExpressionPerformedTo()
                    if msgExpressionPerformedTo == nil then isRegionLoaded = (1 - (quest:IsRegionLoaded("BanditCampEntrance") and 1 or 0)); scratchValue31 = scratchValue32; goto continue_3 end
                    if msgExpressionPerformedTo ~= nil and msgExpressionPerformedTo == "EXPRESSION_WAIT" then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                    end
                    isRegionLoaded = 1 - (quest:IsRegionLoaded("BanditCampEntrance") and 1 or 0)
                    scratchValue31 = scratchValue32
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
                            scratchValue = 10
                            line = scratchValue34 .. tostring(10)
                        end
                        -- TODO(native): xStack_118 = (CCharString)((int)CVar18 + 0xa);
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                        resources:PrepareResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                    end
                    local timeRemaining2 = quest:GetTimer(timerId5) == 0 and IsPlayerThreateningEntity(me) ~= 0
                    local scratchValue6 = timeRemaining2 and quest:IsPlayerHoldingLockTargetButton()
                    if scratchValue6 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                        quest:SetTimer(timerId5, 20)
                    end
                    scratchValue32 = scratchValue31 | 0x40000
                    if me:MsgIsHitByHero() then
                        goto LAB_00e0006b
                    else
                        scratchValue32 = scratchValue31 | 0xc0000
                        if me:MsgIsHitByAnySpecialAbilityFromHero() then
                            scratchValue32 = scratchValue31 | 0x1c0000
                            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e0006b end
                        end
                        predicateResult19 = false
                    end
                    goto FLOW_past_lab_00e0006b
                    ::LAB_00e0006b::
                    predicateResult19 = true
                    ::FLOW_past_lab_00e0006b::
                    if scratchValue32 & 0x100000 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xffefffff
                    end
                    if scratchValue32 & 0x80000 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xfff7ffff
                    end
                    if scratchValue32 & 0x40000 ~= 0 then
                        scratchValue32 = scratchValue32 & 0xfffbffff
                    end
                    if predicateResult19 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
                    end
                    msgExpressionPerformedTo = me:MsgExpressionPerformedTo()
                    if msgExpressionPerformedTo ~= nil then
                        if msgExpressionPerformedTo ~= nil and msgExpressionPerformedTo == "EXPRESSION_WAIT" then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                        end
                    end
                    if not quest:IsDistanceBetweenThingsUnder(me, teleporterMarker, 20.0) then isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance"); scratchValue31 = scratchValue32; goto continue_5 end
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
                    resources:PrepareResource(resource)
                    controlAcquired = resources:TryAcquire(resource, me, 4)
                    goto LAB_00e0035a
                    isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance")
                    scratchValue31 = scratchValue32
                    ::continue_5::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                quest:NewScriptFrame(me)
                predicateResult6 = quest:IsActiveThreadTerminating()
            end
            goto LAB_00e005b1
        end
    end
    ::FLOW_past_lab_00dfebc5::
    goto LAB_00e005d5
    ::LAB_00e0035a::
    if not controlAcquired then
        if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
        controlAcquired = resources:TryAcquire(resource, me, 4)
        goto LAB_00e0035a
    end
    if not quest:IsActiveThreadTerminating() then
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_FREED_10", me, hero, false)
        while quest:IsDistanceBetweenThingsOver(me, teleporterMarker, 2.0) do
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
                resources:PrepareResource(resource)
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
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId2)
    ::LAB_00e005d5::
    quest:DeregisterTimer(timerId4)
    quest:DeregisterTimer(timerId5)
    resources:ReleaseResource(resource)
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

