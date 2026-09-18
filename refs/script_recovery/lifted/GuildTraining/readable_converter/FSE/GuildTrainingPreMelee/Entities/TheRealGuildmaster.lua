-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TheRealGuildmaster.Main (retail 0x00d52e90)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local addQuestInfoCounter, scratchValue, scratchValue13, scratchValue14, scratchValue15
    local scratchValue16, scratchValue19, addNewConversation, switch, scratchValue20, scratchValue21
    local scratchValue23, scratchValue24, scratchValue25, timerId, scratchValue26, scratchValue28
    local scratchValue29, scratchValue30, resource, actorMap, movie, infoCounter, actorMap2
    local scratchValue31, scratchValue32, resource2, timerId2, timerId3, timerId4
    local function __region_LAB_00d555f3_c27()
        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
    end
    local function __region_LAB_00d555f3_c28()
        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
    end
    local function __region_LAB_00d555f3_c29()
        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
    end
    local function __region_LAB_00d555f3_c6()
        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
    end
    local function __region_LAB_00d55c9f_c19()
        quest:PauseAllNonScriptedEntities(scratchValue29 ~= 0)
    end
    local function __region_LAB_00d55c9f_c27()
        quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0)
    end
    local function __region_LAB_00d55c9f_c28()
        quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0)
    end
    local function __region_LAB_00d55c9f_c29()
        quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0)
    end
    local function __region_LAB_00d55c9f_c6()
        quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0)
    end
    local function __region_LAB_00d55c9f_c7()
        quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0)
    end
    local function __region_LAB_00d55cd5_c27()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c28()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c29()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c6()
        quest:PauseAllNonScriptedEntities(false)
    end
    scratchValue13 = quest:GetStateBool("GuildmasterTeleport")
    scratchValue26 = 0
    while not scratchValue13 do
        if not quest:NewScriptFrame(me) then return end
        scratchValue13 = quest:GetStateBool("GuildmasterTeleport")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01", "", "")
    resource2 = resources:NewResource()
    scratchValue = resources:TryAcquire(resource2, me, 4)
    while not scratchValue do
        if not quest:NewScriptFrame(me) then goto LAB_00d55c4f end
        scratchValue = resources:TryAcquire(resource2, me, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d55c4f end
    scratchValue29 = 0
    scratchValue30 = 0
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    scratchValue20 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(me, scratchValue20, false)
    timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 0)
    scratchValue15 = 1
    repeat
        if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
        if me:IsTalkedToByHero() then
            scratchValue15 = 0
        end
        if not quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) or 0 < quest:GetTimer(timerId4) then goto FLOW_native_label_1 end
        addNewConversation = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(addNewConversation, hero)
        quest:SetTimer(timerId4, 5)
        switch = scratchValue26
        repeat
            if switch == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, hero, false)
                me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                scratchValue26 = 1
                break
            else
                if switch == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, hero, false)
                    scratchValue26 = 2
                    goto FLOW_after_lab_00d53316
                elseif switch == 2 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, hero, false)
                    scratchValue26 = 3
                    break
                elseif switch == 3 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, hero, false)
                    scratchValue26 = 2
                end
                ::FLOW_after_lab_00d53316::
            end
        until true
        ::FLOW_native_label_1::
    until scratchValue15 == 0
    if not quest:IsActiveThreadTerminating() then
        quest:SetStateBool("WhisperStopFollowing", true)
        scratchValue31 = resources:NewResource()
        scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
        while not scratchValue do
            if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue31); goto FLOW_after_lab_00d533bb end
            scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
        end
        if quest:IsActiveThreadTerminating() then
            resources:DestroyMovie(scratchValue31)
        else
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", scratchValue31)
            resources:SetActor(actorMap2, "TEACHER", resource2)
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_PREMELEE_PUNCH", actorMap2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeWhisper"), false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap2)
            resources:DestroyMovie(scratchValue31)
            if quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH")
                    scratchValue = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
                        scratchValue = quest:MsgIsGameInfoClickedPast()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        timerId3 = quest:RegisterTimer()
                        timerId = timerId3
                        quest:SetTimer(timerId3, 10)
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                        infoCounter = addQuestInfoCounter
                        quest:DisplayQuestInfo(true)
                        scratchValue19 = quest:GetStateInt("DummyHits")
                        while scratchValue19 < 7 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                            quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                            if 0 ~= quest:GetStateInt("DummyHits") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                quest:SetTimer(timerId, 10)
                            end
                            if quest:GetTimer(timerId) < 1 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, hero, false)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                    end
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                quest:SetTimer(timerId3, 10)
                                timerId = timerId3
                                addQuestInfoCounter = infoCounter
                            end
                            scratchValue19 = quest:GetStateInt("DummyHits")
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:RemoveQuestInfoElement(addQuestInfoCounter)
                            quest:DisplayQuestInfo(false)
                            scratchValue31 = resources:NewResource()
                            scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                            while not scratchValue do
                                if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue31); goto FLOW_after_lab_00d53a0b end
                                scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d53a0b: (native jump target)
                                resources:DestroyMovie(scratchValue31)
                            else
                                actorMap2 = resources:NewActorMap()
                                resources:SetActor(actorMap2, "HERO", scratchValue31)
                                resources:SetActor(actorMap2, "TEACHER", resource2)
                                movie = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_PREMELEE_STICK", actorMap2, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:DestroyActorMap(actorMap2)
                                resources:DestroyMovie(scratchValue31)
                                if quest:IsXbox() then
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                        while not scratchValue do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            -- LAB_00d53c7e: (native jump target)
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(timerId, 10)
                                            addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                            infoCounter = addQuestInfoCounter
                                            quest:DisplayQuestInfo(true)
                                            scratchValue19 = quest:GetStateInt("DummyHits")
                                            while scratchValue19 < 7 do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                                                if 0 ~= quest:GetStateInt("DummyHits") then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                    quest:SetTimer(timerId, 10)
                                                end
                                                if quest:GetTimer(timerId) < 1 then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
                                                    if quest:IsXbox() then
                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    quest:SetTimer(timerId3, 10)
                                                    timerId = timerId3
                                                    addQuestInfoCounter = infoCounter
                                                end
                                                scratchValue19 = quest:GetStateInt("DummyHits")
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:RemoveQuestInfoElement(addQuestInfoCounter)
                                                quest:DisplayQuestInfo(false)
                                                scratchValue32 = nil
                                                movie = resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                scratchValue31 = resources:NewResource()
                                                scratchValue = resources:TryAcquire(0, hero, 4)
                                                while not scratchValue do
                                                    quest:NewScriptFrame(me)
                                                    if quest:IsActiveThreadTerminating() then
                                                        resources:DestroyMovie(scratchValue31)
                                                        resources:DestroyMovie(movie)
                                                        goto FLOW_after_lab_00d53ff2
                                                    end
                                                    SUB(scratchValue31,0)
                                                    scratchValue29 = scratchValue31 >> 16
                                                    scratchValue30 = scratchValue31 >> 24
                                                    scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                                                end
                                                if quest:IsActiveThreadTerminating() then
                                                    -- LAB_00d53ff2: (native jump target)
                                                    resources:DestroyMovie(scratchValue31)
                                                    resources:DestroyMovie(movie)
                                                else
                                                    scratchValue23 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(xStack_dc, "SMASH_DUMMY_01", actorMap, "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(scratchValue23, true, 1.0, true)
                                                    actorMap2 = resources:NewActorMap()
                                                    resources:SetActor(actorMap2, "HERO", scratchValue31)
                                                    resources:SetActor(actorMap2, "TEACHER", resource2)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    quest:CreateExperienceOrb(actorMap, 1)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(nil, 2)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                                                    quest:FixMovieSequenceCamera(false)
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyActorMap(actorMap2)
                                                    resources:DestroyMovie(scratchValue31)
                                                    resources:DestroyMovie(movie)
                                                    if quest:IsXbox() then
                                                        if not quest:IsActiveThreadTerminating() then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                                            while not scratchValue do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                                                                scratchValue = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                -- LAB_00d5439e: (native jump target)
                                                                timerId2 = quest:RegisterTimer()
                                                                quest:SetTimer(timerId2, 10)
                                                                scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                                while scratchValue13 do
                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                    if quest:GetTimer(timerId2) < 1 then
                                                                        addNewConversation = quest:AddNewConversation(me, false, false)
                                                                        quest:AddPersonToConversation(addNewConversation, hero)
                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                                                        quest:SetTimer(timerId2, 10)
                                                                    end
                                                                    scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                                end
                                                                if not quest:IsActiveThreadTerminating() then
                                                                    quest:Pause(0.5)
                                                                    scratchValue31 = resources:NewResource()
                                                                    SUB(scratchValue31,0)
                                                                    scratchValue29 = scratchValue31 >> 16
                                                                    scratchValue30 = scratchValue31 >> 24
                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                    while not scratchValue do
                                                                        if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                                                                        SUB(scratchValue31,0)
                                                                        scratchValue29 = scratchValue31 >> 16
                                                                        scratchValue30 = scratchValue31 >> 24
                                                                        scratchValue = resources:TryAcquire(0, hero, 4)
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then
                                                                        -- LAB_00d54dfa: (native jump target)
                                                                    else
                                                                        actorMap = resources:NewActorMap()
                                                                        resources:SetActor(actorMap, "HERO", scratchValue31)
                                                                        resources:SetActor(actorMap, "TEACHER", resource2)
                                                                        resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", actorMap, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(actorMap)
                                                                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                        quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                        quest:NewScriptFrame(me)
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            quest:NewScriptFrame(me)
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                quest:NewScriptFrame(me)
                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                    quest:NewScriptFrame(me)
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        quest:NewScriptFrame(me)
                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                            if quest:DisplayTutorial(28) then
                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                    scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                    while not scratchValue do
                                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                        scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                        scratchValue14 = 0
                                                                                                        repeat
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                                                                                            if not state:GetBool("WoodsEndPlayed") then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                state:SetBool("WoodsEndPlayed", true)
                                                                                                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                                resource = resources:NewResource()
                                                                                                                addNewConversation = 4
                                                                                                                SUB(resource,0)
                                                                                                                scratchValue29 = resource >> 16
                                                                                                                scratchValue30 = resource >> 24
                                                                                                                scratchValue = resources:TryAcquire(0, hero, 4)
                                                                                                                while not scratchValue do
                                                                                                                    quest:NewScriptFrame(me)
                                                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c6
                                                                                                                    addNewConversation = 4
                                                                                                                    SUB(resource,0)
                                                                                                                    scratchValue29 = resource >> 16
                                                                                                                    scratchValue30 = resource >> 24
                                                                                                                    scratchValue = me:AcquireControl(4)
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then
                                                                                                                    -- LAB_00d55c91_c6: (native jump target)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                actorMap2 = resources:NewActorMap()
                                                                                                                resources:SetActor(actorMap2, "HERO", resource)
                                                                                                                resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                                                resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                                                quest:FixMovieSequenceCamera(true)
                                                                                                                resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue19 < 0 do
                                                                                                                    quest:NewScriptFrame(me)
                                                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then
                                                                                                                    -- LAB_00d55c72_c6: (native jump target)
                                                                                                                    quest:PauseAllNonScriptedEntities(addNewConversation ~= 0)
                                                                                                                    -- LAB_00d55c7f_c6: (native jump target)
                                                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                                                    -- TODO(native): goto LAB_00d55c91_c6
                                                                                                                end
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if scratchValue then
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        -- TODO(native): goto LAB_00d55c7f_c6
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            quest:NewScriptFrame(me)
                                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:FixMovieSequenceCamera(false)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(actorMap2)
                                                                                                            else
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                    -- LAB_00d54f9c_c6: (native jump target)
                                                                                                                    scratchValue = false
                                                                                                                else
                                                                                                                    if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                                                                                                    scratchValue = true
                                                                                                                end
                                                                                                                if scratchValue then
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                    while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                    end
                                                                                                                    scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                                    while not scratchValue do
                                                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                        scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                                    end
                                                                                                                    resources:StartMovie("")
                                                                                                                    quest:StartMovieSequence()
                                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if quest:GetHealth(resources:ScriptThing(resource2)) <= scratchValue16 then
                                                                                                                        -- LAB_00d551d4_c6: (native jump target)
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while scratchValue19 < 0 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                            if scratchValue19 == 1 then
                                                                                                                                if scratchValue then
                                                                                                                                    __region_LAB_00d55c9f_c6()
                                                                                                                                    goto LAB_00d55c2b
                                                                                                                                end
                                                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                quest:Pause(1.0)
                                                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            else
                                                                                                                                if scratchValue then goto LAB_00d55cba_c6 end
                                                                                                                                scratchValue16 = 0.0
                                                                                                                                if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                                    scratchValue29 = 0
                                                                                                                                    scratchValue30 = 0
                                                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                                    while scratchValue13 do
                                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                                    end
                                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c6 end
                                                                                                                                end
                                                                                                                                scratchValue29 = 0
                                                                                                                                scratchValue30 = 0
                                                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                            end
                                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                                            goto LAB_00d55480_c6
                                                                                                                        end
                                                                                                                    else
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c6
                                                                                                                    end
                                                                                                                    ::LAB_00d55cba_c6::
                                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55480_c6::
                                                                                                            if me:IsTalkedToByHero() then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                                                me:ClearCommands()
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                                        scratchValue16 = 0.0
                                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                            scratchValue29 = 0
                                                                                                                            scratchValue30 = 0
                                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue13 do
                                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                        end
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while scratchValue19 < 0 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                            if scratchValue19 == 1 then
                                                                                                                                if not scratchValue then
                                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                    quest:Pause(1.0)
                                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                                    goto LAB_00d5595a_c6
                                                                                                                                end
                                                                                                                                __region_LAB_00d555f3_c6(); goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            if not scratchValue then
                                                                                                                                scratchValue16 = 0.0
                                                                                                                                if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                                    scratchValue29 = 0
                                                                                                                                    scratchValue30 = 0
                                                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                                    while scratchValue13 do
                                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                                    end
                                                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                                end
                                                                                                                                scratchValue29 = 0
                                                                                                                                scratchValue30 = 0
                                                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                                goto LAB_00d5595a_c6
                                                                                                                            end
                                                                                                                        end
                                                                                                                    end
                                                                                                                    __region_LAB_00d55cd5_c6()
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                scratchValue16 = 0.0
                                                                                                                if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                    while scratchValue13 do
                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                    end
                                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                end
                                                                                                                ::LAB_00d5595a_c6::
                                                                                                                quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                            end
                                                                                                            if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                                                quest:AddPersonToConversation(addNewConversation, hero)
                                                                                                                quest:SetTimer(timerId4, 10)
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                    if xStack_154 == 1 then
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                                        -- LAB_00d55b4e_c6: (native jump target)
                                                                                                                    elseif xStack_154 == 2 then
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                                                        -- TODO(native): goto LAB_00d55b4e_c6
                                                                                                                    end
                                                                                                                    -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                                else
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                                end
                                                                                                            end
                                                                                                            if scratchValue14 == 0 then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                if not me:IsPerformingScriptTask() then
                                                                                                                    scratchValue14 = 1
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                                end
                                                                                                            end
                                                                                                        until false
                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                            quest:SetStateBool("HeroSleeps", true)
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:SetTimeOfDay(11.0)
                                                                                                            quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                            quest:ResetPlayerCreatureCombatMultiplier()
                                                                                                        end
                                                                                                        goto FLOW_after_lab_00d54846
                                                                                                    end
                                                                                                end
                                                                                            else
                                                                                                -- LAB_00d54846: (native jump target)
                                                                                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                                scratchValue28 = 0
                                                                                                scratchValue29 = 0
                                                                                                scratchValue30 = 0
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                scratchValue14 = 0
                                                                                                repeat
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        if false then
                                                                                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            end
                                                                                                            scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                            while not scratchValue do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                            end
                                                                                                            resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            scratchValue16 = 0.0
                                                                                                            if quest:GetHealth(resources:ScriptThing(resource2)) <= scratchValue16 then
                                                                                                                -- LAB_00d551d4_c7: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue19 < 0 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                    if scratchValue19 == 1 then
                                                                                                                        if scratchValue then
                                                                                                                            __region_LAB_00d55c9f_c7()
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if scratchValue then goto LAB_00d55cba_c7 end
                                                                                                                        scratchValue16 = 0.0
                                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                            scratchValue29 = 0
                                                                                                                            scratchValue30 = 0
                                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue13 do
                                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c7 end
                                                                                                                        end
                                                                                                                        scratchValue28 = 0
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                scratchValue29 = 0
                                                                                                                scratchValue30 = 0
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue13 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c7
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c7::
                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        goto FLOW_after_lab_00d54f9c
                                                                                                    end
                                                                                                    if not state:GetBool("WoodsEndPlayed") then
                                                                                                        state:SetBool("WoodsEndPlayed", true)
                                                                                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                        resource = resources:NewResource()
                                                                                                        addNewConversation = 4
                                                                                                        SUB(resource,0)
                                                                                                        scratchValue28 = resource >> 8
                                                                                                        scratchValue29 = resource >> 16
                                                                                                        scratchValue30 = resource >> 24
                                                                                                        scratchValue = resources:TryAcquire(0, hero, 4)
                                                                                                        while not scratchValue do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            addNewConversation = 4
                                                                                                            SUB(resource,0)
                                                                                                            scratchValue28 = resource >> 8
                                                                                                            scratchValue29 = resource >> 16
                                                                                                            scratchValue30 = resource >> 24
                                                                                                            scratchValue = me:AcquireControl(4)
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then
                                                                                                            -- LAB_00d55c91: (native jump target)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        actorMap2 = resources:NewActorMap()
                                                                                                        resources:SetActor(actorMap2, "HERO", resource)
                                                                                                        resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while scratchValue19 < 0 do
                                                                                                            quest:NewScriptFrame(me)
                                                                                                            if quest:IsActiveThreadTerminating() then
                                                                                                                quest:PauseAllNonScriptedEntities(addNewConversation ~= 0)
                                                                                                                resources:DestroyActorMap(actorMap2)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then
                                                                                                            -- LAB_00d55c72: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0)
                                                                                                            -- LAB_00d55c7f: (native jump target)
                                                                                                            resources:DestroyActorMap(actorMap2)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                                                                        if scratchValue19 == 1 then
                                                                                                            if scratchValue then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(actorMap2)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if scratchValue then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(actorMap2)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            scratchValue16 = 0.0
                                                                                                            if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                scratchValue29 = 0
                                                                                                                scratchValue30 = 0
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue13 do
                                                                                                                    quest:NewScriptFrame(me)
                                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                                        resources:DestroyActorMap(actorMap2)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then
                                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue29 ~= 0)
                                                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            scratchValue28 = 0
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        resources:DestroyActorMap(actorMap2)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c: (native jump target)
                                                                                                            scratchValue = false
                                                                                                        else
                                                                                                            scratchValue = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                        end
                                                                                                        if scratchValue then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            end
                                                                                                            scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                            while not scratchValue do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                            end
                                                                                                            resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            scratchValue16 = 0.0
                                                                                                            if quest:GetHealth(resources:ScriptThing(resource2)) <= scratchValue16 then
                                                                                                                -- LAB_00d551d4: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue19 < 0 do
                                                                                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue16 ~= 0); goto LAB_00d55c2b end
                                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                    if scratchValue19 == 1 then
                                                                                                                        if scratchValue then
                                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if scratchValue then goto LAB_00d55cba end
                                                                                                                        scratchValue16 = 0.0
                                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                            scratchValue29 = 0
                                                                                                                            scratchValue30 = 0
                                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue13 do
                                                                                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0); goto LAB_00d55c2b end
                                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                                                                                        end
                                                                                                                        scratchValue28 = 0
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                scratchValue28 = 0
                                                                                                                scratchValue29 = 0
                                                                                                                scratchValue30 = 0
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue13 do
                                                                                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0); goto LAB_00d55c2b end
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    while scratchValue19 < 0 do
                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    end
                                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                        if scratchValue19 == 1 then
                                                                                                                            if scratchValue then
                                                                                                                                __region_LAB_00d55c9f_c19()
                                                                                                                                goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        else
                                                                                                                            if scratchValue then goto LAB_00d55cba end
                                                                                                                            scratchValue16 = 0.0
                                                                                                                            if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                                scratchValue29 = 0
                                                                                                                                scratchValue30 = 0
                                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                                while scratchValue13 do
                                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                                end
                                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                                                                                            end
                                                                                                                            scratchValue28 = 0
                                                                                                                            scratchValue29 = 0
                                                                                                                            scratchValue30 = 0
                                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55480
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cba::
                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                    end
                                                                                                    ::FLOW_after_lab_00d54f9c::
                                                                                                    ::LAB_00d55480::
                                                                                                    if me:IsTalkedToByHero() then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue16 = 0.0
                                                                                                                if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                    scratchValue28 = 0
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                    while scratchValue13 do
                                                                                                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0); goto LAB_00d55c2b end
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                    end
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue19 < 0 do
                                                                                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue29 ~= 0); goto LAB_00d55c2b end
                                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                    if scratchValue19 == 1 then
                                                                                                                        if not scratchValue then
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    if not scratchValue then
                                                                                                                        scratchValue16 = 0.0
                                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                            scratchValue29 = 0
                                                                                                                            scratchValue30 = 0
                                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue13 do
                                                                                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0); goto LAB_00d55c2b end
                                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                                                                                        end
                                                                                                                        scratchValue28 = 0
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                        goto LAB_00d5595a
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cd5::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0); goto LAB_00d55c2b end
                                                                                                        scratchValue16 = 0.0
                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                            scratchValue28 = 0
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0); goto LAB_00d55c2b end
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(scratchValue29 ~= 0); goto LAB_00d55c2b end
                                                                                                        end
                                                                                                        ::LAB_00d5595a::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue28 ~= 0)
                                                                                                    end
                                                                                                    if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                        addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                                        quest:AddPersonToConversation(addNewConversation, hero)
                                                                                                        quest:SetTimer(timerId4, 10)
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                            if 0 == 1 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                                -- LAB_00d55b4e: (native jump target)
                                                                                                            elseif 0 == 2 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                                            end
                                                                                                            -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                        else
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                        end
                                                                                                    end
                                                                                                    if scratchValue14 == 0 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                        if not me:IsPerformingScriptTask() then
                                                                                                            scratchValue14 = 1
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                        end
                                                                                                    end
                                                                                                until false
                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                    quest:SetStateBool("HeroSleeps", true)
                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                    quest:SetTimeOfDay(11.0)
                                                                                                    quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                    quest:ResetPlayerCreatureCombatMultiplier()
                                                                                                end
                                                                                            end
                                                                                            ::FLOW_after_lab_00d54846::
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                    ::FLOW_after_lab_00d54dfa::
                                                                end
                                                                ::LAB_00d55c2b::
                                                                quest:DeregisterTimer(timerId2)
                                                            end
                                                        end
                                                    elseif not quest:IsActiveThreadTerminating() then
                                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                            timerId2 = quest:RegisterTimer()
                                                            quest:SetTimer(timerId2, 10)
                                                            scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                            while scratchValue13 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                if quest:GetTimer(timerId2) < 1 then
                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                                                    quest:SetTimer(timerId2, 10)
                                                                end
                                                                scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:Pause(0.5)
                                                                scratchValue31 = resources:NewResource()
                                                                SUB(scratchValue31,0)
                                                                scratchValue29 = scratchValue31 >> 16
                                                                scratchValue30 = scratchValue31 >> 24
                                                                scratchValue = resources:TryAcquire(0, hero, 4)
                                                                while not scratchValue do
                                                                    quest:NewScriptFrame(me)
                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c27
                                                                    SUB(scratchValue31,0)
                                                                    scratchValue29 = scratchValue31 >> 16
                                                                    scratchValue30 = scratchValue31 >> 24
                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                end
                                                                if quest:IsActiveThreadTerminating() then
                                                                    -- LAB_00d54dfa_c27: (native jump target)
                                                                else
                                                                    scratchValue20 = resources:NewActorMap()
                                                                    resources:SetActor(scratchValue20, "HERO", scratchValue31)
                                                                    resources:SetActor(scratchValue20, "TEACHER", resource2)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue20, false, true)
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(scratchValue20)
                                                                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                    quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                    quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                    quest:NewScriptFrame(me)
                                                                    if not quest:IsActiveThreadTerminating() then
                                                                        quest:NewScriptFrame(me)
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            quest:NewScriptFrame(me)
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                quest:NewScriptFrame(me)
                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                    quest:NewScriptFrame(me)
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        if quest:DisplayTutorial(28) then
                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                while not scratchValue do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                    scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                end
                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c27
                                                                                            end
                                                                                        else
                                                                                            -- LAB_00d54846_c27: (native jump target)
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                            scratchValue29 = 0
                                                                                            scratchValue30 = 0
                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                            scratchValue14 = 0
                                                                                            repeat
                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                if not state:GetBool("WoodsEndPlayed") then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                    state:SetBool("WoodsEndPlayed", true)
                                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                    resource = resources:NewResource()
                                                                                                    addNewConversation = 4
                                                                                                    SUB(resource,0)
                                                                                                    scratchValue29 = resource >> 16
                                                                                                    scratchValue30 = resource >> 24
                                                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                                                    while not scratchValue do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c27
                                                                                                        addNewConversation = 4
                                                                                                        SUB(resource,0)
                                                                                                        scratchValue29 = resource >> 16
                                                                                                        scratchValue30 = resource >> 24
                                                                                                        scratchValue = me:AcquireControl(4)
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c91_c27: (native jump target)
                                                                                                        goto LAB_00d55c2b_c27
                                                                                                    end
                                                                                                    actorMap2 = resources:NewActorMap()
                                                                                                    resources:SetActor(actorMap2, "HERO", resource)
                                                                                                    resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    quest:FixMovieSequenceCamera(true)
                                                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue19 < 0 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c72_c27: (native jump target)
                                                                                                        quest:PauseAllNonScriptedEntities(addNewConversation ~= 0)
                                                                                                        -- LAB_00d55c7f_c27: (native jump target)
                                                                                                        resources:DestroyActorMap(actorMap2)
                                                                                                        -- TODO(native): goto LAB_00d55c91_c27
                                                                                                    end
                                                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                                                    if scratchValue19 == 1 then
                                                                                                        if scratchValue then
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- TODO(native): goto LAB_00d55c7f_c27
                                                                                                        end
                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                        quest:Pause(1.0)
                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    else
                                                                                                        if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                        scratchValue16 = 0.0
                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                quest:NewScriptFrame(me)
                                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                        end
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                    end
                                                                                                    quest:FixMovieSequenceCamera(false)
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                                else
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c27: (native jump target)
                                                                                                        scratchValue = false
                                                                                                    else
                                                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                        scratchValue = true
                                                                                                    end
                                                                                                    if scratchValue then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                        end
                                                                                                        scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                        while not scratchValue do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                            scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                        end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        scratchValue16 = 0.0
                                                                                                        if quest:GetHealth(resources:ScriptThing(resource2)) <= scratchValue16 then
                                                                                                            -- LAB_00d551d4_c27: (native jump target)
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue19 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if scratchValue then
                                                                                                                        __region_LAB_00d55c9f_c27()
                                                                                                                        goto LAB_00d55c2b_c27
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue then goto LAB_00d55cba_c27 end
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c27 end
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55480_c27
                                                                                                            end
                                                                                                        else
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c27
                                                                                                        end
                                                                                                        ::LAB_00d55cba_c27::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                        goto LAB_00d55c2b_c27
                                                                                                    end
                                                                                                end
                                                                                                ::LAB_00d55480_c27::
                                                                                                if me:IsTalkedToByHero() then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    me:ClearCommands()
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                            scratchValue16 = 0.0
                                                                                                            if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                scratchValue29 = 0
                                                                                                                scratchValue30 = 0
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue13 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                            end
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue19 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if not scratchValue then
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        goto LAB_00d5595a_c27
                                                                                                                    end
                                                                                                                    __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27
                                                                                                                end
                                                                                                                if not scratchValue then
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    goto LAB_00d5595a_c27
                                                                                                                end
                                                                                                            end
                                                                                                        end
                                                                                                        __region_LAB_00d55cd5_c27()
                                                                                                        goto LAB_00d55c2b_c27
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                    scratchValue16 = 0.0
                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                        while scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                    end
                                                                                                    ::LAB_00d5595a_c27::
                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                end
                                                                                                if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                                                    quest:SetTimer(timerId4, 10)
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                        if xStack_154 == 1 then
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                            -- LAB_00d55b4e_c27: (native jump target)
                                                                                                        elseif xStack_154 == 2 then
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                                            -- TODO(native): goto LAB_00d55b4e_c27
                                                                                                        end
                                                                                                        -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                    else
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                    end
                                                                                                end
                                                                                                if scratchValue14 == 0 then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                    if not me:IsPerformingScriptTask() then
                                                                                                        scratchValue14 = 1
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                    end
                                                                                                end
                                                                                            until false
                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                quest:SetStateBool("HeroSleeps", true)
                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                quest:SetTimeOfDay(11.0)
                                                                                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                quest:ResetPlayerCreatureCombatMultiplier()
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                            ::LAB_00d55c2b_c27::
                                                            quest:DeregisterTimer(timerId2)
                                                        end
                                                    end
                                                end
                                                ::FLOW_after_lab_00d53ff2::
                                                ::LAB_00d55c34::
                                            end
                                        end
                                    end
                                elseif not quest:IsActiveThreadTerminating() then
                                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                        quest:SetStateInt("PreMeleeMode", 2)
                                        quest:SetStateInt("DummyHits", 0)
                                        quest:SetTimer(timerId, 10)
                                        addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                        infoCounter = addQuestInfoCounter
                                        quest:DisplayQuestInfo(true)
                                        scratchValue19 = quest:GetStateInt("DummyHits")
                                        while scratchValue19 < 7 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                            quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                                            if 0 ~= quest:GetStateInt("DummyHits") then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                quest:SetTimer(timerId, 10)
                                            end
                                            if quest:GetTimer(timerId) < 1 then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                addNewConversation = quest:AddNewConversation(me, false, false)
                                                quest:AddPersonToConversation(addNewConversation, hero)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
                                                if quest:IsXbox() then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                else
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                quest:SetTimer(timerId3, 10)
                                                timerId = timerId3
                                                addQuestInfoCounter = infoCounter
                                            end
                                            scratchValue19 = quest:GetStateInt("DummyHits")
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:RemoveQuestInfoElement(addQuestInfoCounter)
                                            quest:DisplayQuestInfo(false)
                                            scratchValue32 = nil
                                            movie = resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            scratchValue31 = resources:NewResource()
                                            scratchValue21 = hero
                                            scratchValue = resources:TryAcquire(0, hero, 4)
                                            while not scratchValue do
                                                quest:NewScriptFrame(me)
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c28
                                                SUB(scratchValue31,0)
                                                scratchValue29 = scratchValue31 >> 16
                                                scratchValue30 = scratchValue31 >> 24
                                                scratchValue21 = hero
                                                scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                                            end
                                            if quest:IsActiveThreadTerminating() then
                                                -- LAB_00d53ff2_c28: (native jump target)
                                                resources:DestroyMovie(scratchValue31)
                                                resources:DestroyMovie(movie)
                                            else
                                                scratchValue24 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                quest:CreateEffect(scratchValue21, "SMASH_DUMMY_01", scratchValue20, "", 0.0, false, false)
                                                quest:FadeOutAndKillEntity(scratchValue24, true, 1.0, true)
                                                actorMap2 = resources:NewActorMap()
                                                resources:SetActor(actorMap2, "HERO", scratchValue31)
                                                resources:SetActor(actorMap2, "TEACHER", resource2)
                                                quest:FixMovieSequenceCamera(true)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                                                quest:PauseAllNonScriptedEntities(true)
                                                quest:CreateExperienceOrb(1, scratchValue20)
                                                -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                quest:EntitySetCutsceneBehaviour(nil, 2)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                                                quest:FixMovieSequenceCamera(false)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(actorMap2)
                                                resources:DestroyMovie(scratchValue31)
                                                resources:DestroyMovie(movie)
                                                if quest:IsXbox() then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c28 end
                                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            -- LAB_00d5439e_c28: (native jump target)
                                                            timerId2 = quest:RegisterTimer()
                                                            quest:SetTimer(timerId2, 10)
                                                            scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                            while scratchValue13 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                if quest:GetTimer(timerId2) < 1 then
                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                                                    quest:SetTimer(timerId2, 10)
                                                                end
                                                                scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:Pause(0.5)
                                                                scratchValue31 = resources:NewResource()
                                                                SUB(scratchValue31,0)
                                                                scratchValue29 = scratchValue31 >> 16
                                                                scratchValue30 = scratchValue31 >> 24
                                                                scratchValue = resources:TryAcquire(0, hero, 4)
                                                                while not scratchValue do
                                                                    quest:NewScriptFrame(me)
                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c28
                                                                    SUB(scratchValue31,0)
                                                                    scratchValue29 = scratchValue31 >> 16
                                                                    scratchValue30 = scratchValue31 >> 24
                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                end
                                                                if quest:IsActiveThreadTerminating() then
                                                                    -- LAB_00d54dfa_c28: (native jump target)
                                                                else
                                                                    scratchValue20 = resources:NewActorMap()
                                                                    resources:SetActor(scratchValue20, "HERO", scratchValue31)
                                                                    resources:SetActor(scratchValue20, "TEACHER", resource2)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue20, false, true)
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(scratchValue20)
                                                                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                    quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                    quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                    quest:NewScriptFrame(me)
                                                                    if not quest:IsActiveThreadTerminating() then
                                                                        quest:NewScriptFrame(me)
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            quest:NewScriptFrame(me)
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                quest:NewScriptFrame(me)
                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                    quest:NewScriptFrame(me)
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        if quest:DisplayTutorial(28) then
                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                while not scratchValue do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                    scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                end
                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c28
                                                                                            end
                                                                                        else
                                                                                            -- LAB_00d54846_c28: (native jump target)
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                            scratchValue29 = 0
                                                                                            scratchValue30 = 0
                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                            scratchValue14 = 0
                                                                                            repeat
                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                if not state:GetBool("WoodsEndPlayed") then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                    state:SetBool("WoodsEndPlayed", true)
                                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                    resource = resources:NewResource()
                                                                                                    addNewConversation = 4
                                                                                                    SUB(resource,0)
                                                                                                    scratchValue29 = resource >> 16
                                                                                                    scratchValue30 = resource >> 24
                                                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                                                    while not scratchValue do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c28
                                                                                                        addNewConversation = 4
                                                                                                        SUB(resource,0)
                                                                                                        scratchValue29 = resource >> 16
                                                                                                        scratchValue30 = resource >> 24
                                                                                                        scratchValue = me:AcquireControl(4)
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c91_c28: (native jump target)
                                                                                                        goto LAB_00d55c2b_c28
                                                                                                    end
                                                                                                    actorMap2 = resources:NewActorMap()
                                                                                                    resources:SetActor(actorMap2, "HERO", resource)
                                                                                                    resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    quest:FixMovieSequenceCamera(true)
                                                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue19 < 0 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c72_c28: (native jump target)
                                                                                                        quest:PauseAllNonScriptedEntities(addNewConversation ~= 0)
                                                                                                        -- LAB_00d55c7f_c28: (native jump target)
                                                                                                        resources:DestroyActorMap(actorMap2)
                                                                                                        -- TODO(native): goto LAB_00d55c91_c28
                                                                                                    end
                                                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                                                    if scratchValue19 == 1 then
                                                                                                        if scratchValue then
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- TODO(native): goto LAB_00d55c7f_c28
                                                                                                        end
                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                        quest:Pause(1.0)
                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    else
                                                                                                        if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                        scratchValue16 = 0.0
                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                quest:NewScriptFrame(me)
                                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                        end
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                    end
                                                                                                    quest:FixMovieSequenceCamera(false)
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                                else
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c28: (native jump target)
                                                                                                        scratchValue = false
                                                                                                    else
                                                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                        scratchValue = true
                                                                                                    end
                                                                                                    if scratchValue then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                        end
                                                                                                        scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                        while not scratchValue do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                            scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                        end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        scratchValue16 = 0.0
                                                                                                        if quest:GetHealth(resources:ScriptThing(resource2)) <= scratchValue16 then
                                                                                                            -- LAB_00d551d4_c28: (native jump target)
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue19 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if scratchValue then
                                                                                                                        __region_LAB_00d55c9f_c28()
                                                                                                                        goto LAB_00d55c2b_c28
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue then goto LAB_00d55cba_c28 end
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c28 end
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55480_c28
                                                                                                            end
                                                                                                        else
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c28
                                                                                                        end
                                                                                                        ::LAB_00d55cba_c28::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                        goto LAB_00d55c2b_c28
                                                                                                    end
                                                                                                end
                                                                                                ::LAB_00d55480_c28::
                                                                                                if me:IsTalkedToByHero() then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    me:ClearCommands()
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                            scratchValue16 = 0.0
                                                                                                            if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                scratchValue29 = 0
                                                                                                                scratchValue30 = 0
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue13 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                            end
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue19 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if not scratchValue then
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        goto LAB_00d5595a_c28
                                                                                                                    end
                                                                                                                    __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28
                                                                                                                end
                                                                                                                if not scratchValue then
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    goto LAB_00d5595a_c28
                                                                                                                end
                                                                                                            end
                                                                                                        end
                                                                                                        __region_LAB_00d55cd5_c28()
                                                                                                        goto LAB_00d55c2b_c28
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    scratchValue16 = 0.0
                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                        while scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    end
                                                                                                    ::LAB_00d5595a_c28::
                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                end
                                                                                                if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                                                    quest:SetTimer(timerId4, 10)
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                        if xStack_154 == 1 then
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                            -- LAB_00d55b4e_c28: (native jump target)
                                                                                                        elseif xStack_154 == 2 then
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                                            -- TODO(native): goto LAB_00d55b4e_c28
                                                                                                        end
                                                                                                        -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                    else
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                    end
                                                                                                end
                                                                                                if scratchValue14 == 0 then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                    if not me:IsPerformingScriptTask() then
                                                                                                        scratchValue14 = 1
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                    end
                                                                                                end
                                                                                            until false
                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                quest:SetStateBool("HeroSleeps", true)
                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                quest:SetTimeOfDay(11.0)
                                                                                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                quest:ResetPlayerCreatureCombatMultiplier()
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                            ::LAB_00d55c2b_c28::
                                                            quest:DeregisterTimer(timerId2)
                                                        end
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c28 end
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                        -- TODO(native): goto LAB_00d5439e_c28
                                                    end
                                                end
                                            end
                                            ::LAB_00d55c34_c28::
                                        end
                                    end
                                end
                            end
                            ::FLOW_after_lab_00d53a0b::
                        end
                        ::LAB_00d55c3d::
                        quest:DeregisterTimer(timerId3)
                    end
                end
            elseif not quest:IsActiveThreadTerminating() then
                quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC")
                scratchValue = quest:MsgIsGameInfoClickedPast()
                while not scratchValue do
                    if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
                    scratchValue = quest:MsgIsGameInfoClickedPast()
                end
                if not quest:IsActiveThreadTerminating() then
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                    timerId3 = quest:RegisterTimer()
                    timerId = timerId3
                    quest:SetTimer(timerId3, 10)
                    quest:SetStateInt("PreMeleeMode", 1)
                    quest:SetStateInt("DummyHits", 0)
                    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                    infoCounter = addQuestInfoCounter
                    quest:DisplayQuestInfo(true)
                    scratchValue19 = quest:GetStateInt("DummyHits")
                    while scratchValue19 < 7 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                        if 0 ~= quest:GetStateInt("DummyHits") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                            -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                            quest:SetTimer(timerId, 10)
                        end
                        if quest:GetTimer(timerId) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, hero, false)
                            if quest:IsXbox() then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                scratchValue = quest:MsgIsGameInfoClickedPast()
                                while not scratchValue do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                end
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                scratchValue = quest:MsgIsGameInfoClickedPast()
                                while not scratchValue do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                            quest:SetTimer(timerId3, 10)
                            timerId = timerId3
                            addQuestInfoCounter = infoCounter
                        end
                        scratchValue19 = quest:GetStateInt("DummyHits")
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(addQuestInfoCounter)
                        quest:DisplayQuestInfo(false)
                        scratchValue31 = resources:NewResource()
                        scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                        while not scratchValue do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53a0b_c29
                            scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- LAB_00d53a0b_c29: (native jump target)
                            resources:DestroyMovie(scratchValue31)
                        else
                            actorMap2 = resources:NewActorMap()
                            resources:SetActor(actorMap2, "HERO", scratchValue31)
                            resources:SetActor(actorMap2, "TEACHER", resource2)
                            movie = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_PREMELEE_STICK", actorMap2, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:DestroyActorMap(actorMap2)
                            resources:DestroyMovie(scratchValue31)
                            if quest:IsXbox() then
                                if not quest:IsActiveThreadTerminating() then
                                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        -- LAB_00d53c7e_c29: (native jump target)
                                        quest:SetStateInt("PreMeleeMode", 2)
                                        quest:SetStateInt("DummyHits", 0)
                                        quest:SetTimer(timerId, 10)
                                        addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                        infoCounter = addQuestInfoCounter
                                        quest:DisplayQuestInfo(true)
                                        scratchValue19 = quest:GetStateInt("DummyHits")
                                        while scratchValue19 < 7 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                            quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                                            if 0 ~= quest:GetStateInt("DummyHits") then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                quest:SetTimer(timerId, 10)
                                            end
                                            if quest:GetTimer(timerId) < 1 then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                addNewConversation = quest:AddNewConversation(me, false, false)
                                                quest:AddPersonToConversation(addNewConversation, hero)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
                                                if quest:IsXbox() then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                else
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                quest:SetTimer(timerId3, 10)
                                                timerId = timerId3
                                                addQuestInfoCounter = infoCounter
                                            end
                                            scratchValue19 = quest:GetStateInt("DummyHits")
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:RemoveQuestInfoElement(addQuestInfoCounter)
                                            quest:DisplayQuestInfo(false)
                                            scratchValue32 = nil
                                            movie = resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            scratchValue31 = resources:NewResource()
                                            scratchValue21 = hero
                                            scratchValue = resources:TryAcquire(0, hero, 4)
                                            while not scratchValue do
                                                quest:NewScriptFrame(me)
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c29
                                                SUB(scratchValue31,0)
                                                scratchValue29 = scratchValue31 >> 16
                                                scratchValue30 = scratchValue31 >> 24
                                                scratchValue21 = hero
                                                scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                                            end
                                            if quest:IsActiveThreadTerminating() then
                                                -- LAB_00d53ff2_c29: (native jump target)
                                                resources:DestroyMovie(scratchValue31)
                                                resources:DestroyMovie(movie)
                                            else
                                                scratchValue25 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                quest:CreateEffect(scratchValue21, "SMASH_DUMMY_01", scratchValue20, "", 0.0, false, false)
                                                quest:FadeOutAndKillEntity(scratchValue25, true, 1.0, true)
                                                actorMap2 = resources:NewActorMap()
                                                resources:SetActor(actorMap2, "HERO", scratchValue31)
                                                resources:SetActor(actorMap2, "TEACHER", resource2)
                                                quest:FixMovieSequenceCamera(true)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                                                quest:PauseAllNonScriptedEntities(true)
                                                quest:CreateExperienceOrb(1, scratchValue20)
                                                -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                quest:EntitySetCutsceneBehaviour(nil, 2)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                                                quest:FixMovieSequenceCamera(false)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(actorMap2)
                                                resources:DestroyMovie(scratchValue31)
                                                resources:DestroyMovie(movie)
                                                if quest:IsXbox() then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c29 end
                                                            scratchValue = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            -- LAB_00d5439e_c29: (native jump target)
                                                            timerId2 = quest:RegisterTimer()
                                                            quest:SetTimer(timerId2, 10)
                                                            scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                            while scratchValue13 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                if quest:GetTimer(timerId2) < 1 then
                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                                                    quest:SetTimer(timerId2, 10)
                                                                end
                                                                scratchValue13 = scratchValue32 ~= nil and scratchValue32:IsAlive()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:Pause(0.5)
                                                                scratchValue31 = resources:NewResource()
                                                                SUB(scratchValue31,0)
                                                                scratchValue29 = scratchValue31 >> 16
                                                                scratchValue30 = scratchValue31 >> 24
                                                                scratchValue = resources:TryAcquire(0, hero, 4)
                                                                while not scratchValue do
                                                                    quest:NewScriptFrame(me)
                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c29
                                                                    SUB(scratchValue31,0)
                                                                    scratchValue29 = scratchValue31 >> 16
                                                                    scratchValue30 = scratchValue31 >> 24
                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                end
                                                                if quest:IsActiveThreadTerminating() then
                                                                    -- LAB_00d54dfa_c29: (native jump target)
                                                                else
                                                                    scratchValue20 = resources:NewActorMap()
                                                                    resources:SetActor(scratchValue20, "HERO", scratchValue31)
                                                                    resources:SetActor(scratchValue20, "TEACHER", resource2)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue20, false, true)
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(scratchValue20)
                                                                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                    quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                    quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                    quest:NewScriptFrame(me)
                                                                    if not quest:IsActiveThreadTerminating() then
                                                                        quest:NewScriptFrame(me)
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            quest:NewScriptFrame(me)
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                quest:NewScriptFrame(me)
                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                    quest:NewScriptFrame(me)
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        if quest:DisplayTutorial(28) then
                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                while not scratchValue do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                    scratchValue = quest:MsgIsTutorialClickedPast()
                                                                                                end
                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c29
                                                                                            end
                                                                                        else
                                                                                            -- LAB_00d54846_c29: (native jump target)
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                            scratchValue29 = 0
                                                                                            scratchValue30 = 0
                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                            scratchValue14 = 0
                                                                                            repeat
                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                if not state:GetBool("WoodsEndPlayed") then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                    state:SetBool("WoodsEndPlayed", true)
                                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                    resource = resources:NewResource()
                                                                                                    addNewConversation = 4
                                                                                                    SUB(resource,0)
                                                                                                    scratchValue29 = resource >> 16
                                                                                                    scratchValue30 = resource >> 24
                                                                                                    scratchValue = resources:TryAcquire(0, hero, 4)
                                                                                                    while not scratchValue do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c29
                                                                                                        addNewConversation = 4
                                                                                                        SUB(resource,0)
                                                                                                        scratchValue29 = resource >> 16
                                                                                                        scratchValue30 = resource >> 24
                                                                                                        scratchValue = me:AcquireControl(4)
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c91_c29: (native jump target)
                                                                                                        goto LAB_00d55c2b_c29
                                                                                                    end
                                                                                                    actorMap2 = resources:NewActorMap()
                                                                                                    resources:SetActor(actorMap2, "HERO", resource)
                                                                                                    resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    quest:FixMovieSequenceCamera(true)
                                                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue19 < 0 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c72_c29: (native jump target)
                                                                                                        quest:PauseAllNonScriptedEntities(addNewConversation ~= 0)
                                                                                                        -- LAB_00d55c7f_c29: (native jump target)
                                                                                                        resources:DestroyActorMap(actorMap2)
                                                                                                        -- TODO(native): goto LAB_00d55c91_c29
                                                                                                    end
                                                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                                                    if scratchValue19 == 1 then
                                                                                                        if scratchValue then
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- TODO(native): goto LAB_00d55c7f_c29
                                                                                                        end
                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                        quest:Pause(1.0)
                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    else
                                                                                                        if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                        scratchValue16 = 0.0
                                                                                                        if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                quest:NewScriptFrame(me)
                                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                        end
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                    end
                                                                                                    quest:FixMovieSequenceCamera(false)
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                                else
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c29: (native jump target)
                                                                                                        scratchValue = false
                                                                                                    else
                                                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                        scratchValue = true
                                                                                                    end
                                                                                                    if scratchValue then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                        end
                                                                                                        scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                        while not scratchValue do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                            scratchValue = resources:TryAcquire(resource2, me, 4)
                                                                                                        end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        scratchValue16 = 0.0
                                                                                                        if quest:GetHealth(resources:ScriptThing(resource2)) <= scratchValue16 then
                                                                                                            -- LAB_00d551d4_c29: (native jump target)
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue19 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if scratchValue then
                                                                                                                        __region_LAB_00d55c9f_c29()
                                                                                                                        goto LAB_00d55c2b_c29
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue then goto LAB_00d55cba_c29 end
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c29 end
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55480_c29
                                                                                                            end
                                                                                                        else
                                                                                                            scratchValue29 = 0
                                                                                                            scratchValue30 = 0
                                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue13 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c29
                                                                                                        end
                                                                                                        ::LAB_00d55cba_c29::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                        goto LAB_00d55c2b_c29
                                                                                                    end
                                                                                                end
                                                                                                ::LAB_00d55480_c29::
                                                                                                if me:IsTalkedToByHero() then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    me:ClearCommands()
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                            scratchValue16 = 0.0
                                                                                                            if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                scratchValue29 = 0
                                                                                                                scratchValue30 = 0
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue13 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                            end
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue19 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue19 == 1 then
                                                                                                                    if not scratchValue then
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        goto LAB_00d5595a_c29
                                                                                                                    end
                                                                                                                    __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29
                                                                                                                end
                                                                                                                if not scratchValue then
                                                                                                                    scratchValue16 = 0.0
                                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                        scratchValue29 = 0
                                                                                                                        scratchValue30 = 0
                                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue13 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    end
                                                                                                                    scratchValue29 = 0
                                                                                                                    scratchValue30 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    goto LAB_00d5595a_c29
                                                                                                                end
                                                                                                            end
                                                                                                        end
                                                                                                        __region_LAB_00d55cd5_c29()
                                                                                                        goto LAB_00d55c2b_c29
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    scratchValue16 = 0.0
                                                                                                    if scratchValue16 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                        scratchValue29 = 0
                                                                                                        scratchValue30 = 0
                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue30,CONCAT12(scratchValue29,CONCAT11( 0,0))))
                                                                                                        scratchValue13 = me:IsPerformingScriptTask()
                                                                                                        while scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                            scratchValue13 = me:IsPerformingScriptTask()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    end
                                                                                                    ::LAB_00d5595a_c29::
                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                end
                                                                                                if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                                                    quest:SetTimer(timerId4, 10)
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                        if xStack_154 == 1 then
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                            -- LAB_00d55b4e_c29: (native jump target)
                                                                                                        elseif xStack_154 == 2 then
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                                            -- TODO(native): goto LAB_00d55b4e_c29
                                                                                                        end
                                                                                                        -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                    else
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                    end
                                                                                                end
                                                                                                if scratchValue14 == 0 then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                    if not me:IsPerformingScriptTask() then
                                                                                                        scratchValue14 = 1
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                    end
                                                                                                end
                                                                                            until false
                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                quest:SetStateBool("HeroSleeps", true)
                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                quest:SetTimeOfDay(11.0)
                                                                                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                quest:ResetPlayerCreatureCombatMultiplier()
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                            ::LAB_00d55c2b_c29::
                                                            quest:DeregisterTimer(timerId2)
                                                        end
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c29 end
                                                        scratchValue = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                        -- TODO(native): goto LAB_00d5439e_c29
                                                    end
                                                end
                                            end
                                            ::LAB_00d55c34_c29::
                                        end
                                    end
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                                scratchValue = quest:MsgIsGameInfoClickedPast()
                                while not scratchValue do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    scratchValue = quest:MsgIsGameInfoClickedPast()
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                    -- TODO(native): goto LAB_00d53c7e_c29
                                end
                            end
                        end
                    end
                    ::LAB_00d55c3d_c29::
                    quest:DeregisterTimer(timerId3)
                end
            end
        end
        ::FLOW_after_lab_00d533bb::
    end
    ::LAB_00d55c46::
    quest:DeregisterTimer(timerId4)
    ::LAB_00d55c4f::
    resources:DestroyMovie(resource2)
end

-- TheRealGuildmaster.Init (retail 0x00d51ed0)
function Init(quest, me)
    state:SetBool("ChatJumped", false)
    state:SetBool("WoodsEndPlayed", false)
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

