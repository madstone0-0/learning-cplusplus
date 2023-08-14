#include <cstdio>
#include <iterator>
#include <cstddef>

int main() {
    /* Integers */
    // Binary
    unsigned short a = 0b10101010;
    printf("%hu\n", a);

    // Octal
    int b = 0123;
    printf("%d\n", b);

    // Hexadecimal
    unsigned long long d = 0xFFFFFFFFFFFFFFFF;
    printf("%llu\n", d);

    // Decimal to Hexadecimal in printf
    unsigned int c = 3669732608;
    printf("Yabba %x!\n", c);

    // 69 in decimal and octal format
    unsigned int e = 69;
    printf("There are %u,%o leaves here.\n", e, e);

    /* Floating points */
    // Precision is set to double by default
    float f = 0.1F;
    double g = 0.2;
    long double h = 0.3L;

    double plancks_constant = 6.62607004e-34;

    double avogadros_number = 6.02214076e23;
    printf("Avogadros number: %le %lf %lg\n", avogadros_number,
           avogadros_number, avogadros_number);

    double hp = 9.75;
    printf("Hogwarts' Platform %e %f %g\n", hp, hp, hp);

    /* Characters */

    char i = 'M';
    wchar_t j = L'Z';
    printf("Windows binaries start with %c%lc\n", i, j);

    /* Boolean */
    bool k = true;
    bool l = false;
    printf("%d %d\n", k, l);

    /* Size */
    size_t size_c = sizeof(char);
    printf("char: %zu\n", size_c);

    size_t size_s = sizeof(short);
    printf("short: %zu\n", size_s);

    size_t size_i = sizeof(int);
    printf("int: %zu\n", size_i);

    size_t size_l = sizeof(long);
    printf("long: %zu\n", size_l);

    size_t size_ll = sizeof(long long);
    printf("long long: %zu\n", size_ll);

    /* Arrays */
    unsigned long max = 0;
    unsigned long values[] = {10, 50, 20, 40, 0};
    for (size_t i = 0; i < sizeof(values) / sizeof(unsigned long); i++) {
        if (values[i] > max) {
            max = values[i];
        }
    }

    for (size_t i = 0; i < std::size(values); i++) {
        if (values[i] > max) {
            max = values[i];
        }
    }

    for (unsigned long value : values) {
        if (value > max) {
            max = value;
        }
    }

    printf("Max: %lu\n", max);

    /* Strings */
    // C-Style
    char name[] = "Madiba Hudson-Quansah";
    printf("My name is %s\n", name);

    char16_t runic = u'\u16A0';

    char alphabet[27];

    for (int i = 0; i < 26; i++) {
        alphabet[i] = i + 97;
    }

    alphabet[26] = 0;
    printf("%s\n", alphabet);

    for (int i = 0; i < 26; i++) {
        alphabet[i] = i + 65;
    }
    printf("%s\n", alphabet);

    /* User Defined */
    // Enumeration
    enum class Realm {
        Jotunheimr,
        Asgardr,
        Vanaheimr,
        Svartalfheimr,
        Ljosalfheimr,
        Midgardr,
        Muspelheimr,
        Niflheimr,
        Nidavellir,
    };

    Realm realm = Realm::Nidavellir;

    switch (realm) {
        case Realm::Jotunheimr:
            printf("Land of the Jotunar\n");
            break;
        case Realm::Asgardr:
            printf("Land of the Asynjur\n");
            break;
        case Realm::Vanaheimr:
            printf("Land of the Vanir\n");
            break;
        case Realm::Svartalfheimr:
            printf("Land of the Svartalfar\n");
            break;
        case Realm::Ljosalfheimr:
            printf("Land of the Ljosalfar\n");
            break;
        case Realm::Midgardr:
            printf("Land of the humans\n");
            break;
        case Realm::Muspelheimr:
            printf("Land of the Fire Jotunar\n");
            break;
        case Realm::Niflheimr:
            printf("Land of the dishonored dead");
            break;
        case Realm::Nidavellir:
            printf("Land of the Dwarves\n");
            break;
        default:
            printf("Not of this plane of existence\n");
    }

    // Structs / Data classes
    struct Book {
        char name[256];
        int year;
        int pages;
        bool hardcover;
    };

    Book hobbit = {"The Hobbit", 1937, 310, true};

    Book books[] = {{"The Hobbit", 1937, 310, true},
                    {"The Lord of the Rings", 1954, 1216, true},
                    {"The Silmarillion", 1977, 432, true}};

    for (Book book : books) {
        printf("Book: %s\n", book.name);
        printf("Year: %d\n", book.year);
        printf("Pages: %d\n", book.pages);
        printf("Hardcover: %d\n", book.hardcover);
        printf("\n");
    }

    // Unions
    union Variant {
        char string[10];
        int integer;
        double floating_point;
    };

    Variant v;
    v.integer = 90;
    printf("Integer: %d\n", v.integer);
    v.floating_point = 3.142;
    printf("Floating point: %f\n", v.floating_point);
    printf("Dumpster fire: %d\n", v.integer);

    /* Actual classes */

    struct ClockOfTheLongNow {
        void add_year() { year++; }

        bool set_year(int new_year) {
            if (new_year < 2023)
                return false;
            else {
                year = new_year;
                return true;
            }
        }

        int get_year() { return year; }

       private:
        int year{};
    };

    class ClockOfTheLongNowClass {
        int year{};

       public:
        void add_year() { year++; }

        bool set_year(int new_year) {
            if (new_year < 2023)
                return false;
            else {
                year = new_year;
                return true;
            }
        }

        int get_year() { return year; }
    };

    ClockOfTheLongNow clockStruct;
    ClockOfTheLongNowClass clock;
    clock.set_year(2023);
    clock.add_year();
    printf("Year: %d\n", clock.get_year());
    clock.add_year();
    printf("Year: %d\n", clock.get_year());

    return 0;
}
