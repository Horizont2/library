#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

struct Date {
    int month;
    int year;
};

struct BOOK {
    string name;
    string author;
    Date printDate;
    double cost;
    BOOK* next;
};

void loadFromFile(BOOK*& head);
void displayList(BOOK* head);
void deleteBooksByYear(BOOK*& head, int year);
void addBooksToEnd(BOOK*& head, int n);
void saveToFile(BOOK* head, const string& filename);
void saveLast10BooksSorted(BOOK* head);

int main() {
    BOOK* head = nullptr;
    loadFromFile(head);

    int choice, year, n;
    do {
        cout << "\n1. Показати список\n"
             << "2. Видалити книги за роком\n"
             << "3. Додати N нових книг\n"
             << "4. Зберегти у файл SHOP_NEW.DAT\n"
             << "5. Зберегти останні 10 книг у BOOK.DAT\n"
             << "6. Вийти\n"
             << "Виберіть опцію: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayList(head);
                break;
            case 2:
                cout << "Введіть рік: ";
                cin >> year;
                deleteBooksByYear(head, year);
                break;
            case 3:
                cout << "Введіть кількість книг для додавання: ";
                cin >> n;
                addBooksToEnd(head, n);
                break;
            case 4:
                saveToFile(head, "SHOP_NEW.DAT");
                break;
            case 5:
                saveLast10BooksSorted(head);
                break;
            case 6:
                cout << "Вихід із програми.\n";
                break;
            default:
                cout << "Неправильний вибір. Спробуйте ще раз.\n";
        }
    } while (choice != 6);

    return 0;
}

void loadFromFile(BOOK*& head) {
    ifstream file("SHOP.dat", ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу.\n";
        return;
    }

    BOOK* current = nullptr;
    while (file) {
        BOOK* newBook = new BOOK;
        file.read(reinterpret_cast<char*>(newBook), sizeof(BOOK));
        if (!file) break;

        newBook->next = nullptr;
        if (!head) {
            head = newBook;
            current = head;
        } else {
            current->next = newBook;
            current = newBook;
        }
    }
    file.close();
    cout << "Список завантажено з файлу.\n";
}

void displayList(BOOK* head) {
    BOOK* temp = head;
    cout << left << setw(20) << "Назва книги" << setw(20) << "Автор" 
         << setw(10) << "Місяць" << setw(10) << "Рік" << setw(10) << "Ціна\n";
    while (temp) {
        cout << setw(20) << temp->name << setw(20) << temp->author
             << setw(10) << temp->printDate.month << setw(10) << temp->printDate.year
             << setw(10) << temp->cost << endl;
        temp = temp->next;
    }
}

void deleteBooksByYear(BOOK*& head, int year) {
    BOOK *current = head, *prev = nullptr;
    while (current) {
        if (current->printDate.year == year) {
            if (current == head) {
                head = head->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            current = (prev) ? prev->next : head;
        } else {
            prev = current;
            current = current->next;
        }
    }
    cout << "Книги за " << year << " рік видалено.\n";
}

void addBooksToEnd(BOOK*& head, int n) {
    BOOK* current = head;
    while (current && current->next) current = current->next;

    for (int i = 0; i < n; ++i) {
        BOOK* newBook = new BOOK;
        cout << "Введіть назву книги: ";
        cin.ignore();
        getline(cin, newBook->name);
        cout << "Введіть автора: ";
        getline(cin, newBook->author);
        cout << "Введіть місяць друку: ";
        cin >> newBook->printDate.month;
        cout << "Введіть рік друку: ";
        cin >> newBook->printDate.year;
        cout << "Введіть ціну книги: ";
        cin >> newBook->cost;

        newBook->next = nullptr;
        if (!head) {
            head = newBook;
        } else {
            current->next = newBook;
            current = newBook;
        }
    }
    cout << n << " нових книг додано.\n";
}

void saveToFile(BOOK* head, const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу.\n";
        return;
    }

    BOOK* current = head;
    while (current) {
        file.write(reinterpret_cast<const char*>(current), sizeof(BOOK));
        current = current->next;
    }
    file.close();
    cout << "Список збережено у файл " << filename << ".\n";
}

void saveLast10BooksSorted(BOOK* head) {
    vector<BOOK> books;
    BOOK* current = head;

    while (current) {
        books.push_back(*current);
        current = current->next;
    }

    if (books.size() > 10) {
        books.erase(books.begin(), books.end() - 10);
    }

    sort(books.begin(), books.end(), [](const BOOK& a, const BOOK& b) {
        return a.author < b.author;
    });

    ofstream file("BOOK.DAT", ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу.\n";
        return;
    }

    for (const auto& book : books) {
        file.write(reinterpret_cast<const char*>(&book), sizeof(BOOK));
    }
    file.close();
    cout << "Останні 10 книг збережено у файл BOOK.DAT у відсортованому порядку.\n";
}
