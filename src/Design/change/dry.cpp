#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

static constexpr double TAX_RATE{1.25};

class Money {
   public:
    Money(double amount = 0) : amount{amount} {}
    Money operator*(double factor) const { return Money{amount * factor}; }

    Money operator+(Money rhs) const { return Money{amount + rhs.amount}; }

    friend ostream& operator<<(ostream& s, const Money& m) {
        s << "$" << fixed << setprecision(2) << m.amount << ' ';
        return s;
    }

   private:
    double amount;
};

class Item {
   public:
    virtual ~Item() = default;
    [[nodiscard]] virtual Money price() const = 0;
};

class CppBook : public Item {
   public:
    CppBook(string title, string author, Money price)
        : title_{std::move(title)}, author_{std::move(author)}, priceWithTax_{price * TAX_RATE} {}

    [[nodiscard]] string const& title() const { return title_; }
    [[nodiscard]] string const& author() const { return author_; }
    [[nodiscard]] Money price() const override { return priceWithTax_; }

   private:
    string title_;
    string author_;
    Money priceWithTax_;
};

class ConferenceTicket : public Item {
   public:
    explicit ConferenceTicket(string name, Money price) : name_{std::move(name)}, priceWithTax_{price * TAX_RATE} {}

    [[nodiscard]] string const& name() const { return name_; }
    [[nodiscard]] Money price() const override { return priceWithTax_; }

   private:
    string name_;
    Money priceWithTax_;
};

int main() {
    vector<unique_ptr<Item>> items{};
    items.emplace_back(std::make_unique<CppBook>("Effective C++", "", 19.99));
    items.emplace_back(std::make_unique<CppBook>("C++ Templates", "", 49.99));
    items.emplace_back(std::make_unique<ConferenceTicket>("CppCon", 999.0));
    items.emplace_back(std::make_unique<ConferenceTicket>("Meeting C++", 699.0));
    items.emplace_back(std::make_unique<ConferenceTicket>("C++ on Sea", 499.0));

    const Money totalPrice = accumulate(items.begin(), items.end(), Money{},
                                        [](Money acc, auto const& item) { return acc + item->price(); });
    cout << "Total Price: " << totalPrice << endl;
    return EXIT_SUCCESS;
}
