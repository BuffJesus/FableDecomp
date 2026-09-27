-- Generated native draft: V_BookCollecting. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local alive = true
    local bVar1 = quest:IsRegionLoaded("BowerstoneSlums")
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:AddEntityBinding("BS_Teacher", "V_BookCollecting/Entities/BS_Teacher")
                quest:FinalizeEntityBindings()
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsRegionLoaded("BowerstoneSlums")
    end
end

function Init(quest)
    local iVar1
    quest:SetStateBool("ReadingBook", false)
    quest:SetStateBool("DoneIntro", false)
    quest:SetStateBool("HatRewarded", false)
    quest:SetStateBool("KeyRewarded", false)
    quest:SetStateInt("GossipState", 0)
    quest:SetStateBool("HasInformation", false)
    quest:SetStateInt("BooksDonated", 0)
    quest:SetStateInt("GoodBooksDonated", 0)
    quest:SetStateInt("LastBookRequested", 0xffffffff)
    iVar1 = quest:ReadGlobalGameData(0x4ec)
    quest:SetStateInt("BooksInGame", iVar1)
    -- TODO(native): p1 = CCharString::CCharString(xStack_4);
    -- TODO(native): CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)(this + 0x94),iVar1,p1);
    -- TODO(native): std::vector<unsigned_char,std::allocator<unsigned_char>_>::resize((vector<unsigned_char,std::allocator<unsigned_char>_> *)(this + 0xa0), *(int *)(this + 0x90),(int)&local_5);
    -- TODO(native): std::vector<unsigned_char,std::allocator<unsigned_char>_>::resize((vector<unsigned_char,std::allocator<unsigned_char>_> *)(this + 0xac), *(int *)(this + 0x90),(int)&local_5);
    if 0 < quest:GetStateInt("BooksInGame") then
        iVar1 = 0
        repeat
            -- TODO(native): CCharString::operator= ((CCharString *)(*(int *)(this + 0x94) + iVar1 * 4), (CCharString *)(quest:ReadGlobalGameData(0x4d4) + iVar1 * 4));
            -- TODO(native): *(undefined1 *)(iVar1 + *(int *)(this + 0xac)) = 0;
            -- TODO(native): *(undefined1 *)(iVar1 + *(int *)(this + 0xa0)) = 0;
            iVar1 = iVar1 + 1
        until not (iVar1 < quest:GetStateInt("BooksInGame"))
    end
end

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
    -- TODO(native): quest:PersistTransfer(context, "BookDonated", ...)  -- vector<bool,std::allocator<bool>_> member `BookDonated` (callee 0xcdcf80); binding missing
    local bookOwned = quest:GetStateBool("BookOwned") or false
    bookOwned = quest:PersistTransferBool(context, "BookOwned", bookOwned)
    quest:SetStateBool("BookOwned", bookOwned)
end

