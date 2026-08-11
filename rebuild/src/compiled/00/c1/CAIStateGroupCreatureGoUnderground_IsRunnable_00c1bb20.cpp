struct CAIStateGroup_CreatureGoUnderground {
    char m_flag;
    char IsRunnable(float t);
};
char CAIStateGroup_CreatureGoUnderground::IsRunnable(float t)
{
    return this->m_flag = (char)(long)(t * 0.5f);
}