-- Generated native draft: Global_TeleportToHeroGuild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, b2, bVar3, bVar5, cVar4, ePriority, fVar12, fret_0, fret_00, iVar9, pCVar6, pCVar7, pCVar8, pScriptObject, r1, uVar10, uVar11, xStack_10, xStack_1c
    local alive = true
    pCVar6 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    bVar3 = quest:IsTeleportingActive()
    if not bVar3 then goto LAB_00cdda71 end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    r1 = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    iVar9 = 0
    __native_condition_1 = (r1 ~= nil and not r1:IsNull())
    if __native_condition_1 then
        cVar4 = (r1 ~= nil and r1:IsAlive())
        __native_condition_1 = cVar4
    end
    if __native_condition_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        fVar12 = 5.0
        pCVar6 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, fVar12)
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            bVar3 = quest:IsRegionLoaded("HeroGuildComplexInside")
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar3 then
                if bVar5 then
                    return
                end
                pCVar6 = quest:GetHero()
                xStack_1c = quest:GetFollowingEntityList(pCVar6)
                xStack_10 = resources:NewResource()
                ePriority = 4
                pScriptObject = xStack_10
                pCVar6 = quest:GetHero()
                resources:TryAcquire(pScriptObject, pCVar6, ePriority)
                uVar10 = 0
                if #xStack_1c ~= 0 then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            resources:ReleaseResource(xStack_10)
                            return
                        end
                        resources:PerformExpression(xStack_10, xStack_1c[(iVar9) / 0xc + 1], "EXPRESSION_WAIT")
                        uVar10 = uVar10 + 1
                        iVar9 = iVar9 + 0xc
                    until not (uVar10 < (#xStack_1c))
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00cdd9d7: (native jump target)
                    resources:ReleaseResource(xStack_10)
                    return
                end
                pCVar6 = quest:GetHero()
                pCVar8 = quest:GetHero()
                fret_00 = pCVar6:GetAngleXY()
                fVar12 = fret_00
                pCVar7 = pCVar8:GetPos()
                quest:SetGuildSealRecallLocation(pCVar7, fVar12)
                bVar3 = true
                pCVar6 = r1
                pCVar8 = quest:GetHero()
                quest:EntityTeleportToThing(pCVar8, pCVar6, bVar3)
                resources:ReleaseResource(xStack_10)
                goto LAB_00cdda68
            end
            if bVar5 then
                return
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00cdd9e9: (native jump target)
            return
        end
        pCVar7 = quest:GetGuildSealRecallPos()
        if 0.0001 * 0.0001 < pCVar7.z * pCVar7.z + pCVar7.y * pCVar7.y + pCVar7.x * pCVar7.x then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            b2 = true
            uVar11 = true
            fVar12 = 0.0
            pCVar7 = quest:GetGuildSealRecallPos()
            pCVar6 = quest:GetHero()
            quest:EntityTeleportToPosition(pCVar6, pCVar7, fVar12, uVar11, b2)
            uVar11 = true
            fret_0 = quest:GetGuildSealRecallAngleXY()
            fVar12 = fret_0
            pCVar6 = quest:GetHero()
            quest:EntitySetFacingAngle(pCVar6, fVar12, uVar11)
        end
        quest:SetGuildSealRecallLocation({x = 0, y = 0, z = 0}, 0.0)
    end
    ::LAB_00cdda68::
    ::LAB_00cdda71::
    quest:DeactivateQuestLater("Global_TeleportToHeroGuild", 0)
end

function Init(quest)
end

function OnPersist(quest, context)
end

