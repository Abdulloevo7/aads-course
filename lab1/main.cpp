#include <iostream>
#include <string>
#include <vector>
#include <clocale>

using namespace std;

class Book {
private:
    string title;
    string author;
    int year;

    static int bookCount; // Количество существующих объектов Book

public:
    Book(const string& title, const string& author, int year)
        : title(title), author(author), year(year) {
        ++bookCount;
        cout << "Создана книга: " << this->title << '\n';
    }

    ~Book() {
        --bookCount;
        cout << "Удалена книга: " << title << '\n';
    }

    void print() const {
        cout << '"' << title << "\", " << author
             << ", " << year << " г.\n";
    }

    static int getBookCount() {
        return bookCount;
    }
};

int Book::bookCount = 0;

class Library {
private:
    string name;
    vector<Book*> books; // Агрегация: библиотека хранит указатели на книги

public:
    explicit Library(const string& name) : name(name) {
        cout << "Открыта библиотека: " << this->name << '\n';
    }

    ~Library() {
        // Книги не удаляются: библиотека не владеет их временем жизни
        cout << "Закрыта библиотека: " << name << '\n';
    }

    void addBook(Book* book) {
        if (book == nullptr) {
            cout << "Ошибка: передан пустой указатель.\n";
            return;
        }

        books.push_back(book);
        cout << "Книга добавлена в библиотеку.\n";
    }

    void print() const {
        cout << "\nБиблиотека: " << name << '\n';
        for (const Book* book : books) {
            if (book != nullptr) {
                book->print();
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "");

    cout << "Количество книг: " << Book::getBookCount() << '\n';

    Book book1("Война и мир", "Л. Толстой", 1869);
    Book book2("Преступление и наказание", "Ф. Достоевский", 1866);
    Book book3("Евгений Онегин", "А. Пушкин", 1833);

    cout << "\nКоличество книг: " << Book::getBookCount() << '\n';

    {
        Library library("Городская библиотека");
        library.addBook(&book1);
        library.addBook(&book2);
        library.addBook(&book3);
        library.print();
    } // Библиотека уничтожается, книги продолжают существовать

    cout << "\nКниги после закрытия библиотеки:\n";
    book1.print();
    book2.print();
    book3.print();

    cout << "\nКоличество книг: " << Book::getBookCount() << '\n';
    return 0;
}
