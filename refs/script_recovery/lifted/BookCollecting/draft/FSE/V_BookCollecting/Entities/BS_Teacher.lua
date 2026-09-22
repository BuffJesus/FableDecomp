-- Generated native draft: BS_Teacher. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local CVar12, CVar14, CVar2, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, b2, b3, bVar6, cVar7, fVar5, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar1, iVar8, pCVar10, pCVar11, pCVar9, pQuestName, pcVar13, this_00, xStack_10, x_stk_58
    local alive = true
    local function __cleanup_LAB_00e55316()
        iVar8 = nil --[[unresolved native value]]
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    local function __cleanup_LAB_00e559a8()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    local function __cleanup_LAB_00e559f0()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    local function __cleanup_LAB_00e55a1e()
        resources:DestroyMovie(xStack_10)
    end
    local function __cleanup_LAB_00e55c3a()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    local function __cleanup_LAB_00e55c3f()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
    end
    quest:SetThingPersistent(me, true)
    iVar1 = quest:GetStateInt("BooksDonated")
    if (iVar1 < 4) and (quest:GetStateInt("GossipState") == 0) then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        -- TODO(native): helper_E55C60(quest, me, *(this + 0x14))
        quest:SetStateInt("GossipState", 1)
    elseif (iVar1 < 4) or (quest:GetStateInt("GossipState") ~= 1) then
        if (9 < iVar1) and (quest:GetStateInt("GossipState") < 3) then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            -- TODO(native): helper_E55C60(quest, me, *(this + 0x14))
            quest:SetStateInt("GossipState", 3)
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        quest:RemoveRumourCategory("Book Collection - Need Books")
        quest:SetStateInt("GossipState", 2)
    end
    if quest:GetStateInt("BooksDonated") < __native_entity_state:GetStateInt("BooksAccepted") then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            __native_condition_1 = not quest:GetStateBool("HasInformation")
            if not __native_condition_1 then
                iVar8 = quest:GetTimeOfDay()
                __native_condition_2 = iVar8 < 0x650
                if __native_condition_2 then
                    iVar8 = quest:GetTimeOfDay()
                    __native_condition_2 = 0x34c < iVar8
                end
                __native_condition_1 = __native_condition_2
            end
            if __native_condition_1 then
                __native_condition_3 = not quest:GetStateBool("HasInformation")
                if __native_condition_3 then
                    iVar8 = quest:GetTimeOfDay()
                    __native_condition_4 = 0x34d < iVar8
                    if __native_condition_4 then
                        iVar8 = quest:GetTimeOfDay()
                        __native_condition_4 = iVar8 < 0x64f
                    end
                    __native_condition_3 = __native_condition_4
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    quest:SetThingHasInformation(me, false, false, true)
                    quest:SetStateBool("HasInformation", true)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                quest:ClearThingHasInformation(me)
                quest:SetStateBool("HasInformation", false)
            end
            if quest:GetMasterGameState("HeroDollsScriptUsingTeacher") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                cVar7 = quest:GetMasterGameState("HeroDollsScriptUsingTeacher")
                while cVar7 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    cVar7 = quest:GetMasterGameState("HeroDollsScriptUsingTeacher")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                quest:SetStateBool("HasInformation", false)
            end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                xStack_10 = resources:StartMovie("")
                -- TODO(native): CVar12 = *(this + 4)
                CVar12 = nil --[[unresolved native value]]
                quest:PauseAllNonScriptedEntities(true)
                pCVar11 = (this + 0x34)
                resources:PrepareResource(pCVar11)
                bVar6 = resources:TryAcquire(pCVar11, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then __cleanup_LAB_00e559a8(); return end
                    bVar6 = resources:TryAcquire(pCVar11, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then __cleanup_LAB_00e55316(); return end
                if not quest:GetStateBool("DoneIntro") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        __cleanup_LAB_00e559f0()
                        return
                    end
                    if not quest:GetMasterGameState("HeroExposedLadyGrey") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_10)
                            return
                        end
                        -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                        pCVar9 = nil --[[unresolved native value]]
                        fret_00 = quest:GetHealth(nil --[[missing]])
                        fVar5 = 0.0
                        if fVar5 < fret_00 then
                            -- TODO(native): CVar2 = *pCVar11
                            CVar2 = nil --[[unresolved native value]]
                            pcVar13 = "TEXT_QST_B16_INTRO"
                            pCVar9 = quest:GetHero()
                            -- TODO(native): (**(code **)((int)CVar2 + 0x34))(pCVar9,pcVar13);
                            -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- TODO(native): (**(code **)(*(int *)CVar12 + 0x5ec))(0);
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then __cleanup_LAB_00e55316(); return end
                        -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                        pCVar9 = nil --[[unresolved native value]]
                        fret_0 = quest:GetHealth(nil --[[missing]])
                        fVar5 = 0.0
                        if fVar5 < fret_0 then
                            -- TODO(native): CVar2 = *pCVar11
                            CVar2 = nil --[[unresolved native value]]
                            pcVar13 = "TEXT_QST_B16_INTRO_B"
                            pCVar9 = quest:GetHero()
                            -- TODO(native): (**(code **)((int)CVar2 + 0x34))(pCVar9,pcVar13);
                            -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    __cleanup_LAB_00e559a8()
                                    return
                                end
                                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00e55316(); return end
                        end
                    end
                    CVar14 = 0x0
                    pCVar10 = quest:GetActiveQuestName()
                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BOOK_COLLECTION", pCVar10, (CVar14 ~= 0))
                    pQuestName = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01", "", "")
                    quest:SetStateBool("DoneIntro", true)
                else
                    iVar8 = quest:GetTimeOfDay()
                    __native_condition_5 = iVar8 < 0x34d
                    if not __native_condition_5 then
                        iVar8 = quest:GetTimeOfDay()
                        __native_condition_5 = 0x64f < iVar8
                    end
                    if __native_condition_5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            __cleanup_LAB_00e55316()
                            return
                        end
                        -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                        pCVar9 = nil --[[unresolved native value]]
                        fret_04 = quest:GetHealth(nil --[[missing]])
                        fVar5 = 0.0
                        x_stk_58 = nil
                        x_stk_58 = 0
                        if fVar5 < fret_04 then
                            -- TODO(native): xStack_7c = *pCVar11;
                            pcVar13 = "TEXT_QST_B16_BOOK_WRONG_TIME"
                            pCVar9 = quest:GetHero()
                            -- TODO(native): (**(code **)((int)xStack_7c + 0x34))(pCVar9,pcVar13);
                            -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559a8(); return end
                                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- TODO(native): iVar8 = *CVar12
                                iVar8 = nil --[[unresolved native value]]
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                        end
                    else
                        if quest:GetStateInt("LastBookRequested") < 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00e559f0(); return end
                            -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                            pCVar9 = nil --[[unresolved native value]]
                            fret_03 = quest:GetHealth(nil --[[missing]])
                            fVar5 = 0.0
                            if fVar5 < fret_03 then
                                -- TODO(native): CVar12 = *pCVar11
                                CVar12 = nil --[[unresolved native value]]
                                pcVar13 = "TEXT_QST_B16_BOOK_LOOK"
                                pCVar9 = quest:GetHero()
                                -- TODO(native): (**(code **)((int)CVar12 + 0x34))(pCVar9,pcVar13);
                                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                                cVar7 = nil --[[unresolved native value]]
                                while cVar7 ~= 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- TODO(native): iVar8 = *xStack_7c
                                        iVar8 = nil --[[unresolved native value]]
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_10)
                                        return
                                    end
                                    -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                                    cVar7 = nil --[[unresolved native value]]
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e55a1e(); return end
                            end
                            helper_E57530(quest, me)
                            goto LAB_00e55891
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then __cleanup_LAB_00e55316(); return end
                        -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                        pCVar9 = nil --[[unresolved native value]]
                        fret_01 = quest:GetHealth(nil --[[missing]])
                        fVar5 = 0.0
                        if fVar5 < fret_01 then
                            -- TODO(native): CVar2 = *pCVar11
                            CVar2 = nil --[[unresolved native value]]
                            pcVar13 = "TEXT_QST_B16_BOOK_REQUEST_AGAIN"
                            pCVar9 = quest:GetHero()
                            -- TODO(native): (**(code **)((int)CVar2 + 0x34))(pCVar9,pcVar13);
                            -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559f0(); return end
                                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00e55316(); return end
                        end
                        iVar8 = quest:GetStateInt("LastBookRequested")
                        -- TODO(native): iVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0x94)
                        iVar1 = nil --[[unresolved native value]]
                        pCVar9 = quest:GetHero()
                        bVar6 = quest:IsObjectInThingsPossession(pcVar13, pCVar9)
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                __cleanup_LAB_00e55a1e()
                                return
                            end
                            bVar6 = helper_E55CE0(quest, me)
                            if bVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))();
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                quest:SetStateInt("LastBookRequested", 0xffffffff)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- TODO(native): iVar8 = *xStack_7c
                                    iVar8 = nil --[[unresolved native value]]
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                helper_E57530(quest, me)
                            end
                            goto LAB_00e55891
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- LAB_00e55a13: (native jump target)
                            -- TODO(native): iVar8 = *xStack_7c
                            iVar8 = nil --[[unresolved native value]]
                            __cleanup_LAB_00e55c3f(); return
                        end
                        -- TODO(native): pCVar9 = (**(*pCVar11 + 0x30))()
                        pCVar9 = nil --[[unresolved native value]]
                        fret_02 = quest:GetHealth(nil --[[missing]])
                        fVar5 = 0.0
                        if fVar5 < fret_02 then
                            -- TODO(native): CVar12 = *pCVar11
                            CVar12 = nil --[[unresolved native value]]
                            pcVar13 = "TEXT_QST_B16_BOOK_LOST"
                            pCVar9 = quest:GetHero()
                            -- TODO(native): (**(code **)((int)CVar12 + 0x34))(pCVar9,pcVar13);
                            -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))(pCVar9)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e55a1e(); return end
                                -- TODO(native): cVar7 = (**(*pCVar11 + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- TODO(native): iVar8 = *xStack_7c
                                iVar8 = nil --[[unresolved native value]]
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                        end
                        helper_E57530(quest, me)
                    end
                end
                ::LAB_00e55891::
                resources:PrepareResource(pCVar11)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_10)
            end
        until not (quest:GetStateInt("BooksDonated") < __native_entity_state:GetStateInt("BooksAccepted"))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        quest:ClearThingHasInformation(me)
        b3 = 0x0
        b2 = 0x0
        CVar14 = 0x0
        pCVar11 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar11, (CVar14 ~= 0), (b2 ~= 0), (b3 ~= 0))
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            repeat
                if not quest:GetMasterGameState("HeroDollsScriptUsingTeacher") then
                    cVar7 = me:IsTalkedToByHero()
                    if cVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            return
                        end
                        xStack_10 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        this_00 = (this + 0x34)
                        resources:PrepareResource(this_00)
                        bVar6 = resources:TryAcquire(this_00, me, 4)
                        while not bVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                            bVar6 = resources:TryAcquire(this_00, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            __cleanup_LAB_00e55c3a()
                            return
                        end
                        -- TODO(native): pCVar9 = (**(*this_00 + 0x30))()
                        pCVar9 = nil --[[unresolved native value]]
                        fret_05 = quest:GetHealth(nil --[[missing]])
                        fVar5 = 0.0
                        if fVar5 < fret_05 then
                            -- TODO(native): iVar8 = *this_00
                            iVar8 = nil --[[unresolved native value]]
                            pcVar13 = "TEXT_QST_B16_COMPLETE"
                            pCVar9 = quest:GetHero()
                            -- TODO(native): (**(code **)(iVar8 + 0x34))(pCVar9,pcVar13);
                            -- TODO(native): cVar7 = (**(*this_00 + 0x68))(pCVar9)
                            cVar7 = nil --[[unresolved native value]]
                            while cVar7 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- LAB_00e55c16: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                -- TODO(native): cVar7 = (**(*this_00 + 0x68))()
                                cVar7 = nil --[[unresolved native value]]
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00e55c3a(); return end
                        end
                        resources:PrepareResource(this_00)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_10)
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    cVar7 = quest:GetMasterGameState("HeroDollsScriptUsingTeacher")
                    while cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            return
                        end
                        cVar7 = quest:GetMasterGameState("HeroDollsScriptUsingTeacher")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
            until false
        end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("BadBooksForHat", quest:ReadGlobalGameData(0x4f0))
    __native_entity_state:SetStateInt("GoodBooksForHat", quest:ReadGlobalGameData(0x4f4))
    local iVar1 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0x4f8)))
    __native_entity_state:SetStateInt("MoralityReward", iVar1)
    __native_entity_state:SetStateInt("BooksWanted", quest:ReadGlobalGameData(0x4e0))
    __native_entity_state:SetStateInt("BooksAccepted", quest:ReadGlobalGameData(0x4e4))
    __native_entity_state:SetStateInt("BooksComment", quest:ReadGlobalGameData(0x4e8))
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

