class StreamBuffer
{
public:
    long position;
    long stride;
    void StreamBuffer_AdvancePosition(long count);
};
void StreamBuffer::StreamBuffer_AdvancePosition(long count)
{
    position += stride * count * 2;
}