#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/* Protected members */
class Base {
   private:
    int priv{};

   protected:
    int prot{};

   public:
    int pub{};
};

class Derived : public Base {
   public:
    void member() {
        // cout << priv; // Not visible
        cout << prot;
        cout << pub;
    }
};

class Unrelated {
   private:
    Base X;

   public:
    void member() {
        // cout << X.priv; // Not visible
        // cout << X.prot; // Not visible
        cout << X.pub;
    }
};
/* Protected members */

/* Constructors and Destructors */
// class Person {
//    protected:
//     string name;
//     string ssn;
//
//    public:
//     void print() {
//         cout << "Name: " << name << '\n';
//         cout << "SSN: " << ssn << '\n';
//     }
//     Person(const string& name, const string& ssn) : name(name), ssn(ssn) {}
//
//     ~Person() { cout << "Destroying person: " << name << '\n'; }
// };
//
// class Student : public Person {
//    private:
//     string major;
//     int gradYear;
//
//    public:
//     void print() {
//         Person::print();  // call the base class print function
//         cout << "Major: " << major << '\n';
//         cout << "Graduation year: " << gradYear << '\n';
//     }
//
//     Student(const string& name, const string& ssn, const string& major, int gradYear)
//         : Person(name, ssn),  // call the base class constructor
//           major(major),
//           gradYear(gradYear) {}
//
//     void changeMajor(const string& newMajor) { major = newMajor; }
//
//     ~Student() { cout << "Destroying student: " << name << '\n'; }
// };
/* Constructors and Destructors */

/* Dynamic Binding */
class Person {
   protected:
    string name;
    string ssn;

   public:
    virtual void print() {
        cout << "Name: " << name << '\n';
        cout << "SSN: " << ssn << '\n';
    }
    Person(const string& name, const string& ssn) : name(name), ssn(ssn) {}

    virtual ~Person() { cout << "Destroying person: " << name << '\n'; }
};

class Student : public Person {
   private:
    string major;
    int gradYear;

   public:
    void print() override {
        Person::print();  // call the base class print function
        cout << "Major: " << major << '\n';
        cout << "Graduation year: " << gradYear << '\n';
    }

    Student(const string& name, const string& ssn, const string& major, int gradYear)
        : Person(name, ssn),  // call the base class constructor
          major(major),
          gradYear(gradYear) {}

    void changeMajor(const string& newMajor) { major = newMajor; }

    ~Student() override { cout << "Destroying student: " << name << '\n'; }
};
/* Dynamic Binding */

/* Inheritance example */
class Progression {
   protected:
    long first;
    long cur;

    virtual long firstValue() {
        cur = first;
        return cur;
    }

    virtual long nextValue() { return ++cur; }

   public:
    Progression(long f = 0) : first{f}, cur{f} {}

    virtual ~Progression() {}

    void printProgression(const int& n) {
        cout << firstValue();
        for (int i{2}; i <= n; i++) cout << ' ' << nextValue();
        cout << '\n';
    }
};

class ArithmeticProgression : public Progression {
   public:
    explicit ArithmeticProgression(long i = 0) : Progression(), inc{i} {}

   protected:
    long inc;
    long nextValue() override {
        cur += inc;
        return cur;
    }
};

class GeometricProgression : public Progression {
   public:
    explicit GeometricProgression(long r = 2) : Progression{1}, rat{r} {}

   protected:
    long rat;

    long nextValue() override {
        cur *= rat;
        return cur;
    }
};

class FibonnProgression : public Progression {
   public:
    explicit FibonnProgression(long f = 0, long s = 1) : Progression{f}, second{s}, prev{second - first} {}

   protected:
    long prev;
    long second;

    long firstValue() override {
        cur = first;
        prev = second - first;
        return cur;
    }

    long nextValue() override {
        auto temp{prev};
        prev = cur;
        cur += temp;
        return cur;
    }
};
/* Inheritance example */

int main(int argc, char* argv[]) {
    Derived d;
    Unrelated u;
    d.member();
    u.member();

    Student* s{new Student{"Madiba", "323923", "Computer Science", 2026}};
    s->print();
    s->changeMajor("Computer Engineering");
    s->print();

    delete s;

    auto arth{ArithmeticProgression{10}};
    Progression* proPtr = &arth;
    proPtr->printProgression(10);

    auto geo{GeometricProgression{3}};
    proPtr = &geo;
    proPtr->printProgression(10);

    auto fib{FibonnProgression{}};
    proPtr = &fib;
    proPtr->printProgression(10);

    /* Dynamic Casting */
    Person* p = new Student{"Madiba", "32322", "Computer Science", 2026};
    p->print();
    dynamic_cast<Student*>(p)->changeMajor("Compter Engineering");
    p->print();
    /* Dynamic Casting */

    delete p;
    // delete proPtr;
    return 0;
}
