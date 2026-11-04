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
    local CVar12, CVar14, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, b2, b3, bVar6, cVar7, fVar5, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar1, iVar8, pCVar10, pCVar11, pCVar9, pQuestName, r1, r2, r3, r4, r5, r6, r7, this_00, xStack_10, xStack_7c, x_stk_58
    local alive = true
    local function __cleanup_LAB_00e55316()
        iVar8 = nil --[[reviewed native interface-table staging]]
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
        require("V_BookCollecting.native_quest_helpers").helper_E55C60(quest, me, "Book Collection - Need Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_NEED", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS")
        quest:SetStateInt("GossipState", 1)
    elseif (iVar1 < 4) or (quest:GetStateInt("GossipState") ~= 1) then
        if (9 < iVar1) and (quest:GetStateInt("GossipState") < 3) then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            require("V_BookCollecting.native_quest_helpers").helper_E55C60(quest, me, "Book Collection - Given Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_GIVEN", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS")
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
                -- reviewed native interface-pointer staging: CVar12 = *(this + 4)
                CVar12 = nil --[[reviewed native interface-pointer staging]]
                xStack_7c = CVar12
                quest:PauseAllNonScriptedEntities(true)
                pCVar11 = resources:MemberResource("seh_me", me)
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
                        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                        fret_00 = quest:GetHealth(pCVar9)
                        fVar5 = 0.0
                        if fVar5 < fret_00 then
                            pCVar9 = quest:GetHero()
                            r1 = me:Speak(pCVar9, "TEXT_QST_B16_INTRO", 0, false, true, false)
                            cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
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
                        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                        fret_0 = quest:GetHealth(pCVar9)
                        fVar5 = 0.0
                        if fVar5 < fret_0 then
                            pCVar9 = quest:GetHero()
                            r2 = me:Speak(pCVar9, "TEXT_QST_B16_INTRO_B", 0, false, true, false)
                            cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    __cleanup_LAB_00e559a8()
                                    return
                                end
                                cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
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
                        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                        fret_04 = quest:GetHealth(pCVar9)
                        fVar5 = 0.0
                        x_stk_58 = nil
                        x_stk_58 = 0
                        if fVar5 < fret_04 then
                            -- reviewed native dispatch staging: xStack_7c = *pCVar11;
                            pCVar9 = quest:GetHero()
                            r3 = me:Speak(pCVar9, "TEXT_QST_B16_BOOK_WRONG_TIME", 2, false, true, false)
                            cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559a8(); return end
                                cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- reviewed native interface-table staging: iVar8 = *CVar12
                                iVar8 = nil --[[reviewed native interface-table staging]]
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
                            pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                            fret_03 = quest:GetHealth(pCVar9)
                            fVar5 = 0.0
                            if fVar5 < fret_03 then
                                -- reviewed native register-dispatch staging: CVar12 = *pCVar11
                                CVar12 = nil --[[reviewed native register-dispatch staging]]
                                pCVar9 = quest:GetHero()
                                r4 = me:Speak(pCVar9, "TEXT_QST_B16_BOOK_LOOK", 2, false, true, false)
                                cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        -- reviewed native saved-table staging: iVar8 = *xStack_7c
                                        iVar8 = nil --[[reviewed native interface-table staging]]
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_10)
                                        return
                                    end
                                    cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559f0(); return end
                            end
                            helper_E57530(quest, me)
                            CVar12 = xStack_7c
                            goto LAB_00e55891
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then __cleanup_LAB_00e55316(); return end
                        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                        fret_01 = quest:GetHealth(pCVar9)
                        fVar5 = 0.0
                        if fVar5 < fret_01 then
                            pCVar9 = quest:GetHero()
                            r5 = me:Speak(pCVar9, "TEXT_QST_B16_BOOK_REQUEST_AGAIN", 2, false, true, false)
                            cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559f0(); return end
                                cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00e55316(); return end
                        end
                        iVar8 = quest:GetStateInt("LastBookRequested")
                        -- reviewed native definition-vector staging: iVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0x94)
                        iVar1 = nil --[[reviewed native definition-vector staging]]
                        pCVar9 = quest:GetHero()
                        bVar6 = quest:IsObjectInThingsPossession(quest:GetStateStringVectorAt("BookDef", iVar8), pCVar9)
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- LAB_00e55a1e: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                            bVar6 = helper_E55CE0(quest, me, quest:GetStateInt("LastBookRequested"), "TEXT_QST_B16_BOOK_REFUSE_AGAIN")
                            if bVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559f0(); return end
                                quest:SetStateInt("LastBookRequested", 0xffffffff)
                                CVar12 = xStack_7c
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- reviewed native saved-table staging: iVar8 = *xStack_7c
                                    iVar8 = nil --[[reviewed native interface-table staging]]
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                helper_E57530(quest, me)
                                CVar12 = xStack_7c
                            end
                            goto LAB_00e55891
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- LAB_00e55a13: (native jump target)
                            -- reviewed native saved-table staging: iVar8 = *xStack_7c
                            iVar8 = nil --[[reviewed native interface-table staging]]
                            __cleanup_LAB_00e55c3f(); return
                        end
                        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                        fret_02 = quest:GetHealth(pCVar9)
                        fVar5 = 0.0
                        if fVar5 < fret_02 then
                            -- reviewed native register-dispatch staging: CVar12 = *pCVar11
                            CVar12 = nil --[[reviewed native register-dispatch staging]]
                            pCVar9 = quest:GetHero()
                            r6 = me:Speak(pCVar9, "TEXT_QST_B16_BOOK_LOST", 0, false, true, false)
                            cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then __cleanup_LAB_00e559f0(); return end
                                cVar7 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                -- reviewed native saved-table staging: iVar8 = *xStack_7c
                                iVar8 = nil --[[reviewed native interface-table staging]]
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                        end
                        helper_E57530(quest, me)
                        CVar12 = xStack_7c
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
                        this_00 = resources:MemberResource("seh_me", me)
                        resources:PrepareResource(this_00)
                        bVar6 = resources:TryAcquire(this_00, me, 4)
                        while not bVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then __cleanup_LAB_00e559f0(); return end
                            bVar6 = resources:TryAcquire(this_00, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            __cleanup_LAB_00e55c3a()
                            return
                        end
                        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                        fret_05 = quest:GetHealth(pCVar9)
                        fVar5 = 0.0
                        if fVar5 < fret_05 then
                            pCVar9 = quest:GetHero()
                            r7 = me:Speak(pCVar9, "TEXT_QST_B16_COMPLETE", 2, false, true, false)
                            if not r7 then
                                -- LAB_00e55c16: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                return
                            end
                            cVar7 = me:IsPerformingScriptTask()
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    -- LAB_00e55c16: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_10)
                                    return
                                end
                                cVar7 = me:IsPerformingScriptTask()
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

