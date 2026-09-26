-- Generated native draft: CellWhisper. Review coverage report before use.
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
    local bVar5, cVar6, c_stk_85, center, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar9, native_arg_switch_1, pCVar12, pCVar13, pCVar14, pCVar7, pCVar8, pThing, pThing1, pcVar11, ppuVar15, this_00, uVar10, uVar4, u_stk_84, xStack_4c, xStack_80, xStack_9c
    local alive = true
    u_stk_84 = 0
    ppuVar15 = resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    pCVar7 = quest:GetThingWithScriptName("FlickPoint")
    pCVar8 = pCVar7:GetPos()
    center = pCVar8.x
    quest:SetWanderCentrePoint(me, pCVar8)
    pCVar7 = nil
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    quest:SetScriptingStateGroup(me, 4)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    while not bVar5 do
        bVar5 = me:IsTalkedToByHero()
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then break end
            resources:PrepareResource(xStack_9c)
            bVar5 = resources:TryAcquire(xStack_9c, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00f17b3f end
                bVar5 = resources:TryAcquire(xStack_9c, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00f17b3f end
            iVar9 = me:IsPerformingScriptTask()
            if iVar9 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00f17b3f end
            end
            xStack_80 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            iVar9 = math.random(0, 32767)
            native_arg_switch_1 = iVar9 % (quest:GetStateInt("ArenaRound") + -2)
            repeat
                if native_arg_switch_1 == 0 then
                    pCVar7 = resources:ScriptThing(xStack_9c)
                    fret_0 = quest:GetHealth(pCVar7)
                    fVar3 = 0.0
                    if fret_0 <= fVar3 then break end
                    bVar5 = false
                    pCVar14 = 0x1
                    pCVar13 = 0x0
                    pCVar12 = 0x0
                    pcVar11 = "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_THIRD_ROUND"
                    pCVar7 = quest:GetHero()
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00f17af0 end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    goto LAB_00f17734
                else
                    if native_arg_switch_1 == 1 then
                        pCVar7 = resources:ScriptThing(xStack_9c)
                        fret_00 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            bVar5 = false
                            pCVar14 = 0x1
                            pCVar13 = 0x0
                            pCVar12 = 0x0
                            pcVar11 = "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_FOURTH_ROUND"
                            pCVar7 = quest:GetHero()
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00f17af0 end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                            end
                            goto LAB_00f17734
                        end
                        break
                    else
                        if native_arg_switch_1 == 2 then
                            pCVar7 = resources:ScriptThing(xStack_9c)
                            fret_01 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_01 then
                                bVar5 = false
                                pCVar14 = 0x1
                                pCVar13 = 0x0
                                pCVar12 = 0x0
                                pcVar11 = "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_FIFTH_ROUND"
                                pCVar7 = quest:GetHero()
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00f17af0 end
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar6 = iVar9
                                end
                                goto LAB_00f17734
                            end
                            break
                        else
                            if native_arg_switch_1 == 3 then
                                pCVar7 = resources:ScriptThing(xStack_9c)
                                fret_02 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_02 then
                                    bVar5 = false
                                    pCVar14 = 0x1
                                    pCVar13 = 0x0
                                    pCVar12 = 0x0
                                    pcVar11 = "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_SIXTH_ROUND"
                                    pCVar7 = quest:GetHero()
                                    iVar9 = me:IsPerformingScriptTask()
                                    cVar6 = iVar9
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00f17af0 end
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar6 = iVar9
                                    end
                                    goto LAB_00f17734
                                end
                                break
                            else
                                if native_arg_switch_1 == 4 then
                                    pCVar7 = resources:ScriptThing(xStack_9c)
                                    fret_03 = quest:GetHealth(pCVar7)
                                    fVar3 = 0.0
                                    if fVar3 < fret_03 then
                                        bVar5 = false
                                        pCVar14 = 0x1
                                        pCVar13 = 0x0
                                        pCVar12 = 0x0
                                        pcVar11 = "TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_SEVENTH_ROUND"
                                        pCVar7 = quest:GetHero()
                                        iVar9 = me:IsPerformingScriptTask()
                                        cVar6 = iVar9
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00f17af0 end
                                            iVar9 = me:IsPerformingScriptTask()
                                            cVar6 = iVar9
                                        end
                                        goto LAB_00f17734
                                    end
                                end
                            end
                        end
                    end
                end
                goto FLOW_past_lab_00f17734
                ::LAB_00f17734::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then break end
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_80
                goto LAB_00f17b36
                ::FLOW_past_lab_00f17734::
            until not (false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_80)
            goto LAB_00f17a59
        else
            uVar4 = u_stk_84
            u_stk_84 = u_stk_84 | 1
            bVar5 = me:MsgIsHitByHero()
            if bVar5 then
                goto LAB_00f177f7
            else
                uVar10 = uVar4 | 3
                u_stk_84 = uVar10
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    uVar10 = uVar4 | 7
                    u_stk_84 = uVar10
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00f177f7 end
                end
                c_stk_85 = 0
            end
            goto FLOW_past_lab_00f177f7
            ::LAB_00f177f7::
            c_stk_85 = 1
            ::FLOW_past_lab_00f177f7::
            if (uVar10 & 4) ~= 0 then
                uVar10 = uVar10 & 0xfffffffb
                u_stk_84 = uVar10
            end
            if (uVar10 & 2) ~= 0 then
                uVar10 = uVar10 & 0xfffffffd
                u_stk_84 = uVar10
            end
            if (uVar10 & 1) ~= 0 then
                u_stk_84 = uVar10 & 0xfffffffe
            end
            if c_stk_85 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00f17b3f end
                resources:PrepareResource(xStack_9c)
                bVar5 = resources:TryAcquire(xStack_9c, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00f17b3f end
                    bVar5 = resources:TryAcquire(xStack_9c, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00f17b3f end
                iVar9 = me:IsPerformingScriptTask()
                if iVar9 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00f17b3f end
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00f17b3f end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    xStack_4c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar7 = resources:ScriptThing(xStack_9c)
                    fret_04 = quest:GetHealth(pCVar7)
                    fVar3 = 0.0
                    if fVar3 < fret_04 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_WHISPER_ENEMY_BEENHITATSTART_10"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00f17b32
                            end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00f17b32
                        end
                        goto FLOW_past_lab_00f17b32
                        ::LAB_00f17b32::
                        this_00 = xStack_4c
                        goto LAB_00f17b36
                        ::FLOW_past_lab_00f17b32::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    pCVar7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar7)
                    pThing1 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pThing1, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_4c)
                end
                goto LAB_00f17a59
            end
        end
        goto FLOW_past_lab_00f17a59
        ::LAB_00f17a59::
        resources:PrepareResource(xStack_9c)
        ::FLOW_past_lab_00f17a59::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    end
    do return end
    ::LAB_00f17af0::
    quest:PauseAllNonScriptedEntities(false)
    this_00 = xStack_80
    ::LAB_00f17b36::
    resources:DestroyMovie(this_00)
    ::LAB_00f17b3f::
    resources:ReleaseResource(ppuVar15)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

