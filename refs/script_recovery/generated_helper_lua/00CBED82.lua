-- Retail helper 0x00CBED82 (KillAllThingsInVector)
-- Iterate the logical vector through callbacks; never expose native storage to Lua.
return function(things, is_alive, remove_thing, remove_flag)
    for index = 1, #things do
        local thing = things[index]
        if is_alive(thing) then
            remove_thing(thing, remove_flag, true)
        end
    end
end
