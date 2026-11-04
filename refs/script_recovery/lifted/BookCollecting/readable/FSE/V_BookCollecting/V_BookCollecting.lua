-- Readable native conversion: V_BookCollecting. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BooksNumberTotal = 1260,  -- 40
}

-- V_BookCollecting.Main (retail 0x00e54450)
function Main(quest)
    local isRegionLoaded = quest:IsRegionLoaded("BowerstoneSlums")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:AddEntityBinding("BS_Teacher", "V_BookCollecting/Entities/BS_Teacher")
            quest:FinalizeEntityBindings()
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("BowerstoneSlums")
    end
end

-- V_BookCollecting.Init (retail 0x00e54990)
function Init(quest)
    local index
    quest:SetStateBool("ReadingBook", false)
    quest:SnapshotGlobalConversations("BookReactions", 1224)
    quest:SetStateBool("DoneIntro", false)
    quest:SetStateBool("HatRewarded", false)
    quest:SetStateBool("KeyRewarded", false)
    quest:SetStateInt("GossipState", 0)
    quest:SetStateBool("HasInformation", false)
    quest:SetStateInt("BooksDonated", 0)
    quest:SetStateInt("GoodBooksDonated", 0)
    quest:SetStateInt("LastBookRequested", 0xffffffff)
    local scratchValue = quest:ReadGlobalGameData(SCRIPT_DEF.BooksNumberTotal)
    quest:SetStateInt("BooksInGame", scratchValue)
    quest:StateStringVectorResize("BookDef", scratchValue)
    quest:StateFlagVectorResize("BookDonated", quest:GetStateInt("BooksInGame"))
    quest:StateFlagVectorResize("BookOwned", quest:GetStateInt("BooksInGame"))
    if not (0 < quest:GetStateInt("BooksInGame")) then return end
    index = 0
    repeat
        quest:SetStateStringVectorAt("BookDef", index, quest:ReadGlobalGameDataStringAt(1236, index))
        quest:SetStateFlagVectorAt("BookOwned", index, false)
        quest:SetStateFlagVectorAt("BookDonated", index, false)
        index = index + 1
    until not (index < quest:GetStateInt("BooksInGame"))
end

-- V_BookCollecting.OnPersist (retail 0x00e54880)
function OnPersist(quest, context)
    local doneIntro = quest:GetStateBool("DoneIntro") or false
    doneIntro = quest:PersistTransferBool(context, "DoneIntro", doneIntro)
    quest:SetStateBool("DoneIntro", doneIntro)
    local hatRewarded = quest:GetStateBool("HatRewarded") or false
    hatRewarded = quest:PersistTransferBool(context, "HatRewarded", hatRewarded)
    quest:SetStateBool("HatRewarded", hatRewarded)
    local keyRewarded = quest:GetStateBool("KeyRewarded") or false
    keyRewarded = quest:PersistTransferBool(context, "KeyRewarded", keyRewarded)
    quest:SetStateBool("KeyRewarded", keyRewarded)
    local gossipState = quest:GetStateInt("GossipState") or 0
    gossipState = quest:PersistTransferInt(context, "GossipState", gossipState)
    quest:SetStateInt("GossipState", gossipState)
    local hasInformation = quest:GetStateBool("HasInformation") or false
    hasInformation = quest:PersistTransferBool(context, "HasInformation", hasInformation)
    quest:SetStateBool("HasInformation", hasInformation)
    local booksDonated = quest:GetStateInt("BooksDonated") or 0
    booksDonated = quest:PersistTransferInt(context, "BooksDonated", booksDonated)
    quest:SetStateInt("BooksDonated", booksDonated)
    local goodBooksDonated = quest:GetStateInt("GoodBooksDonated") or 0
    goodBooksDonated = quest:PersistTransferInt(context, "GoodBooksDonated", goodBooksDonated)
    quest:SetStateInt("GoodBooksDonated", goodBooksDonated)
    local lastBookRequested = quest:GetStateInt("LastBookRequested") or 0
    lastBookRequested = quest:PersistTransferInt(context, "LastBookRequested", lastBookRequested)
    quest:SetStateInt("LastBookRequested", lastBookRequested)
    quest:PersistStateFlagVector(context, "BookDonated")
    quest:PersistStateFlagVector(context, "BookOwned")
end

