-- Generated from the same native helper bodies as the quest draft.
local helper_E3E320, helper_E3E6B0, helper_E3E720
function helper_E3E320(quest, me)
    local bVar3, bVar4, bVar5, bVar6, cVar8, pCVar7
    bVar5 = false
    bVar6 = false
    pCVar7 = quest:GetHero()
    bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS")
    if bVar3 then
        goto LAB_00e3e3d9
    else
        bVar5 = true
        bVar6 = false
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS_GOOD")
        if bVar3 then goto LAB_00e3e3d9 end
        bVar5 = true
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS_EVIL")
        cVar8 = 0
        if bVar3 then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    cVar8 = 1
    ::FLOW_past_lab_00e3e3d9::
    if bVar6 then
    end
    if bVar5 then
    end
    bVar5 = false
    bVar6 = false
    pCVar7 = quest:GetHero()
    bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS")
    if bVar3 then
        goto LAB_00e3e4c2
    else
        bVar5 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS_GOOD")
        if bVar3 then goto LAB_00e3e4c2 end
        bVar5 = true
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar4 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS_EVIL")
        bVar3 = false
        if bVar4 then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    bVar3 = true
    ::FLOW_past_lab_00e3e4c2::
    if bVar6 then
    end
    if bVar5 then
    end
    bVar6 = false
    if bVar3 then
        cVar8 = cVar8 + 1
    end
    pCVar7 = quest:GetHero()
    bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS")
    if bVar5 then
        goto LAB_00e3e5ad
    else
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS_GOOD")
        if bVar5 then goto LAB_00e3e5ad end
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS_EVIL")
        bVar5 = false
        if bVar3 then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    bVar5 = true
    ::FLOW_past_lab_00e3e5ad::
    if bVar6 then
    end
    if bVar5 then
        cVar8 = cVar8 + 1
    end
    pCVar7 = quest:GetHero()
    bVar6 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_HAT_WHOREWIG")
    if bVar6 then
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_NO_BOOTS")
        bVar6 = true
        if bVar5 then goto LAB_00e3e66d end
    end
    bVar6 = false
    ::LAB_00e3e66d::
    if bVar6 then
        cVar8 = cVar8 + 1
    end
    return cVar8 == '\x04'
end

function helper_E3E6B0(quest, me)
    local bVar1
    if quest:GetStateBool("HeroTricking") then
        bVar1 = helper_E3E320(quest, me)
        if bVar1 then
            return "_HEROWHORE"
        end
    end
    bVar1 = helper_E3E320(quest, me)
    if bVar1 then
        return "_HEROLADY"
    end
    return "_HEROMAN"
end

function helper_E3E720(quest, me, native_arg_param_2, native_arg_param_3)
    local resources = quest:RetailResources()
    local bVar6
    local alive = true
    quest:SetStateBool("CutscenePlaying", true)
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_1c = resources:NewActorMap()
    resources:SetActor(xStack_1c, "HERO", xStack_10)
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piStack_20 = *(int **)(this + 0x80);
    -- TODO(native): local piVar2 = *(pCVar7 + 0xc)
    local uVar3 = quest:GetStateInt("self_0x7c")
    if piVar2 ~= piStack_20 then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piStack_20;
        if piStack_20 ~= nil then
            -- TODO(native): *piStack_20 = *piStack_20 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piStack_20 = *(int **)(this + 0x90);
    -- TODO(native): piVar2 = *(pCVar7 + 0xc)
    piVar2 = nil --[[unresolved native value]]
    uVar3 = quest:GetStateInt("self_0x8c")
    if piVar2 ~= piStack_20 then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piStack_20;
        if piStack_20 ~= nil then
            -- TODO(native): *piStack_20 = *piStack_20 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piVar2 = *(this + 0xa0)
    piVar2 = nil --[[unresolved native value]]
    -- TODO(native): local piVar4 = *(pCVar7 + 0xc)
    uVar3 = quest:GetStateInt("self_0x9c")
    if piVar4 ~= piVar2 then
        if piVar4 ~= nil then
            -- TODO(native): *piVar4 = *piVar4 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piVar2;
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piVar2 = *(this + 0xb0)
    piVar2 = nil --[[unresolved native value]]
    -- TODO(native): piVar4 = *(pCVar7 + 0xc)
    piVar4 = nil --[[unresolved native value]]
    uVar3 = quest:GetStateInt("self_0xac")
    if piVar4 ~= piVar2 then
        if piVar4 ~= nil then
            -- TODO(native): *piVar4 = *piVar4 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piVar2;
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + 1;
        end
    end
    quest:FixMovieSequenceCamera(true)
    local cVar5 = native_arg_param_3
    if native_arg_param_3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            resources:DestroyActorMap(xStack_1c)
            resources:ReleaseResource(xStack_10)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    -- TODO(native): RunCutsceneMacro_Func((CCharString *)&native_arg_param_2,xStack_1c,(void *)0x0,(CTCCarryable *)(this + 0xb4),false,true);
    if cVar5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            -- LAB_00e3ea29: (native jump target)
            resources:DestroyActorMap(xStack_1c)
            resources:ReleaseResource(xStack_10)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(false)
    end
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("CutscenePlaying", false)
    -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)(this + 0xb4));
    resources:DestroyActorMap(xStack_1c)
    resources:ReleaseResource(xStack_10)
end

return {helper_E3E320 = helper_E3E320, helper_E3E6B0 = helper_E3E6B0, helper_E3E720 = helper_E3E720}
