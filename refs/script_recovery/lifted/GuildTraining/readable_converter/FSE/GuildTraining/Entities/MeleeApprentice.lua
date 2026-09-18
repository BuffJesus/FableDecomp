-- Readable native conversion: MeleeApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local willWoodsChatDone, waitingForFight

-- MeleeApprentice.Main (retail 0x00d40cf0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, movie, position, meleeApprenticeMarker, movie2, movie3, movie4, movie5
    local movie6, resource
    local function __cleanup_LAB_00d419fe()
        local movie = movie6
        resources:DestroyMovie(movie)
        resources:ReleaseResource(0)
    end
    local function __cleanup_LAB_00d41a02()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(0)
    end
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySheatheWeapons(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAllowBossPhaseChanges(me, false)
    me:SetFriendsWithEverythingFlag(1)
    meleeApprenticeMarker = quest:GetThingWithScriptName("MeleeApprenticeMarker")
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") ~= 0 then
            while quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") ~= 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(0)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
            while not resources:TryAcquire(0, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(0)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
        end
        if quest:GetStateBool("StartedMeleeTesting") then
            while quest:GetStateBool("StartedMeleeTesting") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(0)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
            while not resources:TryAcquire(0, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(0)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
            me:ClearCommands()
            quest:EntitySheatheWeapons(me, false)
        end
        predicateResult = quest:IsActiveThreadTerminating()
        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
            if predicateResult then
                resources:ReleaseResource(0)
                return
            end
            if not willWoodsChatDone then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK"):GetPos(), 3.0, 1, false, true)
                willWoodsChatDone = true
            else
                if not me:IsTalkedToByHero() then quest:NewScriptFrame(me); goto continue_6 end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                me:ClearCommands()
                movie3 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                    me:Speak(hero, "TEXT_QST_028_WHISPER_SCORPION_WOODS", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie3
                            resources:DestroyMovie(movie3)
                            -- TODO(native): goto LAB_00d41a07_c14
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie3
                        resources:DestroyMovie(movie3)
                        -- TODO(native): goto LAB_00d41a07_c15
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK"):GetPos(), 3.0, 1, false, true)
            end
        else
            if predicateResult then
                resources:ReleaseResource(0)
                return
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                me:ClearCommands()
                if quest:IsQuestActive("Q_GuildTrainingSkill") then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                    movie5 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                        me:Speak(hero, "TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie5
                                __cleanup_LAB_00d41a02(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            __cleanup_LAB_00d41a02(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie5
                    resources:DestroyMovie(movie5)
                else
                    if quest:IsQuestActive("Q_GuildTrainingWill") then
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                        movie4 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                            me:Speak(hero, "TEXT_QST_028_TEEN_WHISPER_WILL_MOAN", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie4
                                    __cleanup_LAB_00d41a02(); do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie4
                                __cleanup_LAB_00d41a02(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie4
                        resources:DestroyMovie(movie4)
                        goto FLOW_after_lab_00d41813
                    end
                    if quest:IsQuestActive("Q_GuildTrainingDeparture") then
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                        if quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
                            movie2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                me:Speak(hero, "TEXT_QST_028_WHISPER_END_MOAN", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        movie = movie2
                                        resources:DestroyMovie(movie2)
                                        resources:ReleaseResource(0)
                                        do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie2
                                    __cleanup_LAB_00d41a02()
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie2
                        else
                            movie6 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                me:Speak(hero, "TEXT_QST_028_WHISPER_MELEE_MOAN", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        __cleanup_LAB_00d419fe(); do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    __cleanup_LAB_00d419fe()
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie6
                        end
                        resources:DestroyMovie(movie)
                        goto FLOW_after_lab_00d41813
                    end
                end
                ::FLOW_after_lab_00d41813::
                if meleeApprenticeMarker ~= nil and not meleeApprenticeMarker:IsNull() then
                    position = meleeApprenticeMarker:GetPos()
                end
                me:MoveToPosition(position, 3.0, 1, false, true)
            end
            if quest:IsDistanceBetweenThingsOver(me, meleeApprenticeMarker, 4.0) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then
                    -- LAB_00d41a07: (native jump target)
                    resources:ReleaseResource(0)
                    return
                end
                if meleeApprenticeMarker ~= nil and not meleeApprenticeMarker:IsNull() then
                    position = meleeApprenticeMarker:GetPos()
                end
                me:MoveToPosition(position, 3.0, 1, false, true)
            end
        end
        quest:NewScriptFrame(me)
        ::continue_6::
    until false
end

-- MeleeApprentice.Init (retail 0x00d40cc0)
function Init(quest, me)
    waitingForFight = true
    willWoodsChatDone = false
end

-- MeleeApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- MeleeApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

