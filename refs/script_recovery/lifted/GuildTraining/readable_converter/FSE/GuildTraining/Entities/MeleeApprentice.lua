-- Readable native conversion: MeleeApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local willWoodsChatDone, waitingForFight

-- MeleeApprentice.Main (retail 0x00d40cf0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, movie, getPos, getPos2, meleeApprenticeMarker, movie6, resource
    local function ReleaseEverything()
        local movie = movie6
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySheatheWeapons(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAllowBossPhaseChanges(me, false)
    me:SetFriendsWithEverythingFlag(true)
    meleeApprenticeMarker = quest:GetThingWithScriptName("MeleeApprenticeMarker")
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            meleeApprenticeMarker = nil
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        end
        if quest:GetStateBool("StartedMeleeTesting") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while quest:GetStateBool("StartedMeleeTesting") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:ClearCommands()
            quest:EntitySheatheWeapons(me, false)
        end
        local isQuestActive = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        local predicateResult31 = quest:IsActiveThreadTerminating()
        if isQuestActive then
            if predicateResult31 then
                resources:ReleaseResource(resource)
                return
            end
            if not willWoodsChatDone then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local position = quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK"):GetPos()
                me:MoveToPosition(position, 3.0, ENTITY_MOVE_RUN, false, true)
                willWoodsChatDone = true
            else
                if not me:IsTalkedToByHero() then
                    quest:NewScriptFrame(me);
                    predicateResult = quest:IsActiveThreadTerminating();
                    goto continue_6
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                me:ClearCommands()
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue40 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue40) then
                    local line = "TEXT_QST_028_WHISPER_SCORPION_WOODS"
                    local hero = quest:GetHero()
                    if not me:Speak(hero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie3
                        ReleaseEverything2(); return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie3
                            ReleaseEverything2(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie3
                        ReleaseEverything2(); return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                local position2 = quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK"):GetPos()
                me:MoveToPosition(position2, 3.0, ENTITY_MOVE_RUN, false, true)
            end
        else
            if predicateResult31 then
                resources:ReleaseResource(resource)
                return
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                me:ClearCommands()
                if quest:IsQuestActive("Q_GuildTrainingSkill") then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue43 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue43) then
                        local line2 = "TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN"
                        local hero2 = quest:GetHero()
                        if not me:Speak(hero2, line2, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            ReleaseEverything2(); return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie5
                                ReleaseEverything2(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            ReleaseEverything2(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie5
                    goto LAB_00d41813
                else
                    if quest:IsQuestActive("Q_GuildTrainingWill") then
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                        local movie4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue42 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue42) then
                            local line3 = "TEXT_QST_028_TEEN_WHISPER_WILL_MOAN"
                            local hero3 = quest:GetHero()
                            if not me:Speak(hero3, line3, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie4
                                ReleaseEverything2(); return
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie4
                                    ReleaseEverything2(); do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie4
                                ReleaseEverything2(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie4
                        goto LAB_00d41813
                    end
                    if quest:IsQuestActive("Q_GuildTrainingDeparture") then
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                        local isQuestActive5 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
                        local predicateResult32 = quest:IsActiveThreadTerminating()
                        if isQuestActive5 then
                            if predicateResult32 then
                                resources:ReleaseResource(resource)
                                return
                            end
                            local movie2 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            local scratchValue41 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue41) then
                                local line4 = "TEXT_QST_028_WHISPER_END_MOAN"
                                local hero4 = quest:GetHero()
                                if not me:Speak(hero4, line4, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie2
                                    resources:DestroyMovie(movie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        movie = movie2
                                        resources:DestroyMovie(movie)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie2
                                    ReleaseEverything2()
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie2
                        else
                            if predicateResult32 then
                                resources:ReleaseResource(resource)
                                return
                            end
                            movie6 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            local scratchValue44 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue44) then
                                local line5 = "TEXT_QST_028_WHISPER_MELEE_MOAN"
                                local hero5 = quest:GetHero()
                                if not me:Speak(hero5, line5, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    ReleaseEverything(); return
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        ReleaseEverything(); do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    ReleaseEverything()
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie6
                        end
                        goto LAB_00d41813
                    end
                end
                goto FLOW_past_lab_00d41813
                ::LAB_00d41813::
                resources:DestroyMovie(movie)
                ::FLOW_past_lab_00d41813::
                if not (meleeApprenticeMarker ~= nil and not meleeApprenticeMarker:IsNull()) then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = meleeApprenticeMarker:GetPos()
                end
                me:MoveToPosition(getPos, 3.0, ENTITY_MOVE_RUN, false, true)
            end
            local isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(me, meleeApprenticeMarker, 4.0) and not me:IsPerformingScriptTask()
            if isDistanceBetweenThingsOver then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                if not (meleeApprenticeMarker ~= nil and not meleeApprenticeMarker:IsNull()) then
                    getPos2 = {x = 0, y = 0, z = 0}
                else
                    getPos2 = meleeApprenticeMarker:GetPos()
                end
                me:MoveToPosition(getPos2, 3.0, ENTITY_MOVE_RUN, false, true)
            end
        end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_6::
    until false
end

-- MeleeApprentice.Init (retail 0x00d40cc0)
function Init(quest, me)
    waitingForFight = true
    willWoodsChatDone = false
end

-- MeleeApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- MeleeApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

