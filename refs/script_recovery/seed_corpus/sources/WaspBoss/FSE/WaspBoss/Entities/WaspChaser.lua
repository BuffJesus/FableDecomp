function Main(quest, me)
    quest:Log("WaspChaser: Main() started.")
    me:AcquireControl()

    local woman = quest:GetThingWithScriptName("WaspChaseWoman")
    if woman then
        quest:Log("WaspChaser: Found WaspChaseWoman. Following and attacking...")
        me:FollowThing(woman, 1.0, true)
    else
        quest:Log("!!! WARNING: WaspChaser - WaspChaseWoman not found!")
    end

    while woman and woman:IsAlive() and quest:GetHealth(woman) > 0 do
        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    quest:Log("WaspChaser: Target woman dead or removed. Releasing control.")
    me:ReleaseControl()
end
