struct CCharString {
    void* p;
};

struct CDefString {
    void* p;
    bool operator==(const CCharString& rhs) const;
};

struct CThing {
    CDefString GetDefName() const;
};

struct CThingFilter_HasDefName {
    CCharString m_name;
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasDefName::operator()(const CThing& thing) const
{
    return thing.GetDefName() == m_name;
}