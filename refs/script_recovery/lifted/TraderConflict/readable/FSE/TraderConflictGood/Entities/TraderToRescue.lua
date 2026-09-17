-- Readable native conversion: TraderToRescue. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TraderToRescue.Main (retail 0x00dfe0f0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue7
    local scratchValue8, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult13, predicateResult14, scratchValue11, predicateResult18, predicateResult19
    local predicateResult23, scratchValue12, c_stk_161_1, c_stk_161_2, c_stk_161_3, dist, health3
    local health4, scratchValue13, timeRemaining, scratchValue14, scratchValue17, sequence1
    local sequence2, scratchValue19, hero, scratchValue22, getNearestWithScriptName, scratchValue23
    local scratchValue24, scratchValue25, teleporterMarker, scratchValue26, scratchValue27
    local scratchValue28, scratchValue29, timerId, timerId2, scratchValue30, scratchValue31
    local timerId3, scratchValue32, scratchValue33, scratchValue34, timerId4
    if not quest:NewScriptFrame(me) then return end
    scratchValue12 = quest:GetStateBool("IntroDone")
    while not scratchValue12 do
        if not quest:NewScriptFrame(me) then return end
        scratchValue12 = quest:GetStateBool("IntroDone")
    end
    scratchValue29 = resources:NewResource()
    -- TODO(native): xStack_148[0] = 0;
    scratchValue12 = me:AcquireControl(4)
    while not scratchValue12 do
        if not quest:NewScriptFrame(me) then goto LAB_00e005f9 end
        scratchValue12 = me:AcquireControl(4)
    end
    if not quest:IsActiveThreadTerminating() then
        timerId4 = quest:RegisterTimer()
        timerId3 = quest:RegisterTimer()
        getNearestWithScriptName = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        c_stk_161_1 = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(unaff_EBX)
                resources:ReleaseResource(scratchValue29)
                return
            end
            if me:IsTalkedToByHero() then
                predicateResult3 = true
                goto FLOW_after_lab_00dfe32c
            end
            scratchValue26 = unaff_EBP | 3
            scratchValue19 = me:GetDataString()
            if scratchValue19 ~= nil then
                scratchValue13 = scratchValue19 == "TRADERB" and 0 or 1
                if scratchValue13 == 0 then return end  -- TODO(native): goto LAB_00dfe2e9
            end
            if me:MsgExpressionPerformedTo() then
                if "" == nil then
                    predicateResult3 = false
                    goto FLOW_after_lab_00dfe32c
                else
                    -- TODO(native): iVar11 = CBasicString<char>::Compare(*(void **)xStack_168,"EXPRESSION_FOLLOW");
                    if scratchValue13 ~= 0 then
                        predicateResult3 = false
                        goto FLOW_after_lab_00dfe32c
                    end
                end
                predicateResult3 = true
            else
                predicateResult3 = false
            end
            ::FLOW_after_lab_00dfe32c::
            if scratchValue26 & 2 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffffffd
            end
            if scratchValue26 & 1 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffffffe
            end
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue34 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                scratchValue29 = resources:ScriptThing(xStack_14c_2)
                if quest:GetHealth(scratchValue29) <= 0.0 then return end  -- TODO(native): goto LAB_00dfebc5
                scratchValue22 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_INTRO"
                me:Speak(quest:GetHero(), scratchValue22, 0, false, true, false)
                scratchValue12 = me:IsPerformingScriptTask()
                goto LAB_00dfeb57
            end
            timeRemaining = quest:GetTimer(timerId3)
            sequence1 = timeRemaining == 0
            if not sequence1 then
                sequence1 = c_stk_161_1 == 0
                if sequence1 then
                    -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
                    scratchValue12 = nil --[[unresolved native value]]
                    sequence1 = scratchValue12 == 0
                end
            end
            if sequence1 then
                dist = 15.0
                hero = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    sequence2 = c_stk_161_1 == 0
                    if sequence2 then
                        -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
                        scratchValue12 = nil --[[unresolved native value]]
                        sequence2 = scratchValue12 == 0
                    end
                    if sequence2 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        scratchValue28 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                        quest:AddLineToConversation(scratchValue28, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_KEEPERISDEAD", me, quest:GetHero(), false)
                        c_stk_161_1 = 1
                    elseif timeRemaining % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        scratchValue28 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                        quest:AddLineToConversation(scratchValue28, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_OVERHERE", me, quest:GetHero(), false)
                    end
                    quest:SetTimer(timerId4, 10)
                    quest:SetTimer(unaff_EBX, quest:GetTimer(unaff_EBX) + 5)
                end
            end
            scratchValue13 = quest:GetTimer(unaff_EBX)
            if scratchValue13 == 0 then
                scratchValue26 = scratchValue26 | 4
                scratchValue13 = quest:GetHeroTargetedThing()
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
            if scratchValue26 & 4 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffffffb
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue14 = IsPlayerThreateningEntity(me)
                if scratchValue14 == 0 or scratchValue14 % 3 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue28 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                    quest:AddLineToConversation(scratchValue28, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN", me, quest:GetHero(), false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue28 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                    quest:AddLineToConversation(scratchValue28, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATENWEAPON", me, quest:GetHero(), false)
                end
                quest:SetTimer(unaff_EBX, 20)
                scratchValue13 = quest:GetTimer(x_stk_170)
                quest:SetTimer(scratchValue13 + 5, dist)
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
                scratchValue28 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                quest:AddLineToConversation(scratchValue28, ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT", me, quest:GetHero(), false)
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
        resources:DestroyMovie(scratchValue34)
        goto LAB_00e005d5
    end
    scratchValue12 = me:IsPerformingScriptTask()
    goto LAB_00dfeb57
    ::LAB_00dfeb80::
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue34)
    else
        -- LAB_00dfebc5: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue34)
        if not quest:IsActiveThreadTerminating() then
            quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
            quest:ActivateQuest("Q_TraderConflictGood_Extras")
            quest:SetIsPushableByHero(hero, __unknown_push)
            quest:EntityFollowThing(me, quest:GetHero(), nil --[[missing]], nil --[[missing]])
            quest:SetEntityAsRegionFollowing(quest:GetHero(), getNearestWithScriptName, __unknown_push)
            quest:EntitySetOpinionReactionsEnabled(me, false)
            quest:EntitySetDeedReactionsEnabled(me, false)
            quest:EntitySetCombatEnabled(me, false)
            quest:EntitySetInFaction(me, "FACTION_TRADERS")
            quest:DisplayQuestInfo(true)
            scratchValue19 = me:GetDataString()
            if scratchValue19 ~= "TRADERA" then
                scratchValue19 = me:GetDataString()
                if scratchValue19 == "TRADERB" then
                    -- LAB_00dfee05: (native jump target)
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue28 = quest:AddQuestInfoBarHealth(nil --[[missing]], __unknown_push, "HUD_QUEST_ICON_TRADER_HAT_02", {R = 255, G = 0, B = 0, A = 255})
                    state:SetInt("BarIndex", scratchValue28)
                    goto FLOW_after_lab_00dfee1b
                end
                scratchValue19 = me:GetDataString()
                if scratchValue19 == "TRADERC" then return end  -- TODO(native): goto LAB_00dfee05
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue28 = quest:AddQuestInfoBarHealth(nil --[[missing]], __unknown_push, "HUD_QUEST_ICON_TRADER", {R = 255, G = 0, B = 0, A = 255})
                state:SetInt("BarIndex", scratchValue28)
            end
            ::FLOW_after_lab_00dfee1b::
            scratchValue12 = quest:IsEntityFollowingHero(nil --[[missing]])
            while not scratchValue12 do
                if not quest:NewScriptFrame(me) then goto LAB_00e005d5 end
                scratchValue12 = quest:IsEntityFollowingHero(nil --[[missing]])
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
                            scratchValue30 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                            scratchValue19 = scratchValue30 .. tostring(scratchValue2)
                            scratchValue31 = scratchValue19
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not scratchValue12 then
                                scratchValue19 = scratchValue30 .. tostring(10)
                                scratchValue31 = scratchValue19
                            end
                            -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                            scratchValue12 = me:AcquireControl(4)
                            while not scratchValue12 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                                scratchValue12 = me:AcquireControl(4)
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            scratchValue32 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if quest:GetHealth(resources:ScriptThing(timerId2)) > 0.0 then
                                me:Speak(quest:GetHero(), scratchValue22, 0, false, true, false)
                                scratchValue12 = me:IsPerformingScriptTask()
                                while scratchValue12 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue32)
                                        goto LAB_00e005b1
                                    end
                                    scratchValue12 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue32)
                                    goto LAB_00e005b1
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue32)
                        end
                        scratchValue17 = quest:GetTimer(unaff_EBX)
                        scratchValue5 = scratchValue17 == 0
                        if scratchValue5 then
                            scratchValue17 = IsPlayerThreateningEntity(me)
                            scratchValue5 = scratchValue17 ~= 0
                        end
                        scratchValue4 = scratchValue5
                        if scratchValue4 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            scratchValue4 = scratchValue12
                        end
                        if scratchValue4 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            scratchValue28 = quest:AddNewConversation(nil --[[missing]], __unknown_push, false)
                            quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                            scratchValue19 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_THREATEN"
                            quest:AddLineToConversation(scratchValue28, scratchValue19, quest:GetHero(), nil --[[missing]], false)
                            quest:SetTimer(unaff_EBX, 20)
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        if not me:MsgIsHitBy("") then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            if me:MsgIsHitByAnySpecialAbilityFrom("") then
                                scratchValue27 = scratchValue27 | 256
                                predicateResult13 = true
                                if not me:MsgIsHitByHeroSpecialAbility(14) then
                                    goto FLOW_after_lab_00dff3b5
                                end
                            end
                            predicateResult13 = false
                        else
                            scratchValue27 = scratchValue27 | 256
                            predicateResult13 = true
                            if me:MsgIsHitByHeroSpecialAbility(14) then
                                predicateResult13 = false
                                goto FLOW_after_lab_00dff3b5
                            end
                        end
                        ::FLOW_after_lab_00dff3b5::
                        if scratchValue27 & 256 ~= 0 then
                            scratchValue27 = scratchValue27 & 0xfffffeff
                        end
                        if SUB41(scratchValue27,0) < 0 then
                            scratchValue27 = scratchValue27 & 0xffffff7f
                        end
                        if scratchValue27 & 64 ~= 0 then
                            scratchValue27 = scratchValue27 & 0xffffffbf
                        end
                        if predicateResult13 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            health3 = quest:GetHealth(nil --[[missing]])
                            scratchValue6 = health3 <= 5.0
                            if not scratchValue6 then
                                scratchValue17 = quest:GetTimer(scratchValue29)
                                scratchValue6 = scratchValue17 ~= 0
                            end
                            if scratchValue6 then
                                predicateResult14 = false
                            else
                                if me:MsgIsHitByHero() then
                                    predicateResult14 = false
                                    goto FLOW_after_lab_00dff54a
                                end
                                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                    scratchValue26 = scratchValue27 | 3584
                                    if not me:MsgIsHitByHeroSpecialAbility(14) then
                                        predicateResult14 = false
                                        goto FLOW_after_lab_00dff54a
                                    end
                                end
                                predicateResult14 = true
                            end
                            ::FLOW_after_lab_00dff54a::
                            if scratchValue26 & 2048 ~= 0 then
                                scratchValue27 = scratchValue26 & 0xfffff7ff
                            end
                            if scratchValue27 & 1024 ~= 0 then
                                scratchValue27 = scratchValue27 & 0xfffffbff
                            end
                            if scratchValue27 & 512 ~= 0 then
                                scratchValue27 = scratchValue27 & 0xfffffdff
                            end
                            if predicateResult14 then
                                scratchValue17 = me:GetDataString() == "TRADERA" and 0 or 1
                                if scratchValue17 ~= 0 then
                                    scratchValue17 = me:GetDataString() == "TRADERB" and 0 or 1
                                    if scratchValue17 ~= 0 then
                                        scratchValue3 = scratchValue29
                                        scratchValue19 = tostring(scratchValue29)
                                        scratchValue23 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_C_SCREAM_0" .. scratchValue19)
                                        scratchValue11 = scratchValue3 == 4
                                    else
                                        scratchValue3 = scratchValue29
                                        scratchValue19 = tostring(scratchValue29)
                                        scratchValue24 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_B_SCREAM_0" .. scratchValue19)
                                        scratchValue11 = scratchValue3 == 3
                                    end
                                else
                                    scratchValue3 = scratchValue29
                                    scratchValue19 = tostring(scratchValue29)
                                    scratchValue25 = quest:PlaySoundOnThing(nil --[[missing]], "SND_MM_TRADER_A_SCREAM_0" .. scratchValue19)
                                    scratchValue11 = scratchValue3 == 3
                                end
                                if scratchValue11 then
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
                                            scratchValue26 = scratchValue27 | 0x7000
                                            if not me:MsgIsHitByHeroSpecialAbility(14) then goto LAB_00dff880 end
                                        end
                                        predicateResult18 = false
                                        goto FLOW_after_lab_00dff887
                                    end
                                    ::LAB_00dff880::
                                    predicateResult18 = true
                                end
                                ::FLOW_after_lab_00dff887::
                                if scratchValue26 & 0x4000 ~= 0 then
                                    scratchValue27 = scratchValue26 & 0xffffbfff
                                end
                                if scratchValue27 & 0x2000 ~= 0 then
                                    scratchValue27 = scratchValue27 & 0xffffdfff
                                end
                                if scratchValue27 & 4096 ~= 0 then
                                    scratchValue27 = scratchValue27 & 0xffffefff
                                end
                                if predicateResult18 then
                                    scratchValue28 = quest:AddNewConversation(nil --[[missing]], __unknown_push, false)
                                    quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                                    scratchValue19 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONHIT"
                                    quest:AddLineToConversation(scratchValue28, scratchValue19, quest:GetHero(), nil --[[missing]], false)
                                end
                            end
                        end
                        if not me:MsgIsHitByHero() then
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue26 = scratchValue27 | 0x38000
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
                        if scratchValue26 & 0x20000 ~= 0 then
                            scratchValue26 = scratchValue26 & 0xfffdffff
                        end
                        if scratchValue26 & 0x10000 ~= 0 then
                            scratchValue26 = scratchValue26 & 0xfffeffff
                        end
                        if scratchValue26 & 0x8000 ~= 0 then
                            scratchValue26 = scratchValue26 & 0xffff7fff
                        end
                        if predicateResult19 and quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        if me:MsgExpressionPerformedTo() then
                            if scratchValue31 ~= nil then
                                -- TODO(native): iVar21 = CBasicString<char>::Compare(*(void **)xStack_15c,"EXPRESSION_WAIT");
                                if scratchValue17 == 0 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                                end
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
                            scratchValue30 = ("TEXT_QST_B11_" .. me:GetDataString()) .. "_ONTALK_"
                            scratchValue19 = scratchValue30 .. tostring(scratchValue2)
                            scratchValue31 = scratchValue19
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not scratchValue12 then
                                scratchValue19 = scratchValue30 .. tostring(10)
                                scratchValue31 = scratchValue19
                            end
                            -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                            scratchValue12 = me:AcquireControl(4)
                            while not scratchValue12 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                                scratchValue12 = me:AcquireControl(4)
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue33 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue34 = resources:ScriptThing(scratchValue29)
                            if quest:GetHealth(scratchValue34) > 0.0 then
                                me:Speak(quest:GetHero(), scratchValue22, 0, false, true, false)
                                scratchValue12 = me:IsPerformingScriptTask()
                                while scratchValue12 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue33)
                                        goto LAB_00e005ac
                                    end
                                    scratchValue12 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue33)
                                    goto LAB_00e005ac
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue33)
                        end
                        scratchValue17 = quest:GetTimer(unaff_EBX)
                        scratchValue8 = scratchValue17 == 0
                        if scratchValue8 then
                            scratchValue17 = IsPlayerThreateningEntity(me)
                            scratchValue8 = scratchValue17 ~= 0
                        end
                        scratchValue7 = scratchValue8
                        if scratchValue7 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            scratchValue7 = scratchValue12
                        end
                        if scratchValue7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue28 = quest:AddNewConversation(teleporterMarker, __unknown_push, false)
                            quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                            scratchValue19 = ("TEXT_QST_B11_" .. me:GetDataString()) .. scratchValue19
                            quest:AddLineToConversation(scratchValue28, scratchValue19, quest:GetHero(), nil --[[missing]], false)
                            quest:SetTimer(unaff_EBX, 20)
                        end
                        if not me:MsgIsHitByHero() then
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue27 = scratchValue26 | 0x1c0000
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
                        if scratchValue27 & 0x100000 ~= 0 then
                            scratchValue27 = scratchValue27 & 0xffefffff
                        end
                        if scratchValue27 & 0x80000 ~= 0 then
                            scratchValue27 = scratchValue27 & 0xfff7ffff
                        end
                        if scratchValue27 & 0x40000 ~= 0 then
                            scratchValue27 = scratchValue27 & 0xfffbffff
                        end
                        if predicateResult23 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue28 = quest:AddNewConversation(nil --[[missing]], __unknown_push, false)
                            quest:AddPersonToConversation(scratchValue28, quest:GetHero())
                            scratchValue19 = ("TEXT_QST_B11_" .. me:GetDataString()) .. scratchValue19
                            quest:AddLineToConversation(scratchValue28, scratchValue19, quest:GetHero(), nil --[[missing]], false)
                        end
                        if me:MsgExpressionPerformedTo() then
                            if timerId3 ~= nil then
                                -- TODO(native): iVar21 = CBasicString<char>::Compare(*(void **)xStack_164,"EXPRESSION_WAIT");
                                if scratchValue17 == 0 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                                    quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") - 1)
                                end
                            end
                        end
                        if quest:IsDistanceBetweenThingsUnder(me, xStack_114, 20.0) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            quest:SetStateInt("TradersReachedTeleporter", quest:GetStateInt("TradersReachedTeleporter") + 1)
                            quest:EntityStopFollowing(nil --[[missing]])
                            quest:SetEntityAsRegionFollowing(quest:GetHero(), nil --[[missing]], __unknown_push)
                            quest:EntitySetAsScared(nil --[[missing]], __unknown_push)
                            if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                                quest:SetStateBool("OutroStart", true)
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:NewScriptFrame(me)
                                -- TODO(native): Main_InitializeFourierAnalysis_4(*(undefined4 *)(this + 0x14));
                                quest:SetStateBool("OutroDone", true)
                                quest:RemoveThing(nil --[[missing]], __unknown_push, false)
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
        scratchValue28 = quest:AddNewConversation(nil --[[missing]], __unknown_push, false)
        quest:AddPersonToConversation(scratchValue28, quest:GetHero())
        quest:AddLineToConversation(scratchValue28, ("TEXT_QST_B11_" .. me:GetDataString()) .. scratchValue34, quest:GetHero(), nil --[[missing]], false)
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
                    quest:RemoveQuestInfoElement(__unknown_push)
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                scratchValue12 = quest:GetStateBool("OutroDone")
                while not scratchValue12 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                    scratchValue12 = quest:GetStateBool("OutroDone")
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveThing(nil --[[missing]], __unknown_push, false)
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
    resources:ReleaseResource(scratchValue29)
end

-- TraderToRescue.Init (retail 0x00dfb1b0)
function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsScared(me, true)
    quest:EntitySetInFaction(me, "FACTION_NEUTRAL")
    quest:EntitySetAsAllowedToFollowHero(me, true)
    state:SetInt("BarIndex", 0)
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

