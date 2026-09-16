-- Generated native draft: AppleGirl. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar3, cVar4, fVar13, fVar2, fVar20, iVar7, native_arg_sequence_1, native_arg_switch_2, pCVar1, pCVar11, pCVar16, pCVar17, pCVar18, pCVar21, pCVar6, pcVar15, ppVar8, r1, r2, r3, r4, uVar19, uVar5
    local alive = true
    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_84);
    if bVar3 then
    end
    cVar4 = me:AcquireControl(4)
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar4 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    me:SetFriendsWithEverythingFlag(nil --[[missing]])
    quest:EntitySetAsKillable(me, false)
    r1 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, r1)
    __native_entity_state:SetStateInt("AppleMode", 0)
    uVar5 = quest:RegisterTimer()
    quest:SetTimer(uVar5, 0xf)
    __native_entity_state:SetStateInt("CurrentApples", 0)
    __native_entity_state:SetStateBool("ChildAppleMode", false)
    quest:SetThingHasInformation(nil --[[missing]])
    iVar7 = __native_entity_state:GetStateInt("AppleMode")
    while iVar7 == 0 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d3e1b6 end
        fVar20 = 5.5
        pCVar6 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar20)
        __native_condition_1 = bVar3
        if __native_condition_1 then
            iVar7 = quest:GetTimer(uVar5)
            __native_condition_1 = iVar7 < 1
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e1b6 end
            ppVar8 = quest:AddNewConversation(me, false, false)
            uVar5 = quest:GetHero()
            quest:AddPersonToConversation(ppVar8, uVar5)
            uVar5 = quest:GetHero()
            quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_HELP", me, uVar5, false)
            quest:SetTimer(uVar5, 0xf)
        end
        cVar4 = me:IsTalkedToByHero()
        if cVar4 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e1b6 end
            pCVar16 = ""
            quest:StartMovieSequence()
            ppVar8 = 0x1
            quest:PauseAllNonScriptedEntities((ppVar8 ~= 0))
            if not __native_entity_state:GetStateBool("HaveChatted") then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d3e06c end
                __native_entity_state:SetStateBool("HaveChatted", true)
                -- TODO(native): uVar5 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_d8);
                fVar13 = quest:GetHealth(nil --[[missing]])
                fVar2 = _DAT_0122dedc
                if fVar2 < fVar13 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar17 = 0x0
                    pCVar16 = 0x0
                    pcVar15 = "TEXT_QST_028_APPLEGIRL_CHAT"
                    pCVar6 = quest:GetHero()
                    r2 = me:Speak(pCVar6, pcVar15, pCVar16, (pCVar17 ~= 0), (pCVar18 ~= 0), bVar3)
                    bVar3 = me:IsPerformingScriptTask()
                    if bVar3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d3d948 end
                            bVar3 = me:IsPerformingScriptTask()
                        until not (bVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e06c end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_YES", "", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d3e087 end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION_AGAIN", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            end
            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar7 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    quest:DeregisterTimer(uVar5)
                    return
                end
                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e087 end
            alive = not quest:IsActiveThreadTerminating()
            if iVar7 == 1 then
                if not alive then goto LAB_00d3e06c end
                __native_entity_state:SetStateInt("AppleMode", 1)
                cVar4 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
                if cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e087 end
                    __native_entity_state:SetStateBool("ChildAppleMode", true)
                end
            else
                if not alive then goto LAB_00d3e06c end
                -- TODO(native): uVar5 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_d8);
                uVar19 = SUB41(ppVar8,0)
                fVar13 = quest:GetHealth(nil --[[missing]])
                fVar2 = _DAT_0122dedc
                if fVar2 < fVar13 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar17 = 0x0
                    pCVar16 = 0x0
                    pcVar15 = "TEXT_QST_028_APPLEGIRL_IMPLORE"
                    pCVar6 = quest:GetHero()
                    r3 = me:Speak(pCVar6, pcVar15, pCVar16, (pCVar17 ~= 0), (pCVar18 ~= 0), bVar3)
                    bVar3 = me:IsPerformingScriptTask()
                    if bVar3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d3d948 end
                            bVar3 = me:IsPerformingScriptTask()
                        until not (bVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e06c end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
        end
        uVar5 = uVar5
        iVar7 = __native_entity_state:GetStateInt("AppleMode")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        iVar7 = __native_entity_state:GetStateInt("AppleMode")
        while iVar7 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e1b6 end
            if __native_entity_state:GetStateBool("ChildAppleMode") then
                pCVar11 = (me | 1)
                cVar4 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
                if cVar4 then return end  -- TODO(native): goto LAB_00d3d987
                bVar3 = true
            else
                -- LAB_00d3d987: (native jump target)
                bVar3 = false
            end
            if (pCVar11 & 1) ~= 0 then
                pCVar11 = (pCVar11 & 0xfffffffe)
            end
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d3e1b6 end
                quest:RemoveThing(me, false, true)
            end
            fVar20 = 5.5
            pCVar6 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar20)
            __native_condition_2 = bVar3
            if __native_condition_2 then
                iVar7 = quest:GetTimer(uVar5)
                __native_condition_2 = iVar7 < 1
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d3e1b6 end
                if __native_entity_state:GetStateInt("CurrentApples") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e1b6 end
                    ppVar8 = quest:AddNewConversation(me, false, false)
                    uVar5 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar8, uVar5)
                    uVar5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_ANY_APPLES", me, uVar5, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e1b6 end
                    ppVar8 = quest:AddNewConversation(me, false, false)
                    uVar5 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar8, uVar5)
                    uVar5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_MORE_APPLES", me, uVar5, false)
                end
                quest:SetTimer(uVar5, 0xf)
            end
            cVar4 = me:IsTalkedToByHero()
            if not cVar4 then goto FLOW_native_label_1 end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e1b6 end
            while true do
                pCVar11 = (pCVar11 | 2)
                uVar5 = quest:GetHero()
                cVar4 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", uVar5)
                native_arg_sequence_1 = false
                if not cVar4 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    -- TODO(native): ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *) (*(int *)(this + 0x20) + (int)pCStack_ac);
                    if 3 < ppVar10 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    bVar3 = false
                else
                    bVar3 = true
                end
                if (pCVar11 & 2) ~= 0 then
                    pCVar11 = (pCVar11 & 0xfffffffd)
                end
                if not bVar3 then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d3e1b6 end
                -- TODO(native): pCStack_ac = (CScriptThing *)((int)pCStack_ac + 1);
                quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e1b6 end
            if pCStack_ac == 0x1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d3e1b6 end
                ppVar8 = quest:AddNewConversation(me, false, false)
                uVar5 = quest:GetHero()
                quest:AddPersonToConversation(ppVar8, uVar5)
                uVar5 = quest:GetHero()
                quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE", me, uVar5, false)
            else
                if pCStack_ac == nil then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e1b6 end
                    ppVar8 = quest:AddNewConversation(me, false, false)
                    uVar5 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar8, uVar5)
                    uVar5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES", me, uVar5, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d3e1b6 end
                    ppVar8 = quest:AddNewConversation(me, false, false)
                    uVar5 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar8, uVar5)
                    uVar5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES", me, uVar5, false)
                end
            end
            quest:Pause(0x3f800000)
            __native_entity_state:SetStateInt("CurrentApples", __native_entity_state:GetStateInt("CurrentApples") + pCStack_ac)
            if pCStack_ac == nil then goto FLOW_native_label_1 end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d3e1b6 end
            ppVar8 = quest:AddNewConversation(me, false, false)
            native_arg_switch_2 = __native_entity_state:GetStateInt("CurrentApples")
            repeat
                if native_arg_switch_2 == 1 then
                    uVar5 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar8, uVar5)
                    uVar5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_THREE_NEEDED", me, uVar5, false)
                    break
                else
                    if native_arg_switch_2 == 2 then
                        uVar5 = quest:GetHero()
                        quest:AddPersonToConversation(ppVar8, uVar5)
                        uVar5 = quest:GetHero()
                        quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_TWO_NEEDED", me, uVar5, false)
                        break
                    else
                        if native_arg_switch_2 == 3 then
                            uVar5 = quest:GetHero()
                            quest:AddPersonToConversation(ppVar8, uVar5)
                            uVar5 = quest:GetHero()
                            quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_ONE_NEEDED", me, uVar5, false)
                            -- TODO(native): paVar9 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_80;
                            break
                        else
                            if native_arg_switch_2 == 4 then
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&ppuStack_a8);
                                pCVar16 = ""
                                quest:StartMovieSequence()
                                uVar19 = 1
                                quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                -- TODO(native): uVar5 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_d8);
                                fVar13 = quest:GetHealth(nil --[[missing]])
                                fVar2 = _DAT_0122dedc
                                if fVar2 < fVar13 then
                                    bVar3 = false
                                    pCVar18 = 0x1
                                    pCVar17 = 0x0
                                    pCVar16 = 0x0
                                    pcVar15 = "TEXT_QST_028_APPLEGIRL_THANKS"
                                    pCVar6 = quest:GetHero()
                                    r4 = me:Speak(pCVar6, pcVar15, pCVar16, (pCVar17 ~= 0), (pCVar18 ~= 0), bVar3)
                                    bVar3 = me:IsPerformingScriptTask()
                                    if bVar3 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00d3e06c end
                                            bVar3 = me:IsPerformingScriptTask()
                                        until not (bVar3)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d3e087 end
                                end
                                quest:GiveHeroObject("OBJECT_PIE_BLUEBERRY_01", -1)
                                __native_entity_state:SetStateInt("AppleMode", 2)
                                quest:ClearThingHasInformation(me)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_native_label_1
                            else
                                goto FLOW_native_label_1
                            end
                        end
                    end
                end
            until not (false)
            quest:Pause(0x3f800000)
            ::FLOW_native_label_1::
            iVar7 = __native_entity_state:GetStateInt("AppleMode")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            alive = not quest:IsActiveThreadTerminating()
            cVar4 = extraout_AL_30
            while cVar4 == 0 do
                if __native_entity_state:GetStateBool("ChildAppleMode") then
                    pCVar11 = (pCVar11 | 4)
                    cVar4 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
                    if cVar4 then return end  -- TODO(native): goto LAB_00d3e0a0
                    bVar3 = true
                else
                    -- LAB_00d3e0a0: (native jump target)
                    bVar3 = false
                end
                if (pCVar11 & 4) ~= 0 then
                    pCVar11 = 0x0
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    quest:RemoveThing(me, false, true)
                end
                fVar20 = 5.5
                pCVar6 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar20)
                __native_condition_3 = bVar3
                if __native_condition_3 then
                    iVar7 = quest:GetTimer(uVar5)
                    __native_condition_3 = iVar7 < 1
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    ppVar8 = quest:AddNewConversation(me, false, false)
                    uVar5 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar8, uVar5)
                    uVar5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar8, "TEXT_QST_028_APPLEGIRL_THANKS_AGAIN", me, uVar5, false)
                    quest:SetTimer(uVar5, 0xf)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                cVar4 = extraout_AL_33
            end
        end
    end
    ::LAB_00d3e1b6::
    ::LAB_00d3e1bf::
    do return end
    ::LAB_00d3d948::
    quest:PauseAllNonScriptedEntities(false)
    quest:DeregisterTimer(uVar5)
    goto LAB_00d3e1bf
    ::LAB_00d3e06c::
    quest:PauseAllNonScriptedEntities(false)
    goto LAB_00d3e1b6
    ::LAB_00d3e087::
    quest:PauseAllNonScriptedEntities(false)
    goto LAB_00d3e1b6
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HaveChatted", false)
end

function OnPersist(quest, context)
    local appleMode = quest:GetStateBool("AppleMode") or false
    appleMode = quest:PersistTransferBool(context, "AppleMode", appleMode)
    quest:SetStateBool("AppleMode", appleMode)
    local currentApples = quest:GetStateBool("CurrentApples") or false
    currentApples = quest:PersistTransferBool(context, "CurrentApples", currentApples)
    quest:SetStateBool("CurrentApples", currentApples)
end

function OnPredicateFail(quest, me)
end

