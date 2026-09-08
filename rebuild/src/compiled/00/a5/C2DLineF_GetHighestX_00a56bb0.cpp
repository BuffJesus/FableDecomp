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
    float GetLowestX() const;
    float GetHighestX() const;
};
float C2DLineF::GetHighestX() const
{
    return (float)Maximum((double)Point1.X, (double)Point2.X);
}