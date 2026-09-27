-- Generated native draft: V_SingingStones. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local CVar4
    local alive = true
    local bVar1 = quest:IsRegionLoaded("Witchwood2")
    CVar4 = 0
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:AddEntityBinding("SingingStone", "V_SingingStones/Entities/SingingStone")
                quest:AddEntityBinding("ManWithDoorName", "V_SingingStones/Entities/ManWithDoorName")
                quest:FinalizeEntityBindings()
                quest:CreateThread("WatchForCompleteTune")  -- native thread body 0x00ED2BB0: lift it as function WatchForCompleteTune(quest)
                if (CVar4 & 4) ~= 0 then
                    CVar4 = CVar4 & 0xfffffffb
                end
                quest:CreateThread("WatchForSpokenToDemonDoors")  -- native thread body 0x00ED13B0: lift it as function WatchForSpokenToDemonDoors(quest)
                if (CVar4 & 8) ~= 0 then
                    CVar4 = CVar4 & 0xfffffff7
                end
                quest:CreateThread("WatchForRegionLeaving")  -- native thread body CQ_BanditCampScript::WatchForEndOfScript: lift it as function WatchForRegionLeaving(quest)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsRegionLoaded("Witchwood2")
        CVar4 = 0x0
    end
end

function Init(quest)
    quest:SetStateInt("PlayList_1", 1)
    quest:SetStateInt("RudePlayList_2", 1)
    quest:SetStateInt("CurrentPlayListIndex", 0)
    quest:SetStateInt("PlayList_0", 3)
    quest:SetStateInt("PlayList_2", 0)
    quest:SetStateInt("PlayList_3", 2)
    quest:SetStateInt("RudePlayList_0", 2)
    quest:SetStateInt("RudePlayList_1", 3)
    quest:SetStateInt("RudePlayList_3", 0)
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 0),"A");
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 1),"B");
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 2),"C");
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 3),"D");
    quest:SetStateBool("DoorManIntroComplete", false)
    quest:SetStateBool("DoorManAttackedByHero", false)
    quest:SetStateBool("DoorManHasBribe", false)
    quest:SetStateBool("DoorManComplete", false)
end

function OnPersist(quest, context)
    local doorManIntroComplete = quest:GetStateBool("DoorManIntroComplete") or false
    doorManIntroComplete = quest:PersistTransferBool(context, "DoorManIntroComplete", doorManIntroComplete)
    quest:SetStateBool("DoorManIntroComplete", doorManIntroComplete)
    local doorManAttackedByHero = quest:GetStateBool("DoorManAttackedByHero") or false
    doorManAttackedByHero = quest:PersistTransferBool(context, "DoorManAttackedByHero", doorManAttackedByHero)
    quest:SetStateBool("DoorManAttackedByHero", doorManAttackedByHero)
    local doorManHasBribe = quest:GetStateBool("DoorManHasBribe") or false
    doorManHasBribe = quest:PersistTransferBool(context, "DoorManHasBribe", doorManHasBribe)
    quest:SetStateBool("DoorManHasBribe", doorManHasBribe)
    local doorManComplete = quest:GetStateBool("DoorManComplete") or false
    doorManComplete = quest:PersistTransferBool(context, "DoorManComplete", doorManComplete)
    quest:SetStateBool("DoorManComplete", doorManComplete)
end

