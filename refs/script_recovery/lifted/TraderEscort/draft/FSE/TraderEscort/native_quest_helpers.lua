-- Generated from the same native helper bodies as the quest draft.
local MakeTraderComment
function MakeTraderComment(quest, me, native_arg_comment_to_make, native_arg_speaker, native_arg_comment_type)
    return quest:WithRetailResources(function(resources)
    local speakerOwner, nearestOwner, furthestOwner, fallbackOwner, swapOwner, triggerOwner = 0, 0, 0, 0, 0, 0
    local __native_condition_1, bVar2, cVar3, iVar5, iVar6, local_furthest_handle, local_nearest_handle, native_conversation_speaker, native_reply_available, native_rock_troll_match, native_swap_handle, pCVar10, pCVar4, pCVar7, pCVar8, pCVar9, pcVar15, r1, this_00, uVar11, uVar14, xStack_30
    local alive = true
    native_conversation_speaker = native_arg_speaker
    xStack_30 = resources:CopyThingOutput(native_arg_speaker)
    speakerOwner = resources:TrackThingOutput(xStack_30)
    bVar2 = (xStack_30 ~= nil and xStack_30:IsAlive())
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return false
        end
        pCVar4 = quest:GetHero()
        pCVar4 = quest:GetNearestWithScriptName(pCVar4, "DarkwoodTrader")
        fallbackOwner = resources:TrackThingOutput(pCVar4)
        resources:AssignThingOutput(speakerOwner, pCVar4)
        resources:RetireThingOutput(fallbackOwner)
    end
    bVar2 = (xStack_30 ~= nil and xStack_30:IsAlive())
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
            native_arg_speaker = xStack_30:GetDataString()
        end
        pCVar7 = quest:GetHero()
        pCVar9 = native_arg_comment_to_make
        pCVar4 = xStack_30
        uVar14 = false
        pCVar8 = native_arg_comment_to_make
        pCVar10 = ("TEXT_QST_067_" .. native_arg_speaker)
        pCVar10 = (pCVar10 .. "_")
        pCVar8 = (pCVar10 .. pCVar8)
        quest:AddLineToConversation(iVar6, pCVar8, pCVar4, pCVar7, uVar14)
        if native_arg_comment_type ~= 1 then goto LAB_00e02355 end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e01aca end
        native_arg_speaker = "DarkwoodTrader"
        local_furthest_handle = quest:GetNearestWithScriptName(xStack_30, native_arg_speaker)
        if local_furthest_handle == nil then local_furthest_handle = resources:CopyThingOutput(nil) end
        furthestOwner = resources:TrackThingOutput(local_furthest_handle)
        iVar5 = (local_furthest_handle ~= nil and local_furthest_handle:IsAlive())
        __native_condition_1 = not iVar5
        if not __native_condition_1 then
            cVar3 = local_furthest_handle:RetailImplementationIsEqualTo(xStack_30)
            __native_condition_1 = cVar3
        end
        if __native_condition_1 then
            goto LAB_00e02213
        else
            pcVar15 = "_RESPONSE"
            pCVar8 = pCVar9
            pCVar10 = local_furthest_handle:GetDataString()
            pCVar10 = ("TEXT_QST_067_" .. pCVar10)
            pCVar10 = (pCVar10 .. "_")
            pCVar8 = (pCVar10 .. pCVar8)
            pCVar8 = (pCVar8 .. pcVar15)
            bVar2 = quest:TextEntryExists(pCVar8)
            native_reply_available = 1
            if not bVar2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        native_reply_available = 0
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
        if native_reply_available ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return false
            end
            quest:AddPersonToConversation(iVar6, local_furthest_handle)
            pCVar7 = quest:GetHero()
            pCVar4 = local_furthest_handle
            uVar14 = false
            pcVar15 = "_RESPONSE"
            pCVar8 = local_furthest_handle:GetDataString()
            pCVar8 = ("TEXT_QST_067_" .. pCVar8)
            pCVar8 = (pCVar8 .. "_")
            pCVar9 = (pCVar8 .. pCVar9)
            pCVar9 = (pCVar9 .. pcVar15)
            quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
        end
        ::LAB_00e02355::
        resources:RetireThingOutput(furthestOwner)
        furthestOwner = 0
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        return true
    end
    ::FLOW_after_lab_00e02355::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e01aca end
    native_arg_comment_type = "DarkwoodTrader"
    local_nearest_handle = quest:GetNearestWithScriptName(xStack_30, native_arg_comment_type)
    if local_nearest_handle == nil then local_nearest_handle = resources:CopyThingOutput(nil) end
    nearestOwner = resources:TrackThingOutput(local_nearest_handle)
    native_arg_comment_type = "DarkwoodTrader"
    local_furthest_handle = quest:GetFurthestWithScriptName(xStack_30, native_arg_comment_type)
    if local_furthest_handle == nil then local_furthest_handle = resources:CopyThingOutput(nil) end
    furthestOwner = resources:TrackThingOutput(local_furthest_handle)
    cVar3 = local_nearest_handle:RetailImplementationIsEqualTo(local_furthest_handle)
    if not cVar3 then
        goto LAB_00e01afb
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            resources:AssignThingOutput(furthestOwner, nil)
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    pcVar15 = "SCARED"
    this_00 = local_nearest_handle:GetDataString()
    iVar5 = ((this_00 ~= pcVar15) and 1 or 0)
    if iVar5 ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e01ab8 end
        native_swap_handle = resources:CopyThingOutput(local_nearest_handle)
        swapOwner = resources:TrackThingOutput(native_swap_handle)
        resources:AssignThingOutput(nearestOwner, local_furthest_handle)
        resources:AssignThingOutput(furthestOwner, native_swap_handle)
        resources:RetireThingOutput(swapOwner)
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
        native_rock_troll_match = cVar3
        if cVar3 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e01ab8 end
    native_arg_comment_type = "EARTH_TROLL_OFFSCREEN_ROAR"
    native_arg_comment_to_make = "RockTrollTrigger"
    pCVar4 = quest:GetThingWithScriptName(native_arg_comment_to_make)
    triggerOwner = resources:TrackThingOutput(pCVar4)
    r1 = quest:PlayCriteriaSoundOnThing(pCVar4, native_arg_comment_type)
    resources:RetireThingOutput(triggerOwner)
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    iVar6 = quest:AddNewConversation(native_conversation_speaker, false, false)
    pCVar4 = quest:GetHero()
    quest:AddPersonToConversation(iVar6, pCVar4)
    iVar5 = (local_nearest_handle ~= nil and local_nearest_handle:IsAlive())
    if not iVar5 then
        quest:SetTimer(quest:GetStateInt("CommentTimer"), quest:ReadGlobalGameData(0xdf0))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:AddPersonToConversation(iVar6, local_nearest_handle)
        iVar5 = (local_furthest_handle ~= nil and local_furthest_handle:IsAlive())
        if not iVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                pCVar7 = quest:GetHero()
                pCVar4 = xStack_30
                uVar14 = false
                pCVar8 = xStack_30:GetDataString()
                pCVar8 = ("TEXT_QST_067_" .. pCVar8)
                pCVar8 = (pCVar8 .. "_")
                pCVar9 = (pCVar8 .. pCVar9)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = local_nearest_handle
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_NO"
                pCVar9 = local_nearest_handle:GetDataString()
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
                quest:AddPersonToConversation(iVar6, local_furthest_handle)
                pCVar7 = quest:GetHero()
                pCVar4 = xStack_30
                uVar14 = false
                pCVar8 = xStack_30:GetDataString()
                pCVar8 = ("TEXT_QST_067_" .. pCVar8)
                pCVar8 = (pCVar8 .. "_")
                pCVar9 = (pCVar8 .. pCVar9)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = local_nearest_handle
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_NO"
                pCVar9 = local_nearest_handle:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = local_furthest_handle
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_OATH"
                pCVar9 = local_furthest_handle:GetDataString()
                pCVar9 = ("TEXT_QST_067_" .. pCVar9)
                pCVar9 = (pCVar9 .. pcVar15)
                quest:AddLineToConversation(iVar6, pCVar9, pCVar4, pCVar7, uVar14)
                pCVar7 = quest:GetHero()
                pCVar4 = local_nearest_handle
                uVar14 = false
                pcVar15 = "_INTRO_RESPONSE_BAD_NEWS"
                pCVar9 = local_nearest_handle:GetDataString()
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
    end)
end

return {MakeTraderComment = MakeTraderComment}
