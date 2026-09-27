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
    helper_E54AA0(quest, this + 76, DAT_0143e90c + 1224)
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
        -- TODO(native): CCharString::operator= ((CCharString *)(*(int *)(this + 0x94) + iVar1 * 4), (CCharString *)(quest:ReadGlobalGameData(0x4d4) + iVar1 * 4));
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

-- V_BookCollecting.null (retail 0x00e566f0)
function null(quest, param1)
    local resources = quest:RetailResources()
    local readingBook, thing, thing_00, thing_01, thing_02
    local scratchValue = param1
    -- TODO(native): iVar2 = *(native_arg_param_1 * 0x5c + 0x28 + quest:GetStateInt("self_0x4c"))
    --[[unresolved native value]]
    if nil == 0 then
        return
    end
    local boy0 = quest:GetThingWithScriptName("boy0")
    local girl0 = quest:GetThingWithScriptName("girl0")
    local getThingWithScriptName = quest:GetThingWithScriptName(nil --[[missing]])
    local getThingWithScriptName2 = quest:GetThingWithScriptName(nil --[[missing]])
    local this_00 = resources:MemberResource("seh_Boy")
    resources:TryAcquire(this_00, boy0, 4)
    quest:EntityTeleportToThing(boy0, getThingWithScriptName, false)
    if boy0._8_4_ ~= nil then
        -- TODO(native): *xStack_24._8_4_ = *xStack_24._8_4_ + 1;
    end
    -- TODO(native): thing._4_4_ = xStack_24._4_4_;
    -- TODO(native): thing._8_4_ = xStack_24._8_4_;
    quest:SetIsPushableByHero(nil, false)
    -- TODO(native): (**(code **)(*(int *)this_00 + 0x58))();
    -- TODO(native): (**(code **)(*(int *)this_00 + 0x28))();
    local this_01 = resources:MemberResource("seh_Girl")
    resources:TryAcquire(this_01, girl0, 4)
    quest:EntityTeleportToThing(girl0, getThingWithScriptName2, false)
    if girl0._8_4_ ~= nil then
        -- TODO(native): *xStack_30._8_4_ = *xStack_30._8_4_ + 1;
    end
    -- TODO(native): thing_00._4_4_ = xStack_30._4_4_;
    -- TODO(native): thing_00._8_4_ = xStack_30._8_4_;
    quest:SetIsPushableByHero(nil, false)
    -- TODO(native): (**(code **)(*(int *)this_01 + 0x58))();
    -- TODO(native): (**(code **)(*(int *)this_01 + 0x28))();
    readingBook = quest:GetStateBool("ReadingBook")
    param1 = 0
    while not readingBook and param1 < nil do
        if not quest:NewScriptFrame() then return end
        DoConversation(quest, scratchValue, param1)
        readingBook = quest:GetStateBool("ReadingBook")
        param1 = param1 + 1
    end
    if quest:IsActiveThreadTerminating() then return end
    if boy0._8_4_ ~= nil then
        -- TODO(native): *xStack_24._8_4_ = *xStack_24._8_4_ + 1;
    end
    -- TODO(native): thing_01._4_4_ = xStack_24._4_4_;
    -- TODO(native): thing_01._8_4_ = xStack_24._8_4_;
    quest:SetIsPushableByHero(nil, true)
    if girl0._8_4_ ~= nil then
        -- TODO(native): *xStack_30._8_4_ = *xStack_30._8_4_ + 1;
    end
    -- TODO(native): thing_02._4_4_ = xStack_30._4_4_;
    -- TODO(native): thing_02._8_4_ = xStack_30._8_4_;
    quest:SetIsPushableByHero(nil, true)
    -- TODO(native): (**(code **)(*(int *)this_00 + 0x58))();
    -- TODO(native): (**(code **)(*(int *)this_00 + 0x28))();
    resources:PrepareResource(this_00)
    -- TODO(native): (**(code **)(*(int *)this_01 + 0x58))(&xStack_28);
    -- TODO(native): (**(code **)(*(int *)this_01 + 0x28))();
    resources:PrepareResource(this_01)
end

