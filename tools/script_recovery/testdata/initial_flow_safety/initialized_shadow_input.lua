function Main(q)
    local x
    x = false
    do
        local x = q:Value()
        q:Observe(x)
    end
    return x
end
