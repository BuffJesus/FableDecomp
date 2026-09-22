-- Generated native draft: MazeAtTavern. Review coverage report before use.
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
    local b3, bVar13, bVar3, cVar4, c_stk_8d, fVar14, fVar2, fret_0, iVar11, iVar5, i_stk_98, p0, p1, p4, p5, pCVar12, pCVar6, pCVar7, pCVar9, pPosition, r1, uVar10, uVar15, u_stk_7c, xStack_10, xStack_20, xStack_2c, xStack_38, xStack_48, xStack_8c
    local alive = true
    u_stk_7c = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_8c = resources:NewResource()
    resources:PrepareResource(xStack_8c)
    bVar3 = resources:TryAcquire(xStack_8c, me, 3)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_8c)
            return
        end
        bVar3 = resources:TryAcquire(xStack_8c, me, 3)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00e26ef7: (native jump target)
        resources:ReleaseResource(xStack_8c)
        return
    end
    quest:SetThingHasInformation(me, false, true, false)
    i_stk_98 = quest:RegisterTimer()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    uVar15 = 0
    iVar11 = i_stk_98
    repeat
        i_stk_98 = iVar11
        if bVar3 then
            quest:DeregisterTimer(iVar11)
            resources:ReleaseResource(xStack_8c)
            return
        end
        iVar5 = quest:GetTimer(iVar11)
        if iVar5 == 0 then
            fVar14 = 30.0
            pCVar6 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar14)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(iVar11)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
                iVar5 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(iVar5, pCVar6)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar5, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", me, pCVar6, false)
                me:PlayAnimation("ST_HELLO", false, false, false, true, true, false, false)
                quest:SetTimer(i_stk_98, 5)
                uVar15 = u_stk_7c
            end
        end
        if not quest:GetStateBool("GuardianSpokeToHero") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(i_stk_98)
                resources:ReleaseResource(xStack_8c)
                return
            end
            bVar3 = false
            pCVar6 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
        end
        uVar10 = uVar15 | 1
        u_stk_7c = uVar10
        bVar3 = me:MsgIsHitByHero()
        if bVar3 then
            goto LAB_00e270f6
        else
            uVar10 = uVar15 | 3
            u_stk_7c = uVar10
            bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar3 then
                uVar10 = uVar15 | 7
                u_stk_7c = uVar10
                bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar3 then goto LAB_00e270f6 end
            end
            c_stk_8d = 0
        end
        goto FLOW_past_lab_00e270f6
        ::LAB_00e270f6::
        c_stk_8d = 1
        ::FLOW_past_lab_00e270f6::
        if (uVar10 & 4) ~= 0 then
            uVar10 = uVar10 & 0xfffffffb
            u_stk_7c = uVar10
        end
        if (uVar10 & 2) ~= 0 then
            uVar10 = uVar10 & 0xfffffffd
            u_stk_7c = uVar10
        end
        if (uVar10 & 1) ~= 0 then
            u_stk_7c = uVar10 & 0xfffffffe
        end
        if c_stk_8d ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00e27648: (native jump target)
                quest:DeregisterTimer(i_stk_98)
                resources:ReleaseResource(xStack_8c)
                return
            end
            pCVar6 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, pCVar6)
            pCVar7 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(pCVar7, me)
            resources:PrepareResource(xStack_8c)
            bVar3 = resources:TryAcquire(xStack_8c, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(i_stk_98)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
                bVar3 = resources:TryAcquire(xStack_8c, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(i_stk_98)
                resources:ReleaseResource(xStack_8c)
                return
            end
            xStack_48 = resources:StartMovie("")
            pCVar7 = 0x1
            quest:PauseAllNonScriptedEntities(true)
            xStack_2c = resources:ScriptThing(xStack_8c)
            pCVar6 = xStack_2c
            fret_0 = quest:GetHealth(pCVar6)
            fVar2 = 0.0
            if fVar2 < fret_0 then
                p5 = 0
                p4 = 1
                iVar5 = 0
                iVar11 = 0
                p1 = "TEXT_QST_076_MAZE_ON_HIT_10"
                pCVar6 = quest:GetHero()
                r1 = me:Speak(pCVar6, p1, iVar11, (iVar5 ~= 0), (p4 ~= 0), (p5 ~= 0))
                iVar11 = me:IsPerformingScriptTask()
                cVar4 = iVar11
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_48)
                        quest:DeregisterTimer(i_stk_98)
                        resources:ReleaseResource(xStack_8c)
                        return
                    end
                    iVar11 = me:IsPerformingScriptTask()
                    cVar4 = iVar11
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_48)
                    quest:DeregisterTimer(i_stk_98)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
            end
            __native_entity_state:SetStateInt("HitCount", __native_entity_state:GetStateInt("HitCount") + 1)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_48)
        end
        bVar3 = me:IsTalkedToByHero()
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(i_stk_98)
                resources:ReleaseResource(xStack_8c)
                return
            end
            resources:PrepareResource(xStack_8c)
            bVar3 = resources:TryAcquire(xStack_8c, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(i_stk_98)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
                bVar3 = resources:TryAcquire(xStack_8c, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(i_stk_98)
                resources:ReleaseResource(xStack_8c)
                return
            end
            if __native_entity_state:GetStateInt("TalkCounter") == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(i_stk_98)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
                quest:ForceShipsVisible()
                xStack_20 = resources:NewResource()
                iVar5 = 4
                pCVar12 = xStack_20
                pCVar6 = quest:GetHero()
                resources:TryAcquire(pCVar12, pCVar6, iVar5)
                xStack_38 = resources:NewActorMap()
                resources:SetActor(xStack_38, "MAZE", xStack_8c)
                resources:SetActor(xStack_38, "HERO", xStack_20)
                xStack_10 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:RunMacro("CS_GUARDIAN_SISTER_2", xStack_38, false, true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_10)
                resources:DestroyActorMap(xStack_38)
                resources:ReleaseResource(xStack_20)
                quest:SetStateBool("GuardianSpokeToHero", true)
                quest:AddQuestCard("OBJECT_QUEST_CARD_BANDIT_CAMP", "Q_BanditCamp", false, false)
            end
            __native_entity_state:SetStateInt("TalkCounter", __native_entity_state:GetStateInt("TalkCounter") + 1)
            if __native_entity_state:GetStateInt("HitCount") < 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(i_stk_98)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
                resources:PrepareResource(xStack_8c)
                bVar3 = resources:TryAcquire(xStack_8c, me, 3)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:DeregisterTimer(i_stk_98)
                        resources:ReleaseResource(xStack_8c)
                        return
                    end
                    bVar3 = resources:TryAcquire(xStack_8c, me, 3)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:DeregisterTimer(i_stk_98)
                    resources:ReleaseResource(xStack_8c)
                    return
                end
            end
            quest:ClearThingHasInformation(me)
        end
        if quest:GetStateBool("GuardianSpokeToHero") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                me:ClearCommands()
                me:PlayAnimation("ST_TELEPORT_OUT", false, false, false, true, true, false, false)
                bVar13 = false
                bVar3 = false
                fVar14 = 0.0
                pPosition = me:GetPos()
                -- TODO(native): CreateEffect is not a ForgeFSE binding
                quest:CreateEffect("MAZE_TELEPORT_OUT_01", pPosition, "", fVar14, bVar3, bVar13)
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
                b3 = false
                bVar13 = false
                bVar3 = false
                pCVar9 = quest:GetActiveQuestName()
                quest:SetQuestAsCompleted(pCVar9, bVar3, bVar13, b3)
                uVar15 = 0
                pCVar9 = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pCVar9, uVar15)
            end
            -- LAB_00e277c0: (native jump target)
            quest:DeregisterTimer(i_stk_98)
            resources:ReleaseResource(xStack_8c)
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        uVar15 = u_stk_7c
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateInt("TalkCounter", 0)
    __native_entity_state:SetStateBool("WavedOver", false)
    __native_entity_state:SetStateInt("HitCount", 0)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