function helper_E55CE0(quest, me, native_arg_param_1, native_arg_param_2)
    local resources = quest:RetailResources()
    local bVar3, cVar4, ePriority, fVar2, fret_0, fret_00, fret_01, fret_02, iVar6, pCVar15, pCVar17, pCVar5, pCVar7, pOther, pcVar11, pvVar9, r1, r2, r3, r4, uVar10, uVar12, uVar12_pushed, uVar13, uVar14, uVar16, v_stk_45, value, xStack_24, xStack_30, xStack_3c, xStack_58, x_stk_1c
    local alive = true
    value = native_arg_param_1
    uVar10 = 0
    v_stk_45 = false
    pCVar5 = tostring(native_arg_param_1)
    native_arg_param_1 = ("TEXT_QST_B16_OFFER_BOOK_" .. pCVar5)
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
            pCVar17 = resources:MemberResource("seh_me", me)
            x_stk_1c = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
            pCVar7 = x_stk_1c
            fret_01 = quest:GetHealth(pCVar7)
            fVar2 = 0.0
            if fret_01 <= fVar2 then goto LAB_00e566a1 end
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pvVar9 = native_arg_param_2
            uVar12_pushed = uVar12
            uVar12 = quest:GetHero()
            r1 = me:Speak(uVar12, pvVar9, uVar12_pushed, (uVar13 ~= 0), (uVar14 ~= 0), (uVar16 ~= 0))
            if not r1 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            cVar4 = me:IsPerformingScriptTask()
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                cVar4 = me:IsPerformingScriptTask()
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            pCVar17 = resources:MemberResource("seh_me", me)
            x_stk_1c = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
            pCVar7 = x_stk_1c
            fret_02 = quest:GetHealth(pCVar7)
            fVar2 = 0.0
            if fret_02 <= fVar2 then goto LAB_00e566a1 end
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pcVar11 = "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND"
            pCVar7 = quest:GetHero()
            r2 = me:Speak(pCVar7, pcVar11, uVar12, (uVar13 ~= 0), (uVar14 ~= 0), (uVar16 ~= 0))
            if not r2 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            cVar4 = me:IsPerformingScriptTask()
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                cVar4 = me:IsPerformingScriptTask()
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
    quest:TakeObjectFromHero(quest:GetStateStringVectorAt("BookDef", value))
    quest:SetStateFlagVectorAt("BookDonated", value, true)
    quest:SetStateInt("BooksDonated", quest:GetStateInt("BooksDonated") + 1)
    v_stk_45 = true
    if value < __native_entity_state:GetStateInt("BooksWanted") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            do return false end
            goto FLOW_after_lab_00e55dcf
        end
        quest:SetStateInt("GoodBooksDonated", quest:GetStateInt("GoodBooksDonated") + 1)
        pCVar17 = resources:MemberResource("seh_me", me)
        x_stk_1c = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
        pCVar7 = x_stk_1c
        fret_0 = quest:GetHealth(pCVar7)
        fVar2 = 0.0
        if fVar2 < fret_0 then
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pcVar11 = "TEXT_QST_B16_BOOK_ACCEPTED"
            pCVar7 = quest:GetHero()
            r3 = me:Speak(pCVar7, pcVar11, uVar12, (uVar13 ~= 0), (uVar14 ~= 0), (uVar16 ~= 0))
            if not r3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            cVar4 = me:IsPerformingScriptTask()
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                cVar4 = me:IsPerformingScriptTask()
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
        pCVar17 = resources:MemberResource("seh_me", me)
        x_stk_1c = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
        pCVar7 = x_stk_1c
        fret_00 = quest:GetHealth(pCVar7)
        fVar2 = 0.0
        if fVar2 < fret_00 then
            uVar16 = 0
            uVar14 = 1
            uVar13 = 0
            uVar12 = 2
            pcVar11 = "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY"
            pCVar7 = quest:GetHero()
            r4 = me:Speak(pCVar7, pcVar11, uVar12, (uVar13 ~= 0), (uVar14 ~= 0), (uVar16 ~= 0))
            if not r4 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
            cVar4 = me:IsPerformingScriptTask()
            while cVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    do return false end
                    goto FLOW_after_lab_00e55dcf
                end
                cVar4 = me:IsPerformingScriptTask()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                do return false end
                goto FLOW_after_lab_00e55dcf
            end
        end
    end
    pOther = resources:MemberResource("seh_me", me)
    pCVar5 = tostring(value)
    xStack_58 = ("CS_SCHOOLBOOK_" .. pCVar5)
    xStack_24 = resources:NewResource()
    ePriority = 4
    pCVar15 = xStack_24
    pCVar7 = quest:GetHero()
    resources:TryAcquire(pCVar15, pCVar7, ePriority)
    xStack_3c = resources:NewActorMap()
    resources:SetActor(xStack_3c, "Hero", xStack_24)
    resources:SetActor(xStack_3c, "Teacher", pOther)
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
                resources:ClearStringMap(xStack_30)
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
            resources:ClearStringMap(xStack_30)
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
                    resources:ClearStringMap(xStack_30)
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
                if bVar3 then goto LAB_00e560d5_c17 end
                resources:ClearStringMap(xStack_30)
                pcVar11 = "OBJECT_HERO_HAT_WIZARD_GOOD"
                resources:SetString(xStack_30, "$PRIZE", pcVar11)
                resources:SetActor(xStack_3c, "Hero", xStack_24)
                resources:SetActor(xStack_3c, "Teacher", pOther)
                resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", xStack_3c, xStack_30, false, true)
                quest:SetStateBool("HatRewarded", true)
            end
            goto FLOW_after_lab_00e560d5_216
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
                        resources:ClearStringMap(xStack_30)
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
                    if bVar3 then goto LAB_00e560d5_c18 end
                    resources:ClearStringMap(xStack_30)
                    pcVar11 = "OBJECT_HERO_HAT_WIZARD_GOOD"
                    resources:SetString(xStack_30, "$PRIZE", pcVar11)
                    resources:SetActor(xStack_3c, "Hero", xStack_24)
                    resources:SetActor(xStack_3c, "Teacher", pOther)
                    resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", xStack_3c, xStack_30, false, true)
                    quest:SetStateBool("HatRewarded", true)
                end
                goto FLOW_after_lab_00e560d5_216
            end
            resources:ClearStringMap(xStack_30)
            pcVar11 = "OBJECT_SILVER_KEY"
            resources:SetString(xStack_30, "$PRIZE", pcVar11)
            resources:SetActor(xStack_3c, "Hero", xStack_24)
            resources:SetActor(xStack_3c, "Teacher", pOther)
            resources:RunMacroWithStrings("CS_SCHOOLBOOK_TEACHER_GIVES_KEY", xStack_3c, xStack_30, false, true)
            quest:SetStateBool("KeyRewarded", true)
        end
    end
    ::FLOW_after_lab_00e560d5_216::
    quest:FadeScreenIn()
    quest:FixMovieSequenceCamera(false)
    resources:DestroyStringMap(xStack_30)
    resources:DestroyActorMap(xStack_3c)
    resources:ReleaseResource(xStack_24)
    quest:SetStateBool("ReadingBook", false)
    quest:CreateThread("BookReaction", {args = {value}})  -- native parent-quest worker BookReaction, bound values
    ::LAB_00e566a1::
    return v_stk_45
