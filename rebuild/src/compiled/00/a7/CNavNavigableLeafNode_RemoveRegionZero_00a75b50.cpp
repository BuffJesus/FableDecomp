struct CNavNavigableLeafNode {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual int  v5();
    bool RemoveRegionZero();
};

bool CNavNavigableLeafNode::RemoveRegionZero()
{
    return this->v5() == 0;
}