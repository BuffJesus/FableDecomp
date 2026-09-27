-- Readable native conversion: V_BookCollecting. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local scratchValue
    quest:SetStateBool("ReadingBook", false)
    quest:SetStateBool("DoneIntro", false)
    quest:SetStateBool("HatRewarded", false)
    quest:SetStateBool("KeyRewarded", false)
    quest:SetStateInt("GossipState", 0)
    quest:SetStateBool("HasInformation", false)
    quest:SetStateInt("BooksDonated", 0)
    quest:SetStateInt("GoodBooksDonated", 0)
    quest:SetStateInt("LastBookRequested", 0xffffffff)
    quest:SetStateInt("BooksInGame", quest:ReadGlobalGameData(1260))
    -- TODO(native): p1 = CCharString::CCharString(xStack_4);
    -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)(this + 0x94),iVar1,p1);
    -- TODO(native): std::vector<unsigned_char,std::allocator<unsigned_char>_>::resize((vector<unsigned_char,std::allocator<unsigned_char>_> *)(this + 0xa0), *(int *)(this + 0x90),(int)&local_5);
    -- TODO(native): std::vector<unsigned_char,std::allocator<unsigned_char>_>::resize((vector<unsigned_char,std::allocator<unsigned_char>_> *)(this + 0xac), *(int *)(this + 0x90),(int)&local_5);
    if 0 >= quest:GetStateInt("BooksInGame") then return end
    scratchValue = 0
    repeat
        -- TODO(native): CCharString::operator= ((CCharString *)(*(int *)(this + 0x94) + iVar1 * 4), quest:ReadGlobalGameDataStringAt(0x4d4, iVar1));
        -- TODO(native): *(undefined1 *)(iVar1 + *(int *)(this + 0xac)) = 0;
        -- TODO(native): *(undefined1 *)(iVar1 + *(int *)(this + 0xa0)) = 0;
        scratchValue = scratchValue + 1
    until scratchValue >= quest:GetStateInt("BooksInGame")
end

-- V_BookCollecting.OnPersist (retail 0x00e54880)
function OnPersist(quest, context)
    quest:SetStateBool("DoneIntro", quest:PersistTransferBool(context, "DoneIntro", quest:GetStateBool("DoneIntro")))
    quest:SetStateBool("HatRewarded", quest:PersistTransferBool(context, "HatRewarded", quest:GetStateBool("HatRewarded")))
    quest:SetStateBool("KeyRewarded", quest:PersistTransferBool(context, "KeyRewarded", quest:GetStateBool("KeyRewarded")))
    quest:SetStateInt("GossipState", quest:PersistTransferInt(context, "GossipState", quest:GetStateInt("GossipState") or 0))
    quest:SetStateBool("HasInformation", quest:PersistTransferBool(context, "HasInformation", quest:GetStateBool("HasInformation")))
    quest:SetStateInt("BooksDonated", quest:PersistTransferInt(context, "BooksDonated", quest:GetStateInt("BooksDonated") or 0))
    quest:SetStateInt("GoodBooksDonated", quest:PersistTransferInt(context, "GoodBooksDonated", quest:GetStateInt("GoodBooksDonated") or 0))
    quest:SetStateInt("LastBookRequested", quest:PersistTransferInt(context, "LastBookRequested", quest:GetStateInt("LastBookRequested") or 0))
    -- TODO(native): quest:PersistTransfer(context, "BookDonated", ...)  -- vector<bool,std::allocator<bool>_> member `BookDonated` (callee 0xcdcf80); binding missing
    quest:SetStateBool("BookOwned", quest:PersistTransferBool(context, "BookOwned", quest:GetStateBool("BookOwned")))
end

