#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <utility>
using namespace std;
using namespace chrono;

namespace lab {
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}
}

template <typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;

public:
    Pair(const T1& f, const T2& s) : first(f), second(s) {}       
    T1 getFirst() const { return first; }                         
    T2 getSecond() const { return second; }                       
    void setFirst(const T1& f) { first = f; }                     
    void setSecond(const T2& s) { second = s; }                   
};

int exampleA(const vector<int>& a) {            
    int sum = 0;
    for (size_t i = 0; i < a.size(); i++) sum += a[i];
    return sum;
}
int exampleB(const vector<int>& a) {            
    int count = 0;
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < a.size(); j++)
            if (a[i] == a[j]) count++;
    return count;
}
int exampleC(int n) {                           
    int count = 0;
    for (int i = 1; i < n; i *= 2) count++;
    return count;
}
int exampleD(int n) {                           
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = n; j > 0; j /= 2) count++;
    return count;
}
int exampleE(int n) {                         
    int count = 0;
    for (int i = 0; i * i < n; i++) count++;
    return count;
}

template <typename T>
int linearSearch(const vector<T>& arr, const T& value) {         
    for (size_t i = 0; i < arr.size(); i++)
        if (arr[i] == value) return (int)i;
    return -1;
}
template <typename T>
int binarySearch(const vector<T>& arr, const T& value) {        
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == value) return mid;
        if (arr[mid] < value) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

template <typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

volatile long long sink; 

int main() {
    setlocale(LC_ALL, "");

    cout << "=== 1. Шаблонная функция max ===\n";
    cout << "max<int>(5, 10) = " << lab::max(5, 10) << '\n';
    cout << "max<double>(3.14, 2.71) = " << lab::max(3.14, 2.71) << '\n';
    cout << "max<string>(\"Hello\", \"World\") = "
         << lab::max(string("Hello"), string("World")) << '\n';
    cout << "max<char>('a', 'z') = " << lab::max('a', 'z') << '\n';

    cout << "\n=== 2. Шаблонный класс Pair ===\n";
    Pair<string, int> p1("Возраст", 19);
    cout << p1.getFirst() << " = " << p1.getSecond() << '\n';
    p1.setFirst("Курс");
    p1.setSecond(1);
    cout << p1.getFirst() << " = " << p1.getSecond() << '\n';
    Pair<double, char> p2(3.14, 'x');
    cout << p2.getFirst() << ", " << p2.getSecond() << '\n';

    cout << "\n=== 3. Проверка оценок сложности (число операций) ===\n";
    cout << "n\tA O(n)\tC O(log n)\tD O(n log n)\tE O(sqrt n)\n";
    for (int n : {1000, 2000, 4000, 8000}) {
        cout << n << '\t' << n << '\t' << exampleC(n) << "\t\t"
             << exampleD(n) << "\t\t" << exampleE(n) << '\n';
    }
    {
        cout << "\nB O(n^2): время выполнения (мкс)\n";
        for (int n : {2000, 4000, 8000}) {
            vector<int> a(n);
            for (int i = 0; i < n; i++) a[i] = i;
            long long t = measureTime([&]() { sink = exampleB(a); });
            cout << "n = " << n << ": " << t << " мкс\n";
        }
    }

    cout << "\n=== 4. Сравнение: Pair и std::pair (100 проходов по 10^6 элементов) ===\n";
    {
        const int N = 1000000, PASSES = 100;
        vector<Pair<int, int>> mine;
        vector<pair<int, int>> stdp;
        for (int i = 0; i < N; i++) {
            mine.push_back(Pair<int, int>(i, i + 1));
            stdp.push_back(pair<int, int>(i, i + 1));
        }
        long long t1 = measureTime([&]() {
            long long s = 0;
            for (int k = 0; k < PASSES; k++)
                for (int i = 0; i < N; i++) {
                    mine[i].setFirst(mine[i].getFirst() ^ k);
                    s += mine[i].getFirst() + mine[i].getSecond();
                }
            sink = s;
        });
        long long t2 = measureTime([&]() {
            long long s = 0;
            for (int k = 0; k < PASSES; k++)
                for (int i = 0; i < N; i++) {
                    stdp[i].first ^= k;
                    s += stdp[i].first + stdp[i].second;
                }
            sink = s;
        });
        cout << "Pair<int,int>:      " << t1 << " мкс\n";
        cout << "std::pair<int,int>: " << t2 << " мкс\n";
    }

    cout << "\n=== 5. Сравнение: линейный и бинарный поиск ===\n";
    cout << "Размер массива\tЛинейный (мкс)\tБинарный (мкс)\n";
    for (int n : {10000, 100000, 500000, 1000000}) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = i;
        int target = n - 1; // худший случай: последний элемент
        long long lt = measureTime([&]() {
            for (int k = 0; k < 1000; k++) sink = linearSearch(arr, target);
        });
        long long bt = measureTime([&]() {
            for (int k = 0; k < 1000; k++) sink = binarySearch(arr, target);
        });
        cout << n << "\t\t" << lt / 1000.0 << "\t\t" << bt / 1000.0 << '\n';
    }
    return 0;
}