function null(quest, native_arg_param_1)
    local resources = quest:RetailResources()
    local CVar1, bVar4, iVar2, lVar3, r1, r2, r3, r4, thing, thing_00, thing_01, thing_02, this_00, this_01
    local alive = true
    lVar3 = native_arg_param_1
    iVar2 = quest:GlobalConversations(0x4c8)[(native_arg_param_1) + 1].Lines
    if iVar2 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        return
    end
    r1 = quest:GetThingWithScriptName("boy0")
    r2 = quest:GetThingWithScriptName("girl0")
    r3 = quest:GetThingWithScriptName(nil --[[missing]])
    r4 = quest:GetThingWithScriptName(nil --[[missing]])
    this_00 = resources:MemberResource("seh_Boy")
    resources:TryAcquire(this_00, r1, 4)
    quest:EntityTeleportToThing(r1, r3, false)
    if r1._8_4_ ~= nil then
        -- TODO(native): *xStack_24._8_4_ = *xStack_24._8_4_ + 1;
    end
    -- TODO(native): thing._4_4_ = xStack_24._4_4_;
    thing = nil
    -- TODO(native): thing._8_4_ = xStack_24._8_4_;
    quest:SetIsPushableByHero(thing, false)
    -- TODO(native): (**(code **)(*(int *)this_00 + 0x58))();
    -- TODO(native): (**(code **)(*(int *)this_00 + 0x28))();
    this_01 = resources:MemberResource("seh_Girl")
    resources:TryAcquire(this_01, r2, 4)
    quest:EntityTeleportToThing(r2, r4, false)
    if r2._8_4_ ~= nil then
        -- TODO(native): *xStack_30._8_4_ = *xStack_30._8_4_ + 1;
    end
    -- TODO(native): thing_00._4_4_ = xStack_30._4_4_;
    thing_00 = nil
    -- TODO(native): thing_00._8_4_ = xStack_30._8_4_;
    quest:SetIsPushableByHero(thing_00, false)
    -- TODO(native): (**(code **)(*(int *)this_01 + 0x58))();
    -- TODO(native): (**(code **)(*(int *)this_01 + 0x28))();
    CVar1 = quest:GetStateBool("ReadingBook")
    native_arg_param_1 = 0
    while (not CVar1 and (native_arg_param_1 < iVar2)) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e56993 end
        helper_E569D0(quest, lVar3, native_arg_param_1)
        CVar1 = quest:GetStateBool("ReadingBook")
        native_arg_param_1 = native_arg_param_1 + 1
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        if r1._8_4_ ~= nil then
            -- TODO(native): *xStack_24._8_4_ = *xStack_24._8_4_ + 1;
        end
        -- TODO(native): thing_01._4_4_ = xStack_24._4_4_;
        thing_01 = nil
        -- TODO(native): thing_01._8_4_ = xStack_24._8_4_;
        quest:SetIsPushableByHero(thing_01, true)
        if r2._8_4_ ~= nil then
            -- TODO(native): *xStack_30._8_4_ = *xStack_30._8_4_ + 1;
        end
        -- TODO(native): thing_02._4_4_ = xStack_30._4_4_;
        thing_02 = nil
        -- TODO(native): thing_02._8_4_ = xStack_30._8_4_;
        quest:SetIsPushableByHero(thing_02, true)
        -- TODO(native): (**(code **)(*(int *)this_00 + 0x58))();
        -- TODO(native): (**(code **)(*(int *)this_00 + 0x28))();
        resources:PrepareResource(this_00)
        -- TODO(native): (**(code **)(*(int *)this_01 + 0x58))(&xStack_28);
        -- TODO(native): (**(code **)(*(int *)this_01 + 0x28))();
        resources:PrepareResource(this_01)
    end
    ::LAB_00e56993::
end