-- V_BookCollecting.BookReaction (retail 0x00e566f0)
function BookReaction(quest, param1)
    local resources = quest:RetailResources()
    local readingBook
    local index = param1
    local getStateConversations = quest:GetStateConversations("BookReactions")[param1 + 1].Lines
    if getStateConversations == 0 then
        quest:IsActiveThreadTerminating()
        return
    end
    param1 = "boy0"
    local getThingWithScriptName = quest:GetThingWithScriptName(param1)
    param1 = "girl0"
    local getThingWithScriptName2 = quest:GetThingWithScriptName(param1)
    local getThingWithScriptName3 = quest:GetThingWithScriptName(quest:ReadGlobalGameDataStringAt(1200, index))
    local getThingWithScriptName4 = quest:GetThingWithScriptName(quest:ReadGlobalGameDataStringAt(1212, index))
    local this_00 = resources:MemberResource("seh_Boy")
    resources:TryAcquire(this_00, getThingWithScriptName, 4)
    quest:EntityTeleportToThing(getThingWithScriptName, getThingWithScriptName3, false)
    quest:SetIsPushableByHero(getThingWithScriptName, false)
    resources:ClearAllActionsIncludingLoopingAnimations(this_00)
    resources:ClearCommands(this_00)
    local this_01 = resources:MemberResource("seh_Girl")
    resources:TryAcquire(this_01, getThingWithScriptName2, 4)
    quest:EntityTeleportToThing(getThingWithScriptName2, getThingWithScriptName4, false)
    quest:SetIsPushableByHero(getThingWithScriptName2, false)
    resources:ClearAllActionsIncludingLoopingAnimations(this_01)
    resources:ClearCommands(this_01)
    readingBook = quest:GetStateBool("ReadingBook")
    param1 = 0
    while not readingBook and param1 < getStateConversations do
        if not quest:NewScriptFrame() then return end
        DoConversation(quest, index, param1)
        readingBook = quest:GetStateBool("ReadingBook")
        param1 = param1 + 1
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetIsPushableByHero(getThingWithScriptName, true)
    quest:SetIsPushableByHero(getThingWithScriptName2, true)
    resources:ClearAllActionsIncludingLoopingAnimations(this_00)
    resources:ClearCommands(this_00)
    resources:PrepareResource(this_00)
    resources:ClearAllActionsIncludingLoopingAnimations(this_01)
    resources:ClearCommands(this_01)
    resources:PrepareResource(this_01)
end

-- V_BookCollecting.AddGossip (retail 0x00e55c60)
-- E55C60: bsim names this body NScript::CV_BookCollectingScript::AddGossip (a homologous script member); no PDB name
function AddGossip(quest, strParam1, strParam2, strParam3, strParam4)
    quest:AddRumourCategory(strParam1)
    quest:AddNewRumourToCategory(strParam1, strParam2)
    quest:AddGossipVillage(strParam1, strParam3)
    quest:AddGossipFactionToCategory(strParam1, strParam4)
end

-- V_BookCollecting.DoConversation (retail 0x00e569d0)
-- E569D0: bsim names this body NScript::CV_BookCollectingScript::DoConversation (a homologous script member); no PDB name
function DoConversation(quest, param1, param)
    local resources = quest:RetailResources()
    local scratchValue, conversationId, scratchValue5, scratchValue7, memberResource
    scratchValue = param1 * 92
    scratchValue5 = param * 4
    local getStateConversations = quest:GetStateConversations("BookReactions")[param1 + 1]["Speaker"][param + 1]
    local getStateConversations2 = quest:GetStateConversations("BookReactions")[param1 + 1]["Dialogue"][param + 1]
    local getStateConversations3 = quest:GetStateConversations("BookReactions")[param1 + 1]["Animation"][param + 1]
    param = quest:GetStateConversations("BookReactions")[param1 + 1]["AnimLoop"][param + 1]
    local getThingWithScriptName = quest:GetThingWithScriptName(getStateConversations)
    scratchValue7 = -1
    if getStateConversations2 ~= "NULL" then
        if quest:IsActiveThreadTerminating() then return end
        local conversationId2 = quest:AddNewConversation(getThingWithScriptName, false, false)
        scratchValue7 = conversationId2
        local hero = quest:GetHero()
        quest:AddPersonToConversation(conversationId2, hero)
        local hero2 = quest:GetHero()
        quest:AddLineToConversation(conversationId2, getStateConversations2, getThingWithScriptName, hero2, false)
    end
    memberResource = 0
    if getStateConversations == nil then
        do goto FLOW_past_lab_00e56b54 end
        goto LAB_00e56b84
        goto FLOW_hoist_lab_00e56b84_1
    else
        if getStateConversations == "boy0" then goto LAB_00e56b54 end
        conversationId = scratchValue7
        if getStateConversations == "girl0" then goto LAB_00e56b84 end
    end
    goto FLOW_past_lab_00e56b84
    ::LAB_00e56b84::
    if quest:IsActiveThreadTerminating() then return end
    memberResource = resources:MemberResource("seh_Girl")
    ::FLOW_hoist_lab_00e56b84_1::
    goto FLOW_hoist_lab_00e56b54_1
    ::FLOW_past_lab_00e56b84::
    goto FLOW_past_lab_00e56b54
    ::LAB_00e56b54::
    if quest:IsActiveThreadTerminating() then return end
    memberResource = resources:MemberResource("seh_Boy")
    ::FLOW_hoist_lab_00e56b54_1::
    conversationId = scratchValue7
    if memberResource ~= nil then
        if quest:IsActiveThreadTerminating() then return end
        if param ~= 0 and param == "NULL" then
            if quest:IsActiveThreadTerminating() then return end
            resources:PlayAnimation(memberResource, getStateConversations3, false, true, false, true, true, false, false)
            conversationId = scratchValue7
            goto FLOW_past_lab_00e56b54
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:PlayAnimation(memberResource, getStateConversations3, false, true, false, false, true, false, false)
        resources:PlayLoopingAnimation(memberResource, param, -1, false, false, true, false, true, false, false)
        conversationId = scratchValue7
    end
    ::FLOW_past_lab_00e56b54::
    repeat
        local isConversationActive = quest:IsConversationActive(conversationId)
        if not (not isConversationActive and not resources:IsPerformingScriptTask(memberResource) or quest:GetStateBool("ReadingBook")) then
            quest:NewScriptFrame()
        else
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveConversation(conversationId, false)
            end
            break
            quest:NewScriptFrame()
        end
    until quest:IsActiveThreadTerminating()
end

