-- Generated from the same native helper bodies as the quest draft.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_TraderCommentDelay = 3568,  -- 5
}
local MakeTraderComment
function MakeTraderComment(quest, me, commentToMake, speaker, commentType)
    local scratchValue, scratchValue2, scratchValue3, getTimer, conversationId, getDataString
    local scratchValue4, getHero, getDataString2, getDataString3, scratchValue5, scratchValue6
    local scratchValue7, darkwoodTrader, darkwoodTrader2, darkwoodTrader3, scratchValue8, this_00
    local scratchValue9, scratchValue10
    local commentTimer = quest:GetStateInt("CommentTimer")
    local hero = quest:GetHero()
    -- TODO(native): xStack_30._4_4_ = *(undefined4 *)(native_arg_speaker + 0x4);
    -- TODO(native): xStack_30 = *(int **)(native_arg_speaker + 0x8);
    local scratchValue11 = nil
    if scratchValue11 ~= nil then
        -- TODO(native): *xStack_30 = *xStack_30 + 1;
    end
    -- TODO(native): bVar2 = (*PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)(xStack_30)
    scratchValue2 = nil --[[unresolved native value]]
    if not scratchValue2 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            return false
        end
        scratchValue4 = hero
        scratchValue4 = quest:GetNearestWithScriptName(scratchValue4, "DarkwoodTrader")
        -- TODO(native): piVar13 = *(pCVar4 + 0x8)
        scratchValue7 = nil --[[unresolved native value]]
        -- TODO(native): piVar1 = *(pCVar4 + 0x4)
        scratchValue6 = nil --[[unresolved native value]]
        if scratchValue11 ~= scratchValue7 then
            -- TODO(native): xStack_30._4_4_ = piVar1;
            if scratchValue7 ~= nil then
                -- TODO(native): *piVar13 = *piVar13 + 1;
            end
        end
    end
    -- TODO(native): bVar2 = (**(xStack_30._0_4_ + 0x12c))(xStack_30)
    scratchValue2 = nil --[[unresolved native value]]
    if not scratchValue2 then
        return false
    end
    if commentType ~= 2 then
        getTimer = quest:GetTimer(commentTimer)
        if 0 < getTimer then
            return false
        end
        conversationId = quest:AddNewConversation(scratchValue11, false, false)
        scratchValue4 = hero
        quest:AddPersonToConversation(conversationId, scratchValue4)
        if not (scratchValue11 ~= nil and not scratchValue11:IsNull()) then
            speaker = ""
        else
            scratchValue11:GetDataString()
        end
        getHero = hero
        getDataString3 = commentToMake
        scratchValue4 = scratchValue11
        scratchValue10 = 0
        scratchValue7 = "_"
        getDataString2 = commentToMake
        getDataString = "TEXT_QST_067_" .. speaker
        getDataString = getDataString .. "_"
        getDataString2 = getDataString .. getDataString2
        quest:AddLineToConversation(conversationId, getDataString2, scratchValue4, getHero, scratchValue10 ~= 0)
        if commentType ~= 1 then goto LAB_00e02355 end
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then goto LAB_00e01aca end
        darkwoodTrader = quest:GetNearestWithScriptName(scratchValue11, "DarkwoodTrader")
        getTimer = darkwoodTrader ~= nil and darkwoodTrader:IsAlive()
        scratchValue = not getTimer
        if not scratchValue then
            scratchValue3 = darkwoodTrader ~= nil and darkwoodTrader:IsEqualTo(scratchValue11._4_4_)
            scratchValue = scratchValue3
        end
        if scratchValue then
            goto LAB_00e02213
        else
            scratchValue5 = "_RESPONSE"
            scratchValue7 = "_"
            getDataString2 = getDataString3
            getDataString = darkwoodTrader:GetDataString()
            getDataString = "TEXT_QST_067_" .. getDataString
            getDataString = getDataString .. "_"
            getDataString2 = getDataString .. getDataString2
            getDataString2 = getDataString2 .. scratchValue5
            scratchValue2 = quest:TextEntryExists(getDataString2)
            speaker = CONCAT31(speaker._1_3_,1)
            if not scratchValue2 then goto LAB_00e02213 end
        end
        goto FLOW_past_lab_00e02213
        ::LAB_00e02213::
        ::FLOW_past_lab_00e02213::
        scratchValue9 = 31
        if 31 & 16 ~= 0 then
            scratchValue9 = 31 & 0xffffffef
        end
        if scratchValue9 & 8 ~= 0 then
            scratchValue9 = scratchValue9 & 0xfffffff7
        end
        if scratchValue9 & 4 ~= 0 then
            scratchValue9 = scratchValue9 & 0xfffffffb
        end
        if scratchValue9 & 2 ~= 0 then
            scratchValue9 = scratchValue9 & 0xfffffffd
        end
        if speaker ~= 0 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                return false
            end
            quest:AddPersonToConversation(conversationId, darkwoodTrader)
            getHero = hero
            scratchValue4 = darkwoodTrader
            scratchValue10 = 0
            scratchValue5 = "_RESPONSE"
            scratchValue7 = "_"
            getDataString2 = darkwoodTrader:GetDataString()
            getDataString2 = "TEXT_QST_067_" .. getDataString2
            getDataString2 = getDataString2 .. "_"
            getDataString3 = getDataString2 .. getDataString3
            getDataString3 = getDataString3 .. scratchValue5
            quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
        end
        ::LAB_00e02355::
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        return true
    end
    ::FLOW_after_lab_00e02355::
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then goto LAB_00e01aca end
    darkwoodTrader2 = quest:GetNearestWithScriptName(scratchValue11, "DarkwoodTrader")
    darkwoodTrader3 = quest:GetFurthestWithScriptName(scratchValue11, "DarkwoodTrader")
    scratchValue3 = darkwoodTrader2 ~= nil and darkwoodTrader2:IsEqualTo(darkwoodTrader3._4_4_)
    if not scratchValue3 then
        goto LAB_00e01afb
    else
        scratchValue2 = quest:IsActiveThreadTerminating()
        if not scratchValue2 then
            -- TODO(native): iVar5 = CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&xStack_c);
            -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,iVar5);
            goto LAB_00e01afb
        end
    end
    goto FLOW_past_lab_00e01afb
    ::LAB_00e01afb::
    scratchValue5 = "SCARED"
    this_00 = darkwoodTrader2:GetDataString()
    getTimer = this_00 ~= scratchValue5 and 1 or 0
    if getTimer ~= 0 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then goto LAB_00e01ab8 end
        -- TODO(native): xStack_24 = xStack_18;
        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_18,(int)&xStack_c);
    end
    getDataString3 = commentToMake
    if commentToMake == nil then
        scratchValue2 = false
        if scratchValue2 then
            goto LAB_00e01ba2
        end
    else
        getTimer = commentToMake == "ROCK_TROLL_CLOSE" and 0 or 1
        scratchValue3 = getTimer == 0
        commentToMake = CONCAT31(commentToMake._1_3_,scratchValue3)
        if scratchValue3 then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then goto LAB_00e01ab8 end
    scratchValue4 = quest:GetThingWithScriptName("EARTH_TROLL_OFFSCREEN_ROAR")
    scratchValue8 = quest:PlayCriteriaSoundOnThing(scratchValue4, getDataString2)
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    conversationId = quest:AddNewConversation(darkwoodTrader3, false, false)
    scratchValue4 = hero
    quest:AddPersonToConversation(conversationId, scratchValue4)
    getTimer = darkwoodTrader2 ~= nil and darkwoodTrader2:IsAlive()
    if not getTimer then
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        do return true end
        goto FLOW_after_lab_00e02355
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if not scratchValue2 then
        quest:AddPersonToConversation(conversationId, darkwoodTrader2)
        getTimer = darkwoodTrader3 ~= nil and darkwoodTrader3:IsAlive()
        if not getTimer then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                getHero = hero
                scratchValue4 = scratchValue11
                scratchValue10 = 0
                scratchValue7 = "_"
                getDataString2 = scratchValue11:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
                getHero = hero
                scratchValue4 = darkwoodTrader2
                scratchValue10 = 0
                scratchValue5 = "_INTRO_RESPONSE_NO"
                getDataString3 = darkwoodTrader2:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue5
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
                scratchValue4 = hero
                scratchValue10 = 0
                scratchValue5 = "_INTRO_RESPONSE_REWARD"
                getHero = scratchValue11
                getDataString3 = getHero:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue5
                quest:AddLineToConversation(conversationId, getDataString3, getHero, scratchValue4, scratchValue10 ~= 0)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                do return true end
                goto FLOW_after_lab_00e01fb0
            end
        else
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                quest:AddPersonToConversation(conversationId, darkwoodTrader3)
                getHero = hero
                scratchValue4 = scratchValue11
                scratchValue10 = 0
                scratchValue7 = "_"
                getDataString2 = scratchValue11:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
                getHero = hero
                scratchValue4 = darkwoodTrader2
                scratchValue10 = 0
                scratchValue5 = "_INTRO_RESPONSE_NO"
                getDataString3 = darkwoodTrader2:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue5
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
                getHero = hero
                scratchValue4 = darkwoodTrader3
                scratchValue10 = 0
                scratchValue5 = "_INTRO_RESPONSE_OATH"
                getDataString3 = darkwoodTrader3:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue5
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
                getHero = hero
                scratchValue4 = darkwoodTrader2
                scratchValue10 = 0
                scratchValue5 = "_INTRO_RESPONSE_BAD_NEWS"
                getDataString3 = darkwoodTrader2:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue5
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue4, getHero, scratchValue10 ~= 0)
                scratchValue4 = hero
                scratchValue10 = 0
                scratchValue5 = "_INTRO_RESPONSE_REWARD"
                getHero = scratchValue11
                getDataString3 = getHero:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue5
                quest:AddLineToConversation(conversationId, getDataString3, getHero, scratchValue4, scratchValue10 ~= 0)
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
