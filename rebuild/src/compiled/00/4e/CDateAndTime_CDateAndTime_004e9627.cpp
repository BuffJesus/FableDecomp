#pragma optimize("s",on)
struct CDateAndTime {
    float  f00;
    int    pad04[4];
    int    m14;
    int    m18;
    int    m1c;
    int    m20;
    int    m24;
    int    m28;
};
CDateAndTime * __fastcall CDateAndTime(CDateAndTime *this_)
{
    this_->f00 = 0.0f;
    this_->m14 = 0;
    this_->m18 = 0;
    this_->m1c = 0;
    this_->m20 = 0;
    this_->m24 = 0;
    this_->m28 = 0;
    return this_;
}