-- Generated native draft: Expression_Follow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    return quest:WithRetailResources(function(resources)
    local bVar2, iVar5, pCVar3, pppuVar4, u_stk_38, xStack_20, xStack_30
    local alive = true
    u_stk_38 = 0
    pCVar3 = quest:GetHero()
    quest:RegisterQuestConsciousCondition(pCVar3)
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_30 = resources:NewResource()
        iVar5 = 4
        pppuVar4 = xStack_30
        pCVar3 = quest:GetHero()
        bVar2 = resources:TryAcquire(pppuVar4, pCVar3, iVar5)
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00eea22f end
            iVar5 = 4
            pppuVar4 = xStack_30
            pCVar3 = quest:GetHero()
            bVar2 = resources:TryAcquire(pppuVar4, pCVar3, iVar5)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            xStack_20 = resources:StartMovie("")
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            quest:CreateThread("FollowThread")  -- native thread body CExpression_FollowScript__FollowThread: lift it as function FollowThread(quest)
            quest:CameraDefault()
            quest:Pause(1.0)
            quest:FadeScreenIn()
            resources:DestroyMovie(xStack_20)
        end
        ::LAB_00eea22f::
        resources:ReleaseResource(xStack_30)
    end
    end)
end

function Init(quest)
end

function OnPersist(quest, context)
end

function FollowThread(quest)
    return quest:WithRetailResources(function(resources)
    local followTargetOwner = 0
    local bVar3, hero, pQuestName, xStack_10
    local alive = true
    local r1 = quest:GetHeroTargetedThing()
    followTargetOwner = resources:TrackThingOutput(r1)
    local __native_condition_1 = r1 == nil
    local cVar2
    if not __native_condition_1 then
        cVar2 = (r1 ~= nil and r1:IsAlive())
        __native_condition_1 = not cVar2
    end
    if __native_condition_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            pQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pQuestName, 0)
            resources:RetireThingOutput(followTargetOwner); followTargetOwner = 0
            return
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_10 = resources:NewResource()
            resources:TryAcquire(xStack_10, r1, 4)
            hero = quest:GetHero()
            resources:FollowThing(xStack_10, hero, 1.0, true)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            while not bVar3 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            end
            resources:ReleaseResource(xStack_10)
            resources:RetireThingOutput(followTargetOwner); followTargetOwner = 0
            return
        end
    end
    resources:RetireThingOutput(followTargetOwner); followTargetOwner = 0
    end)
end

