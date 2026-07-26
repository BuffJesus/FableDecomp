
struct Node { char pad[0x14]; long value; /* +0x14 */ };

struct Iter { Node* p; };

struct TimerMap {
    Node* begin;                 // +0x0  m_pBegin
    Node* end;                   // +0x4
    Iter LowerBound(const long* key);
};

struct CGameScriptInterface {
    char pad[0x3c];
    TimerMap map;                // +0x3c
    long GetTimer(long key) const;
};

long CGameScriptInterface::GetTimer(long key) const
{
    const TimerMap* m = &this->map;
    Iter it = ((TimerMap*)m)->LowerBound(&key);
    if (it.p != m->begin)
        return it.p->value;
    return 0;
}