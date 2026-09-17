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
    local resources = quest:RetailResources()
    local scratchValue, scratchValue13, scratchValue14, scratchValue15, scratchValue16
    local scratchValue17, scratchValue20, scratchValue21, switch2, scratchValue22, scratchValue23
    local scratchValue25, scratchValue26, scratchValue27, timerId, scratchValue28, scratchValue30
    local scratchValue31, scratchValue32, scratchValue33, scratchValue34, scratchValue35
    local scratchValue36, scratchValue37, scratchValue38, scratchValue39, scratchValue40, timerId2
    local timerId3, timerId4
    local function __region_LAB_00d555f3_c27()
        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
    end
    local function __region_LAB_00d555f3_c28()
        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
    end
    local function __region_LAB_00d555f3_c29()
        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
    end
    local function __region_LAB_00d555f3_c6()
        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
    end
    local function __region_LAB_00d55c9f_c19()
        quest:PauseAllNonScriptedEntities(scratchValue31 ~= 0)
    end
    local function __region_LAB_00d55c9f_c27()
        quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0)
    end
    local function __region_LAB_00d55c9f_c28()
        quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0)
    end
    local function __region_LAB_00d55c9f_c29()
        quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0)
    end
    local function __region_LAB_00d55c9f_c6()
        quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0)
    end
    local function __region_LAB_00d55c9f_c7()
        quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0)
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
    scratchValue14 = quest:GetStateBool("GuildmasterTeleport")
    scratchValue28 = 0
    scratchValue33 = 0
    while not scratchValue14 do
        if not quest:NewScriptFrame(me) then return end
        scratchValue14 = quest:GetStateBool("GuildmasterTeleport")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01", "", "")
    scratchValue40 = resources:NewResource()
    scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
    while not scratchValue13 do
        if not quest:NewScriptFrame(me) then goto LAB_00d55c4f end
        scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d55c4f end
    scratchValue31 = 0
    scratchValue32 = 0
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    scratchValue22 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(me, scratchValue22, false)
    timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 0)
    scratchValue16 = '\x01'
    repeat
        if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
        if me:IsTalkedToByHero() then
            scratchValue16 = 0
        end
        if not quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) or 0 < quest:GetTimer(timerId4) then goto FLOW_native_label_1 end
        scratchValue21 = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(scratchValue21, quest:GetHero())
        quest:SetTimer(timerId4, 5)
        switch2 = scratchValue28
        repeat
            if switch2 == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, quest:GetHero(), false)
                me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, DAT_01375748, false, false)
                scratchValue28 = 1
                break
            else
                if switch2 == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, quest:GetHero(), false)
                    scratchValue28 = 2
                    goto FLOW_after_lab_00d53316
                elseif switch2 == 2 then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, quest:GetHero(), false)
                    scratchValue28 = 3
                    break
                elseif switch2 == 3 then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, quest:GetHero(), false)
                    scratchValue28 = 2
                end
                ::FLOW_after_lab_00d53316::
            end
        until true
        ::FLOW_native_label_1::
    until scratchValue16 == 0
    if not quest:IsActiveThreadTerminating() then
        quest:SetStateBool("WhisperStopFollowing", true)
        scratchValue38 = resources:NewResource()
        scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
        while not scratchValue13 do
            if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue38); goto FLOW_after_lab_00d533bb end
            scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
        end
        if quest:IsActiveThreadTerminating() then
            resources:DestroyMovie(scratchValue38)
        else
            scratchValue37 = resources:NewActorMap()
            resources:SetActor(scratchValue37, "HERO", scratchValue38)
            resources:SetActor(scratchValue37, "TEACHER", scratchValue40)
            scratchValue36 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_PREMELEE_PUNCH", scratchValue37, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeWhisper"), false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue36)
            resources:DestroyActorMap(scratchValue37)
            resources:DestroyMovie(scratchValue38)
            if quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH")
                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue13 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        timerId3 = quest:RegisterTimer()
                        timerId = timerId3
                        quest:SetTimer(timerId3, 10)
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        scratchValue = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                        quest:DisplayQuestInfo(true)
                        scratchValue20 = quest:GetStateInt("DummyHits")
                        while scratchValue20 < 7 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                            quest:UpdateQuestInfoCounter(scratchValue, quest:GetStateInt("DummyHits"), -1)
                            if 0 ~= quest:GetStateInt("DummyHits") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                quest:SetTimer(timerId, 10)
                            end
                            if quest:GetTimer(timerId) < 1 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                scratchValue21 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, quest:GetHero(), false)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue13 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    end
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue13 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                quest:SetTimer(timerId3, 10)
                                timerId = timerId3
                                scratchValue = scratchValue
                            end
                            scratchValue20 = quest:GetStateInt("DummyHits")
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:RemoveQuestInfoElement(scratchValue)
                            quest:DisplayQuestInfo(false)
                            scratchValue38 = resources:NewResource()
                            scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
                            while not scratchValue13 do
                                if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue38); goto FLOW_after_lab_00d53a0b end
                                scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d53a0b: (native jump target)
                                resources:DestroyMovie(scratchValue38)
                            else
                                scratchValue37 = resources:NewActorMap()
                                resources:SetActor(scratchValue37, "HERO", scratchValue38)
                                resources:SetActor(scratchValue37, "TEACHER", scratchValue40)
                                scratchValue36 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_PREMELEE_STICK", scratchValue37, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue36)
                                resources:DestroyActorMap(scratchValue37)
                                resources:DestroyMovie(scratchValue38)
                                if quest:IsXbox() then
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                        while not scratchValue13 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            -- LAB_00d53c7e: (native jump target)
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(timerId, 10)
                                            scratchValue = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                            quest:DisplayQuestInfo(true)
                                            scratchValue20 = quest:GetStateInt("DummyHits")
                                            while scratchValue20 < 7 do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                quest:UpdateQuestInfoCounter(scratchValue, quest:GetStateInt("DummyHits"), -1)
                                                if 0 ~= quest:GetStateInt("DummyHits") then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                    quest:SetTimer(timerId, 10)
                                                end
                                                if quest:GetTimer(timerId) < 1 then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    scratchValue21 = quest:AddNewConversation(me, false, false)
                                                    quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, quest:GetHero(), false)
                                                    if quest:IsXbox() then
                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue13 do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue13 do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    quest:SetTimer(timerId3, 10)
                                                    timerId = timerId3
                                                    scratchValue = scratchValue
                                                end
                                                scratchValue20 = quest:GetStateInt("DummyHits")
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:RemoveQuestInfoElement(scratchValue)
                                                quest:DisplayQuestInfo(false)
                                                scratchValue39 = nil
                                                scratchValue36 = resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                scratchValue38 = resources:NewResource()
                                                scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                while not scratchValue13 do
                                                    quest:NewScriptFrame(me)
                                                    if quest:IsActiveThreadTerminating() then
                                                        resources:DestroyMovie(scratchValue38)
                                                        resources:DestroyMovie(scratchValue36)
                                                        goto FLOW_after_lab_00d53ff2
                                                    end
                                                    SUB41(scratchValue38,0)
                                                    scratchValue31 = scratchValue38 >> 16
                                                    scratchValue32 = scratchValue38 >> 24
                                                    scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
                                                end
                                                if quest:IsActiveThreadTerminating() then
                                                    -- LAB_00d53ff2: (native jump target)
                                                    resources:DestroyMovie(scratchValue38)
                                                    resources:DestroyMovie(scratchValue36)
                                                else
                                                    scratchValue25 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(xStack_dc, "SMASH_DUMMY_01", scratchValue35, "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(scratchValue25, true, 1.0, true)
                                                    scratchValue37 = resources:NewActorMap()
                                                    resources:SetActor(scratchValue37, "HERO", scratchValue38)
                                                    resources:SetActor(scratchValue37, "TEACHER", scratchValue40)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", scratchValue37, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    quest:CreateExperienceOrb(scratchValue35, 1)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(nil, 2)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED", scratchValue37, false, true)
                                                    quest:FixMovieSequenceCamera(false)
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyActorMap(scratchValue37)
                                                    resources:DestroyMovie(scratchValue38)
                                                    resources:DestroyMovie(scratchValue36)
                                                    if quest:IsXbox() then
                                                        if not quest:IsActiveThreadTerminating() then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                            while not scratchValue13 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                                                                scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                -- LAB_00d5439e: (native jump target)
                                                                timerId2 = quest:RegisterTimer()
                                                                quest:SetTimer(timerId2, 10)
                                                                scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                                while scratchValue14 do
                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                    if quest:GetTimer(timerId2) < 1 then
                                                                        scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                        quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                        quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, quest:GetHero(), false)
                                                                        quest:SetTimer(timerId2, 10)
                                                                    end
                                                                    scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                                end
                                                                if not quest:IsActiveThreadTerminating() then
                                                                    quest:Pause(0.5)
                                                                    scratchValue38 = resources:NewResource()
                                                                    SUB41(scratchValue38,0)
                                                                    scratchValue31 = scratchValue38 >> 16
                                                                    scratchValue32 = scratchValue38 >> 24
                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                    while not scratchValue13 do
                                                                        if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                                                                        SUB41(scratchValue38,0)
                                                                        scratchValue31 = scratchValue38 >> 16
                                                                        scratchValue32 = scratchValue38 >> 24
                                                                        scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then
                                                                        -- LAB_00d54dfa: (native jump target)
                                                                    else
                                                                        scratchValue35 = resources:NewActorMap()
                                                                        resources:SetActor(scratchValue35, "HERO", scratchValue38)
                                                                        resources:SetActor(scratchValue35, "TEACHER", scratchValue40)
                                                                        resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue35, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(scratchValue35)
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
                                                                                                    scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not scratchValue13 do
                                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                        scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                        scratchValue16 = '\x01'
                                                                                                        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                        scratchValue15 = 0
                                                                                                        repeat
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                                                                                            if not state:GetBool("WoodsEndPlayed") then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                state:SetBool("WoodsEndPlayed", true)
                                                                                                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                                scratchValue34 = resources:NewResource()
                                                                                                                scratchValue21 = 4
                                                                                                                SUB41(scratchValue34,0)
                                                                                                                scratchValue31 = scratchValue34 >> 16
                                                                                                                scratchValue32 = scratchValue34 >> 24
                                                                                                                scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                                                                while not scratchValue13 do
                                                                                                                    quest:NewScriptFrame(me)
                                                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c6
                                                                                                                    scratchValue21 = 4
                                                                                                                    SUB41(scratchValue34,0)
                                                                                                                    scratchValue31 = scratchValue34 >> 16
                                                                                                                    scratchValue32 = scratchValue34 >> 24
                                                                                                                    scratchValue13 = me:AcquireControl(4)
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then
                                                                                                                    -- LAB_00d55c91_c6: (native jump target)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                scratchValue37 = resources:NewActorMap()
                                                                                                                resources:SetActor(scratchValue37, "HERO", scratchValue34)
                                                                                                                resources:SetActor(scratchValue37, "GUARD", scratchValue40)
                                                                                                                resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                                                quest:FixMovieSequenceCamera(true)
                                                                                                                resources:RunMacro("CS_GUILD_MELEE_WOODSWON", scratchValue37, false, true)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue20 < 0 do
                                                                                                                    quest:NewScriptFrame(me)
                                                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then
                                                                                                                    -- LAB_00d55c72_c6: (native jump target)
                                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue21 ~= 0)
                                                                                                                    -- LAB_00d55c7f_c6: (native jump target)
                                                                                                                    resources:DestroyActorMap(scratchValue37)
                                                                                                                    -- TODO(native): goto LAB_00d55c91_c6
                                                                                                                end
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if scratchValue13 then
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        -- TODO(native): goto LAB_00d55c7f_c6
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue13 then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_0 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            quest:NewScriptFrame(me)
                                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:FixMovieSequenceCamera(false)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(scratchValue37)
                                                                                                            else
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                    -- LAB_00d54f9c_c6: (native jump target)
                                                                                                                    scratchValue13 = false
                                                                                                                else
                                                                                                                    scratchValue33 = scratchValue33 | 1
                                                                                                                    if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                                                                                                    scratchValue13 = true
                                                                                                                end
                                                                                                                if scratchValue33 & 1 ~= 0 then
                                                                                                                    scratchValue33 = scratchValue33 & 0xfffffffe
                                                                                                                end
                                                                                                                if scratchValue13 then
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                    while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                    end
                                                                                                                    scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                                    while not scratchValue13 do
                                                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                        scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                                    end
                                                                                                                    resources:StartMovie("")
                                                                                                                    quest:StartMovieSequence()
                                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if fret_00 <= scratchValue17 then
                                                                                                                        -- LAB_00d551d4_c6: (native jump target)
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while scratchValue20 < 0 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                                            scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                            if scratchValue20 == 1 then
                                                                                                                                if scratchValue13 then
                                                                                                                                    __region_LAB_00d55c9f_c6()
                                                                                                                                    goto LAB_00d55c2b
                                                                                                                                end
                                                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                quest:Pause(1.0)
                                                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            else
                                                                                                                                if scratchValue13 then goto LAB_00d55cba_c6 end
                                                                                                                                quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                                scratchValue17 = 0.0
                                                                                                                                if scratchValue17 < fret_01 then
                                                                                                                                    scratchValue31 = 0
                                                                                                                                    scratchValue32 = 0
                                                                                                                                    me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                                    while scratchValue14 do
                                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                                    end
                                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c6 end
                                                                                                                                end
                                                                                                                                scratchValue31 = 0
                                                                                                                                scratchValue32 = 0
                                                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                            end
                                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                                            goto LAB_00d55480_c6
                                                                                                                        end
                                                                                                                    else
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c6
                                                                                                                    end
                                                                                                                    ::LAB_00d55cba_c6::
                                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
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
                                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                        scratchValue17 = 0.0
                                                                                                                        if scratchValue17 < fret_03 then
                                                                                                                            scratchValue31 = 0
                                                                                                                            scratchValue32 = 0
                                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue14 do
                                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                        end
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while scratchValue20 < 0 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                                                            scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                            if scratchValue20 == 1 then
                                                                                                                                if not scratchValue13 then
                                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                    quest:Pause(1.0)
                                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                                    goto LAB_00d5595a_c6
                                                                                                                                end
                                                                                                                                __region_LAB_00d555f3_c6(); goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            if not scratchValue13 then
                                                                                                                                quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                                scratchValue17 = 0.0
                                                                                                                                if scratchValue17 < fret_04 then
                                                                                                                                    scratchValue31 = 0
                                                                                                                                    scratchValue32 = 0
                                                                                                                                    me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                                    while scratchValue14 do
                                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                                    end
                                                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                                end
                                                                                                                                scratchValue31 = 0
                                                                                                                                scratchValue32 = 0
                                                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                                goto LAB_00d5595a_c6
                                                                                                                            end
                                                                                                                        end
                                                                                                                    end
                                                                                                                    __region_LAB_00d55cd5_c6()
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                scratchValue17 = 0.0
                                                                                                                if scratchValue17 < fret_02 then
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                    while scratchValue14 do
                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                    end
                                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                end
                                                                                                                ::LAB_00d5595a_c6::
                                                                                                                quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                            end
                                                                                                            if scratchValue16 ~= 0 then
                                                                                                                if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                    scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                                                                    quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                                                                    quest:SetTimer(timerId4, 10)
                                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                        if xStack_154 == 1 then
                                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                                            -- LAB_00d55b4e_c6: (native jump target)
                                                                                                                        elseif xStack_154 == 2 then
                                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, quest:GetHero(), false)
                                                                                                                            -- TODO(native): goto LAB_00d55b4e_c6
                                                                                                                        end
                                                                                                                        -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                                    else
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                        quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            if scratchValue15 == 0 then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                if not me:IsPerformingScriptTask() then
                                                                                                                    scratchValue15 = '\x01'
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                end
                                                                                                            end
                                                                                                        until scratchValue16 == 0
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
                                                                                                scratchValue30 = 0
                                                                                                scratchValue31 = 0
                                                                                                scratchValue32 = 0
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                scratchValue16 = '\x01'
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                scratchValue15 = 0
                                                                                                repeat
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        if scratchValue33 & 1 ~= 0 then
                                                                                                            scratchValue33 = scratchValue33 & 0xfffffffe
                                                                                                        end
                                                                                                        if false then
                                                                                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            end
                                                                                                            scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                            while not scratchValue13 do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                            end
                                                                                                            resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                            scratchValue17 = 0.0
                                                                                                            if fret_00 <= scratchValue17 then
                                                                                                                -- LAB_00d551d4_c7: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue20 < 0 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                    if scratchValue20 == 1 then
                                                                                                                        if scratchValue13 then
                                                                                                                            __region_LAB_00d55c9f_c7()
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if scratchValue13 then goto LAB_00d55cba_c7 end
                                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                        scratchValue17 = 0.0
                                                                                                                        if scratchValue17 < fret_01 then
                                                                                                                            scratchValue31 = 0
                                                                                                                            scratchValue32 = 0
                                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue14 do
                                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c7 end
                                                                                                                        end
                                                                                                                        scratchValue30 = 0
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                scratchValue31 = 0
                                                                                                                scratchValue32 = 0
                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue14 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c7
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c7::
                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        goto FLOW_after_lab_00d54f9c
                                                                                                    end
                                                                                                    if not state:GetBool("WoodsEndPlayed") then
                                                                                                        state:SetBool("WoodsEndPlayed", true)
                                                                                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                        scratchValue34 = resources:NewResource()
                                                                                                        scratchValue21 = 4
                                                                                                        SUB41(scratchValue34,0)
                                                                                                        scratchValue30 = scratchValue34 >> 8
                                                                                                        scratchValue31 = scratchValue34 >> 16
                                                                                                        scratchValue32 = scratchValue34 >> 24
                                                                                                        scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                                                        while not scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            scratchValue21 = 4
                                                                                                            SUB41(scratchValue34,0)
                                                                                                            scratchValue30 = scratchValue34 >> 8
                                                                                                            scratchValue31 = scratchValue34 >> 16
                                                                                                            scratchValue32 = scratchValue34 >> 24
                                                                                                            scratchValue13 = me:AcquireControl(4)
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then
                                                                                                            -- LAB_00d55c91: (native jump target)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        scratchValue37 = resources:NewActorMap()
                                                                                                        resources:SetActor(scratchValue37, "HERO", scratchValue34)
                                                                                                        resources:SetActor(scratchValue37, "GUARD", scratchValue40)
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", scratchValue37, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while scratchValue20 < 0 do
                                                                                                            quest:NewScriptFrame(me)
                                                                                                            if quest:IsActiveThreadTerminating() then
                                                                                                                quest:PauseAllNonScriptedEntities(scratchValue21 ~= 0)
                                                                                                                resources:DestroyActorMap(scratchValue37)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then
                                                                                                            -- LAB_00d55c72: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0)
                                                                                                            -- LAB_00d55c7f: (native jump target)
                                                                                                            resources:DestroyActorMap(scratchValue37)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                        if scratchValue20 == 1 then
                                                                                                            if scratchValue13 then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(scratchValue37)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if scratchValue13 then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(scratchValue37)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                            scratchValue17 = 0.0
                                                                                                            if scratchValue17 < fret_0 then
                                                                                                                scratchValue31 = 0
                                                                                                                scratchValue32 = 0
                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue14 do
                                                                                                                    quest:NewScriptFrame(me)
                                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                                        resources:DestroyActorMap(scratchValue37)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then
                                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue31 ~= 0)
                                                                                                                    resources:DestroyActorMap(scratchValue37)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            scratchValue30 = 0
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        resources:DestroyActorMap(scratchValue37)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c: (native jump target)
                                                                                                            scratchValue13 = false
                                                                                                        else
                                                                                                            scratchValue33 = scratchValue33 | 1
                                                                                                            scratchValue13 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                        end
                                                                                                        if scratchValue33 & 1 ~= 0 then
                                                                                                            scratchValue33 = scratchValue33 & 0xfffffffe
                                                                                                        end
                                                                                                        if scratchValue13 then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                            end
                                                                                                            scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                            while not scratchValue13 do
                                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                                                                                scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                            end
                                                                                                            resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                            scratchValue17 = 0.0
                                                                                                            if fret_00 <= scratchValue17 then
                                                                                                                -- LAB_00d551d4: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue20 < 0 do
                                                                                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue17 ~= 0); goto LAB_00d55c2b end
                                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                    if scratchValue20 == 1 then
                                                                                                                        if scratchValue13 then
                                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if scratchValue13 then goto LAB_00d55cba end
                                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                        scratchValue17 = 0.0
                                                                                                                        if scratchValue17 < fret_01 then
                                                                                                                            scratchValue31 = 0
                                                                                                                            scratchValue32 = 0
                                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue14 do
                                                                                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0); goto LAB_00d55c2b end
                                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                                                                                        end
                                                                                                                        scratchValue30 = 0
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                scratchValue30 = 0
                                                                                                                scratchValue31 = 0
                                                                                                                scratchValue32 = 0
                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue14 do
                                                                                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0); goto LAB_00d55c2b end
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    while scratchValue20 < 0 do
                                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    end
                                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                                        scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                        if scratchValue20 == 1 then
                                                                                                                            if scratchValue13 then
                                                                                                                                __region_LAB_00d55c9f_c19()
                                                                                                                                goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        else
                                                                                                                            if scratchValue13 then goto LAB_00d55cba end
                                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                            scratchValue17 = 0.0
                                                                                                                            if scratchValue17 < fret_01 then
                                                                                                                                scratchValue31 = 0
                                                                                                                                scratchValue32 = 0
                                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                                while scratchValue14 do
                                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                                end
                                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                                                                                            end
                                                                                                                            scratchValue30 = 0
                                                                                                                            scratchValue31 = 0
                                                                                                                            scratchValue32 = 0
                                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55480
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cba::
                                                                                                            quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
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
                                                                                                                quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                scratchValue17 = 0.0
                                                                                                                if scratchValue17 < fret_03 then
                                                                                                                    scratchValue30 = 0
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                    while scratchValue14 do
                                                                                                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0); goto LAB_00d55c2b end
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                    end
                                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while scratchValue20 < 0 do
                                                                                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue31 ~= 0); goto LAB_00d55c2b end
                                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                                    scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                    if scratchValue20 == 1 then
                                                                                                                        if not scratchValue13 then
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    if not scratchValue13 then
                                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                        scratchValue17 = 0.0
                                                                                                                        if scratchValue17 < fret_04 then
                                                                                                                            scratchValue31 = 0
                                                                                                                            scratchValue32 = 0
                                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            while scratchValue14 do
                                                                                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0); goto LAB_00d55c2b end
                                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                            end
                                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                                                                                        end
                                                                                                                        scratchValue30 = 0
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                        goto LAB_00d5595a
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cd5::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0); goto LAB_00d55c2b end
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if scratchValue17 < fret_02 then
                                                                                                            scratchValue30 = 0
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0); goto LAB_00d55c2b end
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(scratchValue31 ~= 0); goto LAB_00d55c2b end
                                                                                                        end
                                                                                                        ::LAB_00d5595a::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue30 ~= 0)
                                                                                                    end
                                                                                                    if scratchValue16 ~= 0 then
                                                                                                        if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                            scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                                                            quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                                                            quest:SetTimer(timerId4, 10)
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                if 0 == 1 then
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                                    -- LAB_00d55b4e: (native jump target)
                                                                                                                elseif 0 == 2 then
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, quest:GetHero(), false)
                                                                                                                end
                                                                                                                -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                            else
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if scratchValue15 == 0 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                                                        if not me:IsPerformingScriptTask() then
                                                                                                            scratchValue15 = '\x01'
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                        end
                                                                                                    end
                                                                                                until scratchValue16 == 0
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
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue13 do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
                                                            timerId2 = quest:RegisterTimer()
                                                            quest:SetTimer(timerId2, 10)
                                                            scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                            while scratchValue14 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                if quest:GetTimer(timerId2) < 1 then
                                                                    scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, quest:GetHero(), false)
                                                                    quest:SetTimer(timerId2, 10)
                                                                end
                                                                scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:Pause(0.5)
                                                                scratchValue38 = resources:NewResource()
                                                                SUB41(scratchValue38,0)
                                                                scratchValue31 = scratchValue38 >> 16
                                                                scratchValue32 = scratchValue38 >> 24
                                                                scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                while not scratchValue13 do
                                                                    quest:NewScriptFrame(me)
                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c27
                                                                    SUB41(scratchValue38,0)
                                                                    scratchValue31 = scratchValue38 >> 16
                                                                    scratchValue32 = scratchValue38 >> 24
                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                end
                                                                if quest:IsActiveThreadTerminating() then
                                                                    -- LAB_00d54dfa_c27: (native jump target)
                                                                else
                                                                    scratchValue22 = resources:NewActorMap()
                                                                    resources:SetActor(scratchValue22, "HERO", scratchValue38)
                                                                    resources:SetActor(scratchValue22, "TEACHER", scratchValue40)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue22, false, true)
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(scratchValue22)
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
                                                                                                scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                while not scratchValue13 do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                    scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                end
                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c27
                                                                                            end
                                                                                        else
                                                                                            -- LAB_00d54846_c27: (native jump target)
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                            scratchValue31 = 0
                                                                                            scratchValue32 = 0
                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                            scratchValue16 = '\x01'
                                                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                            scratchValue15 = 0
                                                                                            repeat
                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                if not state:GetBool("WoodsEndPlayed") then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                    state:SetBool("WoodsEndPlayed", true)
                                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                    scratchValue34 = resources:NewResource()
                                                                                                    scratchValue21 = 4
                                                                                                    SUB41(scratchValue34,0)
                                                                                                    scratchValue31 = scratchValue34 >> 16
                                                                                                    scratchValue32 = scratchValue34 >> 24
                                                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                                                    while not scratchValue13 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c27
                                                                                                        scratchValue21 = 4
                                                                                                        SUB41(scratchValue34,0)
                                                                                                        scratchValue31 = scratchValue34 >> 16
                                                                                                        scratchValue32 = scratchValue34 >> 24
                                                                                                        scratchValue13 = me:AcquireControl(4)
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c91_c27: (native jump target)
                                                                                                        goto LAB_00d55c2b_c27
                                                                                                    end
                                                                                                    scratchValue37 = resources:NewActorMap()
                                                                                                    resources:SetActor(scratchValue37, "HERO", scratchValue34)
                                                                                                    resources:SetActor(scratchValue37, "GUARD", scratchValue40)
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    quest:FixMovieSequenceCamera(true)
                                                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", scratchValue37, false, true)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue20 < 0 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c72_c27: (native jump target)
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue21 ~= 0)
                                                                                                        -- LAB_00d55c7f_c27: (native jump target)
                                                                                                        resources:DestroyActorMap(scratchValue37)
                                                                                                        -- TODO(native): goto LAB_00d55c91_c27
                                                                                                    end
                                                                                                    scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                    if scratchValue20 == 1 then
                                                                                                        if scratchValue13 then
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- TODO(native): goto LAB_00d55c7f_c27
                                                                                                        end
                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                        quest:Pause(1.0)
                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    else
                                                                                                        if scratchValue13 then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if scratchValue17 < fret_0 then
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                quest:NewScriptFrame(me)
                                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                        end
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                    end
                                                                                                    quest:FixMovieSequenceCamera(false)
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    resources:DestroyActorMap(scratchValue37)
                                                                                                else
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c27: (native jump target)
                                                                                                        scratchValue13 = false
                                                                                                    else
                                                                                                        scratchValue33 = scratchValue33 | 1
                                                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                        scratchValue13 = true
                                                                                                    end
                                                                                                    if scratchValue33 & 1 ~= 0 then
                                                                                                        scratchValue33 = scratchValue33 & 0xfffffffe
                                                                                                    end
                                                                                                    if scratchValue13 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                        end
                                                                                                        scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                        while not scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                                                            scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                        end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if fret_00 <= scratchValue17 then
                                                                                                            -- LAB_00d551d4_c27: (native jump target)
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue20 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if scratchValue13 then
                                                                                                                        __region_LAB_00d55c9f_c27()
                                                                                                                        goto LAB_00d55c2b_c27
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue13 then goto LAB_00d55cba_c27 end
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_01 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c27 end
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55480_c27
                                                                                                            end
                                                                                                        else
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c27
                                                                                                        end
                                                                                                        ::LAB_00d55cba_c27::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
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
                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                            scratchValue17 = 0.0
                                                                                                            if scratchValue17 < fret_03 then
                                                                                                                scratchValue31 = 0
                                                                                                                scratchValue32 = 0
                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue14 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                            end
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue20 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if not scratchValue13 then
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        goto LAB_00d5595a_c27
                                                                                                                    end
                                                                                                                    __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27
                                                                                                                end
                                                                                                                if not scratchValue13 then
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_04 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    goto LAB_00d5595a_c27
                                                                                                                end
                                                                                                            end
                                                                                                        end
                                                                                                        __region_LAB_00d55cd5_c27()
                                                                                                        goto LAB_00d55c2b_c27
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                    scratchValue17 = 0.0
                                                                                                    if scratchValue17 < fret_02 then
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                        while scratchValue14 do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                    end
                                                                                                    ::LAB_00d5595a_c27::
                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                end
                                                                                                if scratchValue16 ~= 0 then
                                                                                                    if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                        scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                                                        quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                                                        quest:SetTimer(timerId4, 10)
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                            if xStack_154 == 1 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                                -- LAB_00d55b4e_c27: (native jump target)
                                                                                                            elseif xStack_154 == 2 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, quest:GetHero(), false)
                                                                                                                -- TODO(native): goto LAB_00d55b4e_c27
                                                                                                            end
                                                                                                            -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                        else
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                        end
                                                                                                    end
                                                                                                end
                                                                                                if scratchValue15 == 0 then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                                                    if not me:IsPerformingScriptTask() then
                                                                                                        scratchValue15 = '\x01'
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                    end
                                                                                                end
                                                                                            until scratchValue16 == 0
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
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue13 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                        quest:SetStateInt("PreMeleeMode", 2)
                                        quest:SetStateInt("DummyHits", 0)
                                        quest:SetTimer(timerId, 10)
                                        scratchValue = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                        quest:DisplayQuestInfo(true)
                                        scratchValue20 = quest:GetStateInt("DummyHits")
                                        while scratchValue20 < 7 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                            quest:UpdateQuestInfoCounter(scratchValue, quest:GetStateInt("DummyHits"), -1)
                                            if 0 ~= quest:GetStateInt("DummyHits") then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                quest:SetTimer(timerId, 10)
                                            end
                                            if quest:GetTimer(timerId) < 1 then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                scratchValue21 = quest:AddNewConversation(me, false, false)
                                                quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, quest:GetHero(), false)
                                                if quest:IsXbox() then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue13 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                else
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue13 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                                                quest:SetTimer(timerId3, 10)
                                                timerId = timerId3
                                                scratchValue = scratchValue
                                            end
                                            scratchValue20 = quest:GetStateInt("DummyHits")
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:RemoveQuestInfoElement(scratchValue)
                                            quest:DisplayQuestInfo(false)
                                            scratchValue39 = nil
                                            scratchValue36 = resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            scratchValue38 = resources:NewResource()
                                            scratchValue23 = quest:GetHero()
                                            scratchValue13 = resources:TryAcquire(0, scratchValue23, 4)
                                            while not scratchValue13 do
                                                quest:NewScriptFrame(me)
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c28
                                                SUB41(scratchValue38,0)
                                                scratchValue31 = scratchValue38 >> 16
                                                scratchValue32 = scratchValue38 >> 24
                                                scratchValue23 = quest:GetHero()
                                                scratchValue13 = resources:TryAcquire(scratchValue38, scratchValue23, 4)
                                            end
                                            if quest:IsActiveThreadTerminating() then
                                                -- LAB_00d53ff2_c28: (native jump target)
                                                resources:DestroyMovie(scratchValue38)
                                                resources:DestroyMovie(scratchValue36)
                                            else
                                                scratchValue26 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                quest:CreateEffect(scratchValue23, "SMASH_DUMMY_01", scratchValue22, "", 0.0, false, false)
                                                quest:FadeOutAndKillEntity(scratchValue26, true, 1.0, true)
                                                scratchValue37 = resources:NewActorMap()
                                                resources:SetActor(scratchValue37, "HERO", scratchValue38)
                                                resources:SetActor(scratchValue37, "TEACHER", scratchValue40)
                                                quest:FixMovieSequenceCamera(true)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", scratchValue37, false, false)
                                                quest:PauseAllNonScriptedEntities(true)
                                                quest:CreateExperienceOrb(1, scratchValue22)
                                                -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                quest:EntitySetCutsceneBehaviour(nil, 2)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED", scratchValue37, false, true)
                                                quest:FixMovieSequenceCamera(false)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(scratchValue37)
                                                resources:DestroyMovie(scratchValue38)
                                                resources:DestroyMovie(scratchValue36)
                                                if quest:IsXbox() then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue13 do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c28 end
                                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            -- LAB_00d5439e_c28: (native jump target)
                                                            timerId2 = quest:RegisterTimer()
                                                            quest:SetTimer(timerId2, 10)
                                                            scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                            while scratchValue14 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                if quest:GetTimer(timerId2) < 1 then
                                                                    scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, quest:GetHero(), false)
                                                                    quest:SetTimer(timerId2, 10)
                                                                end
                                                                scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:Pause(0.5)
                                                                scratchValue38 = resources:NewResource()
                                                                SUB41(scratchValue38,0)
                                                                scratchValue31 = scratchValue38 >> 16
                                                                scratchValue32 = scratchValue38 >> 24
                                                                scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                while not scratchValue13 do
                                                                    quest:NewScriptFrame(me)
                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c28
                                                                    SUB41(scratchValue38,0)
                                                                    scratchValue31 = scratchValue38 >> 16
                                                                    scratchValue32 = scratchValue38 >> 24
                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                end
                                                                if quest:IsActiveThreadTerminating() then
                                                                    -- LAB_00d54dfa_c28: (native jump target)
                                                                else
                                                                    scratchValue22 = resources:NewActorMap()
                                                                    resources:SetActor(scratchValue22, "HERO", scratchValue38)
                                                                    resources:SetActor(scratchValue22, "TEACHER", scratchValue40)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue22, false, true)
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(scratchValue22)
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
                                                                                                scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                while not scratchValue13 do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                    scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                end
                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c28
                                                                                            end
                                                                                        else
                                                                                            -- LAB_00d54846_c28: (native jump target)
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                            scratchValue31 = 0
                                                                                            scratchValue32 = 0
                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                            scratchValue16 = '\x01'
                                                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                            scratchValue15 = 0
                                                                                            repeat
                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                if not state:GetBool("WoodsEndPlayed") then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                    state:SetBool("WoodsEndPlayed", true)
                                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                    scratchValue34 = resources:NewResource()
                                                                                                    scratchValue21 = 4
                                                                                                    SUB41(scratchValue34,0)
                                                                                                    scratchValue31 = scratchValue34 >> 16
                                                                                                    scratchValue32 = scratchValue34 >> 24
                                                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                                                    while not scratchValue13 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c28
                                                                                                        scratchValue21 = 4
                                                                                                        SUB41(scratchValue34,0)
                                                                                                        scratchValue31 = scratchValue34 >> 16
                                                                                                        scratchValue32 = scratchValue34 >> 24
                                                                                                        scratchValue13 = me:AcquireControl(4)
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c91_c28: (native jump target)
                                                                                                        goto LAB_00d55c2b_c28
                                                                                                    end
                                                                                                    scratchValue37 = resources:NewActorMap()
                                                                                                    resources:SetActor(scratchValue37, "HERO", scratchValue34)
                                                                                                    resources:SetActor(scratchValue37, "GUARD", scratchValue40)
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    quest:FixMovieSequenceCamera(true)
                                                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", scratchValue37, false, true)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue20 < 0 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c72_c28: (native jump target)
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue21 ~= 0)
                                                                                                        -- LAB_00d55c7f_c28: (native jump target)
                                                                                                        resources:DestroyActorMap(scratchValue37)
                                                                                                        -- TODO(native): goto LAB_00d55c91_c28
                                                                                                    end
                                                                                                    scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                    if scratchValue20 == 1 then
                                                                                                        if scratchValue13 then
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- TODO(native): goto LAB_00d55c7f_c28
                                                                                                        end
                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                        quest:Pause(1.0)
                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    else
                                                                                                        if scratchValue13 then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if scratchValue17 < fret_0 then
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                quest:NewScriptFrame(me)
                                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                        end
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                    end
                                                                                                    quest:FixMovieSequenceCamera(false)
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    resources:DestroyActorMap(scratchValue37)
                                                                                                else
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c28: (native jump target)
                                                                                                        scratchValue13 = false
                                                                                                    else
                                                                                                        scratchValue33 = scratchValue33 | 1
                                                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                        scratchValue13 = true
                                                                                                    end
                                                                                                    if scratchValue33 & 1 ~= 0 then
                                                                                                        scratchValue33 = scratchValue33 & 0xfffffffe
                                                                                                    end
                                                                                                    if scratchValue13 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                        end
                                                                                                        scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                        while not scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                                            scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                        end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if fret_00 <= scratchValue17 then
                                                                                                            -- LAB_00d551d4_c28: (native jump target)
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue20 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if scratchValue13 then
                                                                                                                        __region_LAB_00d55c9f_c28()
                                                                                                                        goto LAB_00d55c2b_c28
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue13 then goto LAB_00d55cba_c28 end
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_01 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c28 end
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55480_c28
                                                                                                            end
                                                                                                        else
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c28
                                                                                                        end
                                                                                                        ::LAB_00d55cba_c28::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
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
                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                            scratchValue17 = 0.0
                                                                                                            if scratchValue17 < fret_03 then
                                                                                                                scratchValue31 = 0
                                                                                                                scratchValue32 = 0
                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue14 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                            end
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue20 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if not scratchValue13 then
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        goto LAB_00d5595a_c28
                                                                                                                    end
                                                                                                                    __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28
                                                                                                                end
                                                                                                                if not scratchValue13 then
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_04 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    goto LAB_00d5595a_c28
                                                                                                                end
                                                                                                            end
                                                                                                        end
                                                                                                        __region_LAB_00d55cd5_c28()
                                                                                                        goto LAB_00d55c2b_c28
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                    scratchValue17 = 0.0
                                                                                                    if scratchValue17 < fret_02 then
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                        while scratchValue14 do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    end
                                                                                                    ::LAB_00d5595a_c28::
                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                end
                                                                                                if scratchValue16 ~= 0 then
                                                                                                    if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                        scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                                                        quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                                                        quest:SetTimer(timerId4, 10)
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                            if xStack_154 == 1 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                                -- LAB_00d55b4e_c28: (native jump target)
                                                                                                            elseif xStack_154 == 2 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, quest:GetHero(), false)
                                                                                                                -- TODO(native): goto LAB_00d55b4e_c28
                                                                                                            end
                                                                                                            -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                        else
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                        end
                                                                                                    end
                                                                                                end
                                                                                                if scratchValue15 == 0 then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                                    if not me:IsPerformingScriptTask() then
                                                                                                        scratchValue15 = '\x01'
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                    end
                                                                                                end
                                                                                            until scratchValue16 == 0
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
                                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue13 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c28 end
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
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
                scratchValue13 = quest:MsgIsGameInfoClickedPast()
                while not scratchValue13 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                end
                if not quest:IsActiveThreadTerminating() then
                    -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                    timerId3 = quest:RegisterTimer()
                    timerId = timerId3
                    quest:SetTimer(timerId3, 10)
                    quest:SetStateInt("PreMeleeMode", 1)
                    quest:SetStateInt("DummyHits", 0)
                    scratchValue = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                    quest:DisplayQuestInfo(true)
                    scratchValue20 = quest:GetStateInt("DummyHits")
                    while scratchValue20 < 7 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                        quest:UpdateQuestInfoCounter(scratchValue, quest:GetStateInt("DummyHits"), -1)
                        if 0 ~= quest:GetStateInt("DummyHits") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                            -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                            quest:SetTimer(timerId, 10)
                        end
                        if quest:GetTimer(timerId) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                            scratchValue21 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, quest:GetHero(), false)
                            if quest:IsXbox() then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                while not scratchValue13 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                end
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                while not scratchValue13 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                            quest:SetTimer(timerId3, 10)
                            timerId = timerId3
                            scratchValue = scratchValue
                        end
                        scratchValue20 = quest:GetStateInt("DummyHits")
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(scratchValue)
                        quest:DisplayQuestInfo(false)
                        scratchValue38 = resources:NewResource()
                        scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
                        while not scratchValue13 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53a0b_c29
                            scratchValue13 = resources:TryAcquire(scratchValue38, quest:GetHero(), 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- LAB_00d53a0b_c29: (native jump target)
                            resources:DestroyMovie(scratchValue38)
                        else
                            scratchValue37 = resources:NewActorMap()
                            resources:SetActor(scratchValue37, "HERO", scratchValue38)
                            resources:SetActor(scratchValue37, "TEACHER", scratchValue40)
                            scratchValue36 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_PREMELEE_STICK", scratchValue37, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue36)
                            resources:DestroyActorMap(scratchValue37)
                            resources:DestroyMovie(scratchValue38)
                            if quest:IsXbox() then
                                if not quest:IsActiveThreadTerminating() then
                                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    while not scratchValue13 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        -- LAB_00d53c7e_c29: (native jump target)
                                        quest:SetStateInt("PreMeleeMode", 2)
                                        quest:SetStateInt("DummyHits", 0)
                                        quest:SetTimer(timerId, 10)
                                        scratchValue = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                        quest:DisplayQuestInfo(true)
                                        scratchValue20 = quest:GetStateInt("DummyHits")
                                        while scratchValue20 < 7 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                            quest:UpdateQuestInfoCounter(scratchValue, quest:GetStateInt("DummyHits"), -1)
                                            if 0 ~= quest:GetStateInt("DummyHits") then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                quest:SetTimer(timerId, 10)
                                            end
                                            if quest:GetTimer(timerId) < 1 then
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                scratchValue21 = quest:AddNewConversation(me, false, false)
                                                quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, quest:GetHero(), false)
                                                if quest:IsXbox() then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue13 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                else
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue13 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                                quest:SetTimer(timerId3, 10)
                                                timerId = timerId3
                                                scratchValue = scratchValue
                                            end
                                            scratchValue20 = quest:GetStateInt("DummyHits")
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:RemoveQuestInfoElement(scratchValue)
                                            quest:DisplayQuestInfo(false)
                                            scratchValue39 = nil
                                            scratchValue36 = resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            scratchValue38 = resources:NewResource()
                                            scratchValue23 = quest:GetHero()
                                            scratchValue13 = resources:TryAcquire(0, scratchValue23, 4)
                                            while not scratchValue13 do
                                                quest:NewScriptFrame(me)
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c29
                                                SUB41(scratchValue38,0)
                                                scratchValue31 = scratchValue38 >> 16
                                                scratchValue32 = scratchValue38 >> 24
                                                scratchValue23 = quest:GetHero()
                                                scratchValue13 = resources:TryAcquire(scratchValue38, scratchValue23, 4)
                                            end
                                            if quest:IsActiveThreadTerminating() then
                                                -- LAB_00d53ff2_c29: (native jump target)
                                                resources:DestroyMovie(scratchValue38)
                                                resources:DestroyMovie(scratchValue36)
                                            else
                                                scratchValue27 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                quest:CreateEffect(scratchValue23, "SMASH_DUMMY_01", scratchValue22, "", 0.0, false, false)
                                                quest:FadeOutAndKillEntity(scratchValue27, true, 1.0, true)
                                                scratchValue37 = resources:NewActorMap()
                                                resources:SetActor(scratchValue37, "HERO", scratchValue38)
                                                resources:SetActor(scratchValue37, "TEACHER", scratchValue40)
                                                quest:FixMovieSequenceCamera(true)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", scratchValue37, false, false)
                                                quest:PauseAllNonScriptedEntities(true)
                                                quest:CreateExperienceOrb(1, scratchValue22)
                                                -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                quest:EntitySetCutsceneBehaviour(nil, 2)
                                                resources:RunMacro("CS_GUILD_PREMELEE_PASSED", scratchValue37, false, true)
                                                quest:FixMovieSequenceCamera(false)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(scratchValue37)
                                                resources:DestroyMovie(scratchValue38)
                                                resources:DestroyMovie(scratchValue36)
                                                if quest:IsXbox() then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        while not scratchValue13 do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c29 end
                                                            scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            -- LAB_00d5439e_c29: (native jump target)
                                                            timerId2 = quest:RegisterTimer()
                                                            quest:SetTimer(timerId2, 10)
                                                            scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                            while scratchValue14 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                if quest:GetTimer(timerId2) < 1 then
                                                                    scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, quest:GetHero(), false)
                                                                    quest:SetTimer(timerId2, 10)
                                                                end
                                                                scratchValue14 = scratchValue39 ~= nil and scratchValue39:IsAlive()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:Pause(0.5)
                                                                scratchValue38 = resources:NewResource()
                                                                SUB41(scratchValue38,0)
                                                                scratchValue31 = scratchValue38 >> 16
                                                                scratchValue32 = scratchValue38 >> 24
                                                                scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                while not scratchValue13 do
                                                                    quest:NewScriptFrame(me)
                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c29
                                                                    SUB41(scratchValue38,0)
                                                                    scratchValue31 = scratchValue38 >> 16
                                                                    scratchValue32 = scratchValue38 >> 24
                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                end
                                                                if quest:IsActiveThreadTerminating() then
                                                                    -- LAB_00d54dfa_c29: (native jump target)
                                                                else
                                                                    scratchValue22 = resources:NewActorMap()
                                                                    resources:SetActor(scratchValue22, "HERO", scratchValue38)
                                                                    resources:SetActor(scratchValue22, "TEACHER", scratchValue40)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue22, false, true)
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(scratchValue22)
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
                                                                                                scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                while not scratchValue13 do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                    scratchValue13 = quest:MsgIsTutorialClickedPast()
                                                                                                end
                                                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c29
                                                                                            end
                                                                                        else
                                                                                            -- LAB_00d54846_c29: (native jump target)
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                            scratchValue31 = 0
                                                                                            scratchValue32 = 0
                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                            scratchValue16 = '\x01'
                                                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                            scratchValue15 = 0
                                                                                            repeat
                                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                if not state:GetBool("WoodsEndPlayed") then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                    state:SetBool("WoodsEndPlayed", true)
                                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                                    scratchValue34 = resources:NewResource()
                                                                                                    scratchValue21 = 4
                                                                                                    SUB41(scratchValue34,0)
                                                                                                    scratchValue31 = scratchValue34 >> 16
                                                                                                    scratchValue32 = scratchValue34 >> 24
                                                                                                    scratchValue13 = resources:TryAcquire(0, quest:GetHero(), 4)
                                                                                                    while not scratchValue13 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c29
                                                                                                        scratchValue21 = 4
                                                                                                        SUB41(scratchValue34,0)
                                                                                                        scratchValue31 = scratchValue34 >> 16
                                                                                                        scratchValue32 = scratchValue34 >> 24
                                                                                                        scratchValue13 = me:AcquireControl(4)
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c91_c29: (native jump target)
                                                                                                        goto LAB_00d55c2b_c29
                                                                                                    end
                                                                                                    scratchValue37 = resources:NewActorMap()
                                                                                                    resources:SetActor(scratchValue37, "HERO", scratchValue34)
                                                                                                    resources:SetActor(scratchValue37, "GUARD", scratchValue40)
                                                                                                    resources:StartMovie("")
                                                                                                    quest:StartMovieSequence()
                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                    quest:FixMovieSequenceCamera(true)
                                                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", scratchValue37, false, true)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue20 < 0 do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then
                                                                                                        -- LAB_00d55c72_c29: (native jump target)
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue21 ~= 0)
                                                                                                        -- LAB_00d55c7f_c29: (native jump target)
                                                                                                        resources:DestroyActorMap(scratchValue37)
                                                                                                        -- TODO(native): goto LAB_00d55c91_c29
                                                                                                    end
                                                                                                    scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                    if scratchValue20 == 1 then
                                                                                                        if scratchValue13 then
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- TODO(native): goto LAB_00d55c7f_c29
                                                                                                        end
                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                        quest:Pause(1.0)
                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    else
                                                                                                        if scratchValue13 then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if scratchValue17 < fret_0 then
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                quest:NewScriptFrame(me)
                                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                        end
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                    end
                                                                                                    quest:FixMovieSequenceCamera(false)
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    resources:DestroyActorMap(scratchValue37)
                                                                                                else
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c29: (native jump target)
                                                                                                        scratchValue13 = false
                                                                                                    else
                                                                                                        scratchValue33 = scratchValue33 | 1
                                                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                        scratchValue13 = true
                                                                                                    end
                                                                                                    if scratchValue33 & 1 ~= 0 then
                                                                                                        scratchValue33 = scratchValue33 & 0xfffffffe
                                                                                                    end
                                                                                                    if scratchValue13 then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                        end
                                                                                                        scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                        while not scratchValue13 do
                                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                            scratchValue13 = resources:TryAcquire(scratchValue40, me, 4)
                                                                                                        end
                                                                                                        resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                        scratchValue17 = 0.0
                                                                                                        if fret_00 <= scratchValue17 then
                                                                                                            -- LAB_00d551d4_c29: (native jump target)
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue20 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if scratchValue13 then
                                                                                                                        __region_LAB_00d55c9f_c29()
                                                                                                                        goto LAB_00d55c2b_c29
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if scratchValue13 then goto LAB_00d55cba_c29 end
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_01 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c29 end
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                end
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55480_c29
                                                                                                            end
                                                                                                        else
                                                                                                            scratchValue31 = 0
                                                                                                            scratchValue32 = 0
                                                                                                            me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            while scratchValue14 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c29
                                                                                                        end
                                                                                                        ::LAB_00d55cba_c29::
                                                                                                        quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
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
                                                                                                            quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                            scratchValue17 = 0.0
                                                                                                            if scratchValue17 < fret_03 then
                                                                                                                scratchValue31 = 0
                                                                                                                scratchValue32 = 0
                                                                                                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                while scratchValue14 do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                            end
                                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                            scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            while scratchValue20 < 0 do
                                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                            end
                                                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                                                scratchValue13 = quest:IsActiveThreadTerminating()
                                                                                                                if scratchValue20 == 1 then
                                                                                                                    if not scratchValue13 then
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        goto LAB_00d5595a_c29
                                                                                                                    end
                                                                                                                    __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29
                                                                                                                end
                                                                                                                if not scratchValue13 then
                                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                                    scratchValue17 = 0.0
                                                                                                                    if scratchValue17 < fret_04 then
                                                                                                                        scratchValue31 = 0
                                                                                                                        scratchValue32 = 0
                                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        while scratchValue14 do
                                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                                        end
                                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    end
                                                                                                                    scratchValue31 = 0
                                                                                                                    scratchValue32 = 0
                                                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 0x3f800000, 0, false, true)
                                                                                                                    goto LAB_00d5595a_c29
                                                                                                                end
                                                                                                            end
                                                                                                        end
                                                                                                        __region_LAB_00d55cd5_c29()
                                                                                                        goto LAB_00d55c2b_c29
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    quest:GetHealth(resources:ScriptThing(scratchValue40))
                                                                                                    scratchValue17 = 0.0
                                                                                                    if scratchValue17 < fret_02 then
                                                                                                        scratchValue31 = 0
                                                                                                        scratchValue32 = 0
                                                                                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", 0, false, true, CONCAT13(scratchValue32,CONCAT12(scratchValue31,CONCAT11( 0,0))))
                                                                                                        scratchValue14 = me:IsPerformingScriptTask()
                                                                                                        while scratchValue14 do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                            scratchValue14 = me:IsPerformingScriptTask()
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    end
                                                                                                    ::LAB_00d5595a_c29::
                                                                                                    quest:PauseAllNonScriptedEntities(scratchValue32 ~= 0)
                                                                                                end
                                                                                                if scratchValue16 ~= 0 then
                                                                                                    if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                        scratchValue21 = quest:AddNewConversation(me, false, false)
                                                                                                        quest:AddPersonToConversation(scratchValue21, quest:GetHero())
                                                                                                        quest:SetTimer(timerId4, 10)
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                            if xStack_154 == 1 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                                -- LAB_00d55b4e_c29: (native jump target)
                                                                                                            elseif xStack_154 == 2 then
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, quest:GetHero(), false)
                                                                                                                -- TODO(native): goto LAB_00d55b4e_c29
                                                                                                            end
                                                                                                            -- TODO(native): xStack_154 = (CCharString)(1 - (int)xStack_154);
                                                                                                        else
                                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, quest:GetHero(), false)
                                                                                                        end
                                                                                                    end
                                                                                                end
                                                                                                if scratchValue15 == 0 then
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                    if not me:IsPerformingScriptTask() then
                                                                                                        scratchValue15 = '\x01'
                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                                                                                                    end
                                                                                                end
                                                                                            until scratchValue16 == 0
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
                                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    while not scratchValue13 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c29 end
                                                        scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
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
                                scratchValue13 = quest:MsgIsGameInfoClickedPast()
                                while not scratchValue13 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    scratchValue13 = quest:MsgIsGameInfoClickedPast()
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
    resources:DestroyMovie(scratchValue40)
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

