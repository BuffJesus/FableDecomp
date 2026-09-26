-- Generated from the same native helper bodies as the quest draft.
local MakeTraderComment
function MakeTraderComment(quest, me, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local __native_condition_1, bVar2, cVar3, iVar5, iVar6, pCVar10, pCVar4, pCVar7, pCVar8, pCVar9, pcVar15, piVar1, piVar13, r1, r2, r3, r4, this_00, uVar11, uVar14, xStack_30
    local alive = true
    -- TODO(native): xStack_30._4_4_ = *(undefined4 *)(native_arg_speaker + 0x4);
    -- TODO(native): xStack_30 = *(int **)(native_arg_speaker + 0x8);
    xStack_30 = nil
    if xStack_30 ~= nil then
        -- TODO(native): *xStack_30 = *xStack_30 + 1;
    end
    -- TODO(native): bVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)(xStack_30)
    bVar2 = nil --[[unresolved native value]]
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return false
        end
        pCVar4 = quest:GetHero()
        pCVar4 = quest:GetNearestWithScriptName(pCVar4, "DarkwoodTrader")
        -- TODO(native): piVar13 = *(pCVar4 + 0x8)
        piVar13 = nil --[[unresolved native value]]
        -- TODO(native): piVar1 = *(pCVar4 + 0x4)
        piVar1 = nil --[[unresolved native value]]
        if xStack_30 ~= piVar13 then
            -- TODO(native): xStack_30._4_4_ = piVar1;
            if piVar13 ~= nil then
                -- TODO(native): *piVar13 = *piVar13 + 1;
            end
        end
    end
    -- TODO(native): bVar2 = (**(xStack_30._0_4_ + 0x12c))(xStack_30)
    bVar2 = nil --[[unresolved native value]]
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    if native_arg_comment_type ~= 2 then
        iVar5 = quest:GetTimer(quest:GetStateInt("CommentTimer"))
        if 0 < iVar5 then
            alive = not quest:IsActiveThreadTerminating()
            return false
        end
        iVar6 = quest:AddNewConversation(xStack_30, false, false)
        pCVar4 = quest:GetHero()
        quest:AddPersonToConversation(iVar6, pCVar4)
        if not (xStack_30 ~= nil and not xStack_30:IsNull()) then
            native_arg_speaker = ""
        else
            xStack_30:GetDataString()
        end
        pCVar7 = quest:GetHero()
        pCVar9 = native_arg_comment_to_make
        pCVar4 = xStack_30
        uVar14 = false
        piVar13 = "_"
        pCVar8 = native_arg_comment_to_make
        pCVar10 = ("TEXT_QST_067_" .. native_arg_speaker)
        pCVar10 = (pCVar10 .. "_")
        pCVar8 = (pCVar10 .. pCVar8)
        quest:AddLineToConversation(iVar6, pCVar8, pCVar4, pCVar7, uVar14)
        if native_arg_comment_type ~= 1 then goto LAB_00e02355 end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e01aca end
        r1 = quest:GetNearestWithScriptName(xStack_30, "DarkwoodTrader")
        iVar5 = (r1 ~= nil and r1:IsAlive())
        __native_condition_1 = not iVar5
        if not __native_condition_1 then
            cVar3 = (r1 ~= nil and r1:IsEqualTo(xStack_30._4_4_))
            __native_condition_1 = cVar3
        end
        if __native_condition_1 then
            goto LAB_00e02213
        else
            pcVar15 = "_RESPONSE"
            piVar13 = "_"
            pCVar8 = pCVar9
            pCVar10 = r1:GetDataString()
            pCVar10 = ("TEXT_QST_067_" .. pCVar10)
            pCVar10 = (pCVar10 .. "_")
            pCVar8 = (pCVar10 .. pCVar8)
            pCVar8 = (pCVar8 .. pcVar15)
            bVar2 = quest:TextEntryExists(pCVar8)
            native_arg_speaker = CONCAT31(native_arg_speaker._1_3_,1)
            if not bVar2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        uVar11 = 0x1f
        if (0x1f & 0x10) ~= 0 then
            uVar11 = 0x1f & 0xffffffef
        end
        if (uVar11 & 8) ~= 0 then
            uVar11 = uVar11 & 0xfffffff7
        end
        if (uVar11 & 4) ~= 0 then
            uVar11 = uVar11 & 0xfffffffb
        end
        if (uVar11 & 2) ~= 0 then
            uVar11 = uVar11 & 0xfffffffd
        end
        if native_arg_speaker ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return false
            end
            quest:AddPersonToConversation(iVar6, r1)
            pCVar7 = quest:GetHero()
            pCVar4 = r1
            uVar14 = false
            pcVar15 = "_RESPONSE"
            piVar13 = "_"
            pCVar8 = r1:GetDataString()
            pCVar8 = ("TEXT_QST_067_" .. pCVar8)
            pCVar8 = (pCVar8 .. "_")
            pCVar9 = (pCVar8 .. pCVar9)
            pCVar9 = (pCVar9 .. pcVar15)
            quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
        end
        ::LAB_00e02355::
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        return true
    end
    ::FLOW_after_lab_00e02355::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e01aca end
    r2 = quest:GetNearestWithScriptName(xStack_30, "DarkwoodTrader")
    r3 = quest:GetFurthestWithScriptName(xStack_30, "DarkwoodTrader")
    cVar3 = (r2 ~= nil and r2:IsEqualTo(r3._4_4_))
    if not cVar3 then
        goto LAB_00e01afb
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_c);
            -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,iVar5);
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    pcVar15 = "SCARED"
    this_00 = r2:GetDataString()
    iVar5 = ((this_00 ~= pcVar15) and 1 or 0)
    if iVar5 ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e01ab8 end
        -- TODO(native): xStack_24 = xStack_18;
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,(int)&xStack_c);
    end
    pCVar9 = native_arg_comment_to_make
    if native_arg_comment_to_make == nil then
        bVar2 = false
        if bVar2 then
            goto LAB_00e01ba2
        end
    else
        iVar5 = ((native_arg_comment_to_make == "ROCK_TROLL_CLOSE") and 0 or 1)
        cVar3 = not (iVar5 ~= 0)
        native_arg_comment_to_make = CONCAT31(native_arg_comment_to_make._1_3_,cVar3)
        if cVar3 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e01ab8 end
    pCVar4 = quest:GetThingWithScriptName("RockTrollTrigger")
    r4 = quest:PlayCriteriaSoundOnThing(pCVar4, "EARTH_TROLL_OFFSCREEN_ROAR")
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    iVar6 = quest:AddNewConversation(r3, false, false)
    pCVar4 = quest:GetHero()
    quest:AddPersonToConversation(iVar6, pCVar4)
    iVar5 = (r2 ~= nil and r2:IsAlive())
    if not iVar5 then
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:AddPersonToConversation(iVar6, r2)
        iVar5 = (r3 ~= nil and r3:IsAlive())
        if not iVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                pCVar7 = quest:GetHero()
                pCVar4 = xStack_30
                uVar14 = false
                piVar13 = "_"
                pCVar8 = xStack_30:GetDataString()
                pCVar8 = ("TEXT_QST_067_" .. pCVar8)
                pCVar8 = (pCVar8 .. "_")
                pCVar9 = (pCVar8 .. pCVar9)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r2
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_NO"
                pCVar9 = r2:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar4 = quest:GetHero()
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = xStack_30
                pCVar9 = pCVar7:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar7, pCVar4, uVar14)
                -- LAB_00e01ff9_c2: (native jump target)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:AddPersonToConversation(iVar6, r3)
                pCVar7 = quest:GetHero()
                pCVar4 = xStack_30
                uVar14 = false
                piVar13 = "_"
                pCVar8 = xStack_30:GetDataString()
                pCVar8 = ("TEXT_QST_067_" .. pCVar8)
                pCVar8 = (pCVar8 .. "_")
                pCVar9 = (pCVar8 .. pCVar9)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r2
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_NO"
                pCVar9 = r2:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r3
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_OATH"
                pCVar9 = r3:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = r2
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_BAD_NEWS"
                pCVar9 = r2:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar4 = quest:GetHero()
                -- LAB_00e01fb0: (native jump target)
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = xStack_30
                pCVar9 = pCVar7:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar7, pCVar4, uVar14)
                -- LAB_00e01ff9: (native jump target)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
                return true
            end
        end
        ::FLOW_after_lab_00e01fb0::
    end
    ::FLOW_past_lab_00e01afb::
    ::LAB_00e01ab8::
    ::LAB_00e01aca::
    return false
end

return {MakeTraderComment = MakeTraderComment}