function helper_E569D0(quest, native_arg_param_1, native_arg_param_2)
    local resources = quest:RetailResources()
    local bVar2, cVar3, iVar6, iVar8, i_stk_10, native_arg_sequence_1, p0, pCVar4, pCVar5, r1, xStack_14, xStack_18, xStack_1c
    local alive = true
    iVar6 = native_arg_param_1 * 0x5c
    iVar8 = native_arg_param_2 * 4
    xStack_14 = quest:GlobalConversations(0x4c8)[(native_arg_param_1) + 1]["Speaker"][(native_arg_param_2) + 1]
    xStack_18 = quest:GlobalConversations(0x4c8)[(native_arg_param_1) + 1]["Dialogue"][(native_arg_param_2) + 1]
    xStack_1c = quest:GlobalConversations(0x4c8)[(native_arg_param_1) + 1]["Animation"][(native_arg_param_2) + 1]
    native_arg_param_2 = quest:GlobalConversations(0x4c8)[(native_arg_param_1) + 1]["AnimLoop"][(native_arg_param_2) + 1]
    r1 = quest:GetThingWithScriptName(nil --[[missing]])
    i_stk_10 = -1
    iVar6 = ((xStack_18 ~= 0x12393e4) and 1 or 0)
    if iVar6 ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        iVar8 = quest:AddNewConversation(r1, false, false)
        i_stk_10 = iVar8
        pCVar4 = quest:GetHero()
        quest:AddPersonToConversation(iVar8, pCVar4)
        pCVar4 = quest:GetHero()
        quest:AddLineToConversation(iVar8, nil --[[missing]], r1, pCVar4, (xStack_18 ~= 0))
    end
    pCVar5 = 0x0
    if xStack_14 == nil then
        bVar2 = false
        if bVar2 then
            goto LAB_00e56b54
        else
            bVar2 = false
            if not bVar2 then goto LAB_00e56c30 end
            goto LAB_00e56b84
        end
        goto FLOW_hoist_lab_00e56b84_1
    else
        -- TODO(native): p0 = *xStack_14
        p0 = nil --[[unresolved native value]]
        -- TODO(native): iVar6 = CBasicString<char>::Compare(p0,"boy0");
        if iVar6 == 0 then goto LAB_00e56b54 end
        -- TODO(native): iVar8 = CBasicString<char>::Compare(p0,"girl0");
        iVar6 = i_stk_10
        if iVar8 == 0 then goto LAB_00e56b84 end
    end
    goto FLOW_past_lab_00e56b84
    ::LAB_00e56b84::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e56cd0 end
    pCVar5 = resources:MemberResource("seh_Girl")
    ::FLOW_hoist_lab_00e56b84_1::
    goto FLOW_hoist_lab_00e56b54_1
    ::FLOW_past_lab_00e56b84::
    goto FLOW_past_lab_00e56b54
    ::LAB_00e56b54::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e56cd0 end
    pCVar5 = resources:MemberResource("seh_Boy")
    ::FLOW_hoist_lab_00e56b54_1::
    if pCVar5 ~= nil then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e56cd0 end
        if native_arg_param_2 == 0 then
            bVar2 = false
            if bVar2 then
                goto LAB_00e56c84
            end
        else
            -- TODO(native): iVar6 = CBasicString<char>::Compare(*(void **)native_arg_param_2,"NULL");
            if iVar6 == 0 then goto LAB_00e56c84 end
        end
        goto FLOW_past_lab_00e56c84
        ::LAB_00e56c84::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e56cd0 end
        -- TODO(native): (**(code **)(*(int *)pCVar5 + 0x48))(xStack_1c,0,1,0,1,true,0,0);
        goto LAB_00e56c30
        ::FLOW_past_lab_00e56c84::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e56cd0 end
        -- TODO(native): (**(code **)(*(int *)pCVar5 + 0x48))(xStack_1c,0,1,0,0,true,0,0);
        -- TODO(native): (**(code **)(*(int *)pCVar5 + 0x50))(&xStack_18,0xffffffff,0,0,1,0,true,0,0);
    end
    ::FLOW_past_lab_00e56b54::
    ::LAB_00e56c30::
    repeat
        bVar2 = quest:IsConversationActive(nil --[[missing]])
        native_arg_sequence_1 = false
        if not bVar2 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            -- TODO(native): cVar3 = (**(*pCVar5 + 0x68))()
            cVar3 = nil --[[unresolved native value]]
            if not cVar3 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if not native_arg_sequence_1 then
            if quest:GetStateBool("ReadingBook") then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:RemoveConversation(false)
            end
            break
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until not (not bVar2)
    ::LAB_00e56cd0::
end

function helper_E55C60(quest, native_arg_strParam_1)
    quest:AddRumourCategory(native_arg_strParam_1)
    quest:AddNewRumourToCategory(native_arg_strParam_1, nil --[[missing]])
    quest:AddGossipVillage(native_arg_strParam_1, nil --[[missing]])
    quest:AddGossipFactionToCategory(native_arg_strParam_1, nil --[[missing]])
end

