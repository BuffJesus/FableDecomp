template<class T> inline const T& Minimum(const T& left, const T& right)
{
    return right < left ? right : left;
}
template<class T> inline const T& Maximum(const T& left, const T& right)
{
    return left < right ? right : left;
}
struct C2DVector { float X; float Y; };
struct C2DLineF {
    C2DVector Point1;
    C2DVector Point2;
    float GetLowestY() const;
    float GetHighestY() const;
};
float C2DLineF::GetLowestY() const
{
    return (float)Minimum((double)Point1.Y, (double)Point2.Y);
}