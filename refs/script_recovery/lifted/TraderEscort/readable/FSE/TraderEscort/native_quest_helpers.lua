-- Generated from the same native helper bodies as the quest draft.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_TraderCommentDelay = 3568,  -- 5
}
local MakeTraderComment
function MakeTraderComment(quest, me, commentToMake, speaker, commentType)
    local scratchValue, scratchValue2, scratchValue3, isActiveThreadTerminating, scratchValue4
    local scratchValue5, p0, getDataString, getDataString2, scratchValue6, scratchValue7, hero2
    local conversationId, darkwoodTrader, darkwoodTrader2, darkwoodTrader3, scratchValue8, this_00
    local scratchValue9, getHero, conversationId2, scratchValue10
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    -- TODO(native): local_2c = *(CCharString *)(native_arg_speaker + 0x4);
    -- TODO(native): local_28 = *(CCharString *)(native_arg_speaker + 0x8);
    local scratchValue11 = nil
    if local_28 ~= nil then
        -- TODO(native): *(int *)local_28 = *(int *)local_28 + 1;
    end
    -- TODO(native): cVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)()
    scratchValue4 = nil --[[unresolved native value]]
    if scratchValue4 == 0 then
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return false
        end
        scratchValue5 = quest:GetNearestWithScriptName(hero, "DarkwoodTrader")
        -- TODO(native): CVar8 = *(iVar4 + 8)
        scratchValue2 = nil --[[unresolved native value]]
        -- TODO(native): CVar1 = *(iVar4 + 4)
        scratchValue = nil --[[unresolved native value]]
        if local_28 ~= scratchValue2 then
            if scratchValue2 ~= nil then
                -- TODO(native): *(int *)CVar8 = *(int *)CVar8 + 1;
            end
        end
    end
    -- TODO(native): cVar2 = (*xStack_24[0x4b])()
    scratchValue4 = nil --[[unresolved native value]]
    if scratchValue4 == 0 then
        return false
    end
    if commentType ~= 2 then
        scratchValue5 = quest:GetTimer(commentTimer)
        if 0 < scratchValue5 then
            return false
        end
        conversationId = quest:AddNewConversation(nil --[[missing]], false, nil --[[missing]])
        local hero7 = hero
        quest:AddPersonToConversation(nil --[[missing]], hero7)
        if xStack_34 ~= nil then
            -- TODO(native): (**(code **)(*(int *)xStack_34 + 0xc))();
        end
        local hero8 = hero
        scratchValue7 = "_"
        getDataString2 = "TEXT_QST_067_" .. scratchValue11
        getDataString2 = getDataString2 .. scratchValue7
        (getDataString2 .. scratchValue2)
        quest:AddLineToConversation(nil --[[missing]], scratchValue7, hero8, nil --[[missing]])
        if commentType ~= 1 then goto LAB_00e02355 end
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then goto LAB_00e01aca end
        darkwoodTrader = quest:GetNearestWithScriptName(nil --[[missing]], "DarkwoodTrader")
        -- TODO(native): iVar4 = &xStack_18:IsAlive()
        scratchValue5 = nil --[[unresolved native value]]
        scratchValue3 = not scratchValue5
        if not scratchValue3 then
            scratchValue4 = xStack_c:IsEqualTo(nil --[[missing]])
            scratchValue3 = scratchValue4
        end
        if scratchValue3 then
            goto LAB_00e02213
        else
            scratchValue6 = "_RESPONSE"
            scratchValue7 = "_"
            -- TODO(native): pCVar7 = &xStack_18:GetDataString()
            getDataString2 = nil --[[unresolved native value]]
            getDataString2 = "TEXT_QST_067_" .. getDataString2
            getDataString2 = getDataString2 .. scratchValue7
            getDataString2 = getDataString2 .. scratchValue2
            (getDataString2 .. scratchValue6)
            scratchValue4 = quest:TextEntryExists()
            speaker = CONCAT31(speaker._1_3_,1)
            if not scratchValue4 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        scratchValue2 = 31
        if 31 & 16 ~= 0 then
            scratchValue2 = 31 & 0xffffffef
        end
        if scratchValue2 & 8 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffff7
        end
        if scratchValue2 & 4 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffb
        end
        if scratchValue2 & 2 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffd
        end
        if speaker ~= 0 then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return false
            end
            quest:AddPersonToConversation(31, darkwoodTrader)
            local hero9 = hero
            scratchValue6 = "_RESPONSE"
            scratchValue7 = "_"
            getDataString2 = 31:GetDataString()
            getDataString2 = "TEXT_QST_067_" .. getDataString2
            getDataString2 = getDataString2 .. scratchValue7
            getDataString2 = getDataString2 .. xStack_3c
            (getDataString2 .. scratchValue6)
            quest:AddLineToConversation(nil --[[missing]], scratchValue7, hero9, nil --[[missing]])
        end
        ::LAB_00e02355::
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        return true
    end
    ::FLOW_after_lab_00e02355::
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then goto LAB_00e01aca end
    darkwoodTrader2 = quest:GetNearestWithScriptName(nil --[[missing]], "DarkwoodTrader")
    darkwoodTrader3 = quest:GetFurthestWithScriptName(darkwoodTrader2, "DarkwoodTrader")
    scratchValue4 = xStack_c:IsEqualTo(nil --[[missing]])
    if not scratchValue4 then
        goto LAB_00e01afb
    else
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if not isActiveThreadTerminating then
            -- TODO(native): iVar4 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_18);
            -- TODO(native): CScriptThing::operator=((CScriptThing *)&xStack_c,iVar4);
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    scratchValue6 = "SCARED"
    this_00 = xStack_c:GetDataString()
    scratchValue5 = this_00 ~= scratchValue6 and 1 or 0
    if scratchValue5 ~= 0 then
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then goto LAB_00e01ab8 end
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_c,(int)&xStack_18);
        scratchValue10 = scratchValue11
    end
    getDataString2 = commentToMake
    if commentToMake == nil then
        isActiveThreadTerminating = false
        if isActiveThreadTerminating then
            goto LAB_00e01ba2
        end
    else
        scratchValue5 = commentToMake == "ROCK_TROLL_CLOSE" and 0 or 1
        scratchValue4 = scratchValue5 == 0
        commentToMake = CONCAT31(commentToMake._1_3_,scratchValue4)
        if scratchValue4 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then goto LAB_00e01ab8 end
    p0 = quest:GetThingWithScriptName("RockTrollTrigger")
    scratchValue8 = quest:PlayCriteriaSoundOnThing(p0, "EARTH_TROLL_OFFSCREEN_ROAR")
    quest:Pause(nil --[[missing]])
    ::FLOW_past_lab_00e01ba2::
    conversationId2 = quest:AddNewConversation(darkwoodTrader3, nil --[[missing]], nil --[[missing]])
    hero2 = hero
    quest:AddPersonToConversation(nil --[[missing]], hero2)
    scratchValue5 = xStack_c:IsAlive()
    if not scratchValue5 then
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if not isActiveThreadTerminating then
        quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
        -- TODO(native): iVar4 = &xStack_40:IsAlive()
        scratchValue5 = nil --[[unresolved native value]]
        if not scratchValue5 then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if not isActiveThreadTerminating then
                local hero3 = hero
                scratchValue7 = "_"
                getDataString = scratchValue11:GetDataString()
                getDataString = "TEXT_QST_067_" .. getDataString
                getDataString = getDataString .. scratchValue7
                (getDataString .. getDataString2)
                quest:AddLineToConversation(nil --[[missing]], scratchValue7, hero3, nil --[[missing]])
                local hero4 = hero
                scratchValue6 = "_INTRO_RESPONSE_NO"
                -- TODO(native): pCVar7 = &xStack_38:GetDataString()
                getDataString2 = nil --[[unresolved native value]]
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                (getDataString2 .. scratchValue6)
                quest:AddLineToConversation(nil --[[missing]], scratchValue6, hero4, nil --[[missing]])
                getHero = hero
                scratchValue9 = 0
                scratchValue6 = "_INTRO_RESPONSE_REWARD"
                getDataString2 = this_01:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. scratchValue6
                quest:AddLineToConversation(conversationId2, getDataString2, getHero, nil --[[missing]], scratchValue9 ~= 0)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        else
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if not isActiveThreadTerminating then
                quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
                local hero5 = hero
                scratchValue7 = "_"
                -- TODO(native): pCVar6 = &xStack_38:GetDataString()
                getDataString = nil --[[unresolved native value]]
                getDataString = "TEXT_QST_067_" .. getDataString
                getDataString = getDataString .. scratchValue7
                (getDataString .. getDataString2)
                quest:AddLineToConversation(nil --[[missing]], scratchValue7, hero5, nil --[[missing]])
                local hero6 = hero
                scratchValue6 = "_INTRO_RESPONSE_NO"
                -- TODO(native): pCVar7 = &xStack_34:GetDataString()
                getDataString2 = nil --[[unresolved native value]]
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                (getDataString2 .. scratchValue6)
                quest:AddLineToConversation(nil --[[missing]], scratchValue6, hero6, nil --[[missing]])
                local hero10 = hero
                getHero = 0
                scratchValue6 = "_INTRO_RESPONSE_OATH"
                -- TODO(native): pCVar7 = &stack0xffffffb8:GetDataString()
                getDataString2 = nil --[[unresolved native value]]
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. scratchValue6
                quest:AddLineToConversation(conversationId2, getDataString2, hero10, nil --[[missing]], getHero ~= 0)
                getHero = hero
                scratchValue9 = 0
                scratchValue6 = "_INTRO_RESPONSE_BAD_NEWS"
                -- TODO(native): pCVar7 = &stack0xffffff98:GetDataString()
                getDataString2 = nil --[[unresolved native value]]
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. scratchValue6
                quest:AddLineToConversation(conversationId2, getDataString2, getHero, nil --[[missing]], scratchValue9 ~= 0)
                getHero = hero
                scratchValue9 = 0
                scratchValue6 = "_INTRO_RESPONSE_REWARD"
                getDataString2 = this_01:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. scratchValue6
                quest:AddLineToConversation(conversationId2, getDataString2, getHero, nil --[[missing]], scratchValue9 ~= 0)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
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