end

function helper_E56D10(quest, me, native_arg_param_2)
    local cVar3, iVar7, iVar9, pCVar6
    local iVar8 = native_arg_param_2
    local pCVar4 = quest:GetHero()
    quest:EntityPostOpinionDeedToRecipient(pCVar4, iVar8, me)
    iVar7 = 0
    local pCVar5 = tostring(0)
    local xStack_24 = ("boy" .. pCVar5)
    local r1 = quest:GetThingWithScriptName(xStack_24)
    while (r1 ~= nil and not r1:IsNull()) do
        cVar3 = (r1 ~= nil and r1:IsAlive())
        if not cVar3 then break end
        pCVar6 = r1
        iVar9 = native_arg_param_2
        pCVar4 = quest:GetHero()
        quest:EntityPostOpinionDeedToRecipient(pCVar4, iVar9, pCVar6)
        iVar7 = iVar7 + 1
        pCVar5 = tostring(iVar7)
        pCVar5 = ("boy" .. pCVar5)
        xStack_24 = pCVar5
        pCVar6 = quest:GetThingWithScriptName(xStack_24)
        r1 = pCVar6
        pCVar6 = nil
    end
    iVar7 = 0
    while true do
        pCVar5 = tostring(iVar7)
        pCVar5 = ("girl" .. pCVar5)
        xStack_24 = pCVar5
        pCVar6 = quest:GetThingWithScriptName(xStack_24)
        r1 = pCVar6
        pCVar6 = nil
        if not (r1 ~= nil and not r1:IsNull()) then break end
        cVar3 = (r1 ~= nil and r1:IsAlive())
        if not cVar3 then break end
        pCVar6 = r1
        iVar9 = native_arg_param_2
        pCVar4 = quest:GetHero()
        quest:EntityPostOpinionDeedToRecipient(pCVar4, iVar9, pCVar6)
        iVar7 = iVar7 + 1
    end
    r1 = nil
