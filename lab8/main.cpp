#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <stdexcept>
using namespace std;
using namespace chrono;

unsigned int randomState = 12345;

unsigned int nextRandom() {
    randomState = randomState * 1664525u + 1013904223u;
    return randomState >> 8;
}

// Двоичный поиск
int binarySearch(const int arr[], int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int binarySearchRecursive(const int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;
    }
    int mid = left + (right - left) / 2;
    if (arr[mid] == target) {
        return mid;
    }
    if (arr[mid] < target) {
        return binarySearchRecursive(arr, mid + 1, right, target);
    }
    return binarySearchRecursive(arr, left, mid - 1, target);
}

int linearSearch(const int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// Хеш-функции
int hashSimple(int key, int size) {
    return ((key % size) + size) % size;
}

int hashMixed(int key, int size) {
    unsigned long long h = (unsigned int)key;
    h = h * 2654435761ULL;
    h ^= h >> 15;
    return (int)(h % (unsigned int)size);
}

bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    for (int d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

int nextPrime(int n) {
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

// Хеш-таблица с методом цепочек, ключи и значения int
class HashTableChaining {
private:
    struct Node {
        int key;
        int value;
        Node* next;

        Node(int k, int v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    int size;
    int count;
    bool simpleHash;
    bool autoRehash;

    int hash(int key) const {
        return simpleHash ? hashSimple(key, size) : hashMixed(key, size);
    }

    void rehash() {
        int newSize = nextPrime(size * 2);
        Node** newTable = new Node*[newSize];
        for (int i = 0; i < newSize; i++) {
            newTable[i] = nullptr;
        }
        int oldSize = size;
        size = newSize;
        for (int i = 0; i < oldSize; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* next = current->next;
                int index = hash(current->key);
                current->next = newTable[index];
                newTable[index] = current;
                current = next;
            }
        }
        delete[] table;
        table = newTable;
    }

public:
    HashTableChaining(int s = 11, bool simple = false, bool rehashing = true)
        : table(nullptr), size(s), count(0), simpleHash(simple), autoRehash(rehashing) {
        if (size < 1) {
            throw invalid_argument("Размер таблицы должен быть положительным");
        }
        table = new Node*[size];
        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }

    HashTableChaining(const HashTableChaining&) = delete;
    HashTableChaining& operator=(const HashTableChaining&) = delete;

    ~HashTableChaining() {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    bool insert(int key, int value) {
        int index = hash(key);
        for (Node* current = table[index]; current != nullptr; current = current->next) {
            if (current->key == key) {
                current->value = value;
                return false;
            }
        }
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        count++;
        if (autoRehash && count > size * 3 / 4) {
            rehash();
        }
        return true;
    }

    bool find(int key, int& result) const {
        for (Node* current = table[hash(key)]; current != nullptr; current = current->next) {
            if (current->key == key) {
                result = current->value;
                return true;
            }
        }
        return false;
    }

    bool remove(int key) {
        int index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                count--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    int getCount() const { return count; }
    int bucketCount() const { return size; }
    double loadFactor() const { return (double)count / size; }

    int maxChainLength() const {
        int best = 0;
        for (int i = 0; i < size; i++) {
            int len = 0;
            for (Node* current = table[i]; current != nullptr; current = current->next) {
                len++;
            }
            if (len > best) {
                best = len;
            }
        }
        return best;
    }

    void print() const {
        for (int i = 0; i < size; i++) {
            cout << i << ": ";
            for (Node* current = table[i]; current != nullptr; current = current->next) {
                cout << "(" << current->key << ", " << current->value << ") ";
            }
            cout << '\n';
        }
    }
};

// Хеш-таблица с открытой адресацией и линейным пробированием
class HashTableOpenAddressing {
private:
    enum State { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        int key;
        int value;
        State state;

        Entry() : key(0), value(0), state(EMPTY) {}
    };

    Entry* table;
    int size;
    int count;
    int used;

    int hash(int key) const {
        return hashMixed(key, size);
    }

    void rehash() {
        int oldSize = size;
        Entry* oldTable = table;
        size = nextPrime(count * 4 + 11);
        table = new Entry[size];
        count = 0;
        used = 0;
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].state == OCCUPIED) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        delete[] oldTable;
    }

public:
    HashTableOpenAddressing(int s = 11) : table(nullptr), size(s), count(0), used(0) {
        if (size < 1) {
            throw invalid_argument("Размер таблицы должен быть положительным");
        }
        table = new Entry[size];
    }

    HashTableOpenAddressing(const HashTableOpenAddressing&) = delete;
    HashTableOpenAddressing& operator=(const HashTableOpenAddressing&) = delete;

    ~HashTableOpenAddressing() {
        delete[] table;
    }

    bool insert(int key, int value) {
        if ((used + 1) * 2 > size) {
            rehash();
        }
        int index = hash(key);
        int firstDeleted = -1;
        int pos = index;
        for (int i = 0; i < size; i++) {
            pos = (index + i) % size;
            if (table[pos].state == EMPTY) {
                break;
            }
            if (table[pos].state == DELETED) {
                if (firstDeleted < 0) {
                    firstDeleted = pos;
                }
            } else if (table[pos].key == key) {
                table[pos].value = value;
                return false;
            }
        }
        int target = firstDeleted >= 0 ? firstDeleted : pos;
        if (table[target].state == EMPTY) {
            used++;
        }
        table[target].key = key;
        table[target].value = value;
        table[target].state = OCCUPIED;
        count++;
        return true;
    }

    bool find(int key, int& result) const {
        int index = hash(key);
        for (int i = 0; i < size; i++) {
            int pos = (index + i) % size;
            if (table[pos].state == EMPTY) {
                return false;
            }
            if (table[pos].state == OCCUPIED && table[pos].key == key) {
                result = table[pos].value;
                return true;
            }
        }
        return false;
    }

    bool remove(int key) {
        int index = hash(key);
        for (int i = 0; i < size; i++) {
            int pos = (index + i) % size;
            if (table[pos].state == EMPTY) {
                return false;
            }
            if (table[pos].state == OCCUPIED && table[pos].key == key) {
                table[pos].state = DELETED;
                count--;
                return true;
            }
        }
        return false;
    }

    int getCount() const { return count; }
    int bucketCount() const { return size; }
    double loadFactor() const { return (double)count / size; }
};

volatile long long sink;

template <typename Func>
double measureMicro(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration<double, micro>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "");

    cout << "=== 1. Двоичный поиск ===\n";
    int sample[] = {1, 3, 5, 7, 9, 11, 13};
    cout << "Массив: [1, 3, 5, 7, 9, 11, 13]\n";
    for (int target : {7, 1, 13, 4, 0, 14}) {
        int it = binarySearch(sample, 7, target);
        int rec = binarySearchRecursive(sample, 0, 6, target);
        cout << "Поиск " << target << ": итеративно ";
        if (it >= 0) cout << "индекс " << it; else cout << "не найден";
        cout << ", рекурсивно ";
        if (rec >= 0) cout << "индекс " << rec; else cout << "не найден";
        cout << '\n';
    }
    int one[] = {42};
    cout << "Один элемент, поиск 42: " << binarySearch(one, 1, 42) << ", поиск 41: " << binarySearch(one, 1, 41) << '\n';
    cout << "Пустой массив, поиск 1: " << binarySearch(sample, 0, 1) << '\n';
    int dup[] = {2, 4, 4, 4, 8};
    int di = binarySearch(dup, 5, 4);
    cout << "Дубликаты, поиск 4: индекс " << di << (dup[di] == 4 ? " содержит 4" : " ОШИБКА") << '\n';

    {
        const int n = 5000;
        int* arr = new int[n];
        for (int i = 0; i < n; i++) {
            arr[i] = i * 2;
        }
        bool ok = true;
        for (int v = -3; v <= 2 * n + 3; v++) {
            int expected = linearSearch(arr, n, v);
            if (binarySearch(arr, n, v) != expected || binarySearchRecursive(arr, 0, n - 1, v) != expected) {
                ok = false;
            }
        }
        cout << "Сверка с линейным поиском на " << 2 * n + 7 << " значениях: " << (ok ? "совпадает" : "ОШИБКА") << '\n';
        delete[] arr;
    }

    cout << "\n=== 2. Хеш-таблица, метод цепочек ===\n";
    {
        HashTableChaining ht(5, true, false);
        ht.insert(10, 100);
        ht.insert(20, 200);
        ht.insert(30, 300);
        ht.insert(15, 150);
        ht.insert(25, 250);
        ht.print();
        int value = 0;
        cout << "find(20): " << (ht.find(20, value) ? "найден, значение " : "не найден") << (value ? to_string(value) : "") << '\n';
        cout << "find(99): " << (ht.find(99, value) ? "найден" : "не найден") << '\n';
        ht.insert(20, 222);
        ht.find(20, value);
        cout << "insert(20, 222) обновляет значение: " << value << ", элементов: " << ht.getCount() << '\n';
        cout << "remove(20): " << (ht.remove(20) ? "удалён" : "нет ключа") << '\n';
        cout << "remove(20) повторно: " << (ht.remove(20) ? "удалён" : "нет ключа") << '\n';
        cout << "remove(99): " << (ht.remove(99) ? "удалён" : "нет ключа") << '\n';
        cout << "После удаления:\n";
        ht.print();
        ht.insert(-7, 1);
        cout << "Отрицательный ключ -7: " << (ht.find(-7, value) ? "найден" : "не найден") << '\n';
    }

    cout << "\n=== 3. Рехеширование и проверка на большом наборе ===\n";
    {
        HashTableChaining ht(5);
        HashTableOpenAddressing oa(5);
        const int n = 20000;
        for (int i = 0; i < n; i++) {
            ht.insert(i * 37, i);
            oa.insert(i * 37, i);
        }
        bool okChain = true, okOpen = true;
        int v = 0;
        for (int i = 0; i < n; i++) {
            if (!ht.find(i * 37, v) || v != i) okChain = false;
            if (!oa.find(i * 37, v) || v != i) okOpen = false;
        }
        cout << "Вставлено " << n << " ключей. Цепочки: элементов " << ht.getCount() << ", корзин " << ht.bucketCount()
             << ", загрузка " << fixed << setprecision(2) << ht.loadFactor() << ", самая длинная цепочка " << ht.maxChainLength()
             << ", все найдены: " << (okChain ? "да" : "нет") << '\n';
        cout << "Открытая адресация: элементов " << oa.getCount() << ", ячеек " << oa.bucketCount()
             << ", загрузка " << oa.loadFactor() << ", все найдены: " << (okOpen ? "да" : "нет") << '\n';
        for (int i = 0; i < n; i += 2) {
            ht.remove(i * 37);
            oa.remove(i * 37);
        }
        okChain = okOpen = true;
        for (int i = 0; i < n; i++) {
            bool expected = (i % 2 == 1);
            if (ht.find(i * 37, v) != expected) okChain = false;
            if (oa.find(i * 37, v) != expected) okOpen = false;
        }
        cout << "После удаления половины ключей: цепочки " << ht.getCount() << " " << (okChain ? "верно" : "ОШИБКА")
             << ", открытая адресация " << oa.getCount() << " " << (okOpen ? "верно" : "ОШИБКА") << '\n';
        for (int i = 0; i < n; i += 2) {
            oa.insert(i * 37, -i);
        }
        okOpen = oa.find(0, v) && v == 0 && oa.find(2 * 37, v) && v == -2 && oa.getCount() == n;
        cout << "Повторная вставка в освободившиеся ячейки: " << (okOpen ? "верно" : "ОШИБКА") << '\n';
    }

    cout << "\n=== 4. Качество хеш-функции, 500 ключей кратны 1000, 1000 корзин ===\n";
    {
        HashTableChaining simple(1000, true, false);
        HashTableChaining mixed(1000, false, false);
        for (int i = 0; i < 500; i++) {
            simple.insert(i * 1000, i);
            mixed.insert(i * 1000, i);
        }
        cout << "Хеш key % size: самая длинная цепочка " << simple.maxChainLength() << '\n';
        cout << "Перемешивающая хеш-функция: самая длинная цепочка " << mixed.maxChainLength() << '\n';
    }

    cout << "\n=== 5. Сравнение поиска, 200000 запросов, мкс ===\n";
    cout << "Успешный поиск\nN\tДвоичный\tЦепочки\tОткр. адр.\n";
    const int Q = 200000;
    int* hitQueries = new int[Q];
    int* missQueries = new int[Q];
    double missRows[3][3];
    int rowIndex = 0;
    for (int n : {1000, 10000, 100000}) {
        int* arr = new int[n];
        HashTableChaining ht(11);
        HashTableOpenAddressing oa(11);
        for (int i = 0; i < n; i++) {
            arr[i] = i * 2;
            ht.insert(i * 2, i);
            oa.insert(i * 2, i);
        }
        for (int q = 0; q < Q; q++) {
            hitQueries[q] = 2 * (int)(nextRandom() % n);
            missQueries[q] = 2 * (int)(nextRandom() % n) + 1;
        }
        double tb = measureMicro([&]() {
            long long s = 0;
            for (int q = 0; q < Q; q++) s += binarySearch(arr, n, hitQueries[q]);
            sink = s;
        });
        double tc = measureMicro([&]() {
            long long s = 0;
            int v;
            for (int q = 0; q < Q; q++) s += ht.find(hitQueries[q], v) ? v : 0;
            sink = s;
        });
        double to = measureMicro([&]() {
            long long s = 0;
            int v;
            for (int q = 0; q < Q; q++) s += oa.find(hitQueries[q], v) ? v : 0;
            sink = s;
        });
        cout << n << '\t' << fixed << setprecision(0) << tb << "\t\t" << tc << "\t\t" << to << '\n';
        missRows[rowIndex][0] = measureMicro([&]() {
            long long s = 0;
            for (int q = 0; q < Q; q++) s += binarySearch(arr, n, missQueries[q]);
            sink = s;
        });
        missRows[rowIndex][1] = measureMicro([&]() {
            long long s = 0;
            int v;
            for (int q = 0; q < Q; q++) s += ht.find(missQueries[q], v) ? v : 0;
            sink = s;
        });
        missRows[rowIndex][2] = measureMicro([&]() {
            long long s = 0;
            int v;
            for (int q = 0; q < Q; q++) s += oa.find(missQueries[q], v) ? v : 0;
            sink = s;
        });
        rowIndex++;
        delete[] arr;
    }
    cout << "\nНеуспешный поиск\nN\tДвоичный\tЦепочки\tОткр. адр.\n";
    int sizes[] = {1000, 10000, 100000};
    for (int r = 0; r < 3; r++) {
        cout << sizes[r] << '\t' << fixed << setprecision(0) << missRows[r][0] << "\t\t" << missRows[r][1]
             << "\t\t" << missRows[r][2] << '\n';
    }
    delete[] hitQueries;
    delete[] missQueries;
    return 0;
}