-- V_BookCollecting.BookReaction (retail 0x00e566f0)
function BookReaction(quest, param1)
    local resources = quest:RetailResources()
    local readingBook
    local index = param1
    local scratchValue = quest:GlobalConversations(1224)[param1 + 1].Lines
    if scratchValue == 0 then
        return
    end
    local boy0 = quest:GetThingWithScriptName("boy0")
    local girl0 = quest:GetThingWithScriptName("girl0")
    local getThingWithScriptName = quest:GetThingWithScriptName(quest:ReadGlobalGameDataStringAt(1200, index))
    local getThingWithScriptName2 = quest:GetThingWithScriptName(quest:ReadGlobalGameDataStringAt(1212, index))
    local this_00 = resources:MemberResource("seh_Boy")
    resources:TryAcquire(this_00, boy0, 4)
    quest:EntityTeleportToThing(boy0, getThingWithScriptName, false)
    quest:SetIsPushableByHero(boy0, false)
    resources:ClearAllActionsIncludingLoopingAnimations(this_00)
    resources:ClearCommands(this_00)
    local this_01 = resources:MemberResource("seh_Girl")
    resources:TryAcquire(this_01, girl0, 4)
    quest:EntityTeleportToThing(girl0, getThingWithScriptName2, false)
    quest:SetIsPushableByHero(girl0, false)
    resources:ClearAllActionsIncludingLoopingAnimations(this_01)
    resources:ClearCommands(this_01)
    readingBook = quest:GetStateBool("ReadingBook")
    param1 = 0
    while not readingBook and param1 < scratchValue do
        if not quest:NewScriptFrame() then return end
        DoConversation(quest, index, param1)
        readingBook = quest:GetStateBool("ReadingBook")
        param1 = param1 + 1
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetIsPushableByHero(boy0, true)
    quest:SetIsPushableByHero(girl0, true)
    resources:ClearAllActionsIncludingLoopingAnimations(this_00)
    resources:ClearCommands(this_00)
    resources:PrepareResource(this_00)
    resources:ClearAllActionsIncludingLoopingAnimations(this_01)
    resources:ClearCommands(this_01)
    resources:PrepareResource(this_01)
end

-- V_BookCollecting.DoConversation (retail 0x00e569d0)
-- E569D0: bsim names this body NScript::CV_BookCollectingScript::DoConversation (a homologous script member); no PDB name
function DoConversation(quest, param1, param)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local conversationId, scratchValue, memberResource
    local scratchValue5 = quest:GlobalConversations(1224)[param1 + 1]["Speaker"][param + 1]
    local scratchValue6 = quest:GlobalConversations(1224)[param1 + 1]["Dialogue"][param + 1]
    local scratchValue7 = quest:GlobalConversations(1224)[param1 + 1]["Animation"][param + 1]
    param = quest:GlobalConversations(1224)[param1 + 1]["AnimLoop"][param + 1]
    local getThingWithScriptName = quest:GetThingWithScriptName(scratchValue5)
    scratchValue = -1
    conversationId = scratchValue6 ~= "NULL" and 1 or 0
    if conversationId ~= 0 then
        if quest:IsActiveThreadTerminating() then return end
        local conversationId2 = quest:AddNewConversation(getThingWithScriptName, false, false)
        scratchValue = conversationId2
        quest:AddPersonToConversation(conversationId2, hero)
        quest:AddLineToConversation(conversationId2, scratchValue6, getThingWithScriptName, hero, false)
    end
    memberResource = 0
    if scratchValue5 == nil then
        if false then
            goto LAB_00e56b54
        else
            goto LAB_00e56c30
            goto LAB_00e56b84
        end
        goto FLOW_hoist_lab_00e56b84_1
    else
        conversationId = scratchValue5 == "boy0" and 0 or 1
        if conversationId == 0 then goto LAB_00e56b54 end
        conversationId = scratchValue
        if scratchValue5 == "girl0" then goto LAB_00e56b84 end
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
    if memberResource ~= nil then
        if quest:IsActiveThreadTerminating() then return end
        if param ~= 0 then
            conversationId = param == "NULL" and 0 or 1
            if conversationId == 0 then goto LAB_00e56c84 end
        end
        goto FLOW_past_lab_00e56c84
        ::LAB_00e56c84::
        resources:PlayAnimation(memberResource, scratchValue7, false, true, false, true, true, false, false)
        goto LAB_00e56c30
        ::FLOW_past_lab_00e56c84::
        resources:PlayAnimation(memberResource, scratchValue7, false, true, false, false, true, false, false)
        resources:PlayLoopingAnimation(memberResource, param, -1, false, false, true, false, true, false, false)
    end
    ::FLOW_past_lab_00e56b54::
    ::LAB_00e56c30::
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

-- V_BookCollecting.AddGossip (retail 0x00e55c60)
-- E55C60: bsim names this body NScript::CV_BookCollectingScript::AddGossip (a homologous script member); no PDB name
function AddGossip(quest, strParam1, strParam2, strParam3, strParam4)
    quest:AddRumourCategory(strParam1)
    quest:AddNewRumourToCategory(strParam1, strParam2)
    quest:AddGossipVillage(strParam1, strParam3)
    quest:AddGossipFactionToCategory(strParam1, strParam4)
end

