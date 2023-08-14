// Quake fast square root hack
float Q_rsqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y;             // evil floating point bit hack??? tf
    i = 0x5f3759df - (i >> 1);  // wtf
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));

    return y;
}

int main() {
    auto res = Q_rsqrt(0.2f);
    return 0;
}
