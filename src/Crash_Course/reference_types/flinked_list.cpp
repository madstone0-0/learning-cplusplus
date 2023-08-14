#include <cstdio>

struct Element {
    Element* next{};
    Element* previous{};
    char prefix[2];
    short operating_number{};

    void insert_after(Element* new_element) {
        new_element->next = this->next;
        this->next = new_element;

        new_element->previous = this;
    }

    void insert_before(Element* new_element) {
        new_element->previous = this->previous;
        this->previous = new_element;

        new_element->next = this;
    }
};

int main() {
    Element trooper1, trooper2, trooper3, trooper4;
    trooper1.prefix[0] = 'T';
    trooper1.prefix[1] = 'K';
    trooper1.operating_number = 421;
    trooper1.insert_after(&trooper2);
    trooper2.prefix[0] = 'F';
    trooper2.prefix[1] = 'N';
    trooper2.operating_number = 2187;
    trooper2.insert_after(&trooper3);
    trooper3.prefix[0] = 'L';
    trooper3.prefix[1] = 'S';
    trooper3.operating_number = 005;
    trooper3.insert_after(&trooper4);
    trooper4.prefix[0] = 'V';
    trooper4.prefix[1] = 'D';
    trooper4.operating_number = 032;

    for (Element* cursor = &trooper1; cursor; cursor = cursor->next) {
        printf("Stormtrooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1],
               cursor->operating_number);
    }

    printf("\n\n");

    for (auto* cursor = &trooper4; cursor; cursor = cursor->previous) {
        printf("Stormtrooper %c%c-%d\n", cursor->prefix[0], cursor->prefix[1],
               cursor->operating_number);
    }

    return 0;
}
