struct CQuadric {
    float a;
    float b;
    void OperatorPlusEq(CQuadric* other) {
        a = other->a + a;
        b = other->b + b;
    }
};

void force_CQuadric_OperatorPlusEq() { CQuadric q; q.OperatorPlusEq(&q); }