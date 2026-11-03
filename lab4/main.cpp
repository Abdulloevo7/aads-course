#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

template <typename T>
struct Node {
    T data;
    Node<T>* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
struct DNode {
    T data;
    DNode<T>* prev;
    DNode<T>* next;

    DNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

template <typename T>
class SinglyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int count;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    ~SinglyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = newNode;
        }
        count++;
    }

    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    void pop_front() {
        if (head == nullptr) {
            throw underflow_error("Список пуст");
        }
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        count--;
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    void pop_back() {
        if (head == nullptr) {
            throw underflow_error("Список пуст");
        }
        if (head->next == nullptr) {
            delete head;
            head = tail = nullptr;
        } else {
            Node<T>* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        count--;
    }

    Node<T>* find(const T& value) const {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool remove(const T& value) {
        if (head == nullptr) {
            return false;
        }
        if (head->data == value) {
            pop_front();
            return true;
        }
        Node<T>* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }
        if (current->next == nullptr) {
            return false;
        }
        Node<T>* toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == tail) {
            tail = current;
        }
        delete toDelete;
        count--;
        return true;
    }

    T sum() const {
        T total = T();
        for (Node<T>* current = head; current != nullptr; current = current->next) {
            total = total + current->data;
        }
        return total;
    }

    void print() const {
        cout << "[";
        for (Node<T>* current = head; current != nullptr; current = current->next) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << ", ";
            }
        }
        cout << "]\n";
    }

    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

