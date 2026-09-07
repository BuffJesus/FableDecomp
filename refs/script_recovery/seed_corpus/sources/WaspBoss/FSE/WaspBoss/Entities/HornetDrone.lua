local eatNow = 0

function Init(quest, me)
    quest:Log("HornetDrone: Init() started.")
    eatNow = math.random(1, 200)
end

function Main(quest, me)
    quest:Log("HornetDrone: Main() started.")
    me:AcquireControl()

    local deadBody = quest:GetNearestWithScriptName(me, "Q_WB_DeadBody")
    if deadBody then
        quest:Log("HornetDrone: Found nearest Q_WB_DeadBody. Moving to feed...")
        me:MoveToThing(deadBody, 0.3, 1)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then
                me:ReleaseControl()
                return
            end
        end
    else
        quest:Log("HornetDrone: No Q_WB_DeadBody found nearby.")
    end

    quest:Log("HornetDrone: Playing ST_FEED_INTO animation...")
    me:PlayAnimation("ST_FEED_INTO")
    if not quest:NewScriptFrame(me) then
        me:ReleaseControl()
        return
    end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    quest:Log("HornetDrone: Looping ST_FEED_LOOP animation...")
    me:PlayLoopingAnimation("ST_FEED_LOOP")

    local hero = quest:GetHero()
    while true do
        if me:IsAwareOfHero() or me:MsgIsHitBy("SCRIPT_NAME_HERO") or (hero and quest:IsDistanceBetweenThingsUnder(me, hero, 6.0)) then
            quest:Log("HornetDrone: Detected Hero or hit! Breaking out of feeding loop.")
            break
        end
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    me:ClearCommands()
    me:ClearAllActionsIncludingLoopingAnimations()

    quest:Log("HornetDrone: Playing ST_FEED_OUTOF animation...")
    me:PlayAnimation("ST_FEED_OUTOF")
    if not quest:NewScriptFrame(me) then
        me:ReleaseControl()
        return
    end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    if hero then
        quest:Log("HornetDrone: Handing over combat target to Hero.")
        quest:GiveThingBestEnemyTarget(me, hero)
    end

    me:ReleaseControl()
    quest:Log("HornetDrone: Released control to combat AI.")
end