function helper_E55C60(quest, me, native_arg_strParam_1)
    quest:AddRumourCategory(native_arg_strParam_1)
    quest:AddNewRumourToCategory(native_arg_strParam_1, nil --[[missing]])
    quest:AddGossipVillage(native_arg_strParam_1, nil --[[missing]])
    quest:AddGossipFactionToCategory(native_arg_strParam_1, nil --[[missing]])
end

function helper_E55CE0(quest, me, native_arg_param_1)
    local resources = quest:RetailResources()
    local bVar3, cVar4, ePriority, fVar2, fret_0, fret_00, fret_01, fret_02, iVar6, pCVar15, pCVar17, pCVar5, pCVar7, pOther, pcVar11, pvVar9, this_00, uVar10, uVar12, uVar13, uVar14, uVar16, value, xStack_24, xStack_30, xStack_3c, xStack_58
    local alive = true
    value = native_arg_param_1
    uVar10 = 0
    pCVar5 = tostring(native_arg_param_1)
    ("TEXT_QST_B16_OFFER_BOOK_" .. pCVar5)
    quest:GiveHeroYesNoQuestion(native_arg_param_1, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
    while iVar6 < 0 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            do return false end
            goto FLOW_after_lab_00e55dcf
        end
        iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00e55dcf: (native jump target)
        return false
    end
    ::FLOW_after_lab_00e55dcf::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if iVar6 ~= 1 then
        if bVar3 then
            do return false end
            goto FLOW_after_lab_00e55dcf
        end
        if value < __native_entity_state:GetStateInt("BooksWanted") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            quest:SetStateInt("LastBookRequested", value)
            pCVar17 = this + 0x34
            -- TODO(native): pCVar7 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))(xStack_24)
            pCVar7 = nil --[[unresolved native value]]
            fret_01 = quest:GetHealth(nil --[[missing]])
            fVar2 = 0.0
            if fret_01 <= fVar2 then goto LAB_00e566a1 end
            -- TODO(native): iVar6 = *pCVar17
            iVar6 = nil --[[unresolved native value]]
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            uVar12 = quest:GetHero()
            -- TODO(native): (**(code **)(iVar6 + 0x34))(uVar12);
            -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))(uVar12)
            cVar4 = nil --[[unresolved native value]]
            while cVar4 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))()
                cVar4 = nil --[[unresolved native value]]
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            pCVar17 = this + 0x34
            -- TODO(native): pCVar7 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))(xStack_24)
            pCVar7 = nil --[[unresolved native value]]
            fret_02 = quest:GetHealth(uVar12)
            fVar2 = 0.0
            if fret_02 <= fVar2 then goto LAB_00e566a1 end
            -- TODO(native): iVar6 = *pCVar17
            iVar6 = nil --[[unresolved native value]]
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pcVar11 = "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND"
            pCVar7 = quest:GetHero()
            -- TODO(native): (**(code **)(iVar6 + 0x34))(pCVar7,pcVar11,uVar12,uVar13,uVar14,uVar16);
            -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))(pCVar7)
            cVar4 = nil --[[unresolved native value]]
            while cVar4 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))()
                cVar4 = nil --[[unresolved native value]]
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            do return false end
            goto FLOW_after_lab_00e55dcf
        end
        goto LAB_00e566a1
    end
    if bVar3 then
        do return false end
        goto FLOW_after_lab_00e55dcf
    end
    quest:SetStateBool("ReadingBook", true)
    quest:TakeObjectFromHero(pcVar11)
    -- TODO(native): *(undefined1 *)(*(int *)(*(int *)(this + 0x14) + 0xa0) + (int)value) = 1;
    quest:SetStateInt("BooksDonated", quest:GetStateInt("BooksDonated") + 1)
    if value < __native_entity_state:GetStateInt("BooksWanted") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            do return false end
            goto FLOW_after_lab_00e55dcf
        end
        quest:SetStateInt("GoodBooksDonated", quest:GetStateInt("GoodBooksDonated") + 1)
        pCVar17 = this + 0x34
        -- TODO(native): pCVar7 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))(xStack_24)
        pCVar7 = nil --[[unresolved native value]]
        fret_0 = quest:GetHealth(nil --[[missing]])
        fVar2 = 0.0
        if fVar2 < fret_0 then
            -- TODO(native): iVar6 = *pCVar17
            iVar6 = nil --[[unresolved native value]]
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pcVar11 = "TEXT_QST_B16_BOOK_ACCEPTED"
            pCVar7 = quest:GetHero()
            -- TODO(native): (**(code **)(iVar6 + 0x34))(pCVar7,pcVar11,uVar12,uVar13,uVar14,uVar16);
            -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))(pCVar7)
            cVar4 = nil --[[unresolved native value]]
            while cVar4 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))()
                cVar4 = nil --[[unresolved native value]]
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
        end
        quest:GiveHeroMorality(__native_entity_state:GetStateInt("MoralityReward"))
        helper_E56D10(quest, me, 0x2c)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            do return false end
            goto FLOW_after_lab_00e55dcf
        end
        pCVar17 = this + 0x34
        -- TODO(native): pCVar7 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))(xStack_24)
        pCVar7 = nil --[[unresolved native value]]
        fret_00 = quest:GetHealth(nil --[[missing]])
        fVar2 = 0.0
        if fVar2 < fret_00 then
            -- TODO(native): iVar6 = *pCVar17
            iVar6 = nil --[[unresolved native value]]
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pcVar11 = "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY"
            pCVar7 = quest:GetHero()
            -- TODO(native): (**(code **)(iVar6 + 0x34))(pCVar7,pcVar11,uVar12,uVar13,uVar14,uVar16);
            -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))(pCVar7)
            cVar4 = nil --[[unresolved native value]]
            while cVar4 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                -- TODO(native): cVar4 = (**(*pCVar17 + 0x68))()
                cVar4 = nil --[[unresolved native value]]
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
        end
    end
    pOther = this + 0x34
    pCVar5 = tostring(value)
    ("CS_SCHOOLBOOK_" .. pCVar5)
    xStack_24 = resources:NewResource()
    ePriority = 4
    pCVar15 = xStack_24
    pCVar7 = quest:GetHero()
    resources:TryAcquire(pCVar15, pCVar7, ePriority)
    xStack_3c = resources:NewActorMap()
    resources:SetActor(xStack_3c, "Hero", xStack_24)
    -- TODO(native): resources:SetActor(xStack_3c, "Teacher", &pOther)
    xStack_30 = resources:NewStringMap()
    helper_E57020(quest, me, value, xStack_30)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(xStack_58, xStack_3c, xStack_30, false, true)
    if not quest:GetStateBool("HatRewarded") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00e560d5: (native jump target)
            resources:DestroyStringMap(xStack_30)
            resources:DestroyActorMap(xStack_3c)
            resources:ReleaseResource(xStack_24)
            -- LAB_00e560f5: (native jump target)
            return false
        end
        ::FLOW_after_lab_00e560d5::
        if quest:GetStateInt("GoodBooksDonated") < __native_entity_state:GetStateInt("GoodBooksForHat") then
            if __native_entity_state:GetStateInt("BadBooksForHat") <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:DestroyStringMap(xStack_30)
                    resources:DestroyActorMap(xStack_3c)
                    resources:ReleaseResource(xStack_24)
                    -- LAB_00e560f5_c15: (native jump target)
                    do return false end
                    goto FLOW_after_lab_00e560d5
                end
                -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
                pcVar11 = "OBJECT_HERO_HAT_WIZARD_EVIL"
                resources:SetString(xStack_30, "$PRIZE", pcVar11)
                resources:SetActor(xStack_3c, "Hero", xStack_24)
                resources:SetActor(xStack_3c, "Teacher", pOther)
                resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", xStack_3c, xStack_30, false, true)
                quest:SetStateBool("HatRewarded", true)
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:DestroyStringMap(xStack_30)
                resources:DestroyActorMap(xStack_3c)
                resources:ReleaseResource(xStack_24)
                -- LAB_00e560f5_c16: (native jump target)
                do return false end
                goto FLOW_after_lab_00e560d5
            end
            -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
            pcVar11 = "OBJECT_HERO_HAT_WIZARD_GOOD"
            resources:SetString(xStack_30, "$PRIZE", pcVar11)
            resources:SetActor(xStack_3c, "Hero", xStack_24)
            resources:SetActor(xStack_3c, "Teacher", pOther)
            resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", xStack_3c, xStack_30, false, true)
            quest:SetStateBool("HatRewarded", true)
        end
    elseif not quest:GetStateBool("KeyRewarded") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            ::LAB_00e560d5_c17::
            resources:DestroyStringMap(xStack_30)
            resources:DestroyActorMap(xStack_3c)
            resources:ReleaseResource(xStack_24)
            -- LAB_00e560f5_c17: (native jump target)
            do return false end
            if quest:GetStateInt("GoodBooksDonated") < __native_entity_state:GetStateInt("GoodBooksForHat") then
                if __native_entity_state:GetStateInt("BadBooksForHat") <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e560d5_c17 end
                    -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
                    pcVar11 = "OBJECT_HERO_HAT_WIZARD_EVIL"
                    resources:SetString(xStack_30, "$PRIZE", pcVar11)
                    resources:SetActor(xStack_3c, "Hero", xStack_24)
                    resources:SetActor(xStack_3c, "Teacher", pOther)
                    resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", xStack_3c, xStack_30, false, true)
                    quest:SetStateBool("HatRewarded", true)
                end
            end
            goto FLOW_after_lab_00e560d5_213
        end
        if quest:GetStateInt("BooksDonated") == __native_entity_state:GetStateInt("BooksAccepted") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                ::LAB_00e560d5_c18::
                resources:DestroyStringMap(xStack_30)
                resources:DestroyActorMap(xStack_3c)
                resources:ReleaseResource(xStack_24)
                -- LAB_00e560f5_c18: (native jump target)
                do return false end
                if quest:GetStateInt("GoodBooksDonated") < __native_entity_state:GetStateInt("GoodBooksForHat") then
                    if __native_entity_state:GetStateInt("BadBooksForHat") <= quest:GetStateInt("BooksDonated") - quest:GetStateInt("GoodBooksDonated") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e560d5_c18 end
                        -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
                        pcVar11 = "OBJECT_HERO_HAT_WIZARD_EVIL"
                        resources:SetString(xStack_30, "$PRIZE", pcVar11)
                        resources:SetActor(xStack_3c, "Hero", xStack_24)
                        resources:SetActor(xStack_3c, "Teacher", pOther)
                        resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", xStack_3c, xStack_30, false, true)
                        quest:SetStateBool("HatRewarded", true)
                    end
                end
                goto FLOW_after_lab_00e560d5_213
            end
            -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)xStack_30);
            pcVar11 = "OBJECT_SILVER_KEY"
            resources:SetString(xStack_30, "$PRIZE", pcVar11)
            resources:SetActor(xStack_3c, "Hero", xStack_24)
            resources:SetActor(xStack_3c, "Teacher", pOther)
            resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_KEY", xStack_3c, xStack_30, false, true)
            quest:SetStateBool("KeyRewarded", true)
        end
    end
    ::FLOW_after_lab_00e560d5_213::
    quest:FadeScreenIn()
    quest:FixMovieSequenceCamera(false)
    resources:DestroyStringMap(xStack_30)
    resources:DestroyActorMap(xStack_3c)
    resources:ReleaseResource(xStack_24)
    quest:SetStateBool("ReadingBook", false)
    if this_00 == nil then
        this_00 = 0x0
    else
        -- TODO(native): xStack_50 = *(CCharString *)(this + 0x14);
        pCVar5 = extraout_EAX
        pCVar5 = (a .. pCVar5)
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_00,pCVar5,0);
        -- TODO(native): *(code **)(this_00 + 0x34) = BookReaction;
        -- TODO(native): *(CCharString *)(this_00 + 0x38) = xStack_50;
        -- TODO(native): *(CCharString *)(this_00 + 0x3c) = value;
        uVar10 = 7
    end
    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
    if (uVar10 & 4) ~= 0 then
        uVar10 = uVar10 & 0xfffffffb
    end
    if (uVar10 & 2) ~= 0 then
        uVar10 = uVar10 & 0xfffffffd
    end
    if (uVar10 & 1) ~= 0 then
    end
    ::LAB_00e566a1::
    return
