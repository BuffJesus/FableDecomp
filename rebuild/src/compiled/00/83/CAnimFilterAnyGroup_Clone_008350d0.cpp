struct CAnimFilterAnyGroup {
    virtual bool AcceptsGroup(long group) const;
    CAnimFilterAnyGroup* Clone() const;
};

CAnimFilterAnyGroup* CAnimFilterAnyGroup::Clone() const {
    return new CAnimFilterAnyGroup;
}