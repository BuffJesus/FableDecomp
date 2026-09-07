function Main(quest, me)
    quest:Log("QueenHornet: Main() started. Pausing 2.0s before releasing to combat AI...")
    quest:Pause(2.0)
    me:AcquireControl(1)
    me:ReleaseControl()

    while me and me:IsAlive() and quest:GetHealth(me) > 0 do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    quest:Log("QueenHornet: Queen Hornet died or removed.")
end
