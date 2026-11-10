#include <iostream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <utility>
using namespace std;

// Стек на динамическом массиве
template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = 10) : data(nullptr), topIndex(-1), capacity(initialCapacity) {
        if (capacity < 1) {
            throw invalid_argument("Ёмкость должна быть положительной");
        }
        data = new T[capacity];
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    ~Stack() {
        delete[] data;
    }

    void push(const T& value) {
        if (topIndex >= capacity - 1) {
            resize();
        }
        data[++topIndex] = value;
    }

    T pop() {
        if (isEmpty()) {
            throw underflow_error("Стек пуст");
        }
        return data[topIndex--];
    }

    T top() const {
        if (isEmpty()) {
            throw underflow_error("Стек пуст");
        }
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};

// Очередь на кольцевом буфере
template <typename T>
class Queue {
private:
    T* data;
    int frontIndex;
    int rearIndex;
    int count;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = count;
        capacity = newCapacity;
    }

public:
    Queue(int initialCapacity = 10)
        : data(nullptr), frontIndex(0), rearIndex(0), count(0), capacity(initialCapacity) {
        if (capacity < 1) {
            throw invalid_argument("Ёмкость должна быть положительной");
        }
        data = new T[capacity];
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    ~Queue() {
        delete[] data;
    }

    void enqueue(const T& value) {
        if (count >= capacity) {
            resize();
        }
        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw underflow_error("Очередь пуста");
        }
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) {
            throw underflow_error("Очередь пуста");
        }
        return data[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Дек на двусвязном списке
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    ~Deque() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    T pop_front() {
        if (isEmpty()) {
            throw underflow_error("Дек пуст");
        }
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        count--;
        return value;
    }

    T pop_back() {
        if (isEmpty()) {
            throw underflow_error("Дек пуст");
        }
        Node* temp = tail;
        T value = temp->data;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) {
            throw underflow_error("Дек пуст");
        }
        return head->data;
    }

    T back() const {
        if (isEmpty()) {
            throw underflow_error("Дек пуст");
        }
        return tail->data;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Приоритетная очередь на двоичной куче, max-heap
template <typename T>
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;
            if (left < count && heap[left] > heap[largest]) {
                largest = left;
            }
            if (right < count && heap[right] > heap[largest]) {
                largest = right;
            }
            if (largest == index) {
                break;
            }
            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCapacity = 10) : heap(nullptr), count(0), capacity(initialCapacity) {
        if (capacity < 1) {
            throw invalid_argument("Ёмкость должна быть положительной");
        }
        heap = new T[capacity];
    }

    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    ~PriorityQueue() {
        delete[] heap;
    }

    void push(const T& value) {
        if (count >= capacity) {
            resize();
        }
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    T pop() {
        if (isEmpty()) {
            throw underflow_error("Приоритетная очередь пуста");
        }
        T maxValue = heap[0];
        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
        return maxValue;
    }

    T top() const {
        if (isEmpty()) {
            throw underflow_error("Приоритетная очередь пуста");
        }
        return heap[0];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Задача для стека: правильность расстановки скобок
bool checkBrackets(const string& expr) {
    Stack<char> stack;
    for (char ch : expr) {
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.isEmpty()) {
                return false;
            }
            char open = stack.pop();
            if ((ch == ')' && open != '(') ||
                (ch == ']' && open != '[') ||
                (ch == '}' && open != '{')) {
                return false;
            }
        }
    }
    return stack.isEmpty();
}

// Задача для очереди: модель очереди в банке с одним окном
struct Customer {
    int id;
    int arrival;
    int service;
};

void simulateBank(const Customer* customers, int n) {
    Queue<Customer> line(2);
    int clock = 0;
    int busyUntil = 0;
    int next = 0;
    int served = 0;
    int totalWait = 0;
    int maxWait = 0;
    int maxLine = 0;

    cout << "Клиент\tПришёл\tОбслуживание\tНачало\tОжидание\tУшёл\n";
    while (served < n) {
        while (next < n && customers[next].arrival <= clock) {
            line.enqueue(customers[next]);
            next++;
            if (line.size() > maxLine) {
                maxLine = line.size();
            }
        }
        if (clock >= busyUntil && !line.isEmpty()) {
            Customer c = line.dequeue();
            int wait = clock - c.arrival;
            busyUntil = clock + c.service;
            totalWait += wait;
            if (wait > maxWait) {
                maxWait = wait;
            }
            cout << c.id << '\t' << c.arrival << '\t' << c.service << "\t\t"
                 << clock << '\t' << wait << "\t\t" << busyUntil << '\n';
            served++;
        }
        clock++;
    }
    cout << "Обслужено клиентов: " << served << '\n';
    cout << "Среднее ожидание: " << (double)totalWait / n << " мин\n";
    cout << "Максимальное ожидание: " << maxWait << " мин\n";
    cout << "Максимальная длина очереди: " << maxLine << '\n';
    cout << "Окно закрылось в момент: " << busyUntil << '\n';
}

// Задача для дека: палиндром без учёта регистра и небуквенных символов
bool isPalindrome(const string& str) {
    Deque<char> dq;
    for (char ch : str) {
        if (isalnum((unsigned char)ch)) {
            dq.push_back((char)tolower((unsigned char)ch));
        }
    }
    while (dq.size() > 1) {
        char left = dq.pop_front();
        char right = dq.pop_back();
        if (left != right) {
            return false;
        }
    }
    return true;
}

// Задача для приоритетной очереди: K наибольших элементов массива
void kLargest(const int* arr, int n, int k, int* result) {
    if (k < 0 || k > n) {
        throw invalid_argument("K должно быть от 0 до размера массива");
    }
    PriorityQueue<int> pq;
    for (int i = 0; i < n; i++) {
        pq.push(arr[i]);
    }
    for (int i = 0; i < k; i++) {
        result[i] = pq.pop();
    }
}

void printArray(const int* arr, int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) {
            cout << ", ";
        }
    }
    cout << "]";
}

