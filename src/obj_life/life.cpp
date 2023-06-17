#include <cstdio>

/* Automatic Storage Duration */
// Static Variables

// static int rat_things_power = 200;

// void power_up_rat_thing(int nuclear_isotopes) {
//     rat_things_power += nuclear_isotopes;
//     const auto waste_heat = nuclear_isotopes * 5;
//     if (waste_heat > 1000) {
//         printf("Warning: Hot doggie %d\n", waste_heat);
//     }
// }

void power_up_rat_thing(int nuclear_isotopes) {
    static int rat_things_power = 200;
    rat_things_power += nuclear_isotopes;
    const auto waste_heat = nuclear_isotopes * 5;
    if (waste_heat > 1000) {
        printf("Warning: Hot doggie %d\n", waste_heat);
    }
    printf("Rat-thing power: %d\n", rat_things_power);
}

// Static Members

struct RatThing {
    static int rat_power;
    static void power_up_rat_thing(int nuclear_isotopes) {
        rat_power += nuclear_isotopes;
        const auto waste_heat = nuclear_isotopes * 20;
        if (waste_heat > 1000) {
            printf("Warning: Hot doggie %d\n", waste_heat);
        }
        printf("Rat-thing power: %d\n", rat_power);
    }
};

int RatThing::rat_power = 200;

int main() {
    /* Automatic Storage Duration */
    // Static Variables

    // printf("Rat-thing power: %d\n", rat_things_power);
    power_up_rat_thing(100);
    // printf("Rat-thing power: %d\n", rat_things_power);
    power_up_rat_thing(500);
    // printf("Rat-thing power: %d\n", rat_things_power);

    printf("\n");

    // Static Members
    RatThing::power_up_rat_thing(100);
    RatThing::power_up_rat_thing(500);

    /* Dynamic Storage Duration */
    // Dynamic Variables
    int* ptr = new int{42};
    *ptr += 5;
    delete ptr;

    // Dynamic Arrays
    int* array = new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    array[2] += 4;
    delete[] array;

    return 0;
}
