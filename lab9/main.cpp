#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>
#include <climits>
using namespace std;

unsigned int randomState = 2026;

unsigned int nextRandom() {
    randomState = randomState * 1664525u + 1013904223u;
    return randomState >> 8;
}

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Бинарное дерево и обходы
class BinaryTree {
private:
    Node* root;

    void preOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void inOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    void postOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }

    int height(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + max(height(node->left), height(node->right));
    }

    int countNodes(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    int countLeaves(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) {
            return 1;
        }
        return countLeaves(node->left) + countLeaves(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    ~BinaryTree() {
        destroy(root);
    }

    void buildSample() {
        destroy(root);
        root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        root->left->left = new Node(3);
        root->left->right = new Node(7);
        root->right->right = new Node(20);
        root->left->right->left = new Node(6);
        root->right->right->left = new Node(18);
    }

    void preOrder() const { preOrder(root); cout << '\n'; }
    void inOrder() const { inOrder(root); cout << '\n'; }
    void postOrder() const { postOrder(root); cout << '\n'; }

    void levelOrder() const {
        if (root != nullptr) {
            queue<Node*> q;
            q.push(root);
            while (!q.empty()) {
                Node* current = q.front();
                q.pop();
                cout << current->data << " ";
                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
        }
        cout << '\n';
    }

    void inOrderIterative() const {
        stack<Node*> s;
        Node* current = root;
        while (current != nullptr || !s.empty()) {
            while (current != nullptr) {
                s.push(current);
                current = current->left;
            }
            current = s.top();
            s.pop();
            cout << current->data << " ";
            current = current->right;
        }
        cout << '\n';
    }

    int getHeight() const { return height(root); }
    int getNodeCount() const { return countNodes(root); }
    int getLeafCount() const { return countLeaves(root); }
    bool empty() const { return root == nullptr; }
};

// Бинарное дерево поиска
class BST {
private:
    Node* root;

    Node* insert(Node* node, int key, bool& added) {
        if (node == nullptr) {
            added = true;
            return new Node(key);
        }
        if (key < node->data) {
            node->left = insert(node->left, key, added);
        } else if (key > node->data) {
            node->right = insert(node->right, key, added);
        }
        return node;
    }

    Node* search(Node* node, int key) const {
        if (node == nullptr || node->data == key) {
            return node;
        }
        if (key < node->data) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    Node* findMin(Node* node) const {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* findMax(Node* node) const {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    Node* remove(Node* node, int key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (key < node->data) {
            node->left = remove(node->left, key);
        } else if (key > node->data) {
            node->right = remove(node->right, key);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* minNode = findMin(node->right);
            node->data = minNode->data;
            node->right = remove(node->right, minNode->data);
        }
        return node;
    }

    void inOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    int collect(Node* node, int out[], int pos) const {
        if (node == nullptr) {
            return pos;
        }
        pos = collect(node->left, out, pos);
        out[pos++] = node->data;
        return collect(node->right, out, pos);
    }

    bool isValid(Node* node, long long low, long long high) const {
        if (node == nullptr) {
            return true;
        }
        if (node->data <= low || node->data >= high) {
            return false;
        }
        return isValid(node->left, low, node->data) && isValid(node->right, node->data, high);
    }

    int height(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + max(height(node->left), height(node->right));
    }

    int countNodes(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    void printRotated(Node* node, int depth) const {
        if (node == nullptr) {
            return;
        }
        printRotated(node->right, depth + 1);
        for (int i = 0; i < depth; i++) {
            cout << "    ";
        }
        cout << node->data << '\n';
        printRotated(node->left, depth + 1);
    }

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    ~BST() {
        destroy(root);
    }

    bool insert(int key) {
        bool added = false;
        root = insert(root, key, added);
        return added;
    }

    bool search(int key) const {
        return search(root, key) != nullptr;
    }

    bool remove(int key) {
        if (search(root, key) == nullptr) {
            return false;
        }
        root = remove(root, key);
        return true;
    }

    int minimum() const {
        if (root == nullptr) {
            throw underflow_error("Дерево пусто");
        }
        return findMin(root)->data;
    }

    int maximum() const {
        if (root == nullptr) {
            throw underflow_error("Дерево пусто");
        }
        return findMax(root)->data;
    }

    void print() const {
        if (root == nullptr) {
            cout << "(дерево пусто)";
        }
        inOrder(root);
        cout << '\n';
    }

    void printTree() const {
        if (root == nullptr) {
            cout << "(дерево пусто)\n";
        }
        printRotated(root, 0);
    }

    int toArray(int out[]) const { return collect(root, out, 0); }
    bool isValidBST() const { return isValid(root, LLONG_MIN, LLONG_MAX); }
    int getHeight() const { return height(root); }
    int getNodeCount() const { return countNodes(root); }
    bool empty() const { return root == nullptr; }
};

int main() {
    setlocale(LC_ALL, "");

    cout << "=== 1. Бинарное дерево и обходы ===\n";
    BinaryTree tree;
    tree.buildSample();
    cout << "Прямой обход (pre-order): ";
    tree.preOrder();
    cout << "Симметричный (in-order): ";
    tree.inOrder();
    cout << "Обратный (post-order): ";
    tree.postOrder();
    cout << "Уровневый (level-order): ";
    tree.levelOrder();
    cout << "Симметричный без рекурсии: ";
    tree.inOrderIterative();
    cout << "Узлов: " << tree.getNodeCount() << ", листьев: " << tree.getLeafCount()
         << ", высота: " << tree.getHeight() << '\n';
    BinaryTree emptyTree;
    cout << "Пустое дерево, прямой обход: ";
    emptyTree.preOrder();
    cout << "Пустое дерево, уровневый обход: ";
    emptyTree.levelOrder();
    cout << "Пустое дерево, симметричный без рекурсии: ";
    emptyTree.inOrderIterative();
    cout << "Пустое дерево, узлов: " << emptyTree.getNodeCount() << ", высота: " << emptyTree.getHeight() << '\n';

    cout << "\n=== 2. Бинарное дерево поиска ===\n";
    BST bst;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int k : keys) {
        bst.insert(k);
    }
    cout << "Симметричный обход BST: ";
    bst.print();
    cout << "Высота дерева: " << bst.getHeight() << ", узлов: " << bst.getNodeCount() << '\n';
    cout << "Структура дерева, повёрнутая на 90 градусов:\n";
    bst.printTree();
    cout << "Поиск 40: " << (bst.search(40) ? "найден" : "не найден") << '\n';
    cout << "Поиск 100: " << (bst.search(100) ? "найден" : "не найден") << '\n';
    cout << "Повторная вставка 40: " << (bst.insert(40) ? "добавлен" : "ключ уже есть") << '\n';
    cout << "Минимум: " << bst.minimum() << ", максимум: " << bst.maximum() << '\n';

    cout << "\nУдаление, случай 1, лист 20: ";
    bst.remove(20);
    bst.print();
    cout << "Удаление, случай 2, узел 30 с одним потомком: ";
    bst.remove(30);
    bst.print();
    cout << "Удаление, случай 3, корень 50 с двумя потомками: ";
    bst.remove(50);
    bst.print();
    cout << "Структура после удалений:\n";
    bst.printTree();
    cout << "Удаление несуществующего 999: " << (bst.remove(999) ? "удалён" : "ключа нет") << '\n';
    cout << "Дерево остаётся корректным BST: " << (bst.isValidBST() ? "да" : "НЕТ") << '\n';

    cout << "\n=== 3. Пустое дерево ===\n";
    BST none;
    cout << "Обход: ";
    none.print();
    cout << "Поиск 1: " << (none.search(1) ? "найден" : "не найден") << '\n';
    cout << "Удаление 1: " << (none.remove(1) ? "удалён" : "ключа нет") << '\n';
    try {
        none.minimum();
    } catch (const underflow_error& e) {
        cout << "Минимум пустого дерева: " << e.what() << '\n';
    }
    BST single;
    single.insert(5);
    single.remove(5);
    cout << "Дерево из одного узла после удаления корня пусто: " << (single.empty() ? "да" : "нет") << '\n';

    cout << "\n=== 4. Случайная проверка на 20000 операциях ===\n";
    {
        const int range = 500;
        bool present[range] = {false};
        BST t;
        int expectedCount = 0;
        bool ok = true;
        for (int op = 0; op < 20000; op++) {
            int key = (int)(nextRandom() % range);
            if (nextRandom() % 2 == 0) {
                bool added = t.insert(key);
                if (added == present[key]) ok = false;
                if (added) expectedCount++;
                present[key] = true;
            } else {
                bool removed = t.remove(key);
                if (removed != present[key]) ok = false;
                if (removed) expectedCount--;
                present[key] = false;
            }
            if (t.getNodeCount() != expectedCount) ok = false;
            if (op % 500 == 0 && !t.isValidBST()) ok = false;
        }
        int arr[range];
        int n = t.toArray(arr);
        for (int i = 0; i < n; i++) {
            if (!present[arr[i]] || (i > 0 && arr[i - 1] >= arr[i])) ok = false;
        }
        int expectedTotal = 0;
        for (int i = 0; i < range; i++) {
            if (present[i]) expectedTotal++;
        }
        if (n != expectedTotal) ok = false;
        cout << "Итоговое число ключей: " << n << ", результат сверки с массивом: " << (ok ? "совпадает" : "ОШИБКА") << '\n';
    }

    cout << "\n=== 5. Влияние порядка вставки на высоту, 2000 ключей ===\n";
    {
        BST sorted;
        BST random;
        for (int i = 0; i < 2000; i++) {
            sorted.insert(i);
            random.insert((int)(nextRandom() % 100000));
        }
        cout << "Вставка по возрастанию: высота " << sorted.getHeight() << '\n';
        cout << "Вставка случайных ключей: высота " << random.getHeight() << ", узлов " << random.getNodeCount() << '\n';
    }

    cout << "\n=== 6. Применение BST ===\n";
    BST app;
    int sequence[20];
    int added = 0;
    while (added < 20) {
        int value = (int)(nextRandom() % 100);
        if (app.insert(value)) {
            sequence[added++] = value;
        }
    }
    cout << "Случайная последовательность из 20 чисел: ";
    for (int i = 0; i < 20; i++) {
        cout << sequence[i] << " ";
    }
    cout << "\nСимметричный обход, отсортированная последовательность: ";
    app.print();
    cout << "Минимум: " << app.minimum() << ", максимум: " << app.maximum() << '\n';
    cout << "Высота дерева: " << app.getHeight() << '\n';
    cout << "Структура дерева:\n";
    app.printTree();
    cout << "Введите элемент для удаления: ";
    int toRemove;
    if (cin >> toRemove) {
        if (app.remove(toRemove)) {
            cout << "Элемент " << toRemove << " удалён. Дерево после удаления: ";
            app.print();
            app.printTree();
        } else {
            cout << "Элемента " << toRemove << " в дереве нет, дерево не изменилось\n";
        }
    } else {
        cout << "Ввод не является числом, удаление не выполнено\n";
    }
    return 0;
}
