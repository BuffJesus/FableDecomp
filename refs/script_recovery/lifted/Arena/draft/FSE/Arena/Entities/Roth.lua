-- Generated native draft: Roth. Review coverage report before use.
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
    local CVar2, bVar3, cVar4, dist, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, iVar11, iVar20, iVar23, iVar7, native_arg_switch_2, native_arg_switch_3, p1, pCVar10, pCVar12, pCVar15, pCVar16, pCVar18, pCVar19, pCVar22, pCVar22_b3, pCVar5, pCVar6, pQuestName, pThing, pThing1, pcVar13, ppuVar24, uVar14, uVar17, uVar21, xStack_128, xStack_144, xStack_88, xStack_98, xStack_b0, xStack_bc, xStack_c8
    local alive = true
    local function __cleanup_LAB_00f22cd8()
        quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
        pCVar12 = xStack_128
        resources:DestroyMovie(pCVar12)
        quest:DeregisterTimer(iVar7)
        resources:DestroyMovie(xStack_144)
    end
    local function __cleanup_LAB_00f22cec()
        quest:PauseAllNonScriptedEntities((pCVar15 ~= 0))
        pCVar12 = xStack_128
        resources:DestroyMovie(pCVar12)
        quest:DeregisterTimer(iVar7)
        resources:DestroyMovie(xStack_144)
    end
    local function __cleanup_LAB_00f22d1e()
        pCVar12 = xStack_98
        resources:DestroyMovie(pCVar12)
        quest:DeregisterTimer(iVar7)
        resources:DestroyMovie(xStack_144)
    end
    local function __cleanup_LAB_00f22d25()
        resources:DestroyMovie(pCVar12)
        quest:DeregisterTimer(iVar7)
        resources:DestroyMovie(xStack_144)
    end
    ppuVar24 = resources:NewResource()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    iVar23 = quest:GetStateInt("ArenaState")
    while iVar23 == 2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then return end
        iVar23 = quest:GetStateInt("ArenaState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00f22d33: (native jump target)
        resources:DestroyMovie(xStack_144)
        return
    end
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    pCVar5 = quest:GetThingWithScriptName("RothPoint")
    pCVar6 = pCVar5:GetPos()
    CVar2 = pCVar6.x
    p1 = nil
    quest:SetWanderCentrePoint(me, pCVar6)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 4.0)
    quest:SetScriptingStateGroup(me, 4)
    quest:SetThingHasInformation(me, false, true, false)
    iVar7 = quest:RegisterTimer()
    iVar11 = 0
    iVar23 = iVar7
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            quest:DeregisterTimer(iVar7)
            return
        end
        if quest:GetStateBool("PlayerLeaving") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar7)
                resources:ReleaseResource(xStack_144)
                return
            end
            pQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_ARENA_OBJECTIVE_05", "Arena", "KnotholeGlade")
            resources:PrepareResource(xStack_144)
            bVar3 = resources:TryAcquire(xStack_144, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar7)
                    resources:ReleaseResource(xStack_144)
                    return
                end
                bVar3 = resources:TryAcquire(xStack_144, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar7)
                resources:ReleaseResource(xStack_144)
                return
            end
            xStack_b0 = resources:NewResource()
            resources:PrepareResource(xStack_b0)
            iVar20 = 4
            pCVar16 = xStack_b0
            pCVar5 = quest:GetHero()
            bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar20)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f22c81 end
                iVar20 = 4
                pCVar16 = xStack_b0
                pCVar5 = quest:GetHero()
                bVar3 = resources:TryAcquire(pCVar16, pCVar5, iVar20)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                xStack_bc = resources:NewActorMap()
                resources:SetActor(xStack_bc, "Hero", xStack_b0)
                -- TODO(native): resources:SetActor(xStack_bc, "Roth", &xStack_144)
                xStack_c8 = resources:NewStringMap()
                native_arg_switch_2 = quest:GetStateInt("GoldMultiplier")
                repeat
                    if native_arg_switch_2 == 0 or native_arg_switch_2 == 1 then
                        pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_SHORT_ARENA"
                        resources:SetString(xStack_c8, "$SAY", pcVar13)
                        break
                    else
                        if native_arg_switch_2 == 2 or native_arg_switch_2 == 3 then
                            pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_MIDDLE_ARENA"
                            resources:SetString(xStack_c8, "$SAY", pcVar13)
                            break
                        else
                            if native_arg_switch_2 == 4 then
                                pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_GOOD_ARENA"
                                resources:SetString(xStack_c8, "$SAY", pcVar13)
                                break
                            else
                                pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_EXCELLENT_ARENA"
                                resources:SetString(xStack_c8, "$SAY", pcVar13)
                            end
                        end
                    end
                until not (false)
                xStack_88 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                pCVar5 = 0x0
                resources:RunMacroWithStrings("CS_ARENA_ROTHWIMP", xStack_bc, xStack_c8, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:SetStateBool("PlayerLeaving", false)
                __native_entity_state:SetStateBool("EarlyTalk", true)
                __native_entity_state:SetStateBool("ChamTalk", true)
                resources:PrepareResource(xStack_144)
                quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
                resources:DestroyMovie(xStack_88)
                resources:DestroyStringMap(xStack_c8)
                resources:DestroyActorMap(xStack_bc)
                resources:ReleaseResource(xStack_b0)
                goto LAB_00f21f12
            end
            ::LAB_00f22c81::
            resources:ReleaseResource(xStack_b0)
            quest:DeregisterTimer(iVar7)
            resources:DestroyMovie(xStack_144)
            return
        end
        ::LAB_00f21f12::
        if quest:GetStateInt("ArenaState") ~= 5 then goto LAB_00f221ae end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00f22c92: (native jump target)
            quest:DeregisterTimer(iVar7)
            resources:ReleaseResource(xStack_144)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if iVar11 == 2 then
            if not bVar3 then
                dist = 5.0
                pCVar10 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, dist)
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        pcVar13 = quest:AddNewConversation(me, false, false)
                        pCVar5 = quest:GetHero()
                        quest:AddPersonToConversation(pcVar13, pCVar5)
                        pCVar10 = quest:GetHero()
                        pCVar5 = "TEXT_QST_005_V2_ARENA_ROTH_COME_ON"
                        quest:AddLineToConversation(pcVar13, pCVar5, me, pCVar10, false)
                        iVar11 = 1
                        quest:SetTimer(iVar23, 10)
                        pCVar10 = quest:GetThingWithScriptName("RothPoint")
                        pCVar6 = pCVar10:GetPos()
                        uVar14 = pCVar6.x
                        uVar17 = pCVar6.y
                        uVar21 = pCVar6.z
                        -- TODO(native): center._4_4_ = uVar17;
                        -- TODO(native): center._0_4_ = uVar14;
                        -- TODO(native): center._8_4_ = uVar21;
                        quest:SetWanderCentrePoint(me, pCVar6)
                        iVar7 = iVar23
                        iVar23 = iVar7
                        goto LAB_00f221ae
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        iVar11 = quest:GetTimer(iVar7)
                        if 0 < iVar11 then goto LAB_00f221ae end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            pCVar10 = quest:GetHero()
                            pCVar6 = pCVar10:GetPos()
                            uVar14 = pCVar6.x
                            uVar17 = pCVar6.y
                            uVar21 = pCVar6.z
                            -- TODO(native): CVar2._4_4_ = uVar17;
                            -- TODO(native): CVar2._0_4_ = uVar14;
                            -- TODO(native): CVar2._8_4_ = uVar21;
                            quest:SetWanderCentrePoint(me, pCVar6)
                            goto LAB_00f220f0
                        end
                    end
                end
            end
            -- LAB_00f22d2a: (native jump target)
            quest:DeregisterTimer(iVar7)
            resources:DestroyMovie(xStack_144)
            return
        end
        if bVar3 then
            quest:DeregisterTimer(iVar7)
            resources:DestroyMovie(xStack_144)
            return
        end
        iVar20 = quest:GetTimer(iVar7)
        if (iVar20 < 1) and (iVar11 == 1) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar7)
                resources:DestroyMovie(xStack_144)
                return
            end
            pCVar10 = quest:GetHero()
            pCVar6 = pCVar10:GetPos()
            uVar14 = pCVar6.x
            uVar17 = pCVar6.y
            uVar21 = pCVar6.z
            -- TODO(native): center_00._4_4_ = uVar17;
            -- TODO(native): center_00._0_4_ = uVar14;
            -- TODO(native): center_00._8_4_ = uVar21;
            quest:SetWanderCentrePoint(me, pCVar6)
            iVar11 = 2
            goto LAB_00f220f0
        else
            if iVar11 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar7)
                    resources:DestroyMovie(xStack_144)
                    return
                end
                quest:SetTimer(iVar7, 10)
                iVar11 = 1
            end
        end
        goto FLOW_past_lab_00f220f0
        ::LAB_00f220f0::
        quest:SetTimer(iVar7, 3)
        ::FLOW_past_lab_00f220f0::
        ::LAB_00f221ae::
        bVar3 = me:IsTalkedToByHero()
        pCVar22 = CONCAT13(bVar3,(int3)in_stack_fffffeb4)
        if not pCVar22_b3 then
            bVar3 = me:MsgIsHitByHero()
            if bVar3 then
            else
                bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar3 then
                    bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar3 then goto LAB_00f22979 end
                end
            end
            ::LAB_00f22979::
            if (pCVar22 & 0xffffff >> 0x18) ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar7)
                    resources:DestroyMovie(xStack_144)
                    return
                end
                resources:PrepareResource(xStack_144)
                bVar3 = resources:TryAcquire(xStack_144, me, 4)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:DeregisterTimer(iVar7)
                        resources:DestroyMovie(xStack_144)
                        return
                    end
                    bVar3 = resources:TryAcquire(xStack_144, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar7)
                    resources:DestroyMovie(xStack_144)
                    return
                end
                iVar11 = me:IsPerformingScriptTask()
                if iVar11 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:DeregisterTimer(iVar7)
                        resources:DestroyMovie(xStack_144)
                        return
                    end
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:DeregisterTimer(iVar7)
                        resources:DestroyMovie(xStack_144)
                        return
                    end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    xStack_98 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar5 = resources:ScriptThing(xStack_144)
                    fret_07 = quest:GetHealth(pCVar5)
                    pCVar22 = CONCAT13(1,(int3)(pCVar22 & 0xffffff))
                    if fret_07 <= 0.0 then
                        pCVar22 = (pCVar22 & 0xffffff & 0xffffff)
                    end
                    if pCVar22_b3 then
                        bVar3 = false
                        pCVar19 = 0x1
                        pCVar18 = 0x0
                        pCVar15 = 0x0
                        pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_ATTACKED"
                        p1 = quest:GetHero()
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- TODO(native): (**(code **)(*(int *)xStack_12c + 0x5ec))();
                                __cleanup_LAB_00f22d1e(); return
                            end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar4 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            -- TODO(native): (**(code **)(*(int *)xStack_12c + 0x5ec))();
                            __cleanup_LAB_00f22d1e()
                            return
                        end
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    pCVar10 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar10)
                    pThing1 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pThing1, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    -- TODO(native): (**(code **)(*(int *)xStack_12c + 0x5ec))();
                    pCVar12 = xStack_98
                    goto LAB_00f22bf5
                end
                goto LAB_00f22bfa
            end
            goto LAB_00f22c10
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00f22cb5: (native jump target)
            quest:DeregisterTimer(iVar7)
            resources:ReleaseResource(xStack_144)
            return
        end
        resources:PrepareResource(xStack_144)
        bVar3 = resources:TryAcquire(xStack_144, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar7)
                resources:DestroyMovie(xStack_144)
                return
            end
            bVar3 = resources:TryAcquire(xStack_144, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(iVar7)
            resources:DestroyMovie(xStack_144)
            return
        end
        iVar11 = me:IsPerformingScriptTask()
        if iVar11 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar7)
                resources:DestroyMovie(xStack_144)
                return
            end
        end
        xStack_128 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        iVar11 = quest:GetStateInt("ArenaState")
        if (iVar11 == 3) and (not __native_entity_state:GetStateBool("EarlyTalk")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_0 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_0 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_FIRST_CHAT_PRE_CHAM"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
                            pCVar12 = xStack_128
                            __cleanup_LAB_00f22d25(); return
                        end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then __cleanup_LAB_00f22cec(); return end
                end
                __native_entity_state:SetStateBool("EarlyTalk", true)
                goto FLOW_native_label_1
            end
            __cleanup_LAB_00f22cec()
            return
        end
        if (iVar11 == 5) and (not __native_entity_state:GetStateBool("ChamTalk")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_00 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_00 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_FIRST_CHAT_POST_CHAM"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cec(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then __cleanup_LAB_00f22cd8(); return end
                end
                __native_entity_state:SetStateBool("ChamTalk", true)
                goto FLOW_native_label_1
            end
            __cleanup_LAB_00f22cd8()
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then __cleanup_LAB_00f22cec(); return end
        native_arg_switch_3 = __native_entity_state:GetStateInt("HintNumber")
        if not (native_arg_switch_3 == 0 or native_arg_switch_3 == 1 or native_arg_switch_3 == 2 or native_arg_switch_3 == 3 or native_arg_switch_3 == 4 or native_arg_switch_3 == 5) then
            native_arg_switch_3 = 0x7ffffffe
        end
        repeat
            if native_arg_switch_3 == 0 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_01 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_01 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_FIRST_HINT"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cd8(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    goto LAB_00f22807
                end
                goto FLOW_hoist_lab_00f22807_1
            end
            goto FLOW_past_lab_00f22807
            ::LAB_00f22807::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then __cleanup_LAB_00f22cec(); return end
            ::FLOW_hoist_lab_00f22807_1::
            break
            ::FLOW_past_lab_00f22807::
            if native_arg_switch_3 == 1 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_02 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_02 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_SECOND_HINT"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cd8(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    goto LAB_00f22807
                end
                break
            end
            if native_arg_switch_3 == 2 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_03 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_03 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_THIRD_HINT"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cd8(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    goto LAB_00f22807
                end
                break
            end
            if native_arg_switch_3 == 3 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_04 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_04 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_FOURTH_HINT"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cd8(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    goto LAB_00f22807
                end
                break
            end
            if native_arg_switch_3 == 4 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_05 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_05 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_FIFTH_HINT"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cd8(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    goto LAB_00f22807
                end
                break
            end
            if native_arg_switch_3 == 5 then
                pCVar10 = resources:ScriptThing(xStack_144)
                fret_06 = quest:GetHealth(pCVar10)
                -- TODO(native): pCVar22_b3 = !(fret_06 <= (float10)0.0);
                if pCVar22_b3 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar15 = 0x0
                    pCVar5 = 0x0
                    pcVar13 = "TEXT_QST_005_V2_ARENA_ROTH_SIXTH_HINT"
                    p1 = quest:GetHero()
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00f22cd8(); return end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar4 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then __cleanup_LAB_00f22cec(); return end
                end
                __native_entity_state:SetStateInt("HintNumber", 4)
                native_arg_switch_3 = 0x7ffffffe
            end
            if native_arg_switch_3 == 0x7ffffffe then goto FLOW_native_label_1 end
        until not (false)
        __native_entity_state:SetStateInt("HintNumber", __native_entity_state:GetStateInt("HintNumber") + 1)
        ::FLOW_native_label_1::
        quest:PauseAllNonScriptedEntities((native_arg_switch_3 ~= 0))
        pCVar12 = xStack_128
        ::LAB_00f22bf5::
        resources:ReleaseResource(pCVar12)
        ::LAB_00f22bfa::
        resources:PrepareResource(xStack_144)
        ::LAB_00f22c10::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("EarlyTalk", false)
    __native_entity_state:SetStateBool("ChamTalk", false)
    __native_entity_state:SetStateInt("HintNumber", 0)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

function OnPersist(quest, me, context)
    local earlyTalk = quest:GetStateBool("EarlyTalk") or false
    earlyTalk = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    quest:SetStateBool("EarlyTalk", earlyTalk)
    local chamTalk = quest:GetStateBool("ChamTalk") or false
    chamTalk = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
    quest:SetStateBool("ChamTalk", chamTalk)
    local hintNumber = quest:GetStateInt("HintNumber") or 0
    hintNumber = quest:PersistTransferInt(context, "HintNumber", hintNumber)
    quest:SetStateInt("HintNumber", hintNumber)
end

function OnPredicateFail(quest, me)
end

