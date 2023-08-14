#include <cstdio>

// struct ClockOfTheLongNow {
//     ClockOfTheLongNow() { year = 2023; }
//
//     void add_year() { year++; }
//
//     bool set_year(int year) {
//         if (year < 2023) {
//             return false;
//         } else {
//             this->year = year;
//             return true;
//         }
//     }
//
//     int get_year() { return year; }
//
//    private:
//     int year{};
// };

struct College {
    char name[256];
};

// void print_name(College* college_ptr) {
//     // Array decay
//     printf("College name: %s\n", college_ptr->name);
// }

void print_name(College* colleges, size_t n_colleges) {
    for (size_t i = 0; i < n_colleges; i++) {
        printf("College name: %s\n", colleges[i].name);
    }
}

// Const Argument
void petruchio(const char* shrew) {
    printf("Fear not, sweet wench, they shall not touch thee, %s.\n", shrew);
}

// Const Method

struct ClockOfTheLongNow {
    ClockOfTheLongNow() { year = 2023; }
    ClockOfTheLongNow(long year) { this->year = year; }

    void add_year() { year++; }

    bool set_year(int year) {
        if (year < 2023) {
            return false;
        } else {
            this->year = year;
            return true;
        }
    }

    int get_year() const { return year; }

    bool is_leap_year(const ClockOfTheLongNow& clock) {
        if (clock.get_year() % 4 > 0) return false;
        if (clock.get_year() % 100 > 0) return true;
        if (clock.get_year() % 400 > 0) return false;
        return true;
    }

    bool is_leap_year() {
        if (this->year % 4 > 0) return false;
        if (this->year % 100 > 0) return true;
        if (this->year % 400 > 0) return false;
        return true;
    }

   private:
    int year{};
};

void add_year(ClockOfTheLongNow& clock) {
    clock.set_year(clock.get_year() + 1);
}

// Const member variables
// struct Avout {
//     const char* name = "Erasumas";
//     ClockOfTheLongNow apert;
// };

// Member initializer lists
struct Avout {
    const char* name;
    ClockOfTheLongNow apert;

    Avout(const char* name, long year_of_apert)
        : name{name}, apert{year_of_apert} {}

    void announce() const {
        printf("My name is %s and my next apert is %d\n", name,
               apert.get_year());
    }
};

int main() {
    int number{};
    printf("Int: %d\n", number);

    int* number_ptr = &number;
    printf("Ptr: %p\n\n", number_ptr);

    // Deferencing
    *number_ptr = 5;
    printf("Int: %d\n", *number_ptr);
    printf("Ptr: %p\n\n", number_ptr);

    ClockOfTheLongNow clock;
    ClockOfTheLongNow* clock_ptr = &clock;

    clock_ptr->set_year(2035);
    (*clock_ptr).set_year(2035);

    clock_ptr->add_year();
    (*clock_ptr).add_year();
    printf("Year: %d\n", clock_ptr->get_year());
    printf("Year: %d\n", (*clock_ptr).get_year());
    printf("Address: %p\n", clock_ptr);

    // Arrays and Pointers
    College best_colleges[] = {"MIT", "Stanford", "Oxford"};
    College* third = &best_colleges[2];

    // Pointer arithmetic
    College* second = best_colleges + 1;

    print_name(best_colleges, sizeof(best_colleges) / sizeof(College));

    // Low Level Fucking around
    char lower[] = "abc?e";
    char upper[] = "ABC?E";

    char* upper_ptr = upper;

    lower[3] = 'd';
    *(lower + 3) = 'd';

    upper_ptr[3] = 'D';
    *(upper_ptr + 3) = 'D';

    char letter_d = lower[3];
    char letter_D = upper_ptr[3];

    printf("Lower: %s\nUpper: %s\n", lower, upper);

    // lower[7] = 'g';  // Ass
    // *(lower + 7) = 'g'

    // References
    ClockOfTheLongNow clock2;
    printf("Year: %d\n", clock2.get_year());
    add_year(clock2);
    printf("Year: %d\n", clock2.get_year());

    // Const
    printf("%d: %d\n", clock2.get_year(), clock2.is_leap_year());
    printf("%d: %d\n", clock.get_year(), clock.is_leap_year(clock));

    Avout raz{"Erasumas", 3010};
    Avout jad{"Jad", 4000};
    raz.announce();
    jad.announce();

    // Auto
    auto baller{"Baller"};
    auto razz{0.3F};
    const auto& baller_ref = baller;
    const auto* baller_ptr = &baller_ref;
    printf("%s\n", baller_ref);
    printf("%s\n", *baller_ptr);

    return 0;
}
