-- Readable native conversion: TraderToRescue. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local barIndex

-- TraderToRescue.Main (retail 0x00dfe0f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult, predicateResult14, scratchValue10, predicateResult18, predicateResult19
    local predicateResult23, scratchValue11, isRegionLoaded, dist, health, timerId, sequence1
    local sequence, getDataString, scratchValue20, line, banditHostageKeeper, scratchValue21
    local scratchValue22, scratchValue23, hero6, hero7, hero8, hero9, hero10, scratchValue24
    local scratchValue25, conversationId, scratchValue26, scratchValue27, scratchValue28
    local scratchValue29, timerId2, timerId3, scratchValue30, scratchValue31, timerId4
    local scratchValue32, movie3, timerId6
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("IntroDone") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    scratchValue28 = resources:NewResource()
    -- TODO(native): xStack_148[0] = 0;
    resources:PrepareResource(scratchValue28)
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e005f9 end
    end
    if not quest:IsActiveThreadTerminating() then
        local timerId5 = quest:RegisterTimer()
        timerId4 = quest:RegisterTimer()
        banditHostageKeeper = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        isRegionLoaded = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(timerId4)
                quest:DeregisterTimer(unaff_EBX)
                resources:ReleaseResource(scratchValue28)
                return
            end
            scratchValue24 = unaff_EBP | 1
            if me:IsTalkedToByHero() then goto LAB_00dfe32c end
            scratchValue24 = unaff_EBP | 3
            getDataString = me:GetDataString()
            if getDataString ~= nil and getDataString == "TRADERB" then
                if quest:GetStateBool("OpenedCage") then goto LAB_00dfe32c end
            end
            scratchValue32 = me:MsgExpressionPerformedTo()
            if scratchValue32 == nil then goto LAB_00dfe4df end
            if scratchValue32 == nil then
                goto LAB_00dfe4df
            else
                if scratchValue32 ~= "EXPRESSION_FOLLOW" then goto LAB_00dfe4df end
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
            if scratchValue24 & 2 ~= 0 then
                scratchValue24 = scratchValue24 & 0xfffffffd
            end
            if scratchValue24 & 1 ~= 0 then
                scratchValue24 = scratchValue24 & 0xfffffffe
            end
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scratchValue28 = resources:ScriptThing(scratchValue29)
                if quest:GetHealth(scratchValue28) <= 0.0 then goto LAB_00dfebc5 end
                timerId = 0
                line = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_INTRO"
                me:Speak(hero, line, 0, false, true, false)
                scratchValue11 = me:IsPerformingScriptTask()
                goto LAB_00dfeb57
            end
            sequence1 = quest:GetTimer(timerId4) == 0
            if not sequence1 then
                sequence1 = isRegionLoaded == 0
                if sequence1 then
                    -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
                    scratchValue11 = nil --[[unresolved native value]]
                    sequence1 = scratchValue11 == 0
                end
            end
            if sequence1 then
                dist = 15.0
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    sequence = isRegionLoaded == 0
                    if sequence then
                        -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
                        scratchValue11 = nil --[[unresolved native value]]
                        sequence = scratchValue11 == 0
                    end
                    if sequence then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        hero6 = hero
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_KEEPERISDEAD", me, hero, false)
                        isRegionLoaded = 1
                    elseif math.random(0, 32767) % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        hero7 = hero
                        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_OVERHERE", me, hero, false)
                    end
                    quest:SetTimer(timerId5, 10)
                    quest:SetTimer(unaff_EBX, quest:GetTimer(unaff_EBX) + 5)
                end
            end
            if quest:GetTimer(unaff_EBX) == 0 then
                scratchValue24 = scratchValue24 | 4
                -- TODO(native): bVar2 = (**(*(iVar11 + 0x0) + 0x138))((me))
    --[[unresolved native value]]
                if not nil then goto LAB_00dfe63d end
                predicateResult4 = true
                if not quest:IsPlayerHoldingLockTargetButton() then goto LAB_00dfe63d end
            else
                goto LAB_00dfe63d
            end
            goto FLOW_past_lab_00dfe63d
            ::LAB_00dfe63d::
            predicateResult4 = false
            ::FLOW_past_lab_00dfe63d::
            if scratchValue24 & 4 ~= 0 then
                scratchValue24 = scratchValue24 & 0xfffffffb
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                local sequence3 = IsPlayerThreateningEntity(me) == 0 or math.random(0, 32767) % 3 ~= 0
                if sequence3 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    hero8 = hero
                    quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    hero9 = hero
                    quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATENWEAPON", me, hero, false)
                end
                quest:SetTimer(unaff_EBX, 20)
                quest:SetTimer(quest:GetTimer(timerId6) + 5, dist)
            end
            -- TODO(native): unaff_EBP = uVar12 | 8;
            if me:MsgIsHitByHero() then
                goto LAB_00dfe8b5
            else
                -- TODO(native): unaff_EBP = uVar12 | 0x18;
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    -- TODO(native): unaff_EBP = uVar12 | 0x38;
                    if not me:MsgIsHitByHeroSpecialAbility(me) then goto LAB_00dfe8b5 end
                end
                predicateResult5 = false
            end
            goto FLOW_past_lab_00dfe8b5
            ::LAB_00dfe8b5::
            predicateResult5 = true
            ::FLOW_past_lab_00dfe8b5::
            if unaff_EBP & 32 ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xffffffdf;
            end
            if unaff_EBP & 16 ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xffffffef;
            end
            if unaff_EBP & 8 ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xfffffff7;
            end
            if not predicateResult5 then quest:NewScriptFrame(me); predicateResult2 = quest:IsActiveThreadTerminating(); goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            hero10 = hero
            quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
            quest:SetTimer(unaff_EBX, 10)
            quest:NewScriptFrame(me)
            predicateResult2 = quest:IsActiveThreadTerminating()
            ::continue_1::
        until false
    end
    goto LAB_00e005f9
    ::LAB_00dfeb57::
    if scratchValue11 then
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            goto LAB_00e005d5
        end
        scratchValue11 = me:IsPerformingScriptTask()
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
        quest:SetIsPushableByHero(hero, __unknown_push)
        quest:EntityFollowThing(me, hero, nil --[[missing]], nil --[[missing]])
        quest:SetEntityAsRegionFollowing(hero, me, true)
        quest:EntitySetOpinionReactionsEnabled(me, false)
        quest:EntitySetDeedReactionsEnabled(me, false)
        quest:EntitySetCombatEnabled(me, false)
        quest:EntitySetInFaction(me, "FACTION_TRADERS")
        quest:DisplayQuestInfo(true)
        getDataString = me:GetDataString()
        if getDataString ~= "TRADERA" then
            getDataString = me:GetDataString()
            if getDataString == "TRADERB" then
                goto LAB_00dfee05
            end
            goto FLOW_past_lab_00dfee05
            ::LAB_00dfee05::
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            scratchValue20 = "HUD_QUEST_ICON_TRADER_HAT_02"
            goto LAB_00dfee1b
            ::FLOW_past_lab_00dfee05::
            getDataString = me:GetDataString()
            if getDataString == "TRADERC" then goto LAB_00dfee05 end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
            scratchValue20 = "HUD_QUEST_ICON_TRADER"
            goto LAB_00dfee1b
        end
        goto FLOW_past_lab_00dfee1b
        ::LAB_00dfee1b::
        barIndex = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, scratchValue20, 1.0)
        ::FLOW_past_lab_00dfee1b::
        resources:PrepareResource(scratchValue28)
        while not quest:IsEntityFollowingHero(me) do
            if not quest:NewScriptFrame(me) then goto LAB_00e005d5 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + 1)
            scratchValue = 10
            timerId2 = quest:RegisterTimer()
            quest:SetTimer(timerId2, 20)
            timerId3 = quest:RegisterTimer()
            while not quest:IsActiveThreadTerminating() do
                isRegionLoaded = not quest:IsRegionLoaded("BanditCampEntrance")
                while isRegionLoaded do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        scratchValue30 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                        getDataString = scratchValue30 .. tostring(scratchValue)
                        scratchValue31 = getDataString
                        if not quest:TextEntryExists() then
                            getDataString = scratchValue30 .. tostring(10)
                            scratchValue31 = getDataString
                        end
                        -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                        resources:PrepareResource(scratchValue28)
                        while not me:AcquireControl(4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        local movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if quest:GetHealth(resources:ScriptThing(timerId3)) > 0.0 then
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
                        resources:PrepareResource(scratchValue28)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    end
                    local timeRemaining = quest:GetTimer(unaff_EBX) == 0 and IsPlayerThreateningEntity(me) ~= 0
                    local scratchValue2 = timeRemaining and quest:IsPlayerHoldingLockTargetButton()
                    if scratchValue2 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        conversationId = quest:AddNewConversation(banditHostageKeeper, me, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN"
                        quest:AddLineToConversation(conversationId, getDataString, hero, nil --[[missing]], false)
                        quest:SetTimer(unaff_EBX, 20)
                    end
                    scratchValue25 = scratchValue24 | 64
                    if me:MsgIsHitBy("") then goto LAB_00dff3b5 end
                    scratchValue25 = scratchValue24 | 192
                    if me:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00dff3b5 end
                    goto LAB_00dff3f2
                    goto FLOW_past_lab_00dff3f2
                    ::LAB_00dff3f2::
                    predicateResult = false
                    ::FLOW_past_lab_00dff3f2::
                    goto FLOW_past_lab_00dff3b5
                    ::LAB_00dff3b5::
                    scratchValue25 = scratchValue25 | 256
                    predicateResult = true
                    if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff3f2 end
                    ::FLOW_past_lab_00dff3b5::
                    if scratchValue25 & 256 ~= 0 then
                        scratchValue25 = scratchValue25 & 0xfffffeff
                    end
                    if SUB41(scratchValue25,0) < 0 then
                        scratchValue25 = scratchValue25 & 0xffffff7f
                    end
                    if scratchValue25 & 64 ~= 0 then
                        scratchValue25 = scratchValue25 & 0xffffffbf
                    end
                    if predicateResult then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        health = quest:GetHealth(nil --[[missing]])
                        scratchValue24 = scratchValue25
                        local scratchValue4 = health <= 5.0 or quest:GetTimer(scratchValue28) ~= 0
                        if scratchValue4 then
                            goto LAB_00dff54a
                        else
                            scratchValue24 = scratchValue25 | 512
                            if me:MsgIsHitByHero() then goto LAB_00dff54a end
                            scratchValue24 = scratchValue25 | 1536
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue24 = scratchValue25 | 3584
                                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff54a end
                            end
                            predicateResult14 = true
                        end
                        goto FLOW_past_lab_00dff54a
                        ::LAB_00dff54a::
                        predicateResult14 = false
                        ::FLOW_past_lab_00dff54a::
                        scratchValue25 = scratchValue24
                        if scratchValue24 & 2048 ~= 0 then
                            scratchValue25 = scratchValue24 & 0xfffff7ff
                        end
                        if scratchValue25 & 1024 ~= 0 then
                            scratchValue25 = scratchValue25 & 0xfffffbff
                        end
                        if scratchValue25 & 512 ~= 0 then
                            scratchValue25 = scratchValue25 & 0xfffffdff
                        end
                        if predicateResult14 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            if me:GetDataString() ~= "TRADERA" then
                                if me:GetDataString() ~= "TRADERB" then
                                    scratchValue = scratchValue28
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    getDataString = tostring(scratchValue28)
                                    scratchValue21 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_C_SCREAM_0" .. getDataString)
                                    scratchValue10 = scratchValue == 4
                                else
                                    scratchValue = scratchValue28
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    getDataString = tostring(scratchValue28)
                                    scratchValue22 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_B_SCREAM_0" .. getDataString)
                                    scratchValue10 = scratchValue == 3
                                end
                            else
                                scratchValue = scratchValue28
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                getDataString = tostring(scratchValue28)
                                scratchValue23 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_A_SCREAM_0" .. getDataString)
                                scratchValue10 = scratchValue == 3
                            end
                            if scratchValue10 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                scratchValue = 0
                            end
                            -- TODO(native): xStack_148 = (CCharString)((int)CVar13 + 1);
                            quest:SetTimer(4, scratchValue)
                        else
                            health = quest:GetHealth(nil --[[missing]])
                            scratchValue24 = scratchValue25
                            if health <= 5.0 then
                                goto LAB_00dff887
                            else
                                scratchValue24 = scratchValue25 | 4096
                                if not me:MsgIsHitByHero() then
                                    scratchValue24 = scratchValue25 | 0x3000
                                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                        scratchValue24 = scratchValue25 | 0x7000
                                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dff880 end
                                    end
                                    goto LAB_00dff887
                                end
                                ::LAB_00dff880::
                                predicateResult18 = true
                            end
                            goto FLOW_past_lab_00dff887
                            ::LAB_00dff887::
                            predicateResult18 = false
                            ::FLOW_past_lab_00dff887::
                            scratchValue25 = scratchValue24
                            if scratchValue24 & 0x4000 ~= 0 then
                                scratchValue25 = scratchValue24 & 0xffffbfff
                            end
                            if scratchValue25 & 0x2000 ~= 0 then
                                scratchValue25 = scratchValue25 & 0xffffdfff
                            end
                            if scratchValue25 & 4096 ~= 0 then
                                scratchValue25 = scratchValue25 & 0xffffefff
                            end
                            if predicateResult18 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                conversationId = quest:AddNewConversation(nil --[[missing]], me, false)
                                quest:AddPersonToConversation(conversationId, hero)
                                getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT"
                                quest:AddLineToConversation(conversationId, getDataString, hero, nil --[[missing]], false)
                            end
                        end
                    end
                    scratchValue24 = scratchValue25 | 0x8000
                    if me:MsgIsHitByHero() then goto LAB_00dffa46 end
                    scratchValue24 = scratchValue25 | 0x18000
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue24 = scratchValue25 | 0x38000
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00dffa46 end
                    end
                    predicateResult19 = false
                    goto FLOW_past_lab_00dffa46
                    ::LAB_00dffa46::
                    predicateResult19 = true
                    ::FLOW_past_lab_00dffa46::
                    if scratchValue24 & 0x20000 ~= 0 then
                        scratchValue24 = scratchValue24 & 0xfffdffff
                    end
                    if scratchValue24 & 0x10000 ~= 0 then
                        scratchValue24 = scratchValue24 & 0xfffeffff
                    end
                    if scratchValue24 & 0x8000 ~= 0 then
                        scratchValue24 = scratchValue24 & 0xffff7fff
                    end
                    local sequence4 = predicateResult19 and quest:IsActiveThreadTerminating()
                    if sequence4 then goto LAB_00e005b1 end
                    if not me:MsgExpressionPerformedTo() then isRegionLoaded = not quest:IsRegionLoaded("BanditCampEntrance"); scratchValue = 10; goto continue_3 end
                    if scratchValue31 ~= nil and scratchValue31 == "EXPRESSION_WAIT" then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                    end
                    isRegionLoaded = not quest:IsRegionLoaded("BanditCampEntrance")
                    scratchValue = 10
                    ::continue_3::
                end
                if quest:IsActiveThreadTerminating() then break end
                local teleporterMarker = quest:GetThingWithScriptName("TeleporterMarker")
                -- TODO(native): CCharString::CCharString({R = 255, G = 0, B = 0, A = 255},"BanditCampEntrance",-1);
                isRegionLoaded = quest:IsRegionLoaded("")
                while isRegionLoaded do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        scratchValue30 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                        getDataString = scratchValue30 .. tostring(scratchValue)
                        scratchValue31 = getDataString
                        if not quest:TextEntryExists() then
                            getDataString = scratchValue30 .. tostring(10)
                            scratchValue31 = getDataString
                        end
                        -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                        resources:PrepareResource(scratchValue28)
                        while not me:AcquireControl(4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        movie3 = resources:ScriptThing(scratchValue28)
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
                        resources:PrepareResource(scratchValue28)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                    end
                    local timeRemaining2 = quest:GetTimer(unaff_EBX) == 0 and IsPlayerThreateningEntity(me) ~= 0
                    local scratchValue5 = timeRemaining2 and quest:IsPlayerHoldingLockTargetButton()
                    if scratchValue5 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        conversationId = quest:AddNewConversation(teleporterMarker, me, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. getDataString
                        quest:AddLineToConversation(conversationId, getDataString, hero, nil --[[missing]], false)
                        quest:SetTimer(unaff_EBX, 20)
                    end
                    scratchValue25 = scratchValue24 | 0x40000
                    if me:MsgIsHitByHero() then goto LAB_00e0006b end
                    scratchValue25 = scratchValue24 | 0xc0000
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue25 = scratchValue24 | 0x1c0000
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e0006b end
                    end
                    predicateResult23 = false
                    goto FLOW_past_lab_00e0006b
                    ::LAB_00e0006b::
                    predicateResult23 = true
                    ::FLOW_past_lab_00e0006b::
                    if scratchValue25 & 0x100000 ~= 0 then
                        scratchValue25 = scratchValue25 & 0xffefffff
                    end
                    if scratchValue25 & 0x80000 ~= 0 then
                        scratchValue25 = scratchValue25 & 0xfff7ffff
                    end
                    if scratchValue25 & 0x40000 ~= 0 then
                        scratchValue25 = scratchValue25 & 0xfffbffff
                    end
                    if predicateResult23 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        conversationId = quest:AddNewConversation(nil --[[missing]], me, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. getDataString
                        quest:AddLineToConversation(conversationId, getDataString, hero, nil --[[missing]], false)
                    end
                    if me:MsgExpressionPerformedTo() then
                        if timerId4 ~= nil and timerId4 == "EXPRESSION_WAIT" then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                        end
                    end
                    if not quest:IsDistanceBetweenThingsUnder(me, scratchValue27, 20.0) then isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance"); scratchValue = 10; scratchValue24 = scratchValue25; goto continue_5 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                    quest:SetStateInt("TradersReachedTeleporter", quest:GetStateInt("TradersReachedTeleporter") + 1)
                    quest:EntityStopFollowing(nil --[[missing]])
                    quest:SetEntityAsRegionFollowing(hero, nil --[[missing]], me)
                    quest:EntitySetAsScared(nil --[[missing]], me)
                    if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                        quest:SetStateBool("OutroStart", true)
                        quest:SetStateBool("MissionSucceeded", true)
                        quest:NewScriptFrame(me)
                        -- TODO(native): Main_InitializeFourierAnalysis_4(*(undefined4 *)(this + 0x14));
                        quest:SetStateBool("OutroDone", true)
                        goto LAB_00e00595
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                    scratchValue11 = me:AcquireControl(4)
                    goto LAB_00e0035a
                    isRegionLoaded = quest:IsRegionLoaded("BanditCampEntrance")
                    scratchValue = 10
                    scratchValue24 = scratchValue25
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
    if not scratchValue11 then
        if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
        scratchValue11 = me:AcquireControl(4)
        goto LAB_00e0035a
    end
    if not quest:IsActiveThreadTerminating() then
        conversationId = quest:AddNewConversation(nil --[[missing]], me, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:AddLineToConversation(conversationId, ("TEXT_QST_B11_" .. me:GetDataString()) .. movie3, hero, nil --[[missing]], false)
        while quest:IsDistanceBetweenThingsOver(me, scratchValue26, 2.0) do
            if quest:GetStateBool("OutroStart") then break end
            if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                me:MoveToThing(nil --[[missing]], 1.0, ENTITY_MOVE_RUN)
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
    quest:RemoveThing(nil --[[missing]], me, false)
    ::FLOW_past_lab_00e00595::
    ::LAB_00e005ac::
    ::LAB_00e005b1::
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId2)
    ::LAB_00e005d5::
    quest:DeregisterTimer(1)
    quest:DeregisterTimer(10)
    ::LAB_00e005f9::
    resources:ReleaseResource(scratchValue28)
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

