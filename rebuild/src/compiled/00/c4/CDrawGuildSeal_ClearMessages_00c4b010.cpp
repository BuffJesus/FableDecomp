struct CDrawGuildSeal {
    unsigned char Reserved[0x68];
    void* FirstMessage;
    void RemoveMessage(unsigned long index);
    void ClearMessages();
};
void CDrawGuildSeal::ClearMessages()
{
    while (FirstMessage != 0)
        RemoveMessage(0);
}