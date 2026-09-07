struct C2DVector { float x; float y; };
struct C2DBoxF { float minX; float minY; float maxX; float maxY; };
class C2DLineF {
public:
    float x1; float y1; float x2; float y2;
    bool IntersectsWith(const C2DLineF* other, C2DVector* point, float tolerance);
    int IntersectsWith(const C2DBoxF* box);
};
int C2DLineF::IntersectsWith(const C2DBoxF* box) {
    C2DVector point;
    C2DLineF bottom = { box->minX, box->minY, box->maxX, box->minY };
    C2DLineF right = { box->maxX, box->minY, box->maxX, box->maxY };
    C2DLineF top = { box->minX, box->maxY, box->maxX, box->maxY };
    C2DLineF left = { box->minX, box->minY, box->minX, box->maxY };
    extern float g_C2DIntersectionTolerance;
    if (IntersectsWith(&bottom, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&right, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&top, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&left, &point, g_C2DIntersectionTolerance)) return true;
    return false;
}