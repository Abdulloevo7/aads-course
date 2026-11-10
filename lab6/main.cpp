#include <iostream>
#include <stdexcept>
#include <chrono>
#include <cstdlib>
#include <utility>
using namespace std;
using namespace chrono;

// Рекурсивные алгоритмы
long long factorial(int n) {
    if (n < 0 || n > 20) {
        throw out_of_range("Факториал определён для n от 0 до 20");
    }
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

long long factorialIter(int n) {
    if (n < 0 || n > 20) {
        throw out_of_range("Факториал определён для n от 0 до 20");
    }
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long fibRecursive(int n) {
    if (n < 0) {
        throw invalid_argument("n не может быть отрицательным");
    }
    if (n <= 1) {
        return n;
    }
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

long long fibCounted(int n, long long& calls) {
    calls++;
    if (n <= 1) {
        return n;
    }
    return fibCounted(n - 1, calls) + fibCounted(n - 2, calls);
}

long long fibIterative(int n) {
    if (n < 0) {
        throw invalid_argument("n не может быть отрицательным");
    }
    if (n <= 1) {
        return n;
    }
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int digitSum(int n) {
    if (n < 0) {
        n = -n;
    }
    if (n < 10) {
        return n;
    }
    return n % 10 + digitSum(n / 10);
}

int digitSumIter(int n) {
    if (n < 0) {
        n = -n;
    }
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

long long power(long long a, int n) {
    if (n < 0) {
        throw invalid_argument("Показатель степени должен быть неотрицательным");
    }
    if (n == 0) {
        return 1;
    }
    return a * power(a, n - 1);
}

long long powerFast(long long a, int n) {
    if (n < 0) {
        throw invalid_argument("Показатель степени должен быть неотрицательным");
    }
    if (n == 0) {
        return 1;
    }
    long long half = powerFast(a, n / 2);
    if (n % 2 == 0) {
        return half * half;
    }
    return half * half * a;
}

long long powerIter(long long a, int n) {
    if (n < 0) {
        throw invalid_argument("Показатель степени должен быть неотрицательным");
    }
    long long result = 1;
    for (int i = 0; i < n; i++) {
        result *= a;
    }
    return result;
}

// Простые сортировки
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
        }
    }
}

bool isSorted(const int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

long long measureSort(void (*sortFunc)(int[], int), const int arr[], int n, bool& ok) {
    int* copy = new int[n];
    for (int i = 0; i < n; i++) {
        copy[i] = arr[i];
    }
    auto start = high_resolution_clock::now();
    sortFunc(copy, n);
    auto end = high_resolution_clock::now();
    ok = isSorted(copy, n);
    delete[] copy;
    return duration_cast<microseconds>(end - start).count();
}

template <typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

void fillRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }
}

void fillSorted(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

void fillReversed(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void fillAlmostSorted(int arr[], int n) {
    fillSorted(arr, n);
    for (int k = 0; k < n / 100; k++) {
        int i = rand() % n;
        int j = rand() % n;
        swap(arr[i], arr[j]);
    }
}

int main() {
    setlocale(LC_ALL, "");
    srand(42);

    cout << "=== 1. Факториал ===\n";
    for (int n : {0, 1, 5, 10, 20}) {
        cout << n << "! = " << factorial(n) << ", итеративно: " << factorialIter(n) << '\n';
    }
    try {
        factorial(21);
    } catch (const out_of_range& e) {
        cout << "factorial(21): " << e.what() << '\n';
    }
    try {
        factorial(-1);
    } catch (const out_of_range& e) {
        cout << "factorial(-1): " << e.what() << '\n';
    }

    cout << "\n=== 2. Числа Фибоначчи ===\n";
    for (int n : {0, 1, 2, 10, 20, 30}) {
        cout << "F(" << n << ") = " << fibRecursive(n) << ", итеративно: " << fibIterative(n) << '\n';
    }
    cout << "\nЧисло рекурсивных вызовов:\n";
    cout << "n\tВызовов\n";
    for (int n : {5, 10, 20, 30}) {
        long long calls = 0;
        fibCounted(n, calls);
        cout << n << '\t' << calls << '\n';
    }
    cout << "\nВремя вычисления, мкс:\n";
    cout << "n\tРекурсивно\tИтеративно\n";
    for (int n : {20, 30, 35, 40}) {
        long long tr = measureTime([&]() { volatile long long r = fibRecursive(n); (void)r; });
        long long ti = measureTime([&]() { volatile long long r = fibIterative(n); (void)r; });
        cout << n << '\t' << tr << "\t\t" << ti << '\n';
    }

    cout << "\n=== 3. Сумма цифр числа ===\n";
    for (int n : {0, 7, 123, 98765, 1000000, -456, 2147483647}) {
        cout << "digitSum(" << n << ") = " << digitSum(n) << ", итеративно: " << digitSumIter(n) << '\n';
    }

    cout << "\n=== 4. Степень числа ===\n";
    int bases[] = {2, 3, 7, 5, -2, 2};
    int exps[] = {10, 5, 0, 1, 3, 40};
    for (int i = 0; i < 6; i++) {
        cout << (bases[i] < 0 ? "(" : "") << bases[i] << (bases[i] < 0 ? ")" : "") << "^" << exps[i] << " = " << power(bases[i], exps[i])
             << ", быстрое: " << powerFast(bases[i], exps[i])
             << ", итеративно: " << powerIter(bases[i], exps[i]) << '\n';
    }
    try {
        power(2, -1);
    } catch (const invalid_argument& e) {
        cout << "power(2, -1): " << e.what() << '\n';
    }

    cout << "\n=== 5. Проверка совпадения рекурсивных и итеративных версий ===\n";
    bool allEqual = true;
    for (int n = 0; n <= 20; n++) {
        if (factorial(n) != factorialIter(n)) allEqual = false;
    }
    for (int n = 0; n <= 35; n++) {
        if (fibRecursive(n) != fibIterative(n)) allEqual = false;
    }
    for (int n = 0; n < 100000; n += 7) {
        if (digitSum(n) != digitSumIter(n)) allEqual = false;
    }
    for (int a = -5; a <= 5; a++) {
        for (int n = 0; n <= 20; n++) {
            if (power(a, n) != powerIter(a, n) || powerFast(a, n) != powerIter(a, n)) allEqual = false;
        }
    }
    cout << (allEqual ? "Все версии дают одинаковые результаты" : "Обнаружено расхождение") << '\n';

    cout << "\n=== 6. Сравнение сортировок на случайном массиве, мкс ===\n";
    cout << "n\tПузырьковая\tВставками\tВыбором\n";
    for (int n : {1000, 5000, 10000, 20000}) {
        int* arr = new int[n];
        fillRandom(arr, n);
        bool ok1, ok2, ok3;
        long long t1 = measureSort(bubbleSort, arr, n, ok1);
        long long t2 = measureSort(insertionSort, arr, n, ok2);
        long long t3 = measureSort(selectionSort, arr, n, ok3);
        cout << n << '\t' << t1 << "\t\t" << t2 << "\t\t" << t3
             << ((ok1 && ok2 && ok3) ? "" : "  ОШИБКА СОРТИРОВКИ") << '\n';
        delete[] arr;
    }

    cout << "\n=== 7. Влияние порядка данных, n = 10000, мкс ===\n";
    cout << "Данные\t\t\tПузырьковая\tВставками\tВыбором\n";
    {
        const int n = 10000;
        int* arr = new int[n];
        const char* names[] = {"Случайные", "Отсортированные", "Обратный порядок", "Почти отсортированные"};
        for (int k = 0; k < 4; k++) {
            if (k == 0) fillRandom(arr, n);
            if (k == 1) fillSorted(arr, n);
            if (k == 2) fillReversed(arr, n);
            if (k == 3) fillAlmostSorted(arr, n);
            bool ok1, ok2, ok3;
            long long t1 = measureSort(bubbleSort, arr, n, ok1);
            long long t2 = measureSort(insertionSort, arr, n, ok2);
            long long t3 = measureSort(selectionSort, arr, n, ok3);
            cout << names[k] << "\t" << t1 << "\t\t" << t2 << "\t\t" << t3
                 << ((ok1 && ok2 && ok3) ? "" : "  ОШИБКА СОРТИРОВКИ") << '\n';
        }
        delete[] arr;
    }
    return 0;
}
