struct CNavRegion;

struct CNavSwitchableLeafNode {
    int field_0;
    int field_4;
    CNavRegion* region;   // +8
    char flag;            // +0xc

    bool GetRegion(CNavRegion** out);
};

bool CNavSwitchableLeafNode::GetRegion(CNavRegion** out)
{
    if (!flag) {
        *out = region;
        flag = 1;
        return true;
    }
    return false;
}