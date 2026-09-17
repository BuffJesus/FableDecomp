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
    local predicateResult2, predicateResult3, predicateResult4, predicateResult5, predicateResult13
    local predicateResult14, scratchValue11, predicateResult18, predicateResult19, predicateResult23
    local scratchValue12, c_stk_161_1, c_stk_161_3, health2, health3, health4, scratchValue13
    local timeRemaining, scratchValue14, scratchValue15, sequence12, sequence22, scratchValue16
    local scratchValue17, hero, scratchValue18, scratchValue19, getNearestWithScriptName, hero3
    local hero5, hero7, scratchValue21, scratchValue22, scratchValue23, hero9, teleporterMarker
    local hero11, hero13, hero17, hero19, hero21, hero23, scratchValue24, scratchValue25
    local scratchValue26, scratchValue27, timerId, timerId2, scratchValue28, scratchValue29
    local scratchValue30, scratchValue31, scratchValue32, timerId3
    local self_0x14 = state:GetInt("self_0x14")
    if not quest:NewScriptFrame(me) then return end
    scratchValue12 = quest:GetStateBool("IntroDone")
    while not scratchValue12 do
        if not quest:NewScriptFrame(me) then return end
        scratchValue12 = quest:GetStateBool("IntroDone")
    end
    scratchValue27 = resources:NewResource()
    -- TODO(native): xStack_148[0] = 0;
    scratchValue12 = me:AcquireControl(4)
    while not scratchValue12 do
        if not quest:NewScriptFrame(me) then goto LAB_00e005f9 end
        scratchValue12 = me:AcquireControl(4)
    end
    if not quest:IsActiveThreadTerminating() then
        timerId3 = quest:RegisterTimer()
        scratchValue32 = quest:RegisterTimer()
        getNearestWithScriptName = quest:GetNearestWithScriptName(nil --[[missing]], "TC_BanditHostageKeeper")
        c_stk_161_1 = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(scratchValue32)
                quest:DeregisterTimer(unaff_EBX)
                resources:ReleaseResource(scratchValue27)
                return
            end
            if me:IsTalkedToByHero() then
                predicateResult3 = true
                goto FLOW_after_lab_00dfe32c
            end
            scratchValue24 = unaff_EBP | 3
            scratchValue17 = me:GetDataString()
            -- TODO(native): if *pCVar4 == nil then
            scratchValue13 = scratchValue17 == "TRADERB" and 0 or 1
            if scratchValue13 == 0 then return end  -- TODO(native): goto LAB_00dfe2e9
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
            if scratchValue24 & 2 ~= 0 then
                scratchValue24 = scratchValue24 & 0xfffffffd
            end
            if scratchValue24 & 1 ~= 0 then
                scratchValue24 = scratchValue24 & 0xfffffffe
            end
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue31 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                scratchValue27 = resources:ScriptThing(xStack_14c_3)
                if quest:GetHealth(getNearestWithScriptName) <= 0.0 then return end  -- TODO(native): goto LAB_00dfebc5
                scratchValue18 = "_INTRO"
                -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_10c,"TEXT_QST_B11_",pCVar4)
                scratchValue17 = nil --[[unresolved native value]]
                scratchValue17 = scratchValue17 .. scratchValue18
                -- TODO(native): pvVar8 = CCharString::operator_char_const_((CCharString *)pCVar4);
                me:Speak(quest:GetHero(), scratchValue18, pvVar8, false, false, true)
                scratchValue12 = me:IsPerformingScriptTask()
                goto LAB_00dfeb57
            end
            timeRemaining = quest:GetTimer(scratchValue32)
            sequence12 = timeRemaining == 0
            if not sequence12 then
                sequence12 = c_stk_161_1 == 0
                if sequence12 then
                    -- TODO(native): cVar3 = (**(xStack_134 + 0x12c))()
                    scratchValue12 = nil --[[unresolved native value]]
                    sequence12 = scratchValue12 == 0
                end
            end
            if sequence12 then
                hero = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    quest:EntitySetFacingAngleTowardsThing(quest:GetHero(), hero)
                    sequence22 = c_stk_161_1 == 0
                    if sequence22 then
                        -- TODO(native): cVar3 = (**(xStack_134 + 0x12c))()
                        scratchValue12 = nil --[[unresolved native value]]
                        sequence22 = scratchValue12 == 0
                    end
                    if sequence22 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        scratchValue26 = quest:AddNewConversation(nil --[[missing]], 15.0 ~= 0, nil --[[missing]])
                        quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                        hero19 = quest:GetHero()
                        quest:AddLineToConversation(scratchValue26, CCharString_OperatorPlus_API(xStack_fc,"TEXT_QST_B11_",me:GetDataString()) .. "_KEEPERISDEAD", hero19, nil --[[missing]])
                        c_stk_161_1 = 1
                    elseif timeRemaining % 5 == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                        scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                        quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                        hero21 = quest:GetHero()
                        scratchValue18 = "_OVERHERE"
                        -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_f0,"TEXT_QST_B11_",pCVar4)
                        scratchValue17 = nil --[[unresolved native value]]
                        quest:AddLineToConversation(scratchValue26, scratchValue17 .. scratchValue18, hero21, nil --[[missing]])
                    end
                    quest:SetTimer(timerId3, 10)
                    quest:SetTimer(unaff_EBX, quest:GetTimer(unaff_EBX) + 5)
                end
            end
            if quest:GetTimer(unaff_EBX) == 0 then
                scratchValue24 = scratchValue24 | 4
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
            if scratchValue24 & 4 ~= 0 then
                scratchValue24 = scratchValue24 & 0xfffffffb
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue14 = IsPlayerThreateningEntity(me)
                if scratchValue14 == 0 or scratchValue14 % 3 ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                    hero23 = quest:GetHero()
                    scratchValue18 = "_THREATEN"
                    -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_ec,"TEXT_QST_B11_",pCVar4)
                    scratchValue17 = nil --[[unresolved native value]]
                    quest:AddLineToConversation(scratchValue26, scratchValue17 .. scratchValue18, hero23, nil --[[missing]])
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                    hero3 = quest:GetHero()
                    scratchValue18 = "_THREATENWEAPON"
                    -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_dc,"TEXT_QST_B11_",pCVar4)
                    scratchValue17 = nil --[[unresolved native value]]
                    quest:AddLineToConversation(scratchValue26, scratchValue17 .. scratchValue18, hero3, nil --[[missing]])
                end
                quest:SetTimer(unaff_EBX, 20)
                quest:SetTimer(quest:GetTimer(x_stk_170) + 5, nil --[[missing]])
            end
            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
            if me:MsgIsHitBy("") then
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
                scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                hero5 = quest:GetHero()
                scratchValue18 = "_ONHIT"
                -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_10c,"TEXT_QST_B11_",pCVar4)
                scratchValue17 = nil --[[unresolved native value]]
                quest:AddLineToConversation(scratchValue26, scratchValue17 .. scratchValue18, hero5, nil --[[missing]])
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
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(scratchValue31)
        goto LAB_00e005d5
    end
    scratchValue12 = me:IsPerformingScriptTask()
    goto LAB_00dfeb57
    ::LAB_00dfeb80::
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(scratchValue31)
    else
        -- LAB_00dfebc5: (native jump target)
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(scratchValue31)
        if not quest:IsActiveThreadTerminating() then
            quest:DeactivateQuest("Q_TraderConflictGood_Extras", nil --[[missing]])
            quest:ActivateQuest("Q_TraderConflictGood_Extras")
            quest:SetIsPushableByHero(nil --[[missing]], nil --[[missing]])
            quest:EntityFollowThing(me, quest:GetHero(), nil --[[missing]], nil --[[missing]])
            quest:SetEntityAsRegionFollowing(quest:GetHero(), nil --[[missing]], nil --[[missing]])
            quest:EntitySetOpinionReactionsEnabled(me, false)
            quest:EntitySetDeedReactionsEnabled(me, false)
            quest:EntitySetCombatEnabled(me, false)
            quest:EntitySetInFaction(me, "FACTION_TRADERS")
            quest:DisplayQuestInfo(true)
            scratchValue17 = me:GetDataString()
            -- TODO(native): if *pCVar4 == nil then
            if scratchValue17 ~= "TRADERA" then
                scratchValue17 = me:GetDataString()
                -- TODO(native): if *pCVar4 == nil then
                if scratchValue17 == "TRADERB" then
                    -- LAB_00dfee05: (native jump target)
                    if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                    scratchValue18 = "HUD_QUEST_ICON_TRADER_HAT_02"
                    scratchValue26 = quest:AddQuestInfoBarHealth(nil --[[missing]], nil --[[missing]], scratchValue18, nil --[[missing]])
                    state:SetInt("BarIndex", scratchValue26)
                    goto FLOW_after_lab_00dfee1b
                end
                scratchValue17 = me:GetDataString()
                -- TODO(native): if *pCVar4 == nil then
                if scratchValue17 == "TRADERC" then return end  -- TODO(native): goto LAB_00dfee05
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e005d5 end
                scratchValue18 = "HUD_QUEST_ICON_TRADER"
                scratchValue26 = quest:AddQuestInfoBarHealth(nil --[[missing]], nil --[[missing]], scratchValue18, nil --[[missing]])
                state:SetInt("BarIndex", scratchValue26)
            end
            ::FLOW_after_lab_00dfee1b::
            scratchValue12 = quest:IsEntityFollowingHero(nil --[[missing]])
            while not scratchValue12 do
                if not quest:NewScriptFrame(me) then goto LAB_00e005d5 end
                scratchValue12 = quest:IsEntityFollowingHero(nil --[[missing]])
            end
            if not quest:IsActiveThreadTerminating() then
                scratchValue19 = self_0x14 + 84
                -- TODO(native): *piVar9 = *piVar9 + 1;
                scratchValue2 = 10
                timerId = quest:RegisterTimer()
                quest:SetTimer(timerId, 20)
                timerId2 = quest:RegisterTimer()
                while not quest:IsActiveThreadTerminating() do
                    while not quest:IsRegionLoaded("BanditCampEntrance") do
                        if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                        scratchValue12 = me:IsTalkedToByHero()
                        if scratchValue12 then
                            -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_68,"TEXT_QST_B11_",pCVar4)
                            scratchValue17 = nil --[[unresolved native value]]
                            scratchValue17 = scratchValue17 .. scratchValue19
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_158,pCVar4);
                            -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_50,CVar13)
                            scratchValue17 = nil --[[unresolved native value]]
                            scratchValue17 = xStack_158 .. scratchValue17
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not scratchValue12 then
                                scratchValue2 = 10
                                -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_34,10)
                                scratchValue17 = nil --[[unresolved native value]]
                                scratchValue17 = "" .. scratchValue17
                                -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            end
                            -- TODO(native): CStack_12c = (CCharString)((int)CVar13 + 0xa);
                            scratchValue12 = me:AcquireControl(4)
                            while not scratchValue12 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e005b1 end
                                scratchValue12 = me:AcquireControl(4)
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            scratchValue28 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(scratchValue2 ~= 0)
                            scratchValue29 = resources:ScriptThing(timerId2)
                            health2 = quest:GetHealth(nil --[[missing]])
                            if health2 > 0.0 then
                                -- TODO(native): pvVar8 = CCharString::operator_char_const_((CCharString *)&stack0xfffffe84);
                                me:Speak(quest:GetHero(), "", pvVar8, false, false, true)
                                scratchValue12 = me:IsPerformingScriptTask()
                                while scratchValue12 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(true)
                                        resources:DestroyMovie(scratchValue28)
                                        goto LAB_00e005b1
                                    end
                                    scratchValue12 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(10)
                                    resources:DestroyMovie(scratchValue29)
                                    goto LAB_00e005b1
                                end
                            end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(scratchValue28)
                        end
                        scratchValue15 = quest:GetTimer(unaff_EBX)
                        scratchValue4 = scratchValue15 == 0
                        if scratchValue4 then
                            scratchValue15 = IsPlayerThreateningEntity(me)
                            scratchValue4 = scratchValue15 ~= 0
                        end
                        scratchValue3 = scratchValue4
                        if scratchValue3 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            scratchValue3 = scratchValue12
                        end
                        if scratchValue3 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                            hero7 = quest:GetHero()
                            scratchValue17 = CCharString_OperatorPlus_API(xStack_88,"TEXT_QST_B11_",me:GetDataString()) .. scratchValue19
                            quest:AddLineToConversation(scratchValue26, scratchValue17, hero7, nil --[[missing]])
                            quest:SetTimer(unaff_EBX, 20)
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        if not me:MsgIsHitBy("") then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            if me:MsgIsHitByAnySpecialAbilityFrom("") then
                                scratchValue25 = scratchValue25 | 256
                                scratchValue12 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                predicateResult13 = true
                                if not scratchValue12 then
                                    goto FLOW_after_lab_00dff3b5
                                end
                            end
                            predicateResult13 = false
                        else
                            scratchValue25 = scratchValue25 | 256
                            scratchValue12 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                            predicateResult13 = true
                            if scratchValue12 then
                                predicateResult13 = false
                                goto FLOW_after_lab_00dff3b5
                            end
                        end
                        ::FLOW_after_lab_00dff3b5::
                        if scratchValue25 & 256 ~= 0 then
                            scratchValue25 = scratchValue25 & 0xfffffeff
                        end
                        if SUB41(scratchValue25,0) < 0 then
                            scratchValue25 = scratchValue25 & 0xffffff7f
                        end
                        if scratchValue25 & 64 ~= 0 then
                            scratchValue25 = scratchValue25 & 0xffffffbf
                        end
                        if predicateResult13 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                            health3 = quest:GetHealth(nil --[[missing]])
                            scratchValue5 = health3 <= 5.0
                            if not scratchValue5 then
                                scratchValue15 = quest:GetTimer(scratchValue27)
                                scratchValue5 = scratchValue15 ~= 0
                            end
                            if scratchValue5 then
                                predicateResult14 = false
                            else
                                if me:MsgIsHitByHero() then
                                    predicateResult14 = false
                                    goto FLOW_after_lab_00dff54a
                                end
                                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                    scratchValue24 = scratchValue25 | 3584
                                    scratchValue12 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                    if not scratchValue12 then
                                        predicateResult14 = false
                                        goto FLOW_after_lab_00dff54a
                                    end
                                end
                                predicateResult14 = true
                            end
                            ::FLOW_after_lab_00dff54a::
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
                                scratchValue19 = me:GetDataString()
                                scratchValue15 = scratchValue19 == "TRADERA" and 0 or 1
                                if scratchValue15 ~= 0 then
                                    scratchValue19 = me:GetDataString()
                                    scratchValue15 = scratchValue19 == "TRADERB" and 0 or 1
                                    if scratchValue15 ~= 0 then
                                        scratchValue2 = scratchValue27
                                        -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_84,xStack_148)
                                        scratchValue17 = nil --[[unresolved native value]]
                                        -- TODO(native): CCharString_OperatorPlus_API(&xStack_34,"SND_MM_TRADER_C_SCREAM_0",pCVar4);
                                        scratchValue21 = quest:PlaySoundOnThing(nil --[[missing]], scratchValue19)
                                        scratchValue11 = scratchValue2 == 4
                                    else
                                        scratchValue2 = scratchValue27
                                        scratchValue17 = GFIntToCharString_API(xStack_30,scratchValue27)
                                        -- TODO(native): CCharString_OperatorPlus_API(&xStack_7c,"SND_MM_TRADER_B_SCREAM_0",pCVar4);
                                        scratchValue22 = quest:PlaySoundOnThing(nil --[[missing]], "")
                                        scratchValue11 = scratchValue2 == 3
                                    end
                                else
                                    scratchValue2 = scratchValue27
                                    scratchValue17 = GFIntToCharString_API(xStack_44,scratchValue27)
                                    -- TODO(native): CCharString_OperatorPlus_API(&xStack_74,"SND_MM_TRADER_A_SCREAM_0",pCVar4);
                                    scratchValue23 = quest:PlaySoundOnThing(nil --[[missing]], "")
                                    scratchValue11 = scratchValue2 == 3
                                end
                                if scratchValue11 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    scratchValue2 = 0
                                end
                                -- TODO(native): xStack_148 = (CCharString)((int)CVar13 + 1);
                                quest:SetTimer(4, scratchValue2)
                            else
                                health4 = quest:GetHealth(nil --[[missing]])
                                if health4 <= 5.0 then
                                    predicateResult18 = false
                                else
                                    if not me:MsgIsHitByHero() then
                                        if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                            scratchValue24 = scratchValue25 | 0x7000
                                            scratchValue12 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                            if not scratchValue12 then goto LAB_00dff880 end
                                        end
                                        predicateResult18 = false
                                        goto FLOW_after_lab_00dff887
                                    end
                                    ::LAB_00dff880::
                                    predicateResult18 = true
                                end
                                ::FLOW_after_lab_00dff887::
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
                                    scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                    quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                                    hero9 = quest:GetHero()
                                    scratchValue17 = CCharString_OperatorPlus_API(scratchValue28,"TEXT_QST_B11_",me:GetDataString()) .. scratchValue19
                                    quest:AddLineToConversation(scratchValue26, scratchValue17, hero9, nil --[[missing]])
                                end
                            end
                        end
                        if not me:MsgIsHitByHero() then
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue24 = scratchValue25 | 0x38000
                                scratchValue12 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not scratchValue12 then
                                    predicateResult19 = true
                                    goto FLOW_after_lab_00dffa46
                                end
                            end
                            predicateResult19 = false
                        else
                            predicateResult19 = true
                        end
                        ::FLOW_after_lab_00dffa46::
                        if scratchValue24 & 0x20000 ~= 0 then
                            scratchValue24 = scratchValue24 & 0xfffdffff
                        end
                        if scratchValue24 & 0x10000 ~= 0 then
                            scratchValue24 = scratchValue24 & 0xfffeffff
                        end
                        if scratchValue24 & 0x8000 ~= 0 then
                            scratchValue24 = scratchValue24 & 0xffff7fff
                        end
                        if predicateResult19 and quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                        if me:MsgExpressionPerformedTo() then
                            if xStack_15c ~= nil then
                                -- TODO(native): iVar21 = CBasicString<char>::Compare(*(void **)xStack_15c,"EXPRESSION_WAIT");
                                if scratchValue15 == 0 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005b1 end
                                    scratchValue19 = self_0x14 + 84
                                    -- TODO(native): *piVar9 = *piVar9 - 1;
                                end
                            end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then break end
                    teleporterMarker = quest:GetThingWithScriptName("TeleporterMarker")
                    -- TODO(native): CCharString::CCharString({R = 255, G = 0, B = 0, A = 255},"BanditCampEntrance",-1);
                    c_stk_161_3 = quest:IsRegionLoaded(scratchValue18)
                    while c_stk_161_3 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                        scratchValue12 = me:IsTalkedToByHero()
                        if scratchValue12 then
                            -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_64,"TEXT_QST_B11_",pCVar4)
                            scratchValue17 = nil --[[unresolved native value]]
                            scratchValue17 = scratchValue17 .. scratchValue19
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_158,pCVar4);
                            -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_98,CVar13)
                            scratchValue17 = nil --[[unresolved native value]]
                            scratchValue17 = xStack_158 .. scratchValue17
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not scratchValue12 then
                                scratchValue2 = 10
                                -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_88,10)
                                scratchValue17 = nil --[[unresolved native value]]
                                scratchValue17 = xStack_15c .. scratchValue17
                                -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            end
                            -- TODO(native): CStack_12c = (CCharString)((int)CVar13 + 0xa);
                            scratchValue12 = me:AcquireControl(4)
                            while not scratchValue12 do
                                if not quest:NewScriptFrame(me) then goto LAB_00e005ac end
                                scratchValue12 = me:AcquireControl(4)
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue30 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(scratchValue2 ~= 0)
                            scratchValue31 = resources:ScriptThing(scratchValue27)
                            if quest:GetHealth(teleporterMarker) > 0.0 then
                                -- TODO(native): pvVar8 = CCharString::operator_char_const_((CCharString *)&stack0xfffffe84);
                                me:Speak(quest:GetHero(), "", pvVar8, false, false, true)
                                scratchValue12 = me:IsPerformingScriptTask()
                                while scratchValue12 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        resources:DestroyMovie(scratchValue28)
                                        goto LAB_00e005ac
                                    end
                                    scratchValue12 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    resources:DestroyMovie(scratchValue30)
                                    goto LAB_00e005ac
                                end
                            end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(scratchValue30)
                        end
                        scratchValue15 = quest:GetTimer(unaff_EBX)
                        scratchValue7 = scratchValue15 == 0
                        if scratchValue7 then
                            scratchValue15 = IsPlayerThreateningEntity(me)
                            scratchValue7 = scratchValue15 ~= 0
                        end
                        scratchValue6 = scratchValue7
                        if scratchValue6 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            scratchValue6 = scratchValue12
                        end
                        if scratchValue6 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                            hero11 = quest:GetHero()
                            -- TODO(native): pCVar10 = CCharString_OperatorPlus_API(&xStack_64,"TEXT_QST_B11_",pCVar10)
                            scratchValue16 = nil --[[unresolved native value]]
                            scratchValue17 = scratchValue16 .. scratchValue17
                            quest:AddLineToConversation(scratchValue26, scratchValue17, hero11, nil --[[missing]])
                            quest:SetTimer(unaff_EBX, 20)
                        end
                        if not me:MsgIsHitByHero() then
                            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                                scratchValue25 = scratchValue24 | 0x1c0000
                                scratchValue12 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not scratchValue12 then
                                    predicateResult23 = true
                                    goto FLOW_after_lab_00e0006b
                                end
                            end
                            predicateResult23 = false
                        else
                            predicateResult23 = true
                        end
                        ::FLOW_after_lab_00e0006b::
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
                            scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
                            hero13 = quest:GetHero()
                            scratchValue17 = CCharString_OperatorPlus_API(scratchValue31,"TEXT_QST_B11_",me:GetDataString()) .. scratchValue17
                            quest:AddLineToConversation(scratchValue26, scratchValue17, hero13, nil --[[missing]])
                        end
                        if me:MsgExpressionPerformedTo() then
                            if xStack_15c ~= nil then
                                -- TODO(native): iVar21 = CBasicString<char>::Compare(*(void **)xStack_15c,"EXPRESSION_WAIT");
                                if scratchValue15 == 0 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                                    scratchValue19 = self_0x14 + 84
                                    -- TODO(native): *piVar9 = *piVar9 - 1;
                                end
                            end
                        end
                        if quest:IsDistanceBetweenThingsUnder(me, xStack_114_2, 20.0) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            -- TODO(native): *piVar9 = *piVar9 + 1;
                            quest:EntityStopFollowing(nil --[[missing]])
                            quest:SetEntityAsRegionFollowing(quest:GetHero(), nil --[[missing]], nil --[[missing]])
                            quest:EntitySetAsScared(nil --[[missing]], nil --[[missing]])
                            if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                                quest:SetStateBool("OutroStart", true)
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:NewScriptFrame(me)
                                -- TODO(native): Main_InitializeFourierAnalysis_4(*(undefined4 *)(this + 0x14));
                                quest:SetStateBool("OutroDone", true)
                                quest:RemoveThing(nil --[[missing]])
                                goto FLOW_after_lab_00e00595
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e005ac end
                            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh({R = 255, G = 0, B = 0, A = 255});
                            scratchValue12 = me:AcquireControl(4)
                            goto LAB_00e0035a
                        end
                        c_stk_161_3 = quest:IsRegionLoaded("BanditCampEntrance")
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
        scratchValue26 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
        quest:AddPersonToConversation(nil --[[missing]], quest:GetHero())
        hero17 = quest:GetHero()
        quest:AddLineToConversation(scratchValue26, CCharString_OperatorPlus_API(scratchValue31,"TEXT_QST_B11_",me:GetDataString()) .. scratchValue31, hero17, nil --[[missing]])
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
                    quest:RemoveQuestInfoElement(nil --[[missing]])
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
                    quest:RemoveThing(nil --[[missing]])
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
    quest:DeregisterTimer(xStack_160)
    quest:DeregisterTimer(nil --[[missing]])
    ::LAB_00e005f9::
    resources:ReleaseResource(scratchValue27)
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

