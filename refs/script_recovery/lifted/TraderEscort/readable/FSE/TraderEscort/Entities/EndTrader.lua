-- Readable native conversion: EndTrader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM = 0, 1  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local endingCanStart

-- EndTrader.Main (retail 0x00e055b0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local taskRunning, scratchValue29, getPos, endTheQuestHere, this_00, movie, movie3
    local function ReleaseEverything()
        local this_00 = movie
        resources:DestroyMovie(this_00)
    end
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e06038 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e06038 end
    endTheQuestHere = quest:GetThingWithScriptName("M_EndTheQuestHere")
    if not quest:IsActiveThreadTerminating() then
        while quest:GetStateBool("IntroFinished") do
            while not quest:IsDistanceBetweenThingsUnder(endTheQuestHere, hero, 3.0) do
                if not quest:NewScriptFrame(me) then goto LAB_00e0602f end
            end
            local tradersStillAliveCounter = quest:GetStateInt("TradersStillAliveCounter")
            if #quest:GetAllThingsWithScriptName("DarkwoodTrader") == tradersStillAliveCounter and 0 < tradersStillAliveCounter then
                endingCanStart = true
            end
            if endingCanStart then
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:SheatheHeroWeapons()
                quest:Pause(0.2)
                if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00e05c45 end
                me:Speak(hero, "TEXT_QST_067_ENDTRADER_GREETINGS", GROUP_SELECT_FIRST, false, true, false)
                taskRunning = me:IsPerformingScriptTask()
                goto LAB_00e05be8
            end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:Pause(0.5)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto FLOW_after_lab_00e0602a
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    goto FLOW_after_lab_00e0602a
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            while quest:IsRegionLoaded("BarrowFields") do
                if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00e0602a end
                if me:IsTalkedToByHero() then
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_067_ENDTRADER_NOT_ALL_PRESENT", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                ReleaseEverything(); return  -- TODO(native): goto FLOW_after_lab_00e0602a
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            ReleaseEverything()
                            goto FLOW_after_lab_00e0602a
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                end
            end
            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00e0602a end
            if not quest:NewScriptFrame(me) then goto LAB_00e0602f end
        end
        if not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(resource)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
    end
    ::LAB_00e0602f::
    ::LAB_00e06038::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e05be8::
    if taskRunning then
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            goto FLOW_after_lab_00e0602a
        end
        taskRunning = me:IsPerformingScriptTask()
        goto LAB_00e05be8
    end
    if not quest:IsActiveThreadTerminating() then goto LAB_00e05c45 end
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    ::FLOW_after_lab_00e0602a::
    goto FLOW_past_lab_00e05c45
    ::LAB_00e05c45::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    quest:SetStateBool("EndStarted", true)
    if not quest:IsActiveThreadTerminating() then
        local traderEndPos = quest:GetThingWithScriptName("TraderEndPos")
        if traderEndPos == nil then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = traderEndPos:GetPos()
        end
        local scratchValue37 = {x = getPos.x, y = getPos.y, z = getPos.z}
        local scratchValue = resources:ScriptThing(resource)
        if scratchValue ~= nil and scratchValue:IsDistanceFromPositionOver(scratchValue37, 5.0) then
            repeat
                if not quest:NewScriptFrame(me) then goto LAB_00e05fd1 end
                me:MoveToPosition(scratchValue37, 3.0, ENTITY_MOVE_WALK, false, true)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e05fd1 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e05fd1 end
                scratchValue29 = resources:ScriptThing(resource)
            until not (scratchValue29 ~= nil and scratchValue29:IsDistanceFromPositionOver(scratchValue37, 5.0))
        end
        if not quest:IsActiveThreadTerminating() then
            quest:MiniMapRemoveMarker(me)
            resources:PrepareResource(resource)
            if not quest:IsActiveThreadTerminating() then
                repeat
                    if not me:IsTalkedToByHero() then
                        quest:NewScriptFrame(me)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00e05fd1 end
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00e05fd1 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e05fd1 end
                        local movie4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_067_ENDTRADER_THANKS", GROUP_SELECT_RANDOM, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00e05fd1
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e05fd1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        resources:PrepareResource(resource)
                        quest:NewScriptFrame(me)
                    end
                until quest:IsActiveThreadTerminating()
                goto LAB_00e0602f
            end
        end
        ::LAB_00e05fd1::
    end
    ::FLOW_past_lab_00e05c45::
    goto LAB_00e0602f
end

-- EndTrader.Init (retail 0x00e04b50)
function Init(quest, me)
    endingCanStart = false
    quest:SetIsPushableByHero(me, false)
    if quest:GetStateBool("IntroFinished") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    end
end

-- EndTrader.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- EndTrader.OnPredicateFail (retail 0x00e04ae0)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") and endingCanStart then
        quest:SetStateBool("EndStarted", true)
    end
end

