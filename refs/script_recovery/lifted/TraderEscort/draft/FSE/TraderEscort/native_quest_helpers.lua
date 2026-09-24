-- Generated from the same native helper bodies as the quest draft.
local MakeTraderComment
function MakeTraderComment(quest, me, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    local CVar1, CVar8, __native_condition_1, bVar3, cVar2, iVar4, p0, pCVar6, pCVar7, pcVar14, piVar13, r1, r10, r11, r12, r13, r14, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar10, uVar12, uVar5, u_stk_84, xStack_18, xStack_24
    local alive = true
    -- TODO(native): local_2c = *(CCharString *)(native_arg_speaker + 0x4);
    -- TODO(native): local_28 = *(CCharString *)(native_arg_speaker + 0x8);
    xStack_24 = nil
    if local_28 ~= nil then
        -- TODO(native): *(int *)local_28 = *(int *)local_28 + 1;
    end
    -- TODO(native): cVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)()
    cVar2 = nil --[[unresolved native value]]
    if cVar2 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return false
        end
        r1 = quest:GetHero()
        iVar4 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
        -- TODO(native): CVar8 = *(iVar4 + 8)
        CVar8 = nil --[[unresolved native value]]
        -- TODO(native): CVar1 = *(iVar4 + 4)
        CVar1 = nil --[[unresolved native value]]
        if local_28 ~= CVar8 then
            if CVar8 ~= nil then
                -- TODO(native): *(int *)CVar8 = *(int *)CVar8 + 1;
            end
        end
    end
    -- TODO(native): cVar2 = (*xStack_24[0x4b])()
    cVar2 = nil --[[unresolved native value]]
    if cVar2 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        return false
    end
    if native_arg_comment_type ~= 2 then
        iVar4 = quest:GetTimer(quest:GetStateInt("CommentTimer"))
        if 0 < iVar4 then
            alive = not quest:IsActiveThreadTerminating()
            return false
        end
        r2 = quest:AddNewConversation(nil --[[missing]], false, nil --[[missing]])
        r3 = quest:GetHero()
        quest:AddPersonToConversation(nil --[[missing]], r3)
        if xStack_34 == nil then
            -- TODO(native): CCharString::CCharString((CCharString *)xStack_24,(CCharString *)&DAT_0143e8ec);
        else
            -- TODO(native): (**(code **)(*(int *)xStack_34 + 0xc))();
        end
        r4 = quest:GetHero()
        piVar13 = "_"
        pCVar7 = ("TEXT_QST_067_" .. xStack_24)
        pCVar7 = (pCVar7 .. piVar13)
        (pCVar7 .. CVar8)
        quest:AddLineToConversation(nil --[[missing]], piVar13, r4, nil --[[missing]])
        if native_arg_comment_type ~= 1 then goto LAB_00e02355 end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e01aca end
        r5 = quest:GetNearestWithScriptName(nil --[[missing]], "DarkwoodTrader")
        -- TODO(native): iVar4 = &xStack_18:IsAlive()
        iVar4 = nil --[[unresolved native value]]
        __native_condition_1 = not iVar4
        if not __native_condition_1 then
            cVar2 = xStack_c:IsEqualTo(nil --[[missing]])
            __native_condition_1 = cVar2
        end
        if __native_condition_1 then
            goto LAB_00e02213
        else
            pcVar14 = "_RESPONSE"
            piVar13 = "_"
            -- TODO(native): pCVar7 = &xStack_18:GetDataString()
            pCVar7 = nil --[[unresolved native value]]
            pCVar7 = ("TEXT_QST_067_" .. pCVar7)
            pCVar7 = (pCVar7 .. piVar13)
            pCVar7 = (pCVar7 .. CVar8)
            (pCVar7 .. pcVar14)
            cVar2 = quest:TextEntryExists()
            native_arg_speaker = CONCAT31(native_arg_speaker._1_3_,1)
            if not cVar2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        CVar8 = 31
        if (31 & 0x10) ~= 0 then
            CVar8 = 31 & 0xffffffef
        end
        if (CVar8 & 8) ~= 0 then
            CVar8 = CVar8 & 0xfffffff7
        end
        if (CVar8 & 4) ~= 0 then
            CVar8 = CVar8 & 0xfffffffb
        end
        if (CVar8 & 2) ~= 0 then
            CVar8 = CVar8 & 0xfffffffd
        end
        if native_arg_speaker ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return false
            end
            quest:AddPersonToConversation(31, r5)
            r6 = quest:GetHero()
            pcVar14 = "_RESPONSE"
            piVar13 = "_"
            pCVar7 = 31:GetDataString()
            pCVar7 = ("TEXT_QST_067_" .. pCVar7)
            pCVar7 = (pCVar7 .. piVar13)
            pCVar7 = (pCVar7 .. xStack_3c)
            (pCVar7 .. pcVar14)
            quest:AddLineToConversation(nil --[[missing]], piVar13, r6, nil --[[missing]])
        end
        ::LAB_00e02355::
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        return true
    end
    ::FLOW_after_lab_00e02355::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e01aca end
    r7 = quest:GetNearestWithScriptName(nil --[[missing]], "DarkwoodTrader")
    r8 = quest:GetFurthestWithScriptName(r7, "DarkwoodTrader")
    cVar2 = xStack_c:IsEqualTo(nil --[[missing]])
    if not cVar2 then
        goto LAB_00e01afb
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            -- TODO(native): iVar4 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_18);
            -- TODO(native): CScriptThing::operator=((CScriptThing *)&xStack_c,iVar4);
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    pcVar14 = "SCARED"
    this_00 = xStack_c:GetDataString()
    iVar4 = ((this_00 ~= pcVar14) and 1 or 0)
    if iVar4 ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e01ab8 end
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_c,(int)&xStack_18);
        xStack_18 = xStack_24
    end
    pCVar7 = native_arg_comment_to_make
    if native_arg_comment_to_make == nil then
        bVar3 = false
        if bVar3 then
            goto LAB_00e01ba2
        end
    else
        iVar4 = ((native_arg_comment_to_make == "ROCK_TROLL_CLOSE") and 0 or 1)
        cVar2 = not (iVar4 ~= 0)
        native_arg_comment_to_make = CONCAT31(native_arg_comment_to_make._1_3_,cVar2)
        if cVar2 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e01ab8 end
    p0 = quest:GetThingWithScriptName("RockTrollTrigger")
    r9 = quest:PlayCriteriaSoundOnThing(p0, "EARTH_TROLL_OFFSCREEN_ROAR")
    quest:Pause(nil --[[missing]])
    ::FLOW_past_lab_00e01ba2::
    uVar5 = quest:AddNewConversation(r8, nil --[[missing]], nil --[[missing]])
    r10 = quest:GetHero()
    quest:AddPersonToConversation(nil --[[missing]], r10)
    iVar4 = xStack_c:IsAlive()
    if not iVar4 then
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
        -- TODO(native): iVar4 = &xStack_40:IsAlive()
        iVar4 = nil --[[unresolved native value]]
        if not iVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                r11 = quest:GetHero()
                piVar13 = "_"
                pCVar6 = xStack_24:GetDataString()
                pCVar6 = ("TEXT_QST_067_" .. pCVar6)
                pCVar6 = (pCVar6 .. piVar13)
                (pCVar6 .. pCVar7)
                quest:AddLineToConversation(nil --[[missing]], piVar13, r11, nil --[[missing]])
                r12 = quest:GetHero()
                pcVar14 = "_INTRO_RESPONSE_NO"
                -- TODO(native): pCVar7 = &xStack_38:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                (pCVar7 .. pcVar14)
                quest:AddLineToConversation(nil --[[missing]], pcVar14, r12, nil --[[missing]])
                uVar12 = quest:GetHero()
                uVar10 = 0
                pcVar14 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = this_01:GetDataString()
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, uVar12, nil --[[missing]], (uVar10 ~= 0))
                -- LAB_00e01ff9_c2: (native jump target)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
                r13 = quest:GetHero()
                piVar13 = "_"
                -- TODO(native): pCVar6 = &xStack_38:GetDataString()
                pCVar6 = nil --[[unresolved native value]]
                pCVar6 = ("TEXT_QST_067_" .. pCVar6)
                pCVar6 = (pCVar6 .. piVar13)
                (pCVar6 .. pCVar7)
                quest:AddLineToConversation(nil --[[missing]], piVar13, r13, nil --[[missing]])
                r14 = quest:GetHero()
                pcVar14 = "_INTRO_RESPONSE_NO"
                -- TODO(native): pCVar7 = &xStack_34:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                (pCVar7 .. pcVar14)
                quest:AddLineToConversation(nil --[[missing]], pcVar14, r14, nil --[[missing]])
                u_stk_84 = quest:GetHero()
                uVar12 = 0
                pcVar14 = "_INTRO_RESPONSE_OATH"
                -- TODO(native): pCVar7 = &stack0xffffffb8:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, u_stk_84, nil --[[missing]], (uVar12 ~= 0))
                uVar12 = quest:GetHero()
                uVar10 = 0
                pcVar14 = "_INTRO_RESPONSE_BAD_NEWS"
                -- TODO(native): pCVar7 = &stack0xffffff98:GetDataString()
                pCVar7 = nil --[[unresolved native value]]
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, uVar12, nil --[[missing]], (uVar10 ~= 0))
                uVar12 = quest:GetHero()
                -- LAB_00e01fb0: (native jump target)
                uVar10 = 0
                pcVar14 = "_INTRO_RESPONSE_REWARD"
                pCVar7 = this_01:GetDataString()
                pCVar7 = ("TEXT_QST_067_" .. pCVar7)
                pCVar7 = (pCVar7 .. pcVar14)
                quest:AddLineToConversation(uVar5, pCVar7, uVar12, nil --[[missing]], (uVar10 ~= 0))
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
