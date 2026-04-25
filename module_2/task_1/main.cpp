#include <iostream>
#include <string>
#include <vector>

const size_t DEFAULT_SIZE = 8;

class StringHasher1 {
public:
    StringHasher1(size_t prime = 71) : prime(prime) {}

    size_t operator()(const std::string& str) const {
        size_t hash = 0;
        for (size_t i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }
        return hash;
    }

private:
    size_t prime;
};

class StringHasher2 {
public:
    StringHasher2(size_t prime = 137) : prime(prime) {}

    size_t operator()(const std::string& str) const {
        size_t hash = 0;
        for (size_t i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }
        return (hash * 2 + 1);
    }

private:
    size_t prime;
};

template<typename T>
struct HashtableCell {
    HashtableCell() : state(EMPTY) {}

    enum CellState { EMPTY, OCCUPIED, DELETED };

    T data;
    CellState state;
};

template<typename T, typename Hasher1, typename Hasher2>
class Hashtable {
public:
    Hashtable(size_t initial_size = DEFAULT_SIZE) : table(initial_size), size(0) {}

    ~Hashtable() {}

    bool Add(const T& key) {
        if (size * 4 >= table.size() * 3) {
            grow();
        }

        size_t h1 = hasher1(key) % table.size();
        size_t h2 = hasher2(key) % table.size();
        size_t firstDeleted = table.size();

        for (size_t i = 0; i < table.size(); i++) {
            size_t idx = (h1 + i * h2) % table.size();

            if (table[idx].state == HashtableCell<T>::EMPTY) {
                size_t insertIdx = (firstDeleted != table.size()) ? firstDeleted : idx;
                table[insertIdx].data = key;
                table[insertIdx].state = HashtableCell<T>::OCCUPIED;
                size++;
                return true;
            }

            if (table[idx].state == HashtableCell<T>::DELETED) {
                if (firstDeleted == table.size()) {
                    firstDeleted = idx;
                }
            } else if (table[idx].data == key) {
                return false;
            }
        }

        if (firstDeleted != table.size()) {
            table[firstDeleted].data = key;
            table[firstDeleted].state = HashtableCell<T>::OCCUPIED;
            size++;
            return true;
        }

        return false;
    }

    bool Has(const T& key) const {
        size_t h1 = hasher1(key) % table.size();
        size_t h2 = hasher2(key) % table.size();

        for (size_t i = 0; i < table.size(); i++) {
            size_t idx = (h1 + i * h2) % table.size();

            if (table[idx].state == HashtableCell<T>::EMPTY) {
                return false;
            }

            if (table[idx].state == HashtableCell<T>::OCCUPIED && table[idx].data == key) {
                return true;
            }
        }

        return false;
    }

    bool Delete(const T& key) {
        size_t h1 = hasher1(key) % table.size();
        size_t h2 = hasher2(key) % table.size();

        for (size_t i = 0; i < table.size(); i++) {
            size_t idx = (h1 + i * h2) % table.size();

            if (table[idx].state == HashtableCell<T>::EMPTY) {
                return false;
            }

            if (table[idx].state == HashtableCell<T>::OCCUPIED && table[idx].data == key) {
                table[idx].state = HashtableCell<T>::DELETED;
                size--;
                return true;
            }
        }

        return false;
    }

private:
    void grow() {
        std::vector<HashtableCell<T>> newTable(table.size() * 2);

        for (size_t i = 0; i < table.size(); i++) {
            if (table[i].state != HashtableCell<T>::OCCUPIED) {
                continue;
            }

            size_t h1 = hasher1(table[i].data) % newTable.size();
            size_t h2 = hasher2(table[i].data) % newTable.size();

            for (size_t j = 0; j < newTable.size(); j++) {
                size_t idx = (h1 + j * h2) % newTable.size();

                if (newTable[idx].state == HashtableCell<T>::EMPTY) {
                    newTable[idx].data = table[i].data;
                    newTable[idx].state = HashtableCell<T>::OCCUPIED;
                    break;
                }
            }
        }

        std::swap(table, newTable);
    }

    std::vector<HashtableCell<T>> table;
    size_t size;
    Hasher1 hasher1;
    Hasher2 hasher2;
};

int main() {
    Hashtable<std::string, StringHasher1, StringHasher2> table;

    char op;
    std::string key;

    while (std::cin >> op >> key) {
        switch (op) {
            case '?': {
                std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '+': {
                std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-': {
                std::cout << (table.Delete(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
    }

    return 0;
}