end

function helper_E56D10(quest, me, native_arg_param_2)
    local cVar3, iVar7, iVar9, pCVar6, piVar1, piVar2, uStack_14, xStack_24, x_stk_10
    local iVar8 = native_arg_param_2
    local pCVar4 = quest:GetHero()
    quest:EntityPostOpinionDeedToRecipient(pCVar4, iVar8, me)
    iVar7 = 0
    local pCVar5 = tostring(0)
    (DAT_012448ec .. pCVar5)
    local r1 = quest:GetThingWithScriptName(nil --[[missing]])
    while uStack_14 ~= nil do
        -- TODO(native): cVar3 = (**(*uStack_14 + 0x12c))()
        cVar3 = nil --[[unresolved native value]]
        if cVar3 == 0 then break end
        pCVar6 = r1
        iVar9 = native_arg_param_2
        pCVar4 = quest:GetHero()
        quest:EntityPostOpinionDeedToRecipient(pCVar4, iVar9, pCVar6)
        iVar7 = iVar7 + 1
        pCVar5 = tostring(iVar7)
        pCVar5 = (DAT_012448ec .. pCVar5)
        xStack_24 = pCVar5
        pCVar6 = quest:GetThingWithScriptName(xStack_24)
        -- TODO(native): piVar1 = *(pCVar6 + 0x8)
        piVar1 = nil --[[unresolved native value]]
        -- TODO(native): piVar2 = *(pCVar6 + 0x4)
        piVar2 = nil --[[unresolved native value]]
        if x_stk_10 ~= piVar1 then
            uStack_14 = piVar2
            x_stk_10 = piVar1
            if piVar1 ~= nil then
                -- TODO(native): *piVar1 = *piVar1 + 1;
            end
        end
        pCVar6 = nil
    end
    iVar7 = 0
    while true do
        pCVar5 = tostring(iVar7)
        pCVar5 = (DAT_012448f0 .. pCVar5)
        xStack_24 = pCVar5
        pCVar6 = quest:GetThingWithScriptName(xStack_24)
        -- TODO(native): piVar1 = *(pCVar6 + 0x8)
        piVar1 = nil --[[unresolved native value]]
        -- TODO(native): piVar2 = *(pCVar6 + 0x4)
        piVar2 = nil --[[unresolved native value]]
        if x_stk_10 ~= piVar1 then
            uStack_14 = piVar2
            x_stk_10 = piVar1
            if piVar1 ~= nil then
                -- TODO(native): *piVar1 = *piVar1 + 1;
            end
        end
        pCVar6 = nil
        if uStack_14 == nil then break end
        -- TODO(native): cVar3 = (**(*uStack_14 + 0x12c))()
        cVar3 = nil --[[unresolved native value]]
        if cVar3 == 0 then break end
        pCVar6 = r1
        iVar9 = native_arg_param_2
        pCVar4 = quest:GetHero()
        quest:EntityPostOpinionDeedToRecipient(pCVar4, iVar9, pCVar6)
        iVar7 = iVar7 + 1
    end
    r1 = nil