end

function helper_E57020(quest, me, native_arg_param_1, native_arg_param_2)
    local resources = quest:RetailResources()
    local bVar1, cVar2, fret_0, fret_00, fret_01, fret_02, iVar5, pCVar4, pcVar7, this_00
    local alive = true
    if native_arg_param_1 == 0xf then
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
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK15_90D"
        elseif iVar5 < 0x6a5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK15_90A"
        elseif iVar5 < 0x8fd then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK15_90B"
        elseif iVar5 < 0xa29 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK15_90C"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK15_90B"
        end
        -- LAB_00e5750f: (native jump target)
        resources:SetString(native_arg_param_2, native_arg_param_1, pcVar7)
        return
    end
    if native_arg_param_1 == 0x11 then
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
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK17_20A"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK17_20B"
        end
        resources:SetString(native_arg_param_2, native_arg_param_1, pcVar7)
        return
    end
    if native_arg_param_1 == 0x13 then
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
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK19_80A"
        elseif not quest:GetMasterGameState("BanditCampTwinbladeKilled") then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK19_80B"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            native_arg_param_1 = "$ARG1"
            pcVar7 = "TEXT_CS_B16_BOOK19_80C"
        end
        resources:SetString(native_arg_param_2, native_arg_param_1, pcVar7)
        return
    end
    if native_arg_param_1 ~= 0x15 then
        if native_arg_param_1 == 0x16 then
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
                native_arg_param_1 = "$ARG1"
                pcVar7 = "TEXT_CS_B16_BOOK22_20B"
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                native_arg_param_1 = "$ARG1"
                pcVar7 = "TEXT_CS_B16_BOOK22_20A"
            end
        else
            if native_arg_param_1 ~= 0x17 then
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
                    native_arg_param_1 = "$ARG1"
                    pcVar7 = "TEXT_CS_B16_BOOK23_10C"
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    native_arg_param_1 = "$ARG1"
                    pcVar7 = "TEXT_CS_B16_BOOK23_10B"
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                native_arg_param_1 = "$ARG1"
                pcVar7 = "TEXT_CS_B16_BOOK23_10A"
            end
        end
        resources:SetString(native_arg_param_2, native_arg_param_1, pcVar7)
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    native_arg_param_1 = quest:GetMasterGameState("TeddySolution")
    pCVar4 = ("TEXT_CS_B16_BOOK21_20" .. native_arg_param_1)
    this_00 = native_arg_param_2
    resources:SetString(native_arg_param_2, "$ARG1", pCVar4)
    native_arg_param_2 = "$ARG2"
    pCVar4 = ("TEXT_CS_B16_BOOK21_30" .. native_arg_param_1)
    resources:SetString(this_00, native_arg_param_2, pCVar4)
    if native_arg_param_1 == 0 then
        bVar1 = false
        if not bVar1 then goto LAB_00e57345 end
        goto LAB_00e573aa
    else
        iVar5 = ((native_arg_param_1 == "A") and 0 or 1)
        cVar2 = not (iVar5 ~= 0)
        -- reviewed native scratch-byte spill: native_arg_param_2 = (map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)CONCAT31(native_arg_param_2._1_3_,cVar2);
        if cVar2 then goto LAB_00e573aa end
        goto LAB_00e57345
    end
    goto FLOW_past_lab_00e573aa
    ::LAB_00e573aa::
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then goto LAB_00e57381 end
    native_arg_param_2 = "$ARG3"
    pcVar7 = "TEXT_CS_B16_BOOK21_40A"
    ::FLOW_past_lab_00e573aa::
    goto FLOW_past_lab_00e57345
    ::LAB_00e57345::
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then goto LAB_00e57381 end
    native_arg_param_2 = "$ARG3"
    pcVar7 = "TEXT_CS_B16_BOOK21_40B"
    ::FLOW_past_lab_00e57345::
    resources:SetString(this_00, native_arg_param_2, pcVar7)
    ::LAB_00e57381::
