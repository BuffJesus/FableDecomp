-- Retail helper 0x007E7490 (CScriptGameResourceObjectScriptedThingBase::?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ)
-- Return an opaque script-thing token; native output storage never crosses into Lua.
return function(get_resource, make_empty, invoke_resource)
    local resource = get_resource(8)
    if resource == nil then
        return make_empty(12, 10068688)
    end
    return invoke_resource(resource, "GetScriptThing", 48)
end