end

function helper_E57020(quest, me)
    local resources = quest:RetailResources()
    local bVar1, cVar2, fret_0, fret_00, fret_01, fret_02, iVar5, pCVar4, pcVar7, this_00
    local alive = true
    if this == 0xf then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        iVar5 = quest:GetMasterGameState("PostSavePosition")
        if iVar5 < 0x4e3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK15_90D"
        elseif iVar5 < 0x6a5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK15_90A"
        elseif iVar5 < 0x8fd then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK15_90B"
        elseif iVar5 < 0xa29 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK15_90C"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK15_90B"
        end
        -- LAB_00e5750f: (native jump target)
        resources:SetString(param_2, "$ARG1", pcVar7)
        return
    end
    if "$ARG1" == 0x11 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        fret_0 = quest:GetHeroAttractiveness()
        if fret_0 <= 0.5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK17_20A"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK17_20B"
        end
        resources:SetString(param_2, "$ARG1", pcVar7)
        return
    end
    if "$ARG1" == 0x13 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        if quest:GetMasterGameState("PostSavePosition") < 0x2bd then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK19_80A"
        elseif not quest:GetMasterGameState("BanditCampTwinbladeKilled") then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK19_80B"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            pcVar7 = "TEXT_CS_B16_BOOK19_80C"
        end
        resources:SetString(param_2, "$ARG1", pcVar7)
        return
    end
    if "$ARG1" ~= 0x15 then
        if "$ARG1" == 0x16 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            fret_00 = quest:GetHeroScariness()
            if 0.5 < fret_00 then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                pcVar7 = "TEXT_CS_B16_BOOK22_20B"
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                pcVar7 = "TEXT_CS_B16_BOOK22_20A"
            end
        else
            if "$ARG1" ~= 0x17 then
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            fret_01 = quest:GetHeroAttractiveness()
            if -0.5 <= fret_01 then
                fret_02 = quest:GetHeroAttractiveness()
                if 0.5 < fret_02 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    pcVar7 = "TEXT_CS_B16_BOOK23_10C"
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    pcVar7 = "TEXT_CS_B16_BOOK23_10B"
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                pcVar7 = "TEXT_CS_B16_BOOK23_10A"
            end
        end
        resources:SetString(param_2, "$ARG1", pcVar7)
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    -- TODO(native): CCharString::CCharString((CCharString *)&this,(CCharString *)(*(int *)(this + 0x18) + 0x54));
    pCVar4 = ("TEXT_CS_B16_BOOK21_20" .. "$ARG1")
    this_00 = param_2
    resources:SetString(param_2, "$ARG1", pCVar4)
    pCVar4 = ("TEXT_CS_B16_BOOK21_30" .. "$ARG1")
    resources:SetString(this_00, "$ARG2", pCVar4)
    if "$ARG1" == 0 then
        bVar1 = false
        if not bVar1 then goto LAB_00e57345 end
        goto LAB_00e573aa
    else
        -- TODO(native): iVar5 = CBasicString<char>::Compare(*(void **)this,"A");
        cVar2 = not (iVar5 ~= 0)
        -- TODO(native): param_2 = (map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)CONCAT31(param_2._1_3_,cVar2);
        if cVar2 then goto LAB_00e573aa end
        goto LAB_00e57345
    end
    goto FLOW_past_lab_00e573aa
    ::LAB_00e573aa::
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then goto LAB_00e57381 end
    pcVar7 = "TEXT_CS_B16_BOOK21_40A"
    ::FLOW_past_lab_00e573aa::
    goto FLOW_past_lab_00e57345
    ::LAB_00e57345::
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then goto LAB_00e57381 end
    pcVar7 = "TEXT_CS_B16_BOOK21_40B"
    ::FLOW_past_lab_00e57345::
    resources:SetString(this_00, "$ARG3", pcVar7)
    ::LAB_00e57381::
