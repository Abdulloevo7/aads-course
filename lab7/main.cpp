#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <utility>
#include <string>
using namespace std;
using namespace chrono;

// Быстрая сортировка с тремя стратегиями выбора опорного элемента
enum PivotStrategy { LAST, MEDIAN, RANDOM };

void choosePivot(int arr[], int low, int high, PivotStrategy strategy) {
    if (strategy == RANDOM) {
        int index = low + rand() % (high - low + 1);
        swap(arr[index], arr[high]);
    } else if (strategy == MEDIAN) {
        int mid = low + (high - low) / 2;
        if (arr[low] > arr[mid]) {
            swap(arr[low], arr[mid]);
        }
        if (arr[low] > arr[high]) {
            swap(arr[low], arr[high]);
        }
        if (arr[mid] > arr[high]) {
            swap(arr[mid], arr[high]);
        }
        swap(arr[mid], arr[high]);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high, PivotStrategy strategy) {
    if (low < high) {
        choosePivot(arr, low, high, strategy);
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1, strategy);
        quickSort(arr, pi + 1, high, strategy);
    }
}

// Сортировка слиянием
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Пирамидальная сортировка
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void quickLast(int arr[], int n) { quickSort(arr, 0, n - 1, LAST); }
void quickMedian(int arr[], int n) { quickSort(arr, 0, n - 1, MEDIAN); }
void quickRandom(int arr[], int n) { quickSort(arr, 0, n - 1, RANDOM); }
void mergeWrapper(int arr[], int n) { mergeSort(arr, 0, n - 1); }

bool isSorted(const int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

long long checksum(const int arr[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

bool sortsCorrectly(void (*sortFunc)(int[], int), const int src[], int n) {
    int* copy = new int[n > 0 ? n : 1];
    for (int i = 0; i < n; i++) {
        copy[i] = src[i];
    }
    sortFunc(copy, n);
    bool ok = isSorted(copy, n) && checksum(copy, n) == checksum(src, n);
    delete[] copy;
    return ok;
}

double measureSort(void (*sortFunc)(int[], int), const int src[], int n, bool& ok) {
    int reps = 100000 / n;
    if (reps < 1) {
        reps = 1;
    }
    int* buf = new int[(long long)reps * n];
    for (int r = 0; r < reps; r++) {
        for (int i = 0; i < n; i++) {
            buf[(long long)r * n + i] = src[i];
        }
    }
    auto start = high_resolution_clock::now();
    for (int r = 0; r < reps; r++) {
        sortFunc(buf + (long long)r * n, n);
    }
    auto end = high_resolution_clock::now();
    ok = true;
    for (int r = 0; r < reps; r++) {
        if (!isSorted(buf + (long long)r * n, n)) {
            ok = false;
        }
    }
    delete[] buf;
    return duration<double, micro>(end - start).count() / reps;
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
        swap(arr[rand() % n], arr[rand() % n]);
    }
}

void fillDuplicates(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10;
    }
}

void printArray(const int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) {
            cout << ", ";
        }
    }
    cout << "]";
}

void printRow(const char* label, const int src[], int n) {
    bool ok[5];
    double t[5];
    t[0] = measureSort(quickLast, src, n, ok[0]);
    t[1] = measureSort(quickMedian, src, n, ok[1]);
    t[2] = measureSort(quickRandom, src, n, ok[2]);
    t[3] = measureSort(mergeWrapper, src, n, ok[3]);
    t[4] = measureSort(heapSort, src, n, ok[4]);
    cout << label;
    bool allOk = true;
    for (int k = 0; k < 5; k++) {
        cout << '\t' << fixed << setprecision(1) << t[k];
        if (!ok[k]) {
            allOk = false;
        }
    }
    cout << (allOk ? "" : "  ОШИБКА СОРТИРОВКИ") << '\n';
}

