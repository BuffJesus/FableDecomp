-- Readable native conversion: MeleeApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- MeleeApprentice.Main (retail 0x00d40cf0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult, scratchValue3, movie, position, meleeApprenticeMarker
    local movie2, movie3, movie4, movie5, movie6
    local function __cleanup_LAB_00d419fe()
        movie = movie6
        resources:DestroyMovie(movie)
        resources:ReleaseResource(0)
    end
    local function __cleanup_LAB_00d41a02()
        resources:DestroyMovie(movie)
        resources:ReleaseResource(0)
    end
    scratchValue = me:AcquireControl(4)
    while not scratchValue do
        if not quest:NewScriptFrame(me) then return end
        scratchValue = me:AcquireControl(4)
    end
    if quest:IsActiveThreadTerminating() then me:ReleaseControl(); return end
    quest:EntitySheatheWeapons(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAllowBossPhaseChanges(me, false)
    me:SetFriendsWithEverythingFlag(1)
    meleeApprenticeMarker = quest:GetThingWithScriptName("MeleeApprenticeMarker")
    scratchValue = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue then
            return
        end
        if quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") ~= 0 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
            scratchValue3 = quest:GetMasterGameState("MeleeApprenticeNeededForCutscene")
            while scratchValue3 ~= 0 do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(0); return end
                scratchValue3 = quest:GetMasterGameState("MeleeApprenticeNeededForCutscene")
            end
            scratchValue = me:AcquireControl(4)
            while not scratchValue do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(0); return end
                scratchValue = me:AcquireControl(4)
            end
        end
        if quest:GetStateBool("StartedMeleeTesting") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
            scratchValue3 = quest:GetStateBool("StartedMeleeTesting")
            while scratchValue3 do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(0); return end
                scratchValue3 = quest:GetStateBool("StartedMeleeTesting")
            end
            scratchValue = me:AcquireControl(4)
            while not scratchValue do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(0); return end
                scratchValue = me:AcquireControl(4)
            end
            me:ClearCommands()
            quest:EntitySheatheWeapons(me, false)
        end
        predicateResult = quest:IsActiveThreadTerminating()
        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
            if predicateResult then
                resources:ReleaseResource(0)
                return
            end
            if not state:GetBool("WillWoodsChatDone") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK"):GetPos(), 0x40400000, 1, false, true)
                state:SetBool("WillWoodsChatDone", true)
            elseif me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                me:ClearCommands()
                movie3 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                    me:Speak(hero, "TEXT_QST_028_WHISPER_SCORPION_WOODS", 0, false, true, false)
                    scratchValue3 = me:IsPerformingScriptTask()
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie3
                            resources:DestroyMovie(movie)
                            -- TODO(native): goto LAB_00d41a07_c14
                        end
                        scratchValue3 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie3
                        resources:DestroyMovie(movie)
                        -- TODO(native): goto LAB_00d41a07_c15
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK"):GetPos(), 0x40400000, 1, false, true)
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
                        me:Speak(hero, "TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie5
                                __cleanup_LAB_00d41a02(); return
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                            __cleanup_LAB_00d41a02(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie5
                    resources:DestroyMovie(movie)
                else
                    if quest:IsQuestActive("Q_GuildTrainingWill") then
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                        movie4 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                            me:Speak(hero, "TEXT_QST_028_TEEN_WHISPER_WILL_MOAN", 0, false, true, false)
                            scratchValue3 = me:IsPerformingScriptTask()
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie4
                                    __cleanup_LAB_00d41a02(); return
                                end
                                scratchValue3 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie4
                                __cleanup_LAB_00d41a02(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie4
                        resources:DestroyMovie(movie)
                        goto FLOW_after_lab_00d41813
                    end
                    if quest:IsQuestActive("Q_GuildTrainingDeparture") then
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(0); return end
                        if quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
                            movie2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                me:Speak(hero, "TEXT_QST_028_WHISPER_END_MOAN", 0, false, true, false)
                                scratchValue3 = me:IsPerformingScriptTask()
                                while scratchValue3 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        movie = movie2
                                        resources:DestroyMovie(movie)
                                        resources:ReleaseResource(0)
                                        return
                                    end
                                    scratchValue3 = me:IsPerformingScriptTask()
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
                                me:Speak(hero, "TEXT_QST_028_WHISPER_MELEE_MOAN", 0, false, true, false)
                                scratchValue3 = me:IsPerformingScriptTask()
                                while scratchValue3 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        __cleanup_LAB_00d419fe(); return
                                    end
                                    scratchValue3 = me:IsPerformingScriptTask()
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
        scratchValue = quest:IsActiveThreadTerminating()
    until false
end

-- MeleeApprentice.Init (retail 0x00d40cc0)
function Init(quest, me)
    state:SetBool("WaitingForFight", true)
    state:SetBool("WillWoodsChatDone", false)
end

-- MeleeApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- MeleeApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

