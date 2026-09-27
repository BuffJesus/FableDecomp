-- Generated native draft: CS_OakValeRevisited. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local alive = true
    quest:FinalizeEntityBindings()
    local bVar1 = quest:IsRegionLoaded("OakBay")
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:AddLogbookStoryEntry(100)
                helper_EE8390(quest)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsRegionLoaded("OakBay")
    end
end

function OakValeFire(quest)
    local bVar6, ctr_28, iVar10, iVar2, i_stk_24, lst_FirePoint, pCVar9, pPosition, uVar12, u_stk_18
    local alive = true
    local cVar1 = quest:RetailFlags("OakValeFlag"):Get("fire")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        cVar1 = quest:RetailFlags("OakValeFlag"):Get("fire")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        lst_FirePoint = quest:GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT")
        quest:StateListSet("FirePoint", lst_FirePoint)
        iVar10 = 0
        iVar2 = (quest:GetStateListCount("FirePoint") * 0xc)
        quest:StateListResize("Fires", (iVar2 - iVar10) / 0xc)
        u_stk_18 = 0
        if quest:GetStateListCount("FirePoint") ~= 0 then
            ctr_28 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                i_stk_24 = quest:GetStateListRef("Fires")
                bVar6 = false
                pPosition = quest:GetStateListAt("FirePoint", (ctr_28) / 0xc):GetPos()
                pCVar9 = quest:CreateEffectAtPos("OAKVALE_BURNING_PATCH", pPosition, 0.0, bVar6)
                quest:StateListSetAt("Fires", (ctr_28) / 0xc, pCVar9)
                ctr_28 = ctr_28 + 0xc
                u_stk_18 = u_stk_18 + 1
            until not (u_stk_18 < quest:GetStateListCount("FirePoint"))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            cVar1 = quest:RetailFlags("OakValeFlag"):Get("fire")
            while cVar1 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    return
                end
                cVar1 = quest:RetailFlags("OakValeFlag"):Get("fire")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                iVar10 = 0
                iVar2 = (quest:GetStateListCount("FirePoint") * 0xc)
                quest:StateListResize("Fires", (iVar2 - iVar10) / 0xc)
                uVar12 = 0
                if quest:GetStateListCount("Fires") ~= 0 then
                    iVar10 = 0
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            return
                        end
                        quest:RemoveThing(quest:GetStateListAt("Fires", (iVar10) / 0xc), false, true)
                        uVar12 = uVar12 + 1
                        iVar10 = iVar10 + 0xc
                    until not (uVar12 < quest:GetStateListCount("Fires"))
                end
                alive = not quest:IsActiveThreadTerminating()
            end
        end
    end
end

function helper_EE8390(quest)
    local resources = quest:RetailResources()
    local bVar10, cVar7, iVar11, iVar9, v_stk_44, xStack_2c
    local alive = true
    quest:CreateThread("OakValeFire")  -- native thread body NScript::CCS_OakValeRevisitedScript::OakValeFire: lift it as function OakValeFire(quest)
    if not bVar10 then
    end
    local pCVar8 = quest:GetThingWithScriptName("OV_HERO_START_MARKER")
    local pThingToMove = quest:GetHero()
    quest:EntityTeleportToThing(pThingToMove, pCVar8, false)
    pCVar8 = nil
    local xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    local xStack_20 = resources:NewResource()
    local xStack_38 = resources:NewActorMap()
    iVar11 = 4
    local pScriptObject = xStack_20
    pCVar8 = quest:GetHero()
    resources:TryAcquire(pScriptObject, pCVar8, iVar11)
    resources:SetActor(xStack_38, "Hero", xStack_20)
    resources:RunMacroWithFlags("CS_OAKVALE_REVISITED", xStack_38, quest:RetailFlags("OakValeFlag"), false, true)
    resources:DestroyActorMap(xStack_38)
    resources:ReleaseResource(xStack_20)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    iVar9 = 0
    iVar11 = (quest:GetStateListCount("FirePoint") * 0xc)
    quest:StateListResize("Fires", (iVar11 - iVar9) / 0xc)
    v_stk_44 = 0
    if quest:GetStateListCount("Fires") ~= 0 then
        iVar9 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if bVar10 then
                return
            end
            quest:RemoveThing(quest:GetStateListAt("Fires", (iVar9) / 0xc), false, true)
            v_stk_44 = v_stk_44 + 1
            iVar9 = iVar9 + 0xc
        until not (v_stk_44 < quest:GetStateListCount("Fires"))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if not bVar10 then
        quest:DeactivateQuestLater("CS_OakValeRevisited", 0)
        xStack_2c = quest:GetAllThingsWithScriptName("Fence")
        v_stk_44 = 0
        if #xStack_2c ~= 0 then
            iVar9 = 0
            repeat
                cVar7 = xStack_2c[(iVar9) / 0xc + 1]:IsAlive()
                if cVar7 then
                    quest:RemoveThing(xStack_2c[(iVar9) / 0xc + 1], false, true)
                end
                v_stk_44 = v_stk_44 + 1
                iVar9 = iVar9 + 0xc
            until not (v_stk_44 < (#xStack_2c))
        end
    end
end