function WatchForCompleteTune(quest)
    local resources = quest:RetailResources()
    local bVar3, cVar2, c_stk_81, c_stk_82, c_stk_83, conversationID, elem_1, iStack_68, iVar5, i_stk_7c, native_arg_switch_2, p0, pCVar4, pPosition, piVar6, puVar7, r1, r2, r3, r4, u_stk_78, xStack_28, xStack_38
    local alive = true
    c_stk_83 = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    repeat
        if quest:GetStateInt("CurrentPlayListIndex") == 4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            c_stk_82 = 1
            c_stk_81 = 1
            iVar5 = 0
            piVar6 = (this + 0x6c)
            repeat
                -- TODO(native): if piVar6[-8] ~= *piVar6 then
                if false then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    c_stk_82 = 0
                end
                -- TODO(native): if piVar6[-4] ~= *piVar6 then
                if false then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    c_stk_81 = 0
                end
                iVar5 = iVar5 + 1
                piVar6 = piVar6 + 1
            until not (iVar5 < 4)
            if c_stk_82 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                pCVar4 = quest:GetHero()
                r1 = quest:GetNearestWithScriptName(pCVar4, "SpeakMarker")
                conversationID = quest:AddNewConversation(r1, true, true)
                pCVar4 = quest:GetHero()
                quest:AddPersonToConversation(conversationID, pCVar4)
                pCVar4 = quest:GetHero()
                quest:AddLineToConversation(conversationID, "TEXT_QST_060_NAME_DBAC", r1, pCVar4, false)
                quest:SetMasterGameState("SingingStonesInSync", true)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                cVar2 = quest:GetMasterGameState("SingingStonesInSync")
                break
            end
            if (c_stk_81 == 0) or (c_stk_83 ~= 0) then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                iVar5 = 0
                puVar7 = (this + 0x6c)
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    -- TODO(native): native_arg_switch_2 = *puVar7
                    native_arg_switch_2 = nil --[[unresolved native value]]
                    repeat
                        if native_arg_switch_2 == 0 then
                            p0 = "A"
                            break
                        else
                            if native_arg_switch_2 == 1 then
                                p0 = "B"
                                break
                            else
                                if native_arg_switch_2 == 2 then
                                    p0 = "C"
                                    break
                                else
                                    if native_arg_switch_2 == 3 then
                                        p0 = "D"
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                end
                            end
                        end
                    until not (false)
                    -- TODO(native): CCharString::operator+=(&xStack_80,(int)p0);
                    ::FLOW_native_label_1::
                    iVar5 = iVar5 + 1
                    puVar7 = puVar7 + 1
                until not (iVar5 < 4)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00ed3349: (native jump target)
                    return
                end
                xStack_38 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint(r1, nil --[[missing]], 0x0, 0x0, -1.0)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                pCVar4 = quest:GetHero()
                r2 = quest:GetNearestWithScriptName(pCVar4, "SpeakMarker")
                iVar5 = quest:AddNewConversation(r2, true, true)
                pCVar4 = quest:GetHero()
                quest:AddPersonToConversation(iVar5, pCVar4)
                pCVar4 = quest:GetHero()
                quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_", r2, pCVar4, false)
                quest:Pause(2.0)
                quest:CameraDefault()
                -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
                quest:StateListClear("EffectList")
                r2 = nil
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_38)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                c_stk_83 = 1
                xStack_28 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], 0x0, 0x0, -1.0)
                pCVar4 = quest:GetHero()
                r3 = quest:GetNearestWithScriptName(pCVar4, "SpeakMarker")
                iVar5 = quest:AddNewConversation(r3, true, true)
                pCVar4 = quest:GetHero()
                quest:AddPersonToConversation(iVar5, pCVar4)
                pCVar4 = quest:GetHero()
                quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_CDBA", r3, pCVar4, false)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                iStack_68 = quest:GetAllThingsWithScriptName("M_StonesSpawnEnemy")
                u_stk_78 = 0
                if #iStack_68 ~= 0 then
                    i_stk_7c = 0
                    repeat
                        bVar3 = false
                        elem_1 = iStack_68[(i_stk_7c) / 0xc + 1]
                        pPosition = elem_1:GetPos()
                        r4 = quest:CreateCreature("", pPosition, "CREATURE_BALVERINE_01")
                        pCVar4 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(r4, pCVar4)
                        i_stk_7c = i_stk_7c + 0xc
                        u_stk_78 = u_stk_78 + 1
                    until not (u_stk_78 < (#iStack_68))
                end
                quest:Pause(2.0)
                quest:CameraDefault()
                -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
                quest:StateListClear("EffectList")
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_28)
            end
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
    until false
    ::LAB_00ed3294::
    if not cVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
            quest:StateListClear("EffectList")
            quest:DeactivateQuestLater("V_SingingStones", 0)
            quest:DeactivateQuestLater("V_SingingStones_Activate", 0)
        end
        -- LAB_00ed3335: (native jump target)
        return
    end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    cVar2 = quest:GetMasterGameState("SingingStonesInSync")
    goto LAB_00ed3294
end

function WatchForSpokenToDemonDoors(quest)
    local bVar2
    local alive = true
    local cVar1 = quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors")
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:ActivateQuest("V_SingingStones_Activate")
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        cVar1 = quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors")
    end
end

function WatchForRegionLeaving(quest)
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    while true do
        if bVar1 then
            return
        end
        bVar1 = quest:IsRegionLoaded("Witchwood2")
        while not bVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            bVar1 = quest:IsRegionLoaded("Witchwood2")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        bVar1 = quest:IsRegionLoaded("Witchwood2")
        if bVar1 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                bVar1 = quest:IsRegionLoaded("Witchwood2")
            until not (bVar1)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
        -- TODO(native): Std_Vector_Erase_Range(quest:GetStateListRef("EffectList"),quest:GetStateListRef("EffectList"),(quest:GetStateListCount("EffectList") * 0xc));
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    end
end

