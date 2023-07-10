
struct DeadMenOfDunharrow {
    DeadMenOfDunharrow(const char* m = "") : message{m} { oathsToFulfill++; }

    ~DeadMenOfDunharrow() { oathsToFulfill--; }

    const char* message;
    static int oathsToFulfill;
};

int DeadMenOfDunharrow::oathsToFulfill{};
