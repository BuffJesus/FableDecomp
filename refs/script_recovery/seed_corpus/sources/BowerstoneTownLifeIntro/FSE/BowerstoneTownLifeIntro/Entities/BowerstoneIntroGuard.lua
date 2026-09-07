local MY_SCRIPT_NAME = "BowerstoneIntroGuard"

function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false)
end

function Main(quest, me)
    quest:Log("BowerstoneIntroGuard: Main() started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: BowerstoneIntroGuard Main - could not get Hero.")
        return
    end

    if not me:AcquireControl() then
        quest:Log("!!! ERROR: BowerstoneIntroGuard Main - failed to acquire control.")
        return
    end

    if not quest:GetStateBool("DoneGuardSpeech") then
        me:ReleaseControl()

        quest:StartCutscene({ HERO = hero, Guard = me })
        quest:RunCutscene("CS_BOWERSTONE_INTRO_TOWN", true, false)
        quest:AddLogbookTutorialEntry("TEXT_QST_LOG_TOWNS_LAWANDORDER")
        quest:AddLogbookTutorialEntry("TEXT_QST_LOG_TOWNS_PUBS")
        quest:SetStateBool("DoneGuardSpeech", true)
        quest:EndCutscene()
    end

    quest:Log("BowerstoneIntroGuard: Main() completed.")
end
