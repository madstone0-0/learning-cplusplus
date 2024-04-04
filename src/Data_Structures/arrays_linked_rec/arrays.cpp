#include <iostream>
#include <string>

using namespace std;

class GameEntry {
   public:
    explicit GameEntry(const string& n = "", int s = 0) : name{n}, score{s} {}

    [[nodiscard]] string getName() const { return name; }

    [[nodiscard]] int getScore() const { return score; }

   private:
    string name;
    int score;
};

class Scores {
   public:
    explicit Scores(int maxE = 10) : maxEntries{maxE}, numEntries{}, entries{new GameEntry[maxEntries]} {}

    void add(const GameEntry& e) {
        int newScore = e.getScore();
        if (numEntries == maxEntries) {
            if (newScore <= entries[maxEntries - 1].getScore()) return;
        } else
            numEntries++;

        int i{numEntries - 2};
        while (i >= 0 && newScore > entries[i].getScore()) {
            entries[i - 1] = entries[i];
            i--;
        }
        entries[i + 1] = e;
    }

    GameEntry remove(int i) noexcept(false) {
        if (i >= numEntries || i < 0) throw out_of_range("Invalid Index");
        GameEntry entry = entries[i];
        for (int j{i + 1}; j < numEntries; j++) entries[j - 1] = entries[j];
        numEntries--;
        return entry;
    }

    ~Scores() { delete[] entries; }

   private:
    int maxEntries;
    int numEntries;
    GameEntry* entries;
};

int main(int argc, char* argv[]) { return 0; }