end

function helper_E57530(quest, me)
    local __native_condition_1, bVar5, cVar6, fVar4, fret_0, fret_00, fret_01, fret_02, iVar7, pCVar1, pCVar10, pCVar12, pCVar8, pCVar9, pcVar14, pvVar11, uVar15, uVar16, uVar17, value, xStack_28, x_stk_14, x_stk_18
    local alive = true
    value = 0xffffffff
    -- TODO(native): ctr_CVar13 = 0;
    x_stk_14 = 0xffffffff
    if quest:GetStateInt("BooksInGame") < 1 then
        goto LAB_00e57678
    else
        repeat
            if (((ctr_CVar13 == __native_entity_state:GetStateInt("BooksWanted")) or (ctr_CVar13 == __native_entity_state:GetStateInt("BooksAccepted"))) or (ctr_CVar13 == __native_entity_state:GetStateInt("BooksComment"))) and (-1 < value) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                break
            end
            -- TODO(native): iVar7 = *(iVar7 + 0x94)
            iVar7 = nil --[[unresolved native value]]
            pCVar8 = quest:GetHero()
            bVar5 = quest:IsObjectInThingsPossession(nil --[[missing]], pCVar8)
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                -- TODO(native): if (ctr_CVar13 == quest:GetStateInt("LastBookRequested")) or (*(ctr_CVar13 + *(__native_entity_state:GetStateInt("self_0x14") + 0xa0)) ~= 0) then
                if false then
                    if xStack_28 == 0xffffffff then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        value = 0xfffffffe
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    __native_condition_1 = 0 == 0
                    if __native_condition_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        -- TODO(native): xStack_28 = ctr_CVar13;
                        __native_condition_1 = bVar5
                    end
                    if __native_condition_1 then
                        return
                    end
                    -- TODO(native): if *(ctr_CVar13 + *(__native_entity_state:GetStateInt("self_0x14") + 0xac)) == 0 then
                    if false then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        -- TODO(native): *(undefined1 *)(ctr_CVar13 + *(int *)(*(int *)(this + 0x14) + 0xac)) = 1;
                    end
                end
            end
            -- TODO(native): ctr_CVar13 = ctr_CVar13 + 1;
        until not (ctr_CVar13 < quest:GetStateInt("BooksInGame"))
        if value ~= 0xfffffffe then goto LAB_00e57678 end
    end
    goto FLOW_past_lab_00e57678
    ::LAB_00e57678::
    if value < __native_entity_state:GetStateInt("BooksComment") then
        if value ~= 0xffffffff then
            if __native_entity_state:GetStateInt("BooksAccepted") <= value then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                pCVar12 = (this + 0x34)
                -- TODO(native): pCVar9 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))()
                pCVar9 = nil --[[unresolved native value]]
                fret_00 = quest:GetHealth(nil --[[missing]])
                fVar4 = 0.0
                if fret_00 <= fVar4 then
                    return
                end
                -- TODO(native): x_stk_14 = *pCVar12
                x_stk_14 = nil --[[unresolved native value]]
                uVar17 = 1
                uVar16 = 0
                uVar15 = 0
                pCVar10 = tostring(value - __native_entity_state:GetStateInt("BooksAccepted"))
                pCVar10 = ("TEXT_QST_B16_BOOK_COMMENT_" .. pCVar10)
                pvVar11 = pCVar10
                pCVar8 = quest:GetHero()
                -- TODO(native): (**(code **)((int)x_stk_14 + 0x34))(pCVar8,pvVar11,uVar15,uVar16,uVar17);
                -- TODO(native): cVar6 = (**(*pCVar12 + 0x68))(pCVar8)
                cVar6 = nil --[[unresolved native value]]
                if cVar6 ~= 0 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        -- TODO(native): cVar6 = (**(*pCVar12 + 0x68))()
                        cVar6 = nil --[[unresolved native value]]
                    until not (cVar6 ~= 0)
                    alive = not quest:IsActiveThreadTerminating()
                    return
                end
                goto LAB_00e579ce
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            pCVar12 = tostring(value)
            ("TEXT_QST_B16_BOOK_REQUEST_" .. pCVar12)
            pCVar1 = this + 0x34
            -- TODO(native): pCVar9 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))()
            pCVar9 = nil --[[unresolved native value]]
            fret_01 = quest:GetHealth(pCVar8)
            fVar4 = 0.0
            if fVar4 < fret_01 then
                -- TODO(native): iVar7 = *pCVar1
                iVar7 = nil --[[unresolved native value]]
                uVar17 = 1
                uVar16 = 0
                uVar15 = 0
                pvVar11 = x_stk_14
                pCVar8 = quest:GetHero()
                -- TODO(native): (**(code **)(iVar7 + 0x34))(pCVar8,pvVar11,uVar15,uVar16,uVar17);
                -- TODO(native): cVar6 = (**(*pCVar1 + 0x68))(pCVar8)
                cVar6 = nil --[[unresolved native value]]
                while cVar6 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e57924 end
                    -- TODO(native): cVar6 = (**(*pCVar1 + 0x68))()
                    cVar6 = nil --[[unresolved native value]]
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                value = x_stk_18
                if bVar5 then goto LAB_00e57924 end
            end
            helper_E55CE0(quest, me, value)
            ::LAB_00e57924::
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        pCVar1 = this + 0x34
        -- TODO(native): pCVar9 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))()
        pCVar9 = nil --[[unresolved native value]]
        fret_0 = quest:GetHealth(pCVar8)
        fVar4 = 0.0
        if fret_0 <= fVar4 then
            return
        end
        -- TODO(native): iVar7 = *pCVar1
        iVar7 = nil --[[unresolved native value]]
        uVar17 = 1
        uVar16 = 0
        uVar15 = 2
        pcVar14 = "TEXT_QST_B16_BOOK_NONE_FOUND"
        pCVar8 = quest:GetHero()
        -- TODO(native): (**(code **)(iVar7 + 0x34))(pCVar8,pcVar14,uVar15,uVar16,uVar17);
        -- TODO(native): cVar6 = (**(*pCVar1 + 0x68))(pCVar8)
        cVar6 = nil --[[unresolved native value]]
        if cVar6 ~= 0 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                -- TODO(native): cVar6 = (**(*pCVar1 + 0x68))()
                cVar6 = nil --[[unresolved native value]]
            until not (cVar6 ~= 0)
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        goto LAB_00e579ce
    end
    ::FLOW_past_lab_00e57678::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    pCVar1 = this + 0x34
    -- TODO(native): pCVar9 = (**(__native_entity_state:GetStateInt("self_0x34") + 0x30))()
    pCVar9 = nil --[[unresolved native value]]
    fret_02 = quest:GetHealth(pCVar8)
    fVar4 = 0.0
    if fret_02 <= fVar4 then
        return
    end
    -- TODO(native): iVar7 = *pCVar1
    iVar7 = nil --[[unresolved native value]]
    uVar17 = 1
    uVar16 = 0
    uVar15 = 2
    pcVar14 = "TEXT_QST_B16_BOOK_NOT_FOUND"
    pCVar8 = quest:GetHero()
    -- TODO(native): (**(code **)(iVar7 + 0x34))(pCVar8,pcVar14,uVar15,uVar16,uVar17);
    -- TODO(native): cVar6 = (**(*pCVar1 + 0x68))(pCVar8)
    cVar6 = nil --[[unresolved native value]]
    while cVar6 ~= 0 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        -- TODO(native): cVar6 = (**(*pCVar1 + 0x68))()
        cVar6 = nil --[[unresolved native value]]
    end
    ::LAB_00e579ce::
    alive = not quest:IsActiveThreadTerminating()
end

