-- Generated native draft: Expression_Wait. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar3, delay, iVar6, pCVar4, pQuestName, pppuVar5, xStack_20, xStack_30
    local alive = true
    pCVar4 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_30 = resources:NewResource()
        iVar6 = 4
        pppuVar5 = xStack_30
        pCVar4 = quest:GetHero()
        bVar3 = resources:TryAcquire(pppuVar5, pCVar4, iVar6)
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00eea668 end
            iVar6 = 4
            pppuVar5 = xStack_30
            pCVar4 = quest:GetHero()
            bVar3 = resources:TryAcquire(pppuVar5, pCVar4, iVar6)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_20 = resources:StartMovie("")
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(1.0)
            -- TODO(native): CScriptBase::KillSpawnedFunction((CScriptBase *)this,(int)&xStack_34,0);
            quest:DeactivateQuest("Expression_Follow", 0)
            quest:CameraDefault()
            quest:Pause(1.0)
            quest:FadeScreenIn()
            delay = 0
            pQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pQuestName, delay)
            resources:DestroyMovie(xStack_20)
        end
        ::LAB_00eea668::
        resources:ReleaseResource(xStack_30)
    end
end

function Init(quest)
end

function OnPersist(quest, context)
end