int main() {
    setlocale(LC_ALL, "");
    srand(42);

    cout << "=== 1. Работа алгоритмов на примере из методички ===\n";
    int base[] = {64, 34, 25, 12, 22, 11, 90, 5};
    int n = 8;
    cout << "Исходный массив: ";
    printArray(base, n);
    cout << '\n';
    const char* names[] = {"Быстрая, опорный последний", "Быстрая, медиана трёх", "Быстрая, случайный опорный",
                           "Слиянием", "Пирамидальная"};
    void (*funcs[])(int[], int) = {quickLast, quickMedian, quickRandom, mergeWrapper, heapSort};
    for (int k = 0; k < 5; k++) {
        int copy[8];
        for (int i = 0; i < n; i++) {
            copy[i] = base[i];
        }
        funcs[k](copy, n);
        cout << names[k] << ": ";
        printArray(copy, n);
        cout << '\n';
    }

    cout << "\nПостроение кучи для массива [4, 10, 3, 5, 1]: ";
    int h[] = {4, 10, 3, 5, 1};
    for (int i = 5 / 2 - 1; i >= 0; i--) {
        heapify(h, 5, i);
    }
    printArray(h, 5);
    cout << '\n';

    cout << "\n=== 2. Граничные случаи ===\n";
    cout << "Тест\t\t\tБыстрая\tСлиянием\tПирамидальная\n";
    struct Case {
        const char* name;
        int data[8];
        int size;
    };
    Case cases[] = {
        {"Пустой массив", {0}, 0},
        {"Один элемент", {7}, 1},
        {"Два элемента", {2, 1}, 2},
        {"Все элементы равны", {5, 5, 5, 5, 5}, 5},
        {"Отрицательные числа", {3, -1, -7, 0, 3, -1}, 6},
        {"Уже отсортирован", {1, 2, 3, 4, 5, 6, 7, 8}, 8},
        {"Обратный порядок", {8, 7, 6, 5, 4, 3, 2, 1}, 8},
    };
    for (const Case& c : cases) {
        cout << c.name << "\t\t" << (sortsCorrectly(quickMedian, c.data, c.size) ? "ОК" : "ОШИБКА")
             << "\t" << (sortsCorrectly(mergeWrapper, c.data, c.size) ? "ОК" : "ОШИБКА")
             << "\t\t" << (sortsCorrectly(heapSort, c.data, c.size) ? "ОК" : "ОШИБКА") << '\n';
    }

    cout << "\n=== 3. Массовая проверка на случайных массивах ===\n";
    int errors = 0;
    int tests = 0;
    for (int size = 0; size <= 300; size++) {
        int* arr = new int[size > 0 ? size : 1];
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % (size + 1) - size / 2;
        }
        for (int k = 0; k < 5; k++) {
            tests++;
            if (!sortsCorrectly(funcs[k], arr, size)) {
                errors++;
            }
        }
        delete[] arr;
    }
    cout << "Проверено сортировок: " << tests << ", ошибок: " << errors << '\n';

    cout << "\n=== 4. Время на случайных массивах, мкс ===\n";
    cout << "n\tБыстр.(посл.)\tБыстр.(медиана)\tБыстр.(случ.)\tСлиянием\tПирамид.\n";
    for (int size : {100, 1000, 10000, 50000, 100000}) {
        int* arr = new int[size];
        fillRandom(arr, size);
        printRow(to_string(size).c_str(), arr, size);
        delete[] arr;
    }

    cout << "\n=== 5. Влияние порядка данных, n = 10000, мкс ===\n";
    cout << "Данные\t\t\tБыстр.(посл.)\tБыстр.(медиана)\tБыстр.(случ.)\tСлиянием\tПирамид.\n";
    {
        const int size = 10000;
        int* arr = new int[size];
        const char* labels[] = {"Случайные", "Отсортированные", "Обратный порядок", "Почти отсортированные",
                                "Много повторов"};
        for (int k = 0; k < 5; k++) {
            if (k == 0) fillRandom(arr, size);
            if (k == 1) fillSorted(arr, size);
            if (k == 2) fillReversed(arr, size);
            if (k == 3) fillAlmostSorted(arr, size);
            if (k == 4) fillDuplicates(arr, size);
            printRow(labels[k], arr, size);
        }
        delete[] arr;
    }
    return 0;
}
