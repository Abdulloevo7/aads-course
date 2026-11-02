#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>
using namespace std;

template <typename T>
class Vector {
private:
    T* data;
    int sz;
    int cap;

public:
    Vector() : data(nullptr), sz(0), cap(0) {}

    explicit Vector(int n) : data(nullptr), sz(0), cap(0) {
        if (n < 0) {
            throw invalid_argument("Размер не может быть отрицательным");
        }
        data = new T[n];
        sz = n;
        cap = n;
        for (int i = 0; i < sz; i++) {
            data[i] = T();
        }
    }

    Vector(const Vector& other) : data(nullptr), sz(other.sz), cap(other.sz) {
        data = new T[cap];
        for (int i = 0; i < sz; i++) {
            data[i] = other.data[i];
        }
    }

    Vector(Vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    ~Vector() {
        delete[] data;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    T& operator[](int index) {
        if (index < 0 || index >= sz) {
            throw out_of_range("Индекс вне границ вектора");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= sz) {
            throw out_of_range("Индекс вне границ вектора");
        }
        return data[index];
    }

    int size() const { return sz; }
    int capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    void reserve(int newCap) {
        if (newCap <= cap) {
            return;
        }
        T* newData = new T[newCap];
        for (int i = 0; i < sz; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    void shrink_to_fit() {
        if (cap == sz) {
            return;
        }
        if (sz == 0) {
            delete[] data;
            data = nullptr;
            cap = 0;
            return;
        }
        T* newData = new T[sz];
        for (int i = 0; i < sz; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = sz;
    }

    void push_back(const T& value) {
        if (sz >= cap) {
            reserve(cap == 0 ? 1 : cap * 2);
        }
        data[sz++] = value;
    }

    void pop_back() {
        if (empty()) {
            throw underflow_error("Вектор пуст");
        }
        sz--;
    }

    void swap(Vector& other) noexcept {
        T* tmpData = data;
        data = other.data;
        other.data = tmpData;
        int tmp = sz;
        sz = other.sz;
        other.sz = tmp;
        tmp = cap;
        cap = other.cap;
        other.cap = tmp;
    }

    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }
};

template <typename T>
void show(const string& label, const Vector<T>& v) {
    cout << label << ": [";
    bool first = true;
    for (const T& x : v) {
        if (!first) {
            cout << ", ";
        }
        cout << x;
        first = false;
    }
    cout << "], размер=" << v.size() << ", ёмкость=" << v.capacity() << '\n';
}

int main() {
    setlocale(LC_ALL, "");

    cout << "=== 1. Конструкторы и push_back ===\n";
    Vector<int> v;
    cout << "Пустой вектор: размер=" << v.size() << ", ёмкость=" << v.capacity() << '\n';
    for (int i = 1; i <= 10; i++) {
        v.push_back(i * 10);
        cout << "push_back(" << i * 10 << "): размер=" << v.size()
             << ", ёмкость=" << v.capacity() << '\n';
    }
    Vector<int> zeros(3);
    show("Vector(3)", zeros);

    cout << "\n=== 2. pop_back, reserve, shrink_to_fit ===\n";
    v.pop_back();
    v.pop_back();
    show("После двух pop_back", v);
    v.reserve(32);
    show("После reserve(32)", v);
    v.shrink_to_fit();
    show("После shrink_to_fit", v);

    cout << "\n=== 3. Доступ по индексу и итераторы ===\n";
    cout << "v[0] = " << v[0] << ", v[7] = " << v[7] << '\n';
    v[1] = 25;
    cout << "Range-based for:";
    for (int x : v) {
        cout << ' ' << x;
    }
    cout << "\nЧерез итераторы:";
    for (int* it = v.begin(); it != v.end(); ++it) {
        cout << ' ' << *it;
    }
    cout << '\n';

    cout << "\n=== 4. Правило пяти ===\n";
    Vector<int> copy(v);
    copy[0] = 999;
    show("Оригинал после правки копии", v);
    show("Копия (конструктор копирования)", copy);

    Vector<int> assigned;
    assigned.push_back(1);
    assigned = v;
    assigned[0] = -1;
    show("Присваивание копированием", assigned);
    show("Оригинал не изменился", v);

    Vector<int>& ref = assigned;
    assigned = ref;
    show("Самоприсваивание", assigned);

    Vector<int> moved(move(copy));
    show("Новый после перемещения", moved);
    show("Источник после перемещения", copy);

    Vector<int> target;
    target = move(moved);
    show("Присваивание перемещением", target);
    show("Источник после присваивания", moved);

    cout << "\n=== 5. Вектор строк ===\n";
    Vector<string> names;
    names.push_back("Ставрополь");
    names.push_back("Москва");
    names.push_back("Казань");
    show("Города", names);

    cout << "\n=== 6. Обработка ошибок ===\n";
    try {
        cout << v[100] << '\n';
    } catch (const out_of_range& e) {
        cout << "out_of_range: " << e.what() << '\n';
    }
    try {
        Vector<int> empty;
        empty.pop_back();
    } catch (const underflow_error& e) {
        cout << "underflow_error: " << e.what() << '\n';
    }
    try {
        Vector<int> bad(-5);
    } catch (const invalid_argument& e) {
        cout << "invalid_argument: " << e.what() << '\n';
    }

    cout << "\n=== 7. Амортизированный анализ push_back ===\n";
    cout << "n\tКопирований\tВсего операций\tСреднее\n";
    cout << string(50, '-') << '\n';
    for (int n = 1000; n <= 1000000; n *= 10) {
        Vector<int> a;
        long long copies = 0;
        for (int i = 0; i < n; i++) {
            int oldCap = a.capacity();
            int oldSize = a.size();
            a.push_back(i);
            if (a.capacity() != oldCap) {
                copies += oldSize;
            }
        }
        long long total = n + copies;
        cout << n << '\t' << copies << "\t\t" << total << "\t\t"
             << fixed << setprecision(2) << (double)total / n << '\n';
    }
    return 0;
}
