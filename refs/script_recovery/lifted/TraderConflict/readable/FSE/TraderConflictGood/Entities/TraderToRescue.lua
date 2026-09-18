-- Readable native conversion: TraderToRescue. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local barIndex

-- TraderToRescue.Main (retail 0x00dfe0f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue7, scratchValue8
    local predicateResult2, predicateResult3, predicateResult4, predicateResult5, predicateResult
    local predicateResult14, scratchValue, predicateResult18, predicateResult19, predicateResult23
    local scratchValue12, c_stk_161_1, c_stk_161_2, c_stk_161_3, dist, health, health4, sequence
    local sequence2, getDataString, scratchValue17, banditHostageKeeper, scratchValue18
    local scratchValue19, scratchValue20, teleporterMarker, scratchValue21, scratchValue22
    local scratchValue23, scratchValue24, timerId, timerId2, scratchValue25, scratchValue26
    local timerId3, scratchValue27, movie, scratchValue28, scratchValue29, timerId4
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("IntroDone") do
        if not quest:NewScriptFrame(me) then return end
    end
    scratchValue24 = resources:NewResource()
    -- TODO(native): xStack_148[0] = 0;
    scratchValue12 = me:AcquireControl(4)
    while not scratchValue12 do
        if not quest:NewScriptFrame(me) then goto LAB_00e005f9 end
        scratchValue12 = me:AcquireControl(4)
    end
    if not quest:IsActiveThreadTerminating() then
        timerId4 = quest:RegisterTimer()
        timerId3 = quest:RegisterTimer()
        banditHostageKeeper = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        c_stk_161_1 = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(unaff_EBX)
                resources:ReleaseResource(scratchValue24)
                return
            end
            if me:IsTalkedToByHero() then
                predicateResult3 = true
                goto FLOW_after_lab_00dfe32c
            end
            scratchValue21 = unaff_EBP | 3
            getDataString = me:GetDataString()
            if getDataString ~= nil then
                if getDataString == "TRADERB" then return end  -- TODO(native): goto LAB_00dfe2e9
            end
            scratchValue27 = me:MsgExpressionPerformedTo()
            if scratchValue27 ~= nil then
                if scratchValue27 == nil then
                    predicateResult3 = false
                    goto FLOW_after_lab_00dfe32c
                elseif scratchValue27 ~= "EXPRESSION_FOLLOW" then
                    predicateResult3 = false
                    goto FLOW_after_lab_00dfe32c
                end
                predicateResult3 = true
            else
                predicateResult3 = false
            end
            ::FLOW_after_lab_00dfe32c::
            if scratchValue21 & 2 ~= 0 then
                scratchValue21 = scratchValue21 & 0xfffffffd
            end
            if scratchValue21 & 1 ~= 0 then
                scratchValue21 = scratchValue21 & 0xfffffffe
            end
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue29 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                scratchValue24 = resources:ScriptThing(xStack_14c_2)
                if quest:GetHealth(scratchValue24) <= 0.0 then return end  -- TODO(native): goto LAB_00dfebc5
                scratchValue17 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_INTRO"
                me:Speak(hero, scratchValue17, 0, false, true, false)
                scratchValue12 = me:IsPerformingScriptTask()
                goto LAB_00dfeb57
            end
            sequence = quest:GetTimer(timerId3) == 0
            if not sequence then
                sequence = c_stk_161_1 == 0
                if sequence then
                    -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
                    scratchValue12 = nil --[[unresolved native value]]
                    sequence = scratchValue12 == 0
                end
            end
            if sequence then
                dist = 15.0
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    sequence2 = c_stk_161_1 == 0
                    if sequence2 then
                        -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
                        scratchValue12 = nil --[[unresolved native value]]
                        sequence2 = scratchValue12 == 0
                    end
                    if sequence2 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        scratchValue23 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue23, hero)
                        quest:AddLineToConversation(scratchValue23, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_KEEPERISDEAD", me, hero, false)
                        c_stk_161_1 = 1
                    elseif math.random(0, 32767) % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        scratchValue23 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue23, hero)
                        quest:AddLineToConversation(scratchValue23, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_OVERHERE", me, hero, false)
                    end
                    quest:SetTimer(timerId4, 10)
                    quest:SetTimer(unaff_EBX, quest:GetTimer(unaff_EBX) + 5)
                end
            end
            if quest:GetTimer(unaff_EBX) == 0 then
                scratchValue21 = scratchValue21 | 4
                -- TODO(native): bVar2 = (**(*(iVar11 + 0x0) + 0x138))((me))
    --[[unresolved native value]]
                if not nil then
                    predicateResult4 = false
                    goto FLOW_after_lab_00dfe63d
                end
                -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                quest:IsPlayerHoldingLockTargetButton()
                predicateResult4 = true
                if scratchValue12 == 0 then
                    predicateResult4 = false
                    goto FLOW_after_lab_00dfe63d
                end
            else
                predicateResult4 = false
            end
            ::FLOW_after_lab_00dfe63d::
            if scratchValue21 & 4 ~= 0 then
                scratchValue21 = scratchValue21 & 0xfffffffb
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                if IsPlayerThreateningEntity(me) == 0 or math.random(0, 32767) % 3 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue23 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue23, hero)
                    quest:AddLineToConversation(scratchValue23, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue23 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue23, hero)
                    quest:AddLineToConversation(scratchValue23, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATENWEAPON", me, hero, false)
                end
                quest:SetTimer(unaff_EBX, 20)
                quest:SetTimer(quest:GetTimer(x_stk_170) + 5, dist)
            end
            if me:MsgIsHitByHero() then
                predicateResult5 = true
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    -- TODO(native): unaff_EBP = uVar12 | 0x38;
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        predicateResult5 = true
                        goto FLOW_after_lab_00dfe8b5
                    end
                end
                predicateResult5 = false
            end
            ::FLOW_after_lab_00dfe8b5::
            if unaff_EBP & 32 ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xffffffdf;
            end
            if unaff_EBP & 16 ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xffffffef;
            end
            if unaff_EBP & 8 ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xfffffff7;
            end
            if predicateResult5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue23 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue23, hero)
                quest:AddLineToConversation(scratchValue23, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, hero, false)
                quest:SetTimer(unaff_EBX, 10)
            end
            quest:NewScriptFrame(me)
            predicateResult2 = quest:IsActiveThreadTerminating()
        until false
    end
    goto LAB_00e005f9
    ::LAB_00dfeb57::
    if scratchValue12 == 0 then goto LAB_00dfeb80 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue29)
        goto LAB_00e005d5
    end
    scratchValue12 = me:IsPerformingScriptTask()
    goto LAB_00dfeb57
    ::LAB_00dfeb80::
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue29)
    else
        -- LAB_00dfebc5: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue29)
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
                    -- LAB_00dfee05: (native jump target)
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    barIndex = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER_HAT_02", 1.0)
                    goto FLOW_after_lab_00dfee1b
                end
                getDataString = me:GetDataString()
                if getDataString == "TRADERC" then return end  -- TODO(native): goto LAB_00dfee05
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                barIndex = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TRADER", 1.0)
            end
            ::FLOW_after_lab_00dfee1b::
            while not quest:IsEntityFollowingHero(me) do
                if not quest:NewScriptFrame(me) then goto LAB_00e005d5 end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + 1)
                scratchValue2 = 10
                timerId = quest:RegisterTimer()
                quest:SetTimer(timerId, 20)
                timerId2 = quest:RegisterTimer()
                while not quest:IsActiveThreadTerminating() do
                    c_stk_161_2 = not quest:IsRegionLoaded("BanditCampEntrance")
                    while c_stk_161_2 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                        scratchValue12 = me:IsTalkedToByHero()
                        if scratchValue12 then
                            scratchValue25 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                            getDataString = scratchValue25 .. tostring(scratchValue2)
                            scratchValue26 = getDataString
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not scratchValue12 then
                                getDataString = scratchValue25 .. tostring(10)
                                scratchValue26 = getDataString
                            end
                            -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                            scratchValue12 = me:AcquireControl(4)
                            while not scratchValue12 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                                scratchValue12 = me:AcquireControl(4)
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            movie = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if quest:GetHealth(resources:ScriptThing(timerId2)) > 0.0 then
                                me:Speak(hero, scratchValue17, 0, false, true, false)
                                scratchValue12 = me:IsPerformingScriptTask()
                                while scratchValue12 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie)
                                        goto LAB_00e005b1
                                    end
                                    scratchValue12 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00e005b1
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                        end
                        scratchValue5 = quest:GetTimer(unaff_EBX) == 0 and IsPlayerThreateningEntity(me) ~= 0
                        scratchValue4 = scratchValue5
                        if scratchValue5 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            scratchValue4 = scratchValue12
                        end
                        if scratchValue4 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            scratchValue23 = quest:AddNewConversation(banditHostageKeeper, me, false)
                            quest:AddPersonToConversation(scratchValue23, hero)
                            getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN"
                            quest:AddLineToConversation(scratchValue23, getDataString, hero, nil --[[missing]], false)
                            quest:SetTimer(unaff_EBX, 20)
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        if not me:MsgIsHitBy("") then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            if me:MsgIsHitByAnySpecialAbilityFrom("") then
                                scratchValue22 = scratchValue22 | 256
                                predicateResult = true
                                if not me:MsgIsHitByHeroSpecialAbility(14) then
                                    goto FLOW_after_lab_00dff3b5
                                end
                            end
                            predicateResult = false
                        else
                            scratchValue22 = scratchValue22 | 256
                            predicateResult = true
                            if me:MsgIsHitByHeroSpecialAbility(14) then
                                predicateResult = false
                                goto FLOW_after_lab_00dff3b5
                            end
                        end
                        ::FLOW_after_lab_00dff3b5::
                        if scratchValue22 & 256 ~= 0 then
                            scratchValue22 = scratchValue22 & 0xfffffeff
                        end
                        if SUB41(scratchValue22,0) < 0 then
                            scratchValue22 = scratchValue22 & 0xffffff7f
                        end
                        if scratchValue22 & 64 ~= 0 then
                            scratchValue22 = scratchValue22 & 0xffffffbf
                        end
                        if predicateResult then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            health = quest:GetHealth(nil --[[missing]])
                            if health <= 5.0 or quest:GetTimer(scratchValue24) ~= 0 then
                                predicateResult14 = false
                            else
                                if me:MsgIsHitByHero() then
                                    predicateResult14 = false
                                    goto FLOW_after_lab_00dff54a
                                end
                                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                    scratchValue21 = scratchValue22 | 3584
                                    if not me:MsgIsHitByHeroSpecialAbility(14) then
                                        predicateResult14 = false
                                        goto FLOW_after_lab_00dff54a
                                    end
                                end
                                predicateResult14 = true
                            end
                            ::FLOW_after_lab_00dff54a::
                            if scratchValue21 & 2048 ~= 0 then
                                scratchValue22 = scratchValue21 & 0xfffff7ff
                            end
                            if scratchValue22 & 1024 ~= 0 then
                                scratchValue22 = scratchValue22 & 0xfffffbff
                            end
                            if scratchValue22 & 512 ~= 0 then
                                scratchValue22 = scratchValue22 & 0xfffffdff
                            end
                            if predicateResult14 then
                                if me:GetDataString() ~= "TRADERA" then
                                    if me:GetDataString() ~= "TRADERB" then
                                        scratchValue3 = scratchValue24
                                        getDataString = tostring(scratchValue24)
                                        scratchValue18 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_C_SCREAM_0" .. getDataString)
                                        scratchValue = scratchValue3 == 4
                                    else
                                        scratchValue3 = scratchValue24
                                        getDataString = tostring(scratchValue24)
                                        scratchValue19 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_B_SCREAM_0" .. getDataString)
                                        scratchValue = scratchValue3 == 3
                                    end
                                else
                                    scratchValue3 = scratchValue24
                                    getDataString = tostring(scratchValue24)
                                    scratchValue20 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_A_SCREAM_0" .. getDataString)
                                    scratchValue = scratchValue3 == 3
                                end
                                if scratchValue then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    scratchValue3 = 0
                                end
                                -- TODO(native): xStack_148 = (CCharString)((int)CVar13 + 1);
                                quest:SetTimer(4, scratchValue3)
                            else
                                health4 = quest:GetHealth(nil --[[missing]])
                                if health4 <= 5.0 then
                                    predicateResult18 = false
                                else
                                    if not me:MsgIsHitByHero() then
                                        if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                            scratchValue21 = scratchValue22 | 0x7000
                                            if not me:MsgIsHitByHeroSpecialAbility(14) then goto LAB_00dff880 end
                                        end
                                        predicateResult18 = false
                                        goto FLOW_after_lab_00dff887
                                    end
                                    ::LAB_00dff880::
                                    predicateResult18 = true
                                end
                                ::FLOW_after_lab_00dff887::
                                if scratchValue21 & 0x4000 ~= 0 then
                                    scratchValue22 = scratchValue21 & 0xffffbfff
                                end
                                if scratchValue22 & 0x2000 ~= 0 then
                                    scratchValue22 = scratchValue22 & 0xffffdfff
                                end
                                if scratchValue22 & 4096 ~= 0 then
                                    scratchValue22 = scratchValue22 & 0xffffefff
                                end
                                if predicateResult18 then
                                    scratchValue23 = quest:AddNewConversation(nil --[[missing]], me, false)
                                    quest:AddPersonToConversation(scratchValue23, hero)
                                    getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT"
                                    quest:AddLineToConversation(scratchValue23, getDataString, hero, nil --[[missing]], false)
                                end
                            end
                        end
                        if not me:MsgIsHitByHero() then
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue21 = scratchValue22 | 0x38000
                                if not me:MsgIsHitByHeroSpecialAbility(14) then
                                    predicateResult19 = true
                                    goto FLOW_after_lab_00dffa46
                                end
                            end
                            predicateResult19 = false
                        else
                            predicateResult19 = true
                        end
                        ::FLOW_after_lab_00dffa46::
                        if scratchValue21 & 0x20000 ~= 0 then
                            scratchValue21 = scratchValue21 & 0xfffdffff
                        end
                        if scratchValue21 & 0x10000 ~= 0 then
                            scratchValue21 = scratchValue21 & 0xfffeffff
                        end
                        if scratchValue21 & 0x8000 ~= 0 then
                            scratchValue21 = scratchValue21 & 0xffff7fff
                        end
                        if predicateResult19 and quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        if me:MsgExpressionPerformedTo() then
                            if scratchValue26 == nil then
                            elseif scratchValue26 == "EXPRESSION_WAIT" then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                            end
                        end
                        c_stk_161_2 = not quest:IsRegionLoaded("BanditCampEntrance")
                        scratchValue2 = 10
                    end
                    if quest:IsActiveThreadTerminating() then break end
                    teleporterMarker = quest:GetThingWithScriptName("TeleporterMarker")
                    -- TODO(native): CCharString::CCharString({R = 255, G = 0, B = 0, A = 255},"BanditCampEntrance",-1);
                    c_stk_161_3 = quest:IsRegionLoaded("")
                    while c_stk_161_3 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                        scratchValue12 = me:IsTalkedToByHero()
                        if scratchValue12 then
                            scratchValue25 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                            getDataString = scratchValue25 .. tostring(scratchValue2)
                            scratchValue26 = getDataString
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not scratchValue12 then
                                getDataString = scratchValue25 .. tostring(10)
                                scratchValue26 = getDataString
                            end
                            -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                            scratchValue12 = me:AcquireControl(4)
                            while not scratchValue12 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                                scratchValue12 = me:AcquireControl(4)
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue28 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue29 = resources:ScriptThing(scratchValue24)
                            if quest:GetHealth(scratchValue29) > 0.0 then
                                me:Speak(hero, scratchValue17, 0, false, true, false)
                                scratchValue12 = me:IsPerformingScriptTask()
                                while scratchValue12 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue28)
                                        goto LAB_00e005ac
                                    end
                                    scratchValue12 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue28)
                                    goto LAB_00e005ac
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue28)
                        end
                        scratchValue8 = quest:GetTimer(unaff_EBX) == 0 and IsPlayerThreateningEntity(me) ~= 0
                        scratchValue7 = scratchValue8
                        if scratchValue8 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            scratchValue7 = scratchValue12
                        end
                        if scratchValue7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue23 = quest:AddNewConversation(teleporterMarker, me, false)
                            quest:AddPersonToConversation(scratchValue23, hero)
                            getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. getDataString
                            quest:AddLineToConversation(scratchValue23, getDataString, hero, nil --[[missing]], false)
                            quest:SetTimer(unaff_EBX, 20)
                        end
                        if not me:MsgIsHitByHero() then
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue22 = scratchValue21 | 0x1c0000
                                if not me:MsgIsHitByHeroSpecialAbility(14) then
                                    predicateResult23 = true
                                    goto FLOW_after_lab_00e0006b
                                end
                            end
                            predicateResult23 = false
                        else
                            predicateResult23 = true
                        end
                        ::FLOW_after_lab_00e0006b::
                        if scratchValue22 & 0x100000 ~= 0 then
                            scratchValue22 = scratchValue22 & 0xffefffff
                        end
                        if scratchValue22 & 0x80000 ~= 0 then
                            scratchValue22 = scratchValue22 & 0xfff7ffff
                        end
                        if scratchValue22 & 0x40000 ~= 0 then
                            scratchValue22 = scratchValue22 & 0xfffbffff
                        end
                        if predicateResult23 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue23 = quest:AddNewConversation(nil --[[missing]], me, false)
                            quest:AddPersonToConversation(scratchValue23, hero)
                            getDataString = ("TEXT_QST_B11_" .. me:GetDataString()) .. getDataString
                            quest:AddLineToConversation(scratchValue23, getDataString, hero, nil --[[missing]], false)
                        end
                        if me:MsgExpressionPerformedTo() then
                            if timerId3 == nil then
                            elseif timerId3 == "EXPRESSION_WAIT" then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                                quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                            end
                        end
                        if quest:IsDistanceBetweenThingsUnder(me, xStack_114, 20.0) then
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
                                quest:RemoveThing(nil --[[missing]], me, false)
                                goto FLOW_after_lab_00e00595
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh({R = 255, G = 0, B = 0, A = 255});
                            scratchValue12 = me:AcquireControl(4)
                            goto LAB_00e0035a
                        end
                        c_stk_161_3 = quest:IsRegionLoaded("BanditCampEntrance")
                        scratchValue2 = 10
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                    quest:NewScriptFrame(me)
                end
                goto LAB_00e005b1
            end
        end
    end
    goto LAB_00e005d5
    ::LAB_00e0035a::
    if not scratchValue12 then
        if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
        scratchValue12 = me:AcquireControl(4)
        goto LAB_00e0035a
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue23 = quest:AddNewConversation(nil --[[missing]], me, false)
        quest:AddPersonToConversation(scratchValue23, hero)
        quest:AddLineToConversation(scratchValue23, ("TEXT_QST_B11_" .. me:GetDataString()) .. scratchValue29, hero, nil --[[missing]], false)
        while quest:IsDistanceBetweenThingsOver(me, xStack_110, 2.0) do
            if quest:GetStateBool("OutroStart") then break end
            if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
            if not me:IsPerformingScriptTask() then
                me:MoveToThing(nil --[[missing]], 1.0, 1)
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
                while not quest:GetStateBool("OutroDone") do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveThing(nil --[[missing]], me, false)
                end
            end
        end
    end
    ::FLOW_after_lab_00e00595::
    ::LAB_00e005ac::
    ::LAB_00e005b1::
    quest:DeregisterTimer(timerId2)
    quest:DeregisterTimer(timerId)
    ::LAB_00e005d5::
    quest:DeregisterTimer(1)
    quest:DeregisterTimer(10)
    ::LAB_00e005f9::
    resources:ReleaseResource(scratchValue24)
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
function OnPersist(quest, context)
end

-- TraderToRescue.OnPredicateFail (retail 0x00dfb100)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED", true)
    end
end

