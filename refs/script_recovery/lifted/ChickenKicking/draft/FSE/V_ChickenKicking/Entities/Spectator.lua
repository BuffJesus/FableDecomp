-- Generated native draft: Spectator. Review coverage report before use.
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
    local CVar11, CVar4, bVar5, cVar6, center, fVar3, fret_0, fret_00, iVar12, iVar13, iVar15, p1, pCVar7, pCVar8, pCVar9, pCreature, pvVar10, r1, r2, xStack_10, xStack_20, xStack_60, xStack_64, x_stk_2c, x_stk_38
    local alive = true
    local function __cleanup_LAB_00e63fba()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:ReleaseResource(xStack_20)
    end
    local function __cleanup_LAB_00e63fbf()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:ReleaseResource(xStack_20)
    end
    local function __cleanup_LAB_00e63fd8()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:ReleaseResource(xStack_20)
    end
    local function __cleanup_LAB_00e63fea()
        resources:DestroyMovie(xStack_10)
        resources:ReleaseResource(xStack_20)
    end
    xStack_20 = resources:NewResource()
    quest:SetIsPushableByHero(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    pCVar7 = me:GetPos()
    center = pCVar7.x
    quest:SetWanderCentrePoint(me, pCVar7)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 10.0)
    p1 = nil
    quest:SetScriptingStateGroup(me, 4)
    cVar6 = quest:GetStateBool("SpectatorsUnderAttack")
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e63f59 end
        bVar5 = me:IsTalkedToByHero()
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e63f59 end
            quest:EntitySetCutsceneBehaviour(me, 2)
            resources:PrepareResource(xStack_20)
            bVar5 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:ReleaseResource(xStack_20)
                    return
                end
                bVar5 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e63f59 end
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if (unaff_EBX >> 0x10) == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    __cleanup_LAB_00e63fbf()
                    return
                end
                pCVar8 = tostring(__native_entity_state:GetStateInt("SpectatorNumber"))
                xStack_64 = ("TEXT_QST_B17_SPECTATOR_" .. pCVar8)
                pCVar8 = (xStack_64 .. "_GREETING")
                xStack_64 = pCVar8
                x_stk_38 = resources:ScriptThing(xStack_20)
                pCVar9 = x_stk_38
                fret_0 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                x_stk_38 = nil
                x_stk_38 = 0
                if fVar3 < fret_0 then
                    iVar15 = 0
                    iVar13 = 1
                    p1 = 0x0
                    iVar12 = 0
                    pvVar10 = xStack_64
                    pCVar9 = quest:GetHero()
                    r1 = me:Speak(pCVar9, pvVar10, iVar12, (p1 ~= 0), (iVar13 ~= 0), (iVar15 ~= 0))
                    iVar12 = me:IsPerformingScriptTask()
                    cVar6 = iVar12
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            __cleanup_LAB_00e63fd8(); return
                        end
                        iVar12 = me:IsPerformingScriptTask()
                        cVar6 = iVar12
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        __cleanup_LAB_00e63fba()
                        return
                    end
                end
                -- TODO(native): unaff_EBX = 0x10000;
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00e63fdd: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    __cleanup_LAB_00e63fea(); return
                end
                pCVar8 = tostring(__native_entity_state:GetStateInt("SpectatorNumber"))
                xStack_60 = ("TEXT_QST_B17_SPECTATOR_" .. pCVar8)
                pCVar8 = (xStack_60 .. "_TIP")
                xStack_60 = pCVar8
                x_stk_2c = resources:ScriptThing(xStack_20)
                pCVar9 = x_stk_2c
                fret_00 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                x_stk_2c = nil
                x_stk_2c = 0
                if fVar3 < fret_00 then
                    iVar15 = 0
                    iVar13 = 1
                    p1 = 0x0
                    iVar12 = 2
                    pvVar10 = xStack_60
                    pCVar9 = quest:GetHero()
                    r2 = me:Speak(pCVar9, pvVar10, iVar12, (p1 ~= 0), (iVar13 ~= 0), (iVar15 ~= 0))
                    iVar12 = me:IsPerformingScriptTask()
                    cVar6 = iVar12
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            __cleanup_LAB_00e63fba(); return
                        end
                        iVar12 = me:IsPerformingScriptTask()
                        cVar6 = iVar12
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        __cleanup_LAB_00e63fd8()
                        return
                    end
                end
            end
            resources:PrepareResource(xStack_20)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
        end
        -- TODO(native): xStack_64 = xStack_64 | 1;
        bVar5 = me:MsgIsHitByHero()
        if bVar5 then
            goto LAB_00e63e78
        else
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00e63e78 end
            end
            bVar5 = false
        end
        goto FLOW_past_lab_00e63e78
        ::LAB_00e63e78::
        bVar5 = true
        ::FLOW_past_lab_00e63e78::
            -- TODO(native): xStack_64 = CVar11 & 0xfffffffe;
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e63f59 end
            quest:SetStateBool("SpectatorsUnderAttack", true)
        end
        cVar6 = quest:GetStateBool("SpectatorsUnderAttack")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        quest:EntitySetAsScared(me, true)
        bVar5 = true
        quest:SetIsPushableByHero(me, bVar5)
        quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
        until not (not bVar5)
    end
    ::LAB_00e63f59::
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
    local bVar2, fVar4
    local alive = true
    fVar4 = 1.0
    local pCVar3 = quest:GetThingWithScriptName("Spectator1")
    local bVar1 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, fVar4)
    pCVar3 = nil
    if bVar1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            __native_entity_state:SetStateInt("SpectatorNumber", 1)
            return
        end
    else
        fVar4 = 1.0
        pCVar3 = quest:GetThingWithScriptName("Spectator2")
        bVar1 = quest:IsDistanceBetweenThingsUnder(me, pCVar3, fVar4)
        pCVar3 = nil
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar1 then
            if not bVar2 then
                __native_entity_state:SetStateInt("SpectatorNumber", 2)
                return
            end
        elseif not bVar2 then
            __native_entity_state:SetStateInt("SpectatorNumber", 3)
        end
    end
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

