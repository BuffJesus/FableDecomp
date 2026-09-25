-- Generated native draft: DarkwoodAssassinSpawn. Review coverage report before use.
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
    local bVar4, dist, fVar10, f_CVar3, iVar1, iVar8, i_stk_2c, p0, pCVar6, pCVar7, pCVar9, r1, r2, r3, r4, timerId, uVar5, xStack_20, xStack_24
    local alive = true
    local function __cleanup_LAB_00e02ff7()
        quest:DeregisterTimer(i_stk_2c)
    end
    xStack_24 = p0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        return
    end
    timerId = quest:RegisterTimer()
    i_stk_2c = timerId
    quest:SetTimer(i_stk_2c, 2)
    xStack_20 = quest:ReadGlobalGameData(0xe14)
    iVar8 = quest:ReadGlobalGameData(0x3b0)
    iVar1 = quest:ReadGlobalGameData(0x3ac)
    uVar5 = math.random(0, 32767)
    -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0x3ac) + (uVar5 % (uint)(iVar8 - iVar1 >> 2)) * 4),(int)&xStack_28);
    -- TODO(native): CCharString::CCharString(&xStack_30,&xStack_28);
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        f_CVar3 = xStack_20
        -- TODO(native): xStack_20 = CVar3;
        repeat
            dist = f_CVar3
            pCVar6 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, dist)
            if bVar4 then
                -- TODO(native): pCVar7 = (**(*me + 0x18))(me)
                pCVar7 = nil --[[unresolved native value]]
                bVar4 = quest:IsCameraPosOnScreen(nil --[[missing]])
                p0 = xStack_24
                if bVar4 then goto LAB_00e02f91 end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                p0 = xStack_24
                if bVar4 then __cleanup_LAB_00e02ff7(); return end
            else
                goto LAB_00e02f91
            end
            goto FLOW_past_lab_00e02f91
            ::LAB_00e02f91::
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then break end
            quest:SetTimer(i_stk_2c, 2)
            ::FLOW_past_lab_00e02f91::
            iVar8 = quest:GetTimer(i_stk_2c)
            if iVar8 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    __cleanup_LAB_00e02ff7()
                    return
                end
                xStack_24 = quest:ReadGlobalGameDataString(0xe18)
                bVar4 = true
                -- TODO(native): pCVar7 = (**(*p0 + 0x18))(p0)
                pCVar7 = nil --[[unresolved native value]]
                r1 = quest:CreateCreature(xStack_24, nil --[[missing]], "DarkwoodAssassin")
                quest:SetThingPersistent(r1, true)
                pCVar9 = quest:GetActiveQuestName()
                quest:EntityAttachToScript(r1, pCVar9)
                iVar8 = math.random(0, 32767)
                if iVar8 % 5 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        goto LAB_00e03105
                    end
                    goto FLOW_hoist_lab_00e03105_1
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00e03105 end
                    pCVar6 = quest:GetNearestWithScriptName(r1, "DarkwoodTrader")
                    quest:GiveThingBestEnemyTarget(r1, pCVar6)
                end
                goto FLOW_past_lab_00e03105
                ::LAB_00e03105::
                break
                ::FLOW_hoist_lab_00e03105_1::
                pCVar6 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(r1, pCVar6)
                ::FLOW_past_lab_00e03105::
                fVar10 = 18.0
                pCVar6 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsOver(r1, pCVar6, fVar10)
                if not bVar4 then goto LAB_00e0320d end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e03372 end
                fVar10 = 18.0
                pCVar6 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar10)
                if bVar4 then goto LAB_00e031e9 end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00e03372 end
                goto FLOW_after_lab_00e031b3
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
        until not (not bVar4)
    end
    quest:DeregisterTimer(i_stk_2c)
    do return end
    while true do
        fVar10 = 18.0
        pCVar6 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar10)
        if bVar4 then break end
        -- LAB_00e031b3: (native jump target)
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e03372 end
    end
    ::FLOW_after_lab_00e031b3::
    ::LAB_00e031e9::
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then goto LAB_00e03372 end
    r2 = quest:PlaySoundOnThing(r1, "DarkwoodAssassin")
    ::LAB_00e0320d::
    fVar10 = 12.0
    pCVar6 = quest:GetHero()
    bVar4 = quest:IsDistanceBetweenThingsOver(r1, pCVar6, fVar10)
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e03372 end
        fVar10 = 12.0
        pCVar6 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar10)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e03372 end
            fVar10 = 12.0
            pCVar6 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar10)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e03372 end
        r3 = quest:PlaySoundOnThing(r1, nil --[[missing]])
    end
    fVar10 = 8.0
    pCVar6 = quest:GetHero()
    bVar4 = quest:IsDistanceBetweenThingsOver(r1, pCVar6, fVar10)
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e03372 end
        fVar10 = 8.0
        pCVar6 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar10)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e03372 end
            fVar10 = 8.0
            pCVar6 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar10)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00e03372 end
        r4 = quest:PlaySoundOnThing(r1, nil --[[missing]])
    end
    quest:RemoveThing(p0, false, true)
    ::LAB_00e03372::
    quest:DeregisterTimer(i_stk_2c)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

