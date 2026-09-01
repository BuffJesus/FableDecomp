-- Retail helper 0x00CD23B9 (C3DMeshInfo::HasPhysicsMesh)
-- Preserve the unnamed native field through a typed reader.
return function(read_i32)
    return read_i32(8) ~= 0
end
