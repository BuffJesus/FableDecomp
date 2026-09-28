-- Generated native draft: WhisperAlly. Review coverage report before use.
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
    local CVar8, CVar9, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, bUnknown, bVar3, cVar4, c_stk_ed, fVar19, fVar2, f_stk_bc, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, fret_v0, iVar12, iVar20, iVar5, native_arg_switch_1, p0, p2, p3, p4, pCVar16, pCVar17, pCVar18, pCVar6, pCVar7, pQuestName, pThing, pcVar15, r1, r2, r3, scale, timerId, uVar13, uVar14, uVar21_b3, u_stk_f4, xStack_104, xStack_108, xStack_34, xStack_54, xStack_64, xStack_74, xStack_a0, xStack_a4, xStack_a8, xStack_ac, xStack_e8, xStack_ec
    local alive = true
    local function __cleanup_LAB_00f242fc()
        quest:DeregisterTimer(iVar5)
        resources:ReleaseResource(xStack_104)
    end
    u_stk_f4 = 0
    xStack_104 = resources:NewResource()
    cVar4 = quest:GetStateBool("WhisperNeededForCutscene")
    while cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_104)
            return
        end
        cVar4 = quest:GetStateBool("WhisperNeededForCutscene")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        resources:ReleaseResource(xStack_104)
        return
    end
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetTargetingType(me, 0x1a)
    c_stk_ed = 1
    iVar5 = quest:RegisterTimer()
    iVar12 = quest:GetStateInt("ArenaState")
    iVar20 = iVar5
    while iVar12 ~= 8 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            __cleanup_LAB_00f242fc()
            return
        end
        resources:PrepareResource(xStack_104)
        bVar3 = resources:TryAcquire(xStack_104, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            bVar3 = resources:TryAcquire(xStack_104, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(iVar5)
            resources:ReleaseResource(xStack_104)
            return
        end
        iVar12 = 0xd0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
            iVar12 = iVar12 + 4
        until not (iVar12 < 0xdc)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(iVar5)
            resources:ReleaseResource(xStack_104)
            return
        end
        while (xStack_108 == 0 and (quest:GetStateInt("ArenaState") ~= 8)) do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            iVar12 = 0xd0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
                iVar12 = iVar12 + 4
            until not (iVar12 < 0xdc)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then __cleanup_LAB_00f242fc(); return end
            resources:PrepareResource(xStack_104)
            bVar3 = resources:TryAcquire(xStack_104, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                bVar3 = resources:TryAcquire(xStack_104, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            iVar12 = math.random(0, 32767)
            if iVar12 % 0x3c ~= 0 then
                fVar19 = 5.0
                pCVar6 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar19)
                if not bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f25806 end
                    pCVar6 = quest:GetHero()
                    p4 = 1
                    p3 = 0
                    p2 = 1
                    iVar12 = 3.0
                    p0 = pCVar6:GetPos()
                    me:MoveToPosition(p0, iVar12, p2, (p3 ~= 0), (p4 ~= 0))
                end
            end
            if quest:GetStateBool("WhisperNeededForCutscene") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f25806 end
                resources:PrepareResource(xStack_104)
                cVar4 = quest:GetStateBool("WhisperNeededForCutscene")
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f25806 end
                    cVar4 = quest:GetStateBool("WhisperNeededForCutscene")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f25806 end
                resources:PrepareResource(xStack_104)
                bVar3 = resources:TryAcquire(xStack_104, me, 4)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f25806 end
                    bVar3 = resources:TryAcquire(xStack_104, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f25806 end
            end
            bVar3 = me:IsTalkedToByHero()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f25806 end
                xStack_54 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                bVar3 = false
                pCVar18 = 0x1
                pCVar17 = 0x0
                pCVar16 = 0x1
                pcVar15 = "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_CHAT"
                pCVar6 = quest:GetHero()
                iVar12 = me:IsPerformingScriptTask()
                cVar4 = iVar12
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_54)
                        goto LAB_00f25806
                    end
                    iVar12 = me:IsPerformingScriptTask()
                    cVar4 = iVar12
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(xStack_64)
                    goto LAB_00f25806
                end
                -- TODO(native): (**(code **)(*(int *)xStack_c0 + 0x5ec))();
                resources:DestroyMovie(xStack_54)
            end
            uVar14 = u_stk_f4
            u_stk_f4 = u_stk_f4 | 1
            bVar3 = me:MsgIsHitByHero()
            if bVar3 then
            else
                uVar13 = uVar14 | 3
                bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar3 then
                    uVar13 = uVar14 | 7
                    bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar3 then goto LAB_00f2337d end
                end
            end
            ::LAB_00f2337d::
            if (uVar13 & 4) ~= 0 then
                uVar13 = uVar13 & 0xfffffffb
            end
            if (uVar13 & 2) ~= 0 then
                uVar13 = uVar13 & 0xfffffffd
            end
            if (uVar13 & 1) ~= 0 then
                u_stk_f4 = uVar13 & 0xfffffffe
            end
            if (in_stack_fffffeec & 0xffffff >> 0x18) ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f25806 end
                quest:ModifyThingHealth(me, 10000.0, false)
                pCVar6 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, pCVar6)
                pCVar7 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(pCVar7, me)
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then __cleanup_LAB_00f242fc(); return end
        r1 = quest:GetNearestWithScriptName(me, "ArenaEnemy")
        __native_condition_1 = (r1 ~= nil and not r1:IsNull())
        if __native_condition_1 then
            cVar4 = (r1 ~= nil and r1:IsAlive())
            __native_condition_1 = cVar4
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00f243f1: (native jump target)
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            quest:GiveThingBestEnemyTarget(me, r1)
        end
        resources:PrepareResource(xStack_104)
        quest:SetTimer(iVar5, 10)
        while (xStack_108 ~= 0 and (quest:GetStateInt("ArenaState") ~= 8)) do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                r1 = nil
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            uVar14 = u_stk_f4
            u_stk_f4 = u_stk_f4 | 8
            bVar3 = me:MsgIsHitByHero()
            if bVar3 then
            else
                uVar13 = uVar14 | 0x18
                bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar3 then
                    uVar13 = uVar14 | 0x38
                    bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar3 then goto LAB_00f235a1 end
                end
            end
            ::LAB_00f235a1::
            if (uVar13 & 0x20) ~= 0 then
                uVar13 = uVar13 & 0xffffffdf
            end
            if (uVar13 & 0x10) ~= 0 then
                uVar13 = uVar13 & 0xffffffef
            end
            if (uVar13 & 8) ~= 0 then
                uVar13 = uVar13 & 0xfffffff7
            end
            if (in_stack_fffffeec & 0xffffff >> 0x18) ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                pCVar6 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, pCVar6)
                pCVar7 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(pCVar7, me)
            end
            if quest:GetStateInt("ArenaRound") ~= 7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                -- TODO(native): cVar4 = (**(r1 + 0x12c))()
                cVar4 = nil --[[unresolved native value]]
                if not cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        pCVar6 = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                        r1 = pCVar6
                        iVar12 = (r1 ~= nil and r1:IsAlive())
                        if not iVar12 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f243b3 end
                            pCVar6 = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                            r1 = pCVar6
                            iVar12 = (r1 ~= nil and r1:IsAlive())
                            if not iVar12 then goto LAB_00f241aa end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            quest:GiveThingBestEnemyTarget(me, r1)
                            goto LAB_00f241aa
                        end
                        goto LAB_00f243b3
                    end
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                iVar12 = quest:GetTimer(iVar5)
                if 0 < iVar12 then goto LAB_00f241aa end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f243b3 end
                fret_01 = quest:GetHealth(r1)
                if fret_01 < 10.0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f243b3 end
                    iVar5 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar5, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar5, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, pCVar6, false)
                    iVar5 = iVar20
                    goto LAB_00f24190
                end
                goto FLOW_past_lab_00f24190
                ::LAB_00f24190::
                iVar20 = iVar5
                quest:SetTimer(iVar5, 10)
                goto LAB_00f241aa
                ::FLOW_past_lab_00f24190::
                uVar14 = uVar13 | 0x400
                u_stk_f4 = uVar14
                bVar3 = me:MsgIsHitBy("")
                if bVar3 then
                    uVar14 = uVar13 | 0xc00
                    u_stk_f4 = uVar14
                    bVar3 = me:MsgIsHitByHero()
                    if bVar3 then goto LAB_00f2405a end
                else
                end
                ::LAB_00f2405a::
                if (uVar14 & 0x800) ~= 0 then
                    uVar14 = uVar14 & 0xfffff7ff
                    u_stk_f4 = uVar14
                end
                if (uVar14 & 0x400) ~= 0 then
                    u_stk_f4 = uVar14 & 0xfffffbff
                end
                if (in_stack_fffffeec & 0xffffff >> 0x18) == 0 then
                    -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                    bVar3 = me:MsgHitEnemyWithMeleeWeapon()
                    if not bVar3 then goto LAB_00f241aa end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        iVar5 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(iVar5, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar5, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, pCVar6, false)
                        iVar5 = iVar20
                        goto LAB_00f24190
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        iVar5 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(iVar5, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar5, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, pCVar6, false)
                        iVar5 = iVar20
                        goto LAB_00f24190
                    end
                end
                goto LAB_00f243b3
            end
            goto FLOW_past_lab_00f243b3
            ::LAB_00f243b3::
            goto LAB_00f25806
            ::FLOW_past_lab_00f243b3::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00f243c5: (native jump target)
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            if c_stk_ed == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                -- TODO(native): cVar4 = (**(r1 + 0x12c))()
                cVar4 = nil --[[unresolved native value]]
                if not cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        pCVar6 = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                        r1 = pCVar6
                        iVar12 = (r1 ~= nil and r1:IsAlive())
                        if not iVar12 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                pCVar6 = quest:GetNearestWithScriptName(me, "ArenaEnemy")
                                r1 = pCVar6
                                iVar12 = (r1 ~= nil and r1:IsAlive())
                                if iVar12 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00f243b3 end
                                    quest:GiveThingBestEnemyTarget(me, r1)
                                    c_stk_ed = 1
                                end
                                goto LAB_00f241aa
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                quest:GiveThingBestEnemyTarget(me, r1)
                                c_stk_ed = 0
                                goto LAB_00f241aa
                            end
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f243b3 end
                    iVar12 = quest:GetTimer(iVar5)
                    if 0 < iVar12 then goto LAB_00f241aa end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f243b3 end
                    fret_00 = quest:GetHealth(r1)
                    if fret_00 < 10.0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f243b3 end
                        xStack_108 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(xStack_108, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(xStack_108, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, pCVar6, false)
                        goto LAB_00f23d7f
                    end
                    goto FLOW_past_lab_00f23d7f
                    ::LAB_00f23d7f::
                    quest:SetTimer(iVar5, 10)
                    goto LAB_00f241aa
                    ::FLOW_past_lab_00f23d7f::
                    uVar14 = uVar13 | 0x100
                    u_stk_f4 = uVar14
                    bVar3 = me:MsgIsHitBy("")
                    if bVar3 then
                        uVar14 = uVar13 | 0x300
                        u_stk_f4 = uVar14
                        bVar3 = me:MsgIsHitByHero()
                        if bVar3 then goto LAB_00f23cba end
                    else
                    end
                    ::LAB_00f23cba::
                    if (uVar14 & 0x200) ~= 0 then
                        uVar14 = uVar14 & 0xfffffdff
                        u_stk_f4 = uVar14
                    end
                    if (uVar14 & 0x100) ~= 0 then
                        u_stk_f4 = uVar14 & 0xfffffeff
                    end
                    if (in_stack_fffffeec & 0xffffff >> 0x18) == 0 then
                        -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                        bVar3 = me:MsgHitEnemyWithMeleeWeapon()
                        if not bVar3 then goto LAB_00f241aa end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            iVar5 = quest:AddNewConversation(me, false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar5, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar5, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, pCVar6, false)
                            iVar5 = iVar20
                            -- LAB_00f24190_c17: (native jump target)
                            iVar20 = iVar5
                            quest:SetTimer(iVar5, 10)
                            goto LAB_00f241aa
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            xStack_108 = quest:AddNewConversation(me, false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(xStack_108, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(xStack_108, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, pCVar6, false)
                            goto LAB_00f23d7f
                        end
                    end
                end
                goto LAB_00f243b3
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
            r2 = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
            iVar12 = (r2 ~= nil and r2:IsAlive())
            if iVar12 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    pCVar6 = quest:GetNearestWithScriptName(me, "SUMMONED_CREATURE")
                    r1 = pCVar6
                    quest:GiveThingBestEnemyTarget(me, r1)
                    c_stk_ed = 0
                    goto LAB_00f241aa
                end
                goto LAB_00f243a7
            end
            goto FLOW_past_lab_00f243a7
            ::LAB_00f243a7::
            goto LAB_00f243b3
            ::FLOW_past_lab_00f243a7::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f243a7 end
            iVar12 = quest:GetTimer(iVar5)
            if iVar12 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f243a7 end
                fret_0 = quest:GetHealth(r1)
                if 10.0 <= fret_0 then
                    uVar14 = uVar13 | 0x40
                    u_stk_f4 = uVar14
                    bVar3 = me:MsgIsHitBy("")
                    if bVar3 then
                        uVar14 = uVar13 | 0xc0
                        u_stk_f4 = uVar14
                        bVar3 = me:MsgIsHitByHero()
                        if bVar3 then goto LAB_00f2389f end
                    else
                    end
                    ::LAB_00f2389f::
                    if ((uVar14 & 0x80) ~= 0) then
                        uVar14 = uVar14 & 0xffffff7f
                        u_stk_f4 = uVar14
                    end
                    if (uVar14 & 0x40) ~= 0 then
                        u_stk_f4 = uVar14 & 0xffffffbf
                    end
                    if (in_stack_fffffeec & 0xffffff >> 0x18) == 0 then
                        -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                        bVar3 = me:MsgHitEnemyWithMeleeWeapon()
                        if not bVar3 then goto LAB_00f23a17 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f243a7 end
                        xStack_108 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(xStack_108, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(xStack_108, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, pCVar6, false)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f243a7 end
                        xStack_108 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(xStack_108, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(xStack_108, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, pCVar6, false)
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f243a7 end
                    xStack_108 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(xStack_108, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(xStack_108, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_WINNING", me, pCVar6, false)
                end
                quest:SetTimer(iVar5, 10)
            end
            ::LAB_00f23a17::
            ::LAB_00f241aa::
            iVar12 = 0xd0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar5)
                    resources:ReleaseResource(xStack_104)
                    return
                end
                -- TODO(native): xStack_108 = xStack_108 + *(int *)(*(int *)(this + 0x14) + iVar12);
                iVar12 = iVar12 + 4
            until not (iVar12 < 0xdc)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:ReleaseResource(xStack_104)
                return
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        r1 = nil
        if bVar3 then
            -- LAB_00f2431b: (native jump target)
            r1 = nil
            quest:DeregisterTimer(iVar5)
            resources:ReleaseResource(xStack_104)
            return
        end
        iVar12 = quest:GetStateInt("ArenaState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        quest:DeregisterTimer(iVar5)
        resources:ReleaseResource(xStack_104)
        return
    end
    cVar4 = quest:GetStateBool("FinalBattleCS")
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00f243f6: (native jump target)
            quest:DeregisterTimer(iVar5)
            resources:ReleaseResource(xStack_104)
            return
        end
        cVar4 = quest:GetStateBool("FinalBattleCS")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00f243ca: (native jump target)
        quest:DeregisterTimer(iVar5)
        resources:ReleaseResource(xStack_104)
        return
    end
    resources:PrepareResource(xStack_104)
    quest:ClearThingHasInformation(me)
    quest:EntitySetInFaction(me, "FACTION_MONSTER")
    pCVar6 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, pCVar6)
    pCVar6 = quest:GetHero()
    quest:EntityUnsetThingAsAllyOfThing(me, pCVar6)
    pCVar7 = quest:GetHero()
    quest:EntityUnsetThingAsAllyOfThing(pCVar7, me)
    quest:ClearThingHasInformation(me)
    quest:EntitySetTargetingType(me, 0x3a)
    quest:ModifyThingHealth(me, 10000.0, false)
    -- TODO(native): xStack_ac = quest:AddQuestInfoBarHealth(me, &0xffffff00, "HUD_WHISPER_ICON", 1.0)
    xStack_ac = nil --[[unresolved native value]]
    fret_v0 = quest:GetHealth(me)
    CVar8 = math.tointeger(math.modf(fret_v0))
    c_stk_ed = 0
    xStack_a4 = CVar8
    fret_02 = quest:GetHealth(me)
    CVar9 = math.tointeger(math.modf(fret_02 * 0.25))
    timerId = quest:RegisterTimer()
    xStack_ec = timerId
    quest:SetTimer(timerId, 0)
    f_stk_bc = CVar9
    xStack_a8 = f_stk_bc
    fret_03 = quest:GetHealth(me)
    if f_stk_bc < fret_03 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f257fd end
            fret_04 = quest:GetHealth(me)
            __native_condition_3 = fret_04 < (math.tointeger(math.modf((CVar8 * 3) / 4)))
            if __native_condition_3 then
                __native_condition_3 = not uVar21_b3
            end
            __native_condition_2 = __native_condition_3
            if __native_condition_2 then
                iVar12 = quest:GetTimer(timerId)
                __native_condition_2 = iVar12 < 6
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f257fd end
                CVar8 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(CVar8, pCVar6)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(CVar8, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_QUARTER", me, pCVar6, false)
                quest:SetTimer(timerId, 0xd)
                CVar8 = xStack_a4
            end
            fret_05 = quest:GetHealth(me)
            __native_condition_5 = fret_05 < (math.tointeger(math.modf(CVar8 / 2)))
            if __native_condition_5 then
                __native_condition_5 = c_stk_ed == 0
            end
            __native_condition_4 = __native_condition_5
            if __native_condition_4 then
                iVar12 = quest:GetTimer(timerId)
                __native_condition_4 = iVar12 < 6
            end
            if __native_condition_4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f257fd end
                CVar8 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(CVar8, pCVar6)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(CVar8, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_HALF", me, pCVar6, false)
                c_stk_ed = 1
                quest:SetTimer(timerId, 0xd)
            end
            iVar12 = quest:GetTimer(timerId)
            if iVar12 < 6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f257fd end
                bVar3 = me:MsgIsHitBy("")
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f257fd end
                    CVar8 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(CVar8, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(CVar8, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_BEEN_HIT", me, pCVar6, false)
                else
                    -- TODO(native): MsgHitEnemyWithMeleeWeapon is not a ForgeFSE binding
                    bVar3 = me:MsgHitEnemyWithMeleeWeapon()
                    if not bVar3 then goto LAB_00f249f7 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00f257fd end
                    CVar8 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(CVar8, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(CVar8, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHT_HIT", me, pCVar6, false)
                end
                goto LAB_00f249e1
            else
                iVar12 = quest:GetTimer(timerId)
                if iVar12 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        CVar8 = quest:AddNewConversation(me, false, false)
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(CVar8, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(CVar8, "TEXT_QST_005_V2_ARENA_WHISPER_FIGHTING_HERO", me, pCVar6, false)
                        goto LAB_00f249e1
                    end
                    goto LAB_00f257fd
                end
            end
            goto FLOW_past_lab_00f249e1
            ::LAB_00f249e1::
            quest:SetTimer(timerId, 0xf)
            ::FLOW_past_lab_00f249e1::
            ::LAB_00f249f7::
            fret_06 = quest:GetHealth(me)
            CVar8 = xStack_a4
        until not (f_stk_bc < fret_06)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:StopOverrideMusic(false)
        bVar3 = false
        fret_07 = quest:GetHealth(me)
        quest:ModifyThingHealth(me, (xStack_a8 - fret_07), bVar3)
        quest:EntitySetInFaction(me, "FACTION_HERO")
        pCVar6 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, pCVar6)
        pCVar7 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(pCVar7, me)
        quest:EntitySetAsDamageable(me, false)
        quest:RemoveQuestInfoElement(xStack_ac)
        resources:PrepareResource(xStack_104)
        bVar3 = resources:TryAcquire(xStack_104, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f257fd end
            bVar3 = resources:TryAcquire(xStack_104, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_74 = resources:NewResource()
            resources:PrepareResource(xStack_74)
            iVar20 = 4
            pCVar6 = xStack_74
            pCVar7 = quest:GetHero()
            bVar3 = resources:TryAcquire(pCVar6, pCVar7, iVar20)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f24bc4 end
                iVar20 = 4
                pCVar6 = xStack_74
                pCVar7 = quest:GetHero()
                bVar3 = resources:TryAcquire(pCVar6, pCVar7, iVar20)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                goto LAB_00f24bc4
            else
                xStack_54 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                xStack_a0 = resources:NewActorMap()
                resources:SetActor(xStack_a0, "Hero", xStack_74)
                resources:SetActor(xStack_a0, "Whisper", xStack_104)
                resources:RunMacro("CS_ARENA_WHISPER_BEFORESTRIKE", xStack_a0, false, true)
                resources:DestroyActorMap(xStack_a0)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_54)
                resources:ReleaseResource(xStack_74)
                pQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_10", "Arena", "KnotholeGlade")
                bVar3 = true
                pCVar6 = quest:GetThingWithScriptName("ArenaMainExit")
                quest:SetThingAsUsable(pCVar6, bVar3)
                r3 = quest:GetThingWithScriptName("ArenaHeroGate")
                quest:OpenDoor(r3)
                xStack_108 = quest:RegisterTimer()
                quest:SetTimer(xStack_108, 0)
                bVar3 = false
                pCVar6 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                bVar3 = false
                pCVar7 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(pCVar7, me, bVar3)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                while not bVar3 do
                    iVar12 = quest:GetTimer(xStack_108)
                    if 0 < iVar12 then goto LAB_00f25062 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    iVar20 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar20, pCVar6)
                    native_arg_switch_1 = 0x0
                    repeat
                        if native_arg_switch_1 == 0 then
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar20, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIRST_PLEAD", me, pCVar6, false)
                            break
                        else
                            if native_arg_switch_1 == 1 then
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar20, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_SECOND_PLEAD", me, pCVar6, false)
                                break
                            else
                                if native_arg_switch_1 == 2 then
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar20, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_THIRD_PLEAD", me, pCVar6, false)
                                    goto LAB_00f25040
                                else
                                    if native_arg_switch_1 == 3 then
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar20, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FOURTH_PLEAD", me, pCVar6, false)
                                        break
                                    else
                                        if native_arg_switch_1 == 4 then
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar20, "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FIFTH_PLEAD", me, pCVar6, false)
                                            goto LAB_00f25040
                                        end
                                    end
                                end
                                goto FLOW_past_lab_00f25040
                                ::LAB_00f25040::
                                ::FLOW_past_lab_00f25040::
                            end
                        end
                    until not (false)
                    quest:SetTimer(xStack_108, 10)
                    ::LAB_00f25062::
                    uVar14 = u_stk_f4
                    u_stk_f4 = u_stk_f4 | 0x1000
                    bVar3 = me:MsgIsHitByHero()
                    if bVar3 then
                        goto LAB_00f250f0
                    else
                        uVar13 = uVar14 | 0x3000
                        bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar3 then
                            uVar13 = uVar14 | 0x7000
                            bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar3 then goto LAB_00f250f0 end
                        end
                        bVar3 = false
                    end
                    goto FLOW_past_lab_00f250f0
                    ::LAB_00f250f0::
                    bVar3 = true
                    ::FLOW_past_lab_00f250f0::
                    if (uVar13 & 0x4000) ~= 0 then
                        uVar13 = uVar13 & 0xffffbfff
                    end
                    if (uVar13 & 0x2000) ~= 0 then
                        uVar13 = uVar13 & 0xffffdfff
                    end
                    if (uVar13 & 0x1000) ~= 0 then
                        u_stk_f4 = uVar13 & 0xffffefff
                    end
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        xStack_54 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        pCVar6 = resources:ScriptThing(xStack_104)
                        fret_08 = quest:GetHealth(pCVar6)
                        fVar2 = 0.0
                        if fVar2 < fret_08 then
                            bVar3 = false
                            pCVar18 = 0x1
                            pCVar17 = 0x0
                            pCVar16 = 0x0
                            pcVar15 = "TEXT_QST_005_V2_ARENA_WHISPER_FINALBATTLE_FINAL_LINE"
                            pCVar6 = quest:GetHero()
                            iVar12 = me:IsPerformingScriptTask()
                            cVar4 = iVar12
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_54)
                                    goto LAB_00f257f4
                                end
                                iVar12 = me:IsPerformingScriptTask()
                                cVar4 = iVar12
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_54)
                                break
                            end
                        end
                        resources:PrepareResource(xStack_104)
                        pCVar6 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar6)
                        quest:EntitySetAsDamageable(me, true)
                        quest:EntitySetInFaction(me, "FACTION_MONSTER")
                        pCVar6 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(me, pCVar6)
                        pCVar7 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(pCVar7, me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_54)
                        quest:DisplayQuestInfo(true)
                        iVar12 = quest:AddQuestInfoBar(xStack_a4, 0.0, 0xffff0000, {R = 255, G = 0, B = 0, A = 255}, "HUD_WHISPER_ICON", "", 1.0)
                        fret_09 = quest:GetHealth(me)
                        if 1.0 < fret_09 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f257f4 end
                                scale = -1.0
                                fVar19 = -1.0
                                fret_10 = quest:GetHealth(me)
                                quest:UpdateQuestInfoBar(iVar12, fret_10, fVar19, scale)
                                fret_11 = quest:GetHealth(me)
                            until not (1.0 < fret_11)
                        end
                        quest:DisplayQuestInfo(false)
                        quest:RemoveQuestInfoElement(iVar12)
                        xStack_64 = resources:NewResource()
                        xStack_34 = resources:NewResource()
                        resources:PrepareResource(xStack_64)
                        iVar20 = 4
                        pCVar6 = xStack_64
                        pCVar7 = quest:GetHero()
                        bVar3 = resources:TryAcquire(pCVar6, pCVar7, iVar20)
                        while not bVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f257e3 end
                            iVar20 = 4
                            pCVar6 = xStack_64
                            pCVar7 = quest:GetHero()
                            bVar3 = resources:TryAcquire(pCVar6, pCVar7, iVar20)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            resources:PrepareResource(xStack_34)
                            bVar3 = resources:TryAcquire(xStack_34, me, 4)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f257e3 end
                                bVar3 = resources:TryAcquire(xStack_34, me, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                bVar3 = false
                                pCVar6 = quest:GetThingWithScriptName("P_CROWD4")
                                quest:EntityTeleportToThing(me, pCVar6, bVar3)
                                xStack_a0 = resources:NewActorMap()
                                resources:SetActor(xStack_a0, "Hero", xStack_64)
                                quest:EntitySetAsDrawable(me, false)
                                xStack_e8 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                resources:RunMacro("CS_ARENA_WHISPER_KILLED", xStack_a0, false, true)
                                quest:GiveHeroGold(10000)
                                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xaf0))
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_e8)
                                resources:DestroyActorMap(xStack_a0)
                                resources:ReleaseResource(xStack_34)
                                resources:ReleaseResource(xStack_64)
                                bUnknown = false
                                pCVar6 = quest:GetThingWithScriptName("ArenaCellEntrance2")
                                pCVar7 = quest:GetHero()
                                quest:EntityTeleportToThing(pCVar7, pCVar6, bUnknown)
                                quest:SetMasterGameState("WhisperKilledByHero", true)
                                quest:RemoveThing(me, false, true)
                                goto LAB_00f2578b
                            end
                        end
                        ::LAB_00f257e3::
                        resources:ReleaseResource(xStack_34)
                        resources:ReleaseResource(xStack_64)
                        break
                    end
                    ::LAB_00f2578b::
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                end
                ::LAB_00f257f4::
                quest:DeregisterTimer(xStack_108)
            end
            goto FLOW_past_lab_00f24bc4
            ::LAB_00f24bc4::
            resources:ReleaseResource(pCVar6)
            ::FLOW_past_lab_00f24bc4::
        end
    end
    ::LAB_00f257fd::
    quest:DeregisterTimer(xStack_ec)
    ::LAB_00f25806::
    quest:DeregisterTimer(xStack_108)
    resources:ReleaseResource(xStack_104)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

