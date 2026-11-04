-- Readable native conversion: CS_OakValeRevisited. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CS_OakValeRevisited.Main (retail 0x00ee82d0)
function Main(quest)
    quest:FinalizeEntityBindings()
    local isRegionLoaded = quest:IsRegionLoaded("OakBay")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:AddLogbookStoryEntry(100)
            helper_EE8390(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("OakBay")
    end
end

-- CS_OakValeRevisited.OakValeFire (retail 0x00ee8870)
function OakValeFire(quest)
    local ctr_28, firesIndex, scratchValue5, scratchValue6
    while not quest:RetailFlags("OakValeFlag"):Get("fire") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local lst_FirePoint = quest:GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT")
    quest:StateListSet("FirePoint", lst_FirePoint)
    local getStateListCount = quest:GetStateListCount("FirePoint") * 12
    quest:StateListResize("Fires", (getStateListCount - 0) / 12)
    scratchValue6 = 0
    if quest:GetStateListCount("FirePoint") ~= 0 then
        ctr_28 = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            local pPosition = quest:GetStateListAt("FirePoint", ctr_28):GetPos()
            local scratchValue4 = quest:CreateEffectAtPos("OAKVALE_BURNING_PATCH", pPosition, 0.0, false)
            quest:StateListSetAt("Fires", ctr_28, scratchValue4)
            ctr_28 = ctr_28 + 1
            scratchValue6 = scratchValue6 + 1
        until not (scratchValue6 < quest:GetStateListCount("FirePoint"))
    end
    if quest:IsActiveThreadTerminating() then return end
    while quest:RetailFlags("OakValeFlag"):Get("fire") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local getStateListCount2 = quest:GetStateListCount("FirePoint") * 12
    quest:StateListResize("Fires", (getStateListCount2 - 0) / 12)
    scratchValue5 = 0
    if quest:GetStateListCount("Fires") == 0 then quest:IsActiveThreadTerminating(); return end
    firesIndex = 0
    repeat
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveThing(quest:GetStateListAt("Fires", firesIndex), false, true)
        scratchValue5 = scratchValue5 + 1
        firesIndex = firesIndex + 1
    until not (scratchValue5 < quest:GetStateListCount("Fires"))
    quest:IsActiveThreadTerminating()
end

-- CS_OakValeRevisited.helper_EE8390 (retail 0x00ee8390)
function helper_EE8390(quest)
    local resources = quest:RetailResources()
    local scratchValue, firesIndex, scratchValue5, v_stk_44_1, v_stk_44_2
    quest:CreateThread("OakValeFire")  -- native thread body NScript::CCS_OakValeRevisitedScript::OakValeFire: lift it as function OakValeFire(quest)
    if not scratchValue then
    end
    local thing = quest:GetThingWithScriptName("OV_HERO_START_MARKER")
    local pThingToMove = quest:GetHero()
    quest:EntityTeleportToThing(pThingToMove, thing, false)
    thing = nil
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    local resource = resources:NewResource()
    local actorMap = resources:NewActorMap()
    thing = quest:GetHero()
    resources:TryAcquire(resource, thing, 4)
    resources:SetActor(actorMap, "Hero", resource)
    resources:RunMacroWithFlags("CS_OAKVALE_REVISITED", actorMap, quest:RetailFlags("OakValeFlag"), false, true)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    local getStateListCount = quest:GetStateListCount("FirePoint") * 12
    quest:StateListResize("Fires", (getStateListCount - 0) / 12)
    v_stk_44_1 = 0
    if quest:GetStateListCount("Fires") ~= 0 then
        firesIndex = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            quest:RemoveThing(quest:GetStateListAt("Fires", firesIndex), false, true)
            v_stk_44_1 = v_stk_44_1 + 1
            firesIndex = firesIndex + 1
        until not (v_stk_44_1 < quest:GetStateListCount("Fires"))
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuestLater("CS_OakValeRevisited", 0)
    local fence = quest:GetAllThingsWithScriptName("Fence")
    v_stk_44_2 = 0
    if not (#fence ~= 0) then return end
    scratchValue5 = 0
    repeat
        if fence[scratchValue5 + 1]:IsAlive() then
            quest:RemoveThing(fence[scratchValue5 + 1], false, true)
        end
        v_stk_44_2 = v_stk_44_2 + 1
        scratchValue5 = scratchValue5 + 1
    until not (v_stk_44_2 < (#fence))
end

