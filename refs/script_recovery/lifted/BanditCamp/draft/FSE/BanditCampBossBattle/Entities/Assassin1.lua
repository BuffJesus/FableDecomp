-- Generated native draft: Assassin1. Review coverage report before use.
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
    local bVar5, cVar6, c_stk_c5, c_stk_c6, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, iVar10, iVar11, iVar12, iVar13, i_stk_a0, pCVar7, pcVar9, piVar1, r1, r2, r3, r4, r5, r6, r7, r8, this_00, uVar4, uVar8, u_stk_b4, xStack_b0, xStack_c4, xStack_d8, xStack_e8, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_c
    local alive = true
    local function __cleanup_LAB_00d05880()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_c4)
        resources:ReleaseResource(xStack_e8)
    end
    u_stk_b4 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        xStack_e8 = resources:NewResource()
        resources:PrepareResource(xStack_e8)
        bVar5 = resources:TryAcquire(xStack_e8, me, 4)
        while not bVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d058f3 end
            bVar5 = resources:TryAcquire(xStack_e8, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            cVar6 = quest:GetStateBool("AssassinsUnderAttack")
            uVar4 = 0
            c_stk_c5 = bVar5
            while (not cVar6 and (not quest:GetStateBool("AssassinCutsceneTriggered"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d058f3 end
                uVar8 = uVar4 | 1
                u_stk_b4 = uVar8
                cVar6 = me:MsgIsHitByHero()
                if not cVar6 then
                    uVar8 = uVar4 | 3
                    u_stk_b4 = uVar8
                    -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
                    bVar5 = nil --[[unresolved native value]]
                    if bVar5 then
                        uVar8 = uVar4 | 7
                        u_stk_b4 = uVar8
                        -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
                        bVar5 = nil --[[unresolved native value]]
                        if not bVar5 then goto LAB_00d04cdc end
                    end
                    c_stk_c6 = 0
                else
                    goto LAB_00d04cdc
                end
                goto FLOW_past_lab_00d04cdc
                ::LAB_00d04cdc::
                c_stk_c6 = 1
                ::FLOW_past_lab_00d04cdc::
                if (uVar8 & 4) ~= 0 then
                    uVar8 = uVar8 & 0xfffffffb
                    u_stk_b4 = uVar8
                end
                if (uVar8 & 2) ~= 0 then
                    uVar8 = uVar8 & 0xfffffffd
                    u_stk_b4 = uVar8
                end
                if (uVar8 & 1) ~= 0 then
                    u_stk_b4 = uVar8 & 0xfffffffe
                end
                if c_stk_c6 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d058f3 end
                    quest:SetStateBool("AssassinsUnderAttack", true)
                end
                -- TODO(native): bVar5 = (**(*me + 0x6c))(me,"SCRIPT_NAME_HERO")
                bVar5 = nil --[[unresolved native value]]
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d058f3 end
                    if not quest:GetStateBool("Gate3Open") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d058f3 end
                        if not c_stk_c5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                xStack_d8 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_54 = resources:ScriptThing(xStack_e8)
                                pCVar7 = x_stk_54
                                fret_00 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_00 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar9 = "TEXT_QST_009_ASSASSIN1_INTRO"
                                    pCVar7 = quest:GetHero()
                                    r1 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00d0584f end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar6 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00d0580f
                                    end
                                end
                                quest:GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                                while iVar10 < 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00d0584f end
                                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if iVar10 == 1 then
                                        if not bVar5 then
                                            i_stk_a0 = quest:GetHeroGold()
                                            if i_stk_a0 < quest:ReadGlobalGameDataFloat(0xe80) then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if not bVar5 then
                                                    x_stk_3c = resources:ScriptThing(xStack_e8)
                                                    pCVar7 = x_stk_3c
                                                    fret_01 = quest:GetHealth(pCVar7)
                                                    fVar3 = 0.0
                                                    if fVar3 < fret_01 then
                                                        iVar13 = 0
                                                        iVar12 = 1
                                                        iVar11 = 0
                                                        iVar10 = 0
                                                        pcVar9 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR"
                                                        pCVar7 = quest:GetHero()
                                                        r2 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                                        iVar10 = me:IsPerformingScriptTask()
                                                        cVar6 = iVar10
                                                        while cVar6 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar5 = not alive
                                                            if bVar5 then goto LAB_00d0584f end
                                                            iVar10 = me:IsPerformingScriptTask()
                                                            cVar6 = iVar10
                                                        end
                                                        goto LAB_00d05364
                                                    end
                                                    goto LAB_00d05373
                                                end
                                                goto LAB_00d0584f
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if not bVar5 then
                                                x_stk_24 = resources:ScriptThing(xStack_e8)
                                                pCVar7 = x_stk_24
                                                fret_02 = quest:GetHealth(pCVar7)
                                                fVar3 = 0.0
                                                if fVar3 < fret_02 then
                                                    iVar13 = 0
                                                    iVar12 = 1
                                                    iVar11 = 0
                                                    iVar10 = 0
                                                    pcVar9 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES"
                                                    pCVar7 = quest:GetHero()
                                                    r3 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                                    iVar10 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar10
                                                    while cVar6 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then goto LAB_00d0584f end
                                                        iVar10 = me:IsPerformingScriptTask()
                                                        cVar6 = iVar10
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00d0584f end
                                                end
                                                quest:SetStateBool("AssassinCutsceneTriggered", true)
                                                iVar11 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xe80)))
                                                quest:GiveHeroGold(iVar11)
                                                quest:GiveHeroExperience(quest:ReadGlobalGameData(0x3c))
                                                goto LAB_00d05373
                                            end
                                        end
                                        -- LAB_00d05804: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00d0580f
                                    end
                                    if not bVar5 then
                                        x_stk_c = resources:ScriptThing(xStack_e8)
                                        pCVar7 = x_stk_c
                                        fret_03 = quest:GetHealth(pCVar7)
                                        fVar3 = 0.0
                                        if fVar3 < fret_03 then
                                            iVar13 = 0
                                            iVar12 = 1
                                            iVar11 = 0
                                            iVar10 = 0
                                            pcVar9 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO"
                                            pCVar7 = quest:GetHero()
                                            r4 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                            iVar10 = me:IsPerformingScriptTask()
                                            cVar6 = iVar10
                                            while cVar6 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00d0584f end
                                                iVar10 = me:IsPerformingScriptTask()
                                                cVar6 = iVar10
                                            end
                                            goto LAB_00d05364
                                        end
                                        goto FLOW_hoist_lab_00d05364_1
                                    end
                                    goto FLOW_past_lab_00d05364
                                    ::LAB_00d05364::
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00d0584f end
                                    ::FLOW_hoist_lab_00d05364_1::
                                    ::LAB_00d05373::
                                    quest:SetStateBool("TalkedToAssassin", true)
                                    c_stk_c5 = 1
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_d8
                                    goto LAB_00d057a5
                                    ::FLOW_past_lab_00d05364::
                                end
                                ::LAB_00d0584f::
                                quest:PauseAllNonScriptedEntities(false)
                                ::LAB_00d0580f::
                                resources:DestroyMovie(xStack_d8)
                                resources:ReleaseResource(xStack_e8)
                                return
                            end
                            goto LAB_00d058f3
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d058f3 end
                        xStack_c4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar10 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00d05880(); return end
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            __cleanup_LAB_00d05880()
                            return
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if iVar10 == 1 then
                            if bVar5 then
                                -- LAB_00d0585e: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_c4)
                                resources:ReleaseResource(xStack_e8)
                                return
                            end
                            i_stk_a0 = quest:GetHeroGold()
                            if quest:ReadGlobalGameDataFloat(0xe80) <= i_stk_a0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00d05880(); return end
                                x_stk_18 = resources:ScriptThing(xStack_e8)
                                pCVar7 = x_stk_18
                                fret_05 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_05 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar9 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES"
                                    pCVar7 = quest:GetHero()
                                    r5 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then __cleanup_LAB_00d05880(); return end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar6 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        -- LAB_00d0586c: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_c4)
                                        resources:ReleaseResource(xStack_e8)
                                        return
                                    end
                                end
                                quest:SetStateBool("AssassinCutsceneTriggered", true)
                                iVar11 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(0xe80)))
                                quest:GiveHeroGold(iVar11)
                                quest:GiveHeroExperience(quest:ReadGlobalGameData(0x3c))
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00d05880(); return end
                                x_stk_48 = resources:ScriptThing(xStack_e8)
                                pCVar7 = x_stk_48
                                fret_04 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_04 then
                                    iVar13 = 0
                                    iVar12 = 1
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar9 = "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR"
                                    pCVar7 = quest:GetHero()
                                    r6 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then __cleanup_LAB_00d05880(); return end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar6 = iVar10
                                    end
                                    goto LAB_00d05774
                                end
                            end
                        else
                            if bVar5 then __cleanup_LAB_00d05880(); return end
                            x_stk_30 = resources:ScriptThing(xStack_e8)
                            pCVar7 = x_stk_30
                            fret_06 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_06 then
                                iVar13 = 0
                                iVar12 = 1
                                iVar11 = 0
                                iVar10 = 0
                                pcVar9 = "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO"
                                pCVar7 = quest:GetHero()
                                r7 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                                iVar10 = me:IsPerformingScriptTask()
                                cVar6 = iVar10
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then __cleanup_LAB_00d05880(); return end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar6 = iVar10
                                end
                                goto LAB_00d05774
                            end
                        end
                        goto FLOW_past_lab_00d05774
                        ::LAB_00d05774::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00d05880(); return end
                        ::FLOW_past_lab_00d05774::
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_c4
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d058f3 end
                        xStack_b0 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_60 = resources:ScriptThing(xStack_e8)
                        pCVar7 = x_stk_60
                        fret_0 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            iVar13 = 0
                            iVar12 = 1
                            iVar11 = 0
                            iVar10 = 0
                            pcVar9 = "TEXT_QST_009_ASSASSIN1_NOT_NEEDED"
                            pCVar7 = quest:GetHero()
                            r8 = me:Speak(pCVar7, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                            iVar10 = me:IsPerformingScriptTask()
                            cVar6 = iVar10
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_b0)
                                    resources:ReleaseResource(xStack_e8)
                                    return
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar6 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00d057c8: (native jump target)
                                resources:DestroyMovie(xStack_b0)
                                resources:ReleaseResource(xStack_e8)
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_b0
                    end
                    ::LAB_00d057a5::
                end
                cVar6 = quest:GetStateBool("AssassinsUnderAttack")
                uVar4 = u_stk_b4
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                if (not quest:GetStateBool("AssassinCutsceneTriggered")) and (not quest:GetStateBool("AssassinsUnderAttack")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                        until not (not bVar5)
                        resources:ReleaseResource(xStack_e8)
                        return
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        quest:ClearThingHasInformation(me)
                        resources:PrepareResource(xStack_e8)
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                        until not (not bVar5)
                    end
                end
            end
        end
        ::LAB_00d058f3::
        resources:ReleaseResource(xStack_e8)
    end
end

function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

