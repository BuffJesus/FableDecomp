local screamedAtHero = false

function Init(quest, me)
    quest:Log("FleeingWoman: Init() started.")
    screamedAtHero = false
end

function Main(quest, me)
    quest:Log("FleeingWoman: Main() started.")
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: FleeingWoman - could not get Hero!")
        return
    end

    me:AcquireControl(4) -- SCRIPT_AI_PRIORITY_HIGHEST

    -- Wait until hero gets close (within WB_ScreamingVillagerDistance = 30)
    quest:Log("FleeingWoman: Waiting for Hero to approach within 30.0m (WB_ScreamingVillagerDistance)...")
    while not quest:IsDistanceBetweenThingsUnder(me, hero, 30.0) do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    quest:Log("FleeingWoman: Hero approached within 30.0m! Moving to FleeingWomanEscapePos...")
    local exitPos = quest:GetThingWithScriptName("FleeingWomanEscapePos")
    if not exitPos then
        quest:Log("!!! WARNING: FleeingWomanEscapePos not found!")
    end

    while true do
        if not screamedAtHero then
            -- Scream when hero is within WB_ScreamingVillagerScreamsDistance = 10
            if quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) then
                screamedAtHero = true
                quest:Log("FleeingWoman: Screaming at hero: TEXT_QST_072_WOMAN_FLEES_10")
                local convoID = quest:StartAmbientConversation(me, hero, false, false)
                quest:AddLineToConversation(convoID, "TEXT_QST_072_WOMAN_FLEES_10", me, hero, false)
            end
        end

        if exitPos then
            if not me:IsPerformingScriptTask() then
                me:MoveToPosition(exitPos:GetPos(), 1.0, 1)
            end
            if quest:IsDistanceBetweenThingsUnder(me, exitPos, 2.0) then
                quest:Log("FleeingWoman: Reached exit position.")
                break
            end
        else
            break
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    quest:Log("FleeingWoman: Fading out (2.5s WB_ScreamingVillagerFadeOutTime) and killing entity.")
    quest:FadeOutAndKillEntity(me, true, 2.5, true)
    me:ReleaseControl()
end
