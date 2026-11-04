-- Generated from the same native helper bodies as the quest draft.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_TraderCommentDelay = 3568,  -- 5
}
local MakeTraderComment
function MakeTraderComment(quest, me, commentToMake, speaker, commentType)
    local commentTimer = quest:GetStateInt("CommentTimer")
    return quest:WithRetailResources(function(resources)
    local speakerOwner, nearestOwner, furthestOwner, fallbackOwner, swapOwner, triggerOwner = 0, 0, 0, 0, 0, 0
    local scratchValue, scratchValue2, retailImplementationIsEqualTo, getTimer, conversationId
    local local_furthest_handle, native_reply_available, native_rock_troll_match, getDataString
    local scratchValue3, getHero, getDataString2, getDataString3, scratchValue4, scratchValue5
    local predicateResult
    local native_conversation_speaker = speaker
    local scratchValue7 = resources:CopyThingOutput(speaker)
    speakerOwner = resources:TrackThingOutput(scratchValue7)
    scratchValue2 = scratchValue7 ~= nil and scratchValue7:IsAlive()
    if not scratchValue2 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            return false
        end
        scratchValue3 = quest:GetHero()
        scratchValue3 = quest:GetNearestWithScriptName(scratchValue3, "DarkwoodTrader")
        fallbackOwner = resources:TrackThingOutput(scratchValue3)
        resources:AssignThingOutput(speakerOwner, scratchValue3)
        resources:RetireThingOutput(fallbackOwner)
    end
    scratchValue2 = scratchValue7 ~= nil and scratchValue7:IsAlive()
    if not scratchValue2 then
        quest:IsActiveThreadTerminating()
        return false
    end
    if commentType ~= 2 then
        getTimer = quest:GetTimer(commentTimer)
        if 0 < getTimer then
            quest:IsActiveThreadTerminating()
            return false
        end
        conversationId = quest:AddNewConversation(scratchValue7, false, false)
        scratchValue3 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, scratchValue3)
        if not (scratchValue7 ~= nil and not scratchValue7:IsNull()) then
            speaker = ""
        else
            speaker = scratchValue7:GetDataString()
        end
        getHero = quest:GetHero()
        getDataString3 = commentToMake
        scratchValue3 = scratchValue7
        predicateResult = false
        getDataString2 = commentToMake
        getDataString = "TEXT_QST_067_" .. speaker
        getDataString = getDataString .. "_"
        getDataString2 = getDataString .. getDataString2
        quest:AddLineToConversation(conversationId, getDataString2, scratchValue3, getHero, predicateResult)
        if commentType == 1 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then return false end
            speaker = "DarkwoodTrader"
            local_furthest_handle = quest:GetNearestWithScriptName(scratchValue7, speaker)
            if local_furthest_handle == nil then local_furthest_handle = resources:CopyThingOutput(nil) end
            furthestOwner = resources:TrackThingOutput(local_furthest_handle)
            getTimer = local_furthest_handle ~= nil and local_furthest_handle:IsAlive()
            scratchValue = not getTimer
            if not scratchValue then
                retailImplementationIsEqualTo = local_furthest_handle:RetailImplementationIsEqualTo(scratchValue7)
                scratchValue = retailImplementationIsEqualTo
            end
            if scratchValue then
                native_reply_available = 0
            else
                scratchValue4 = "_RESPONSE"
                getDataString2 = getDataString3
                getDataString = local_furthest_handle:GetDataString()
                getDataString = "TEXT_QST_067_" .. getDataString
                getDataString = getDataString .. "_"
                getDataString2 = getDataString .. getDataString2
                getDataString2 = getDataString2 .. scratchValue4
                scratchValue2 = quest:TextEntryExists(getDataString2)
                native_reply_available = 1
                if not scratchValue2 then native_reply_available = 0 end
            end
            if native_reply_available ~= 0 then
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    return false
                end
                quest:AddPersonToConversation(conversationId, local_furthest_handle)
                getHero = quest:GetHero()
                scratchValue3 = local_furthest_handle
                predicateResult = false
                scratchValue4 = "_RESPONSE"
                getDataString2 = local_furthest_handle:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
            end
        end
        resources:RetireThingOutput(furthestOwner)
        furthestOwner = 0
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        return true
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then return false end
    commentType = "DarkwoodTrader"
    local local_nearest_handle = quest:GetNearestWithScriptName(scratchValue7, commentType)
    if local_nearest_handle == nil then local_nearest_handle = resources:CopyThingOutput(nil) end
    nearestOwner = resources:TrackThingOutput(local_nearest_handle)
    commentType = "DarkwoodTrader"
    local_furthest_handle = quest:GetFurthestWithScriptName(scratchValue7, commentType)
    if local_furthest_handle == nil then local_furthest_handle = resources:CopyThingOutput(nil) end
    furthestOwner = resources:TrackThingOutput(local_furthest_handle)
    retailImplementationIsEqualTo = local_nearest_handle:RetailImplementationIsEqualTo(local_furthest_handle)
    if retailImplementationIsEqualTo then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then return false end
        resources:AssignThingOutput(furthestOwner, nil)
    end
    scratchValue4 = "SCARED"
    local this_00 = local_nearest_handle:GetDataString()
    getTimer = this_00 ~= scratchValue4 and 1 or 0
    if getTimer ~= 0 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then return false end
        local native_swap_handle = resources:CopyThingOutput(local_nearest_handle)
        swapOwner = resources:TrackThingOutput(native_swap_handle)
        resources:AssignThingOutput(nearestOwner, local_furthest_handle)
        resources:AssignThingOutput(furthestOwner, native_swap_handle)
        resources:RetireThingOutput(swapOwner)
    end
    getDataString3 = commentToMake
    if commentToMake == nil then
        scratchValue2 = false
        if scratchValue2 then
            goto LAB_00e01ba2
        end
    else
        getTimer = commentToMake == "ROCK_TROLL_CLOSE" and 0 or 1
        retailImplementationIsEqualTo = getTimer == 0
        native_rock_troll_match = retailImplementationIsEqualTo
        if retailImplementationIsEqualTo then goto LAB_00e01ba2 end
    end
    goto FLOW_past_lab_00e01ba2
    ::LAB_00e01ba2::
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then return false end
    commentType = "EARTH_TROLL_OFFSCREEN_ROAR"
    commentToMake = "RockTrollTrigger"
    scratchValue3 = quest:GetThingWithScriptName(commentToMake)
    triggerOwner = resources:TrackThingOutput(scratchValue3)
    scratchValue5 = quest:PlayCriteriaSoundOnThing(scratchValue3, commentType)
    resources:RetireThingOutput(triggerOwner)
    quest:Pause(1.0)
    ::FLOW_past_lab_00e01ba2::
    conversationId = quest:AddNewConversation(native_conversation_speaker, false, false)
    scratchValue3 = quest:GetHero()
    quest:AddPersonToConversation(conversationId, scratchValue3)
    getTimer = local_nearest_handle ~= nil and local_nearest_handle:IsAlive()
    if not getTimer then
        quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
        do return true end
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if not scratchValue2 then
        quest:AddPersonToConversation(conversationId, local_nearest_handle)
        getTimer = local_furthest_handle ~= nil and local_furthest_handle:IsAlive()
        if not getTimer then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                getHero = quest:GetHero()
                scratchValue3 = scratchValue7
                predicateResult = false
                getDataString2 = scratchValue7:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_nearest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_NO"
                getDataString3 = local_nearest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                scratchValue3 = quest:GetHero()
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_REWARD"
                getHero = scratchValue7
                getDataString3 = getHero:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, getHero, scratchValue3, predicateResult)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                do return true end
            end
        else
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                quest:AddPersonToConversation(conversationId, local_furthest_handle)
                getHero = quest:GetHero()
                scratchValue3 = scratchValue7
                predicateResult = false
                getDataString2 = scratchValue7:GetDataString()
                getDataString2 = "TEXT_QST_067_" .. getDataString2
                getDataString2 = getDataString2 .. "_"
                getDataString3 = getDataString2 .. getDataString3
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_nearest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_NO"
                getDataString3 = local_nearest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_furthest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_OATH"
                getDataString3 = local_furthest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                getHero = quest:GetHero()
                scratchValue3 = local_nearest_handle
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_BAD_NEWS"
                getDataString3 = local_nearest_handle:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, scratchValue3, getHero, predicateResult)
                scratchValue3 = quest:GetHero()
                predicateResult = false
                scratchValue4 = "_INTRO_RESPONSE_REWARD"
                getHero = scratchValue7
                getDataString3 = getHero:GetDataString()
                getDataString3 = "TEXT_QST_067_" .. getDataString3
                getDataString3 = getDataString3 .. scratchValue4
                quest:AddLineToConversation(conversationId, getDataString3, getHero, scratchValue3, predicateResult)
                quest:SetTimer(commentTimer, quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDelay))
                return true
            end
        end
    end
    return false
    end)
end

return {MakeTraderComment = MakeTraderComment}