int main() {
    setlocale(LC_ALL, "");

    cout << "=== 1. Стек ===\n";
    {
        Stack<int> s(2);
        for (int i = 1; i <= 5; i++) {
            s.push(i * 10);
        }
        cout << "Размер: " << s.size() << ", вершина: " << s.top() << '\n';
        cout << "Извлечение:";
        while (!s.isEmpty()) {
            cout << ' ' << s.pop();
        }
        cout << '\n';
        try {
            s.pop();
        } catch (const underflow_error& e) {
            cout << "pop из пустого стека: " << e.what() << '\n';
        }
        try {
            s.top();
        } catch (const underflow_error& e) {
            cout << "top пустого стека: " << e.what() << '\n';
        }
    }
    cout << "\nПроверка скобок:\n";
    const char* tests[] = {"()", "([]{})", "{[()]}", "((())", "([)]", "(", ")", "", "a + (b * [c - d])"};
    for (const char* t : tests) {
        cout << '"' << t << "\" : " << (checkBrackets(t) ? "ВЕРНО" : "НЕВЕРНО") << '\n';
    }

    cout << "\n=== 2. Очередь ===\n";
    {
        Queue<int> q(4);
        for (int i = 1; i <= 4; i++) {
            q.enqueue(i);
        }
        cout << "dequeue: " << q.dequeue() << ", dequeue: " << q.dequeue() << '\n';
        q.enqueue(5);
        q.enqueue(6);
        q.enqueue(7);
        cout << "Размер: " << q.size() << ", начало: " << q.front() << '\n';
        cout << "Извлечение:";
        while (!q.isEmpty()) {
            cout << ' ' << q.dequeue();
        }
        cout << '\n';
        try {
            q.dequeue();
        } catch (const underflow_error& e) {
            cout << "dequeue из пустой очереди: " << e.what() << '\n';
        }
        try {
            q.front();
        } catch (const underflow_error& e) {
            cout << "front пустой очереди: " << e.what() << '\n';
        }
    }
    cout << "\nМодель очереди в банке:\n";
    Customer customers[] = {{1, 0, 5}, {2, 1, 3}, {3, 2, 4}, {4, 4, 2}, {5, 6, 3}, {6, 7, 1}, {7, 20, 2}};
    simulateBank(customers, 7);

    cout << "\n=== 3. Дек ===\n";
    {
        Deque<int> d;
        d.push_back(2);
        d.push_back(3);
        d.push_front(1);
        d.push_front(0);
        cout << "Начало: " << d.front() << ", конец: " << d.back() << ", размер: " << d.size() << '\n';
        cout << "pop_front: " << d.pop_front() << ", pop_back: " << d.pop_back() << '\n';
        cout << "Начало: " << d.front() << ", конец: " << d.back() << '\n';
        d.pop_front();
        d.pop_back();
        try {
            d.pop_front();
        } catch (const underflow_error& e) {
            cout << "pop_front из пустого дека: " << e.what() << '\n';
        }
        try {
            d.pop_back();
        } catch (const underflow_error& e) {
            cout << "pop_back из пустого дека: " << e.what() << '\n';
        }
    }
    cout << "\nПроверка палиндромов:\n";
    const char* words[] = {"radar", "hello", "level", "a", "", "ab", "A man, a plan, a canal: Panama"};
    for (const char* w : words) {
        cout << '"' << w << "\" : " << (isPalindrome(w) ? "ДА" : "НЕТ") << '\n';
    }

    cout << "\n=== 4. Приоритетная очередь ===\n";
    {
        PriorityQueue<int> pq(2);
        int values[] = {30, 10, 50, 20, 40, 5};
        for (int v : values) {
            pq.push(v);
        }
        cout << "Размер: " << pq.size() << ", максимум: " << pq.top() << '\n';
        cout << "Извлечение:";
        while (!pq.isEmpty()) {
            cout << ' ' << pq.pop();
        }
        cout << '\n';
        try {
            pq.pop();
        } catch (const underflow_error& e) {
            cout << "pop из пустой очереди: " << e.what() << '\n';
        }
        try {
            pq.top();
        } catch (const underflow_error& e) {
            cout << "top пустой очереди: " << e.what() << '\n';
        }
    }
    cout << "\nK наибольших элементов:\n";
    int arr[] = {7, 2, 19, 4, 25, 11, 3, 18, 9, 30};
    int n = 10;
    int result[10];
    cout << "Массив: ";
    printArray(arr, n);
    cout << '\n';
    for (int k : {3, 1, 10, 0}) {
        kLargest(arr, n, k, result);
        cout << "K = " << k << ": ";
        printArray(result, k);
        cout << '\n';
    }
    try {
        kLargest(arr, n, 11, result);
    } catch (const invalid_argument& e) {
        cout << "K = 11: " << e.what() << '\n';
    }
    return 0;
}
