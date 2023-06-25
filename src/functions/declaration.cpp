#include <cstdio>

/* Prefix Modifiers
*
* static - Function can be used without instantiation of its parent class
* virtual - Function can be overridden by a child class
* inline - Hints to the compiler’s optimizer to put a function directly inline rather than perform a function call.
*
 Prefix Modifiers */

/* Suffix Modifiers
 *
 * noexcept - Function will never throw an exception.
 * const - Function won't modify any instance of its class
 * final - Funtion cannot be overridden by a child class
 * overrride - Function is overriding a parent class definition
 *
 */

// Final and Override

// struct BostonCorbett {
//     virtual void shoot() final { printf("What a God we have...God avenged Abraham Lincoln"); }
// };
//
// struct BostonCorbettJunior : BostonCorbett {
//     void shoot() override {}  // Crash
// };

// struct BostonCorbett final {
//     virtual void shoot() final { printf("What a God we have...God avenged Abraham Lincoln"); }
// };
//
// struct BostonCorbettJunior : BostonCorbett {};  // Crash
//
// int main() {
//     BostonCorbettJunior junior;
//     return 0;
// }

// Volatile

struct Distillate {
    int apply() volatile { return ++applications; }

   private:
    int applications{};
};

int main() {
    volatile Distillate ethanol;
    printf("%d Tequila\n", ethanol.apply());
    printf("%d Tequila\n", ethanol.apply());
    printf("%d Tequila\n", ethanol.apply());
    printf("Floor!");

    return 0;
}

/* Suffix Modifiers*/
