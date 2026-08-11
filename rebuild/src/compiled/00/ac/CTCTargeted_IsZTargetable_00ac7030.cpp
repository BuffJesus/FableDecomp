struct CTCTargeted {
    int a; // +0
    int b; // +4
    int c; // +8
    bool IsZTargetable();
};

bool CTCTargeted::IsZTargetable()
{
    int bb = b;
    bool r = (a == bb) || (bb == c) || (c == a);
    return r;
}