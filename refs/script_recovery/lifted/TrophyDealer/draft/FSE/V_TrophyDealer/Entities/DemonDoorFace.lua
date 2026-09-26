-- Generated native draft: DemonDoorFace. Review coverage report before use.
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
    local bVar3, cVar4, fVar2, fret_0, iVar8, iVar9, p0, p1, p4, p5, pCVar5, pCVar6, pppuVar12, r1, r2, xStack_10, xStack_20, xStack_30, xStack_3c, xStack_48
    local alive = true
    xStack_30 = resources:NewResource()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        resources:ReleaseResource(xStack_30)
        return
    end
    resources:PrepareResource(xStack_30)
    bVar3 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_30)
            return
        end
        bVar3 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00ee7b22: (native jump target)
        resources:ReleaseResource(xStack_30)
        return
    end
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        while true do
            bVar3 = me:IsTalkedToByHero()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00ee808a end
                if not quest:GetMasterGameState("SingingStonesInSync") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00ee808a end
                    xStack_20 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_3c = resources:ScriptThing(xStack_30)
                    pCVar5 = xStack_3c
                    fret_0 = quest:GetHealth(pCVar5)
                    fVar2 = 0.0
                    if fVar2 < fret_0 then
                        p5 = 0
                        p4 = 1
                        iVar9 = 0
                        iVar8 = 0
                        p1 = "TEXT_QST_071_DOOR_LOCKED1"
                        pCVar5 = quest:GetHero()
                        r1 = me:Speak(pCVar5, p1, iVar8, (iVar9 ~= 0), (p4 ~= 0), (p5 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar4 = iVar8
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20)
                                resources:ReleaseResource(xStack_30)
                                return
                            end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar4 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20)
                            resources:ReleaseResource(xStack_30)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20)
                end
                if not quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00ee808a end
                    quest:SetMasterGameState("TrophyDealerHeroSpokenToDemonDoors", true)
                    pCVar6 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(pCVar6, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_03", "Witchwood2", "")
                end
            end
            if quest:GetMasterGameState("SingingStonesInSync") then break end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(xStack_30)
                return
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_20 = resources:NewResource()
            iVar9 = 4
            pppuVar12 = xStack_20
            pCVar5 = quest:GetHero()
            resources:TryAcquire(pppuVar12, pCVar5, iVar9)
            resources:PrepareResource(xStack_30)
            bVar3 = resources:TryAcquire(xStack_30, me, 4)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    resources:ReleaseResource(xStack_20)
                    resources:ReleaseResource(xStack_30)
                    return
                end
                bVar3 = resources:TryAcquire(xStack_30, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                -- LAB_00ee7e82: (native jump target)
                resources:ReleaseResource(xStack_20)
                resources:ReleaseResource(xStack_30)
                return
            end
            xStack_48 = resources:NewActorMap()
            resources:SetActor(xStack_48, "HERO", xStack_20)
            resources:SetActor(xStack_48, "DOOR", xStack_30)
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_TROPHY_DEALER_DOOR_OPENS", xStack_48, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
            resources:DestroyActorMap(xStack_48)
            resources:ReleaseResource(xStack_20)
            pCVar6 = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pCVar6, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_04", "WitchwoodCavern", "")
            r2 = quest:GetThingWithScriptName("DemonDoorDoor")
            quest:SetThingPersistent(r2, true)
            quest:SetMasterGameState("SingingStonesInSync", false)
            quest:RemoveThing(me, false, true)
        end
    end
    ::LAB_00ee808a::
    resources:ReleaseResource(xStack_30)
end

function Init(quest, me)
    __native_entity_state:SetStateInt("TimesSpoken", 0)
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsDamageable(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