-- V_BookCollecting.helper_E54AA0 (retail 0x00e54aa0)
function helper_E54AA0(quest, param2)
    local scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue7, scratchValue9
    if param2 == this then return end
    local scratchValue = param2[1]
    -- TODO(native): pCVar4 = *this
    scratchValue5 = nil --[[unresolved native value]]
    -- TODO(native): pCVar5 = *native_arg_param_2
    scratchValue7 = nil --[[unresolved native value]]
    local scratchValue10 = (scratchValue - scratchValue7) / 92
    -- TODO(native): if ((*(this + 8) - pCVar4) / 0x5c) < uVar1 then
    if false then
        helper_E54E40(quest, scratchValue10, scratchValue7, scratchValue)
        -- TODO(native): *(int *)this = iVar3;
        -- TODO(native): *(uint *)(this + 8) = uVar1 * 0x5c + iVar3;
    else
        -- TODO(native): uVar2 = (*(this + 4) - pCVar4) / 0x5c
    --[[unresolved native value]]
        if nil < scratchValue10 then
            scratchValue3 = (nil * 92) / 92
            if 0 < scratchValue3 then
                repeat
                    Copy(quest, scratchValue5, scratchValue7)
                    scratchValue7 = scratchValue7 + 92
                    scratchValue5 = scratchValue5 + 92
                    scratchValue3 = scratchValue3 - 1
                until scratchValue3 == 0
            end
            -- TODO(native): pCVar4 = *(this + 4)
            scratchValue6 = nil --[[unresolved native value]]
            local scratchValue8 = param2[1]
            scratchValue9 = ((scratchValue6 - *this) / 92) * 92 + *param2
            while scratchValue9 ~= scratchValue8 do
                if scratchValue6 ~= nil then
                    helper_E54CA0(quest, scratchValue6, scratchValue9)
                end
                scratchValue6 = scratchValue6 + 92
                scratchValue9 = scratchValue9 + 92
            end
        else
            scratchValue4 = (scratchValue - scratchValue7) / 92
            if 0 < scratchValue4 then
                repeat
                    Copy(quest, scratchValue5, scratchValue7)
                    scratchValue7 = scratchValue7 + 92
                    scratchValue5 = scratchValue5 + 92
                    scratchValue4 = scratchValue4 - 1
                until scratchValue4 == 0
            end
            -- TODO(native): std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>,std::allocator<CIntelligentPointer<NParticleEngine::CParticleEmitter>_>_> ::_Destroy(pCVar4,*(void **)(this + 4));
        end
    end
    -- TODO(native): *(uint *)(this + 4) = uVar1 * 0x5c + *(int *)this;
end

-- V_BookCollecting.helper_E54E40 (retail 0x00e54e40)
function helper_E54E40(quest, param2, param, param4)
    local scratchValue2
    if param2 == 0 then
        scratchValue2 = 0
    else
        scratchValue2 = malloc(param2 * 92)
    end
    if param ~= param4 then
        local scratchValue = scratchValue2 - param
        repeat
            if (param + scratchValue) ~= nil then
                helper_E54CA0(quest, param + scratchValue, param)
            end
            param = param + 92
        until param == param4
    end
    return scratchValue2
end

-- V_BookCollecting.helper_E54C10 (retail 0x00e54c10)
function helper_E54C10(quest, this)
    local scratchValue, scratchValue2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    scratchValue2 = nil --[[unresolved native value]]
    while scratchValue2 ~= scratchValue do
        -- TODO(native): (**(code **)*puVar2)(0);
        scratchValue2 = scratchValue2 + 23
    end
    -- TODO(native): if *native_arg_this ~= nil then
end

-- V_BookCollecting.Copy (retail 0x00e54c50)
-- E54C50: bsim names this body CConversation::Copy (a homologous script member); no PDB name
function Copy(quest, param1)
    -- TODO(native): CThingBuildingDef::operator=((CThingBuildingDef *)this,(int)native_arg_param_1);
    -- TODO(native): name field 0x28 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x28", *(native_arg_param_1 + 0x28))
    -- TODO(native): CCombatWheel__operator_(this + 0x2c,(int)(native_arg_param_1 + 0x2c));
    -- TODO(native): CCombatWheel__operator_(this + 0x38,(int)(native_arg_param_1 + 0x38));
    -- TODO(native): CCombatWheel__operator_(this + 0x44,(int)(native_arg_param_1 + 0x44));
    -- TODO(native): CCombatWheel__operator_(this + 0x50,(int)(native_arg_param_1 + 0x50));
end

-- V_BookCollecting.helper_E54CA0 (retail 0x00e54ca0)
function helper_E54CA0(quest, param1)
    -- TODO(native): CDefClassBase::CDefClassBase((CDefClassBase *)this,(int)native_arg_param_1);
    -- TODO(native): name field 0x28 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x28", *(native_arg_param_1 + 0x28))
    -- TODO(native): Vector_Copy(this + 0x2c,(int)(native_arg_param_1 + 0x2c));
    -- TODO(native): Vector_Copy(this + 0x38,(int)(native_arg_param_1 + 0x38));
    -- TODO(native): Vector_Copy(this + 0x44,(int)(native_arg_param_1 + 0x44));
    -- TODO(native): Vector_Copy(this + 0x50,(int)(native_arg_param_1 + 0x50));
end

