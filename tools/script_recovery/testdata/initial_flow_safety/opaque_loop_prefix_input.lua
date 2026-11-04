function Main(q)
    local x
    repeat x = q:Resource() until true; goto past
    ::past::
    x = false
    q:Collect()
    x = q:Value()
end
