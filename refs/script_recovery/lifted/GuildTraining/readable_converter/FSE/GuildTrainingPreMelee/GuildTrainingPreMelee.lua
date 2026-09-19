-- Readable native conversion: Q_GuildTrainingPreMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingPreMelee.Main (retail 0x00d51520)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local timerId
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingPreMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("PreMeleeDummy", "GuildTrainingPreMelee/Entities/PreMeleeDummy")
    quest:AddEntityBinding("PreMeleeWhisper", "GuildTrainingPreMelee/Entities/PreMeleeWhisper")
    quest:FinalizeEntityBindings()
    quest:SetMasterGameState("GuildWarningOccuring", true)
    local preMeleeMaze = quest:GetThingWithScriptName("PreMeleeMaze")
    local preMeleeWhisper = quest:GetThingWithScriptName("PreMeleeWhisper")
    local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    quest:SetStateInt("PreMeleeMode", 0)
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, preMeleeMaze, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); goto LAB_00d51de2 end
    if not preMeleeWhisper:AcquireControl(4) then goto LAB_00d51951 end
    if not theRealGuildmaster:AcquireControl(4) then goto LAB_00d51948 end
    if not hero:AcquireControl(4) then goto LAB_00d5193f end
    quest:StartCutscene({HERO = hero, MAZE = preMeleeMaze, WHISPER = preMeleeWhisper, MASTER = theRealGuildmaster}, {}, true)
    quest:RunCutscene("CS_GUILD_PREMELEE_INTRO", true, false)
    quest:RunCutscene("CS_GUILD_PREMELEE_BOOHOO", true, false)
    quest:RunCutscene("CS_GUILD_PREMELEE_WAKEUP", true, false)
    quest:FixMovieSequenceCamera(false)
    quest:ChangeHeroHealthBy(1000.0, true, false)
    quest:ResetPlayerCreatureCombatMultiplier()
    quest:EndCutscene()
    hero:ReleaseControl()
    theRealGuildmaster:ReleaseControl()
    preMeleeWhisper:ReleaseControl()
    resources:ReleaseResource(resource)
    quest:SetStateBool("WhisperCutsceneFinished", true)
    quest:SetStateBool("GuildmasterTeleport", true)
    quest:SetMasterGameState("GuildWarningOccuring", false)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    while 0 < quest:GetTimer(timerId) do
        if not quest:NewScriptFrame() then goto LAB_00d51dd9 end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:GiveHeroExpression("EXPRESSION_FART", -1, true)
        quest:GiveHeroExpression("EXPRESSION_BELCH", -1, true)
        quest:GiveHeroExpression("EXPRESSION_GIGGLE", -1, true)
        while not quest:GetStateBool("HeroSleeps") do
            if not quest:NewScriptFrame() then goto LAB_00d51dd9 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        end
    end
    ::LAB_00d51dd9::
    quest:DeregisterTimer(timerId)
    goto LAB_00d51de2
    ::LAB_00d5193f::
    hero:ReleaseControl()
    ::LAB_00d51948::
    theRealGuildmaster:ReleaseControl()
    ::LAB_00d51951::
    preMeleeWhisper:ReleaseControl()
    resources:ReleaseResource(resource)
    ::LAB_00d51de2::
end

-- Q_GuildTrainingPreMelee.Init (retail 0x00d51450)
function Init(quest)
    quest:SetStateBool("HeroSleeps", false)
    quest:SetStateBool("WhisperCutsceneFinished", false)
    quest:SetStateBool("GuildmasterTeleport", false)
    quest:SetStateBool("WhisperStopFollowing", false)
end