-- V_BookCollecting.DoConversation (retail 0x00e569d0)
-- E569D0: bsim names this body NScript::CV_BookCollectingScript::DoConversation (a homologous script member); no PDB name
function DoConversation(quest, param1, param2)
    local scratchValue, addNewConversation, scratchValue3, sequence, p0, scratchValue4
    local scratchValue5, scratchValue6
    local hero = quest:GetHero()
    addNewConversation = param2 * 4
    -- TODO(native): CCharString::CCharString(&xStack_14,(CCharString *)(*(int *)(iVar6 + 0x2c + *(int *)(this + 0x4c)) + iVar8));
    -- TODO(native): CCharString::CCharString(&xStack_18,(CCharString *)(*(int *)(iVar6 + 0x38 + *(int *)(this + 0x4c)) + iVar8));
    -- TODO(native): CCharString::CCharString(xStack_1c,(CCharString *)(*(int *)(iVar6 + 0x44 + *(int *)(this + 0x4c)) + iVar8));
    -- TODO(native): CCharString::CCharString((CCharString *)&native_arg_param_2, (CCharString *)(*(int *)(iVar6 + 0x50 + *(int *)(this + 0x4c)) + iVar8));
    local getThingWithScriptName = quest:GetThingWithScriptName(nil --[[missing]])
    scratchValue3 = -1
    scratchValue = scratchValue6 ~= 0x12393e4 and 1 or 0
    if scratchValue ~= 0 then
        if quest:IsActiveThreadTerminating() then return end
        addNewConversation = quest:AddNewConversation(getThingWithScriptName, false, false)
        scratchValue3 = addNewConversation
        quest:AddPersonToConversation(addNewConversation, hero)
        quest:AddLineToConversation(addNewConversation, nil --[[missing]], getThingWithScriptName, hero, false)
    end
    if scratchValue5 == nil then
        if false then
            goto LAB_00e56b54
        else
            goto LAB_00e56c30
            goto LAB_00e56b84
        end
        goto FLOW_hoist_lab_00e56b84_1
    else
        -- TODO(native): p0 = *xStack_14
        p0 = nil --[[unresolved native value]]
        -- TODO(native): iVar6 = CBasicString<char>::Compare(p0,"boy0");
        if scratchValue == 0 then goto LAB_00e56b54 end
        -- TODO(native): iVar8 = CBasicString<char>::Compare(p0,"girl0");
        scratchValue = scratchValue3
        if addNewConversation == 0 then goto LAB_00e56b84 end
    end
    goto FLOW_past_lab_00e56b84
    ::LAB_00e56b84::
    if quest:IsActiveThreadTerminating() then return end
    scratchValue4 = this + 104
    ::FLOW_hoist_lab_00e56b84_1::
    goto FLOW_hoist_lab_00e56b54_1
    ::FLOW_past_lab_00e56b84::
    goto FLOW_past_lab_00e56b54
    ::LAB_00e56b54::
    if quest:IsActiveThreadTerminating() then return end
    scratchValue4 = this + 88
    ::FLOW_hoist_lab_00e56b54_1::
    if scratchValue4 ~= nil then
        if quest:IsActiveThreadTerminating() then return end
        if param2 ~= 0 then
            -- TODO(native): iVar6 = CBasicString<char>::Compare(*(void **)native_arg_param_2,"NULL");
            if scratchValue == 0 then goto LAB_00e56c84 end
        end
        goto FLOW_past_lab_00e56c84
        ::LAB_00e56c84::
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): (**(code **)(*(int *)pCVar5 + 0x48))(xStack_1c,0,1,0,1,true,0,0);
        goto LAB_00e56c30
        ::FLOW_past_lab_00e56c84::
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): (**(code **)(*(int *)pCVar5 + 0x48))(xStack_1c,0,1,0,0,true,0,0);
        -- TODO(native): (**(code **)(*(int *)pCVar5 + 0x50))(&xStack_18,0xffffffff,0,0,1,0,true,0,0);
    end
    ::FLOW_past_lab_00e56b54::
    ::LAB_00e56c30::
    repeat
        local isConversationActive = quest:IsConversationActive(nil --[[missing]])
        sequence = not isConversationActive
        if sequence then
            -- TODO(native): cVar3 = (**(*pCVar5 + 0x68))()
    --[[unresolved native value]]
            sequence = not nil
        end
        if sequence or quest:GetStateBool("ReadingBook") then
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveConversation(false)
            end
            break
        end
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
end

-- V_BookCollecting.AddGossip (retail 0x00e55c60)
-- E55C60: bsim names this body NScript::CV_BookCollectingScript::AddGossip (a homologous script member); no PDB name
function AddGossip(quest, strParam1)
    quest:AddRumourCategory(strParam1)
    quest:AddNewRumourToCategory(strParam1, nil --[[missing]])
    quest:AddGossipVillage(strParam1, nil --[[missing]])
    quest:AddGossipFactionToCategory(strParam1, nil --[[missing]])
end

