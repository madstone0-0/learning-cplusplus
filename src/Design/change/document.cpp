#include <iostream>
#include <string>

/* Separation of Concerns */
struct JSON {
    virtual void exportToJson(/* */) = 0;
    virtual ~JSON() = default;
};

struct Serialization {
    virtual void serialize() = 0;
    virtual ~Serialization() = default;
};

class Document {
   public:
    Document(JSON&, Serialization&);
    virtual ~Document() = default;
};
/* Separation of Concerns */

using namespace std;

int main() { return 0; }