template <typename T>
class DoublyLinkedList {
private:
    DNode<T>* head;
    DNode<T>* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    ~DoublyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        DNode<T>* newNode = new DNode<T>(value);
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
        DNode<T>* newNode = new DNode<T>(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void pop_front() {
        if (head == nullptr) {
            throw underflow_error("Список пуст");
        }
        DNode<T>* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        count--;
    }

    void pop_back() {
        if (tail == nullptr) {
            throw underflow_error("Список пуст");
        }
        DNode<T>* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        count--;
    }

    DNode<T>* find(const T& value) const {
        DNode<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void insert_after(DNode<T>* node, const T& value) {
        if (node == nullptr) {
            push_front(value);
            return;
        }
        DNode<T>* newNode = new DNode<T>(value);
        newNode->next = node->next;
        newNode->prev = node;
        if (node->next != nullptr) {
            node->next->prev = newNode;
        } else {
            tail = newNode;
        }
        node->next = newNode;
        count++;
    }

    void erase(DNode<T>* node) {
        if (node == nullptr) {
            return;
        }
        if (node == head) {
            pop_front();
            return;
        }
        if (node == tail) {
            pop_back();
            return;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        count--;
    }

    void reverse() {
        DNode<T>* current = head;
        while (current != nullptr) {
            DNode<T>* temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        DNode<T>* tmp = head;
        head = tail;
        tail = tmp;
    }

    void print() const {
        cout << "[";
        for (DNode<T>* current = head; current != nullptr; current = current->next) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << ", ";
            }
        }
        cout << "]\n";
    }

    void printReverse() const {
        cout << "[";
        for (DNode<T>* current = tail; current != nullptr; current = current->prev) {
            cout << current->data;
            if (current->prev != nullptr) {
                cout << ", ";
            }
        }
        cout << "]\n";
    }

    void clear() {
        while (head != nullptr) {
            DNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }
};

template <typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "");

    cout << "=== 1. Односвязный список ===\n";
    SinglyLinkedList<int> s;
    s.push_back(10);
    s.push_back(20);
    s.push_back(30);
    s.push_front(5);
    cout << "Список: ";
    s.print();
    cout << "Размер: " << s.size() << ", сумма элементов: " << s.sum() << '\n';
    s.pop_front();
    cout << "После pop_front: ";
    s.print();
    s.pop_back();
    cout << "После pop_back: ";
    s.print();
    cout << "find(20): " << (s.find(20) != nullptr ? "найден" : "не найден") << '\n';
    cout << "find(99): " << (s.find(99) != nullptr ? "найден" : "не найден") << '\n';
    s.push_back(40);
    s.push_back(50);
    cout << "Список: ";
    s.print();
    cout << "remove(40): " << (s.remove(40) ? "удалён" : "нет значения") << '\n';
    cout << "remove(50): " << (s.remove(50) ? "удалён" : "нет значения") << '\n';
    cout << "remove(99): " << (s.remove(99) ? "удалён" : "нет значения") << '\n';
    s.push_back(60);
    cout << "Список: ";
    s.print();
    cout << "Размер: " << s.size() << ", сумма элементов: " << s.sum() << '\n';
    s.clear();
    cout << "После clear: размер=" << s.size() << ", пуст: " << (s.empty() ? "да" : "нет")
         << ", сумма=" << s.sum() << '\n';
    try {
        s.pop_front();
    } catch (const underflow_error& e) {
        cout << "pop_front пустого списка: " << e.what() << '\n';
    }
    try {
        s.pop_back();
    } catch (const underflow_error& e) {
        cout << "pop_back пустого списка: " << e.what() << '\n';
    }

    cout << "\n=== 2. Односвязный список строк ===\n";
    SinglyLinkedList<string> names;
    names.push_back("Ставрополь");
    names.push_back("Москва");
    names.push_front("Казань");
    names.print();
    cout << "Конкатенация: " << names.sum() << '\n';

    cout << "\n=== 3. Двусвязный список ===\n";
    DoublyLinkedList<int> d;
    for (int i = 1; i <= 5; i++) {
        d.push_back(i * 10);
    }
    cout << "Прямой порядок: ";
    d.print();
    cout << "Обратный порядок: ";
    d.printReverse();
    d.push_front(5);
    cout << "После push_front(5): ";
    d.print();
    d.insert_after(d.find(30), 35);
    cout << "После insert_after(30, 35): ";
    d.print();
    d.insert_after(d.find(50), 55);
    cout << "После insert_after(50, 55): ";
    d.print();
    d.erase(d.find(20));
    cout << "После erase(20): ";
    d.print();
    d.erase(d.find(55));
    cout << "После erase(55), хвост: ";
    d.print();
    d.pop_back();
    d.pop_front();
    cout << "После pop_back и pop_front: ";
    d.print();
    cout << "Размер: " << d.size() << '\n';

    cout << "\n=== 4. Реверс двусвязного списка ===\n";
    cout << "До реверса: ";
    d.print();
    d.reverse();
    cout << "После реверса, прямой порядок: ";
    d.print();
    cout << "После реверса, обратный порядок: ";
    d.printReverse();
    d.push_back(1);
    d.push_front(99);
    cout << "После push_back(1) и push_front(99): ";
    d.print();
    DoublyLinkedList<int> one;
    one.push_back(7);
    one.reverse();
    cout << "Реверс списка из одного элемента: ";
    one.print();
    DoublyLinkedList<int> none;
    none.reverse();
    cout << "Реверс пустого списка: ";
    none.print();
    try {
        none.pop_back();
    } catch (const underflow_error& e) {
        cout << "pop_back пустого списка: " << e.what() << '\n';
    }

    cout << "\n=== 5. Сравнение pop_back, мкс ===\n";
    cout << "n\tОдносвязный\tДвусвязный\n";
    for (int n : {5000, 10000, 20000, 40000}) {
        SinglyLinkedList<int> a;
        DoublyLinkedList<int> b;
        for (int i = 0; i < n; i++) {
            a.push_back(i);
            b.push_back(i);
        }
        long long ta = measureTime([&]() {
            while (!a.empty()) {
                a.pop_back();
            }
        });
        long long tb = measureTime([&]() {
            while (!b.empty()) {
                b.pop_back();
            }
        });
        cout << n << '\t' << ta << "\t\t" << tb << '\n';
    }
    return 0;
}
