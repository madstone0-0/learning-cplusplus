#include <cstdio>

enum class Operation { Add, Sub, Mul, Div };

struct Calculator {
    Operation op{};
    Calculator(Operation operation) { op = operation; }

    int calculate(int a, int b) {
        switch (op) {
            case Operation::Add:
                return a + b;
            case Operation::Sub:
                return a - b;
            case Operation::Mul:
                return a * b;
            case Operation::Div:
                return a / b;
            default:
                return 0;
        }
    }
};

int main() {
    Calculator add(Operation::Add);
    printf("%d\n", add.calculate(2, 3));
    Calculator sub(Operation::Sub);
    printf("%d\n", sub.calculate(2, 3));
    Calculator mul(Operation::Mul);
    printf("%d\n", mul.calculate(2, 3));
    Calculator div(Operation::Div);
    printf("%d\n", div.calculate(2, 3));
    return 0;
}