end

function helper_E57530(quest, me)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar5, cVar6, ctr_CVar13, fVar4, fret_0, fret_00, fret_01, fret_02, native_scalar_18, pCVar1, pCVar10, pCVar12, pCVar8, pCVar9, pvVar11, r1, r2, r3, r4, uVar15, uVar16, uVar17, value, xStack_18_b3, x_stk_14
    local alive = true
    value = 0xffffffff
    ctr_CVar13 = 0
    xStack_18_b3 = 0
    native_scalar_18 = 0xffffffff
    if quest:GetStateInt("BooksInGame") < 1 then
        goto LAB_00e57678
    else
        repeat
            if (((ctr_CVar13 == __native_entity_state:GetStateInt("BooksWanted")) or (ctr_CVar13 == __native_entity_state:GetStateInt("BooksAccepted"))) or (ctr_CVar13 == __native_entity_state:GetStateInt("BooksComment"))) and (-1 < ((value + 2147483648) % 4294967296 - 2147483648)) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                break
            end
            pCVar8 = quest:GetHero()
            bVar5 = quest:IsObjectInThingsPossession(quest:GetStateStringVectorAt("BookDef", ctr_CVar13), pCVar8)
            value = native_scalar_18
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                if (ctr_CVar13 == quest:GetStateInt("LastBookRequested")) or ((quest:GetStateFlagVectorAt("BookDonated", ctr_CVar13) and 1 or 0) ~= 0) then
                    value = native_scalar_18
                    if native_scalar_18 == 0xffffffff then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        native_scalar_18 = 0xfffffffe
                        value = 0xfffffffe
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    __native_condition_1 = xStack_18_b3 == 0
                    if __native_condition_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        native_scalar_18 = ctr_CVar13
                        __native_condition_1 = bVar5
                    end
                    if __native_condition_1 then
                        return
                    end
                    value = native_scalar_18
                    if (quest:GetStateFlagVectorAt("BookOwned", ctr_CVar13) and 1 or 0) == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        quest:SetStateFlagVectorAt("BookOwned", ctr_CVar13, true)
                        xStack_18_b3 = 1
                    end
                end
            end
            ctr_CVar13 = ctr_CVar13 + 1
        until not (ctr_CVar13 < quest:GetStateInt("BooksInGame"))
        if value ~= 0xfffffffe then goto LAB_00e57678 end
    end
    goto FLOW_past_lab_00e57678
    ::LAB_00e57678::
    if ((value + 2147483648) % 4294967296 - 2147483648) < __native_entity_state:GetStateInt("BooksComment") then
        if value ~= 0xffffffff then
            if __native_entity_state:GetStateInt("BooksAccepted") <= ((value + 2147483648) % 4294967296 - 2147483648) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                pCVar12 = resources:MemberResource("seh_me", me)
                pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
                fret_00 = quest:GetHealth(pCVar9)
                fVar4 = 0.0
                if fret_00 <= fVar4 then
                    return
                end
                x_stk_14 = nil --[[reviewed native dispatch staging]]
                uVar17 = 1
                uVar16 = 0
                uVar15 = 0
                pCVar10 = tostring(((value + 2147483648) % 4294967296 - 2147483648) - __native_entity_state:GetStateInt("BooksAccepted"))
                pCVar10 = ("TEXT_QST_B16_BOOK_COMMENT_" .. pCVar10)
                pvVar11 = pCVar10
                pCVar8 = quest:GetHero()
                r1 = me:Speak(pCVar8, pvVar11, 0, false, true, false)
                cVar6 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                if cVar6 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        cVar6 = resources:IsPerformingScriptTask(resources:MemberResource("seh_me", (me)))
                    until not (cVar6)
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
            pCVar12 = tostring(((value + 2147483648) % 4294967296 - 2147483648))
            x_stk_14 = ("TEXT_QST_B16_BOOK_REQUEST_" .. pCVar12)
            pCVar1 = resources:MemberResource("seh_me", me)
            pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
            fret_01 = quest:GetHealth(pCVar9)
            fVar4 = 0.0
            if fVar4 < fret_01 then
                uVar17 = 1
                uVar16 = 0
                uVar15 = 0
                pvVar11 = x_stk_14
                pCVar8 = quest:GetHero()
                r2 = me:Speak(pCVar8, pvVar11, uVar15, (uVar16 ~= 0), (uVar17 ~= 0), (fVar4 ~= 0))
                if not r2 then
                    goto LAB_00e57924
                end
                cVar6 = me:IsPerformingScriptTask()
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e57924 end
                    cVar6 = me:IsPerformingScriptTask()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                value = native_scalar_18
                if bVar5 then goto LAB_00e57924 end
            end
            helper_E55CE0(quest, me, value, "TEXT_QST_B16_BOOK_REFUSED")
            ::LAB_00e57924::
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        pCVar1 = resources:MemberResource("seh_me", me)
        pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
        fret_0 = quest:GetHealth(pCVar9)
        fVar4 = 0.0
        if fret_0 <= fVar4 then
            return
        end
        uVar17 = 1
        uVar16 = 0
        uVar15 = 2
        pCVar8 = quest:GetHero()
        r3 = me:Speak(pCVar8, "TEXT_QST_B16_BOOK_NONE_FOUND", 2, false, true, false)
        cVar6 = me:IsPerformingScriptTask()
        if cVar6 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                cVar6 = me:IsPerformingScriptTask()
            until not (cVar6)
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
    pCVar1 = resources:MemberResource("seh_me", me)
    pCVar9 = resources:ScriptThing(resources:MemberResource("seh_me", (me)))
    fret_02 = quest:GetHealth(pCVar9)
    fVar4 = 0.0
    if fret_02 <= fVar4 then
        return
    end
    uVar17 = 1
    uVar16 = 0
    uVar15 = 2
    pCVar8 = quest:GetHero()
    r4 = me:Speak(pCVar8, "TEXT_QST_B16_BOOK_NOT_FOUND", 2, false, true, false)
    if not r4 then
        return
    end
    cVar6 = me:IsPerformingScriptTask()
    while cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        cVar6 = me:IsPerformingScriptTask()
    end
    ::LAB_00e579ce::
    alive = not quest:IsActiveThreadTerminating()
end

