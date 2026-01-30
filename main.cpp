#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string PURPLE = "\033[35m";
const string BOLD = "\033[1m";
const string PINK = "\033[38;5;218m";

struct Book {
    int id;
    string title;
    string author;
    string isbn;
    int quantity;
};

class LibrarySystem {
private:
    vector<Book> books;
    int nextId;
    const string SAVE_FILE = "library_data.json";

    void printHeader() {
        cout << PINK << BOLD;
        cout << "\n========================================\n";
        cout << "       LIBRARY MANAGEMENT SYSTEM        \n";
        cout << "========================================\n";
        cout << RESET;
    }

    string escapeJson(string str) {
        string escaped;
        for (char c : str) {
            if (c == '"') escaped += "\\\"";
            else if (c == '\\') escaped += "\\\\";
            else if (c == '\n') escaped += "\\n";
            else if (c == '\t') escaped += "\\t";
            else escaped += c;
        }
        return escaped;
    }

    string unescapeJson(string str) {
        string unescaped;
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '\\' && i + 1 < str.length()) {
                if (str[i + 1] == '"') { unescaped += '"'; i++; }
                else if (str[i + 1] == '\\') { unescaped += '\\'; i++; }
                else if (str[i + 1] == 'n') { unescaped += '\n'; i++; }
                else if (str[i + 1] == 't') { unescaped += '\t'; i++; }
                else unescaped += str[i];
            } else {
                unescaped += str[i];
            }
        }
        return unescaped;
    }

    void reindexBooks() {
        for (int i = 0; i < books.size(); i++) {
            books[i].id = i + 1;
        }
        nextId = books.size() + 1;
    }

public:
    LibrarySystem() {
        nextId = 1;
        loadFromMemory();
    }

    void addBook() {
        printHeader();
        cout << YELLOW << "\n（づ￣3￣）づ╭ Add New Book ～\n" << RESET;

        Book book;
        book.id = nextId++;
        cout << YELLOW << "Book ID: " << RESET << book.id << endl;

        while (true) {
            cout << PURPLE << "Enter Title: " << RESET;
            getline(cin, book.title);
            if (!book.title.empty()) break;
            cout << RED << " ❌ Title is needed!!" << RESET << endl;
        }

        while (true) {
            cout << CYAN << "Enter Author: " << RESET;
            getline(cin, book.author);
            if (!book.author.empty()) break;
            cout << RED << " ❌ Author cannot be empty!" << RESET << endl;
        }

        while (true) {
            cout << CYAN << "Enter ISBN: " << RESET;
            getline(cin, book.isbn);
            if (!book.isbn.empty()) break;
            cout << RED << " ❌ ISBN is required!!" << RESET << endl;
        }

        while (true) {
            cout << CYAN << "Enter Quantity: " << RESET;
            cin >> book.quantity;

            if (cin.fail() || book.quantity < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << " ❌ Invalid quantity. Try again!" << RESET << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }

        books.push_back(book);
        cout << GREEN << "✅ Book added successfully.\n" << RESET;

        saveToMemory();
    }

    void editBook() {
        printHeader();
        cout << YELLOW << "\n(●'◡'●) Edit Book\n" << RESET;

        int bookId;
        cout << CYAN << "Enter Book ID: " << RESET;
        cin >> bookId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int foundIndex = -1;

        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == bookId) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex == -1) {
            cout << RED << "Book not found.\n" << RESET;
            return;
        }

        string input;

        cout << PURPLE << "New Title (leave empty to keep): " << RESET;
        getline(cin, input);
        if (!input.empty())
            books[foundIndex].title = input;

        cout << CYAN << "New Author (leave empty to keep): " << RESET;
        getline(cin, input);
        if (!input.empty())
            books[foundIndex].author = input;

        cout << CYAN << "New ISBN (leave empty to keep): " << RESET;
        getline(cin, input);
        if (!input.empty())
            books[foundIndex].isbn = input;

        cout << CYAN << "New Quantity (-1 to keep): " << RESET;
        int qty;
        cin >> qty;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (qty >= 0)
            books[foundIndex].quantity = qty;

        cout << GREEN << "Book updated successfully.\n" << RESET;

        saveToMemory();
    }

    void deleteBook() {
        printHeader();
        cout << RED << "\n╰(*°▽°*)╯ Delete Book\n" << RESET;

        cout << YELLOW << "Delete Options:\n";
        cout << "1. Delete by ID\n";
        cout << "2. Delete All Books\n";
        cout << CYAN << "Enter choice: " << RESET;

        int deleteChoice;
        cin >> deleteChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (deleteChoice == 1) {

            int bookId;
            cout << CYAN << "Enter Book ID to delete: " << RESET;
            cin >> bookId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool found = false;
            for (int i = 0; i < books.size(); i++) {
                if (books[i].id == bookId) {
                    cout << YELLOW << "Deleting book: " << books[i].title << RESET << endl;
                    books.erase(books.begin() + i);
                    found = true;
                    break;
                }
            }

            if (found) {
                reindexBooks();
                cout << GREEN << "✅ Book deleted successfully!\n";
                cout << YELLOW << "📝 IDs have been reindexed.\n" << RESET;
                saveToMemory();
            } else {
                cout << RED << "❌ Book not found.\n" << RESET;
            }
        }
        else if (deleteChoice == 2) {

            if (books.empty()) {
                cout << YELLOW << "Library is already empty.\n" << RESET;
                return;
            }

            cout << RED << "⚠️  WARNING: This will delete ALL books!\n";
            cout << CYAN << "Are you sure? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (confirm == 'y' || confirm == 'Y') {
                int bookCount = books.size();
                books.clear();
                nextId = 1;
                cout << GREEN << "✅ Successfully deleted " << bookCount << " books.\n";
                cout << YELLOW << "📝 Library has been reset.\n" << RESET;
                saveToMemory();
            } else {
                cout << YELLOW << "Delete operation cancelled.\n" << RESET;
            }
        }
        else {
            cout << RED << "❌ Invalid choice.\n" << RESET;
        }
    }

    void searchBook() {
        printHeader();
        cout << MAGENTA << "\nㄟ(≧◇≦)ㄏ Search Book\n" << RESET;

        cout << PURPLE << "Enter title keyword: " << RESET;
        string keyword;
        getline(cin, keyword);

        bool found = false;

        for (int i = 0; i < books.size(); i++) {
            if (books[i].title.find(keyword) != string::npos) {
                cout << GREEN << "ID: " << books[i].id << RESET
                     << PURPLE << ", Title: " << books[i].title << RESET
                     << GREEN << ", Author: " << books[i].author
                     << ", ISBN: " << books[i].isbn
                     << ", Quantity: " << books[i].quantity
                     << RESET << endl;
                found = true;
            }
        }

        if (!found)
            cout << RED << "No matching books found.\n" << RESET;
    }

    void showAllBooks() {
        printHeader();
        cout << BLUE << "\n^_^ All Books\n" << RESET;

        if (books.empty()) {
            cout << RED << "Library is empty.\n" << RESET;
            return;
        }

        cout << PINK << BOLD << left
             << setw(5) << "ID"
             << setw(25) << "Title"
             << setw(20) << "Author"
             << setw(15) << "ISBN"
             << setw(10) << "Quantity" << RESET << endl;
        cout << YELLOW << string(75, '-') << RESET << endl;

        for (int i = 0; i < books.size(); i++) {
            cout << GREEN << setw(5) << books[i].id << RESET
                 << PURPLE << setw(25) << books[i].title << RESET
                 << CYAN << setw(20) << books[i].author << RESET
                 << BLUE << setw(15) << books[i].isbn << RESET
                 << YELLOW << setw(10) << books[i].quantity << RESET << endl;
        }
    }

    void saveToMemory() {
        printHeader();
        cout << CYAN << BOLD << "\nO_O Saving to Memory...\n" << RESET;

        ofstream outFile(SAVE_FILE);

        if (!outFile) {
            cout << RED << "❌ Error: Unable to save data!\n" << RESET;
            return;
        }

        outFile << "{\n";
        outFile << "  \"nextId\": " << nextId << ",\n";
        outFile << "  \"books\": [\n";

        for (int i = 0; i < books.size(); i++) {
            outFile << "    {\n";
            outFile << "      \"id\": " << books[i].id << ",\n";
            outFile << "      \"title\": \"" << escapeJson(books[i].title) << "\",\n";
            outFile << "      \"author\": \"" << escapeJson(books[i].author) << "\",\n";
            outFile << "      \"isbn\": \"" << escapeJson(books[i].isbn) << "\",\n";
            outFile << "      \"quantity\": " << books[i].quantity << "\n";
            outFile << "    }";
            if (i < books.size() - 1) {
                outFile << ",";
            }
            outFile << "\n";
        }

        outFile << "  ]\n";
        outFile << "}\n";

        outFile.close();
        cout << GREEN << "✅ Library data saved successfully!\n";
        cout << YELLOW << "📝 Total books saved: " << books.size() << "\n" << RESET;
    }

    void loadFromMemory() {
        ifstream inFile(SAVE_FILE);

        if (!inFile) {
            return;
        }

        string line;
        string content;

        while (getline(inFile, line)) {
            content += line;
        }
        inFile.close();

        size_t nextIdPos = content.find("\"nextId\":");
        if (nextIdPos != string::npos) {
            size_t start = content.find_first_of("0123456789", nextIdPos);
            size_t end = content.find_first_not_of("0123456789", start);
            if (start != string::npos) {
                nextId = stoi(content.substr(start, end - start));
            }
        }

        size_t booksPos = content.find("\"books\":");
        if (booksPos == string::npos) return;

        size_t arrayStart = content.find("[", booksPos);
        size_t arrayEnd = content.rfind("]");

        if (arrayStart == string::npos || arrayEnd == string::npos) return;

        string booksContent = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);

        size_t pos = 0;
        int bookCount = 0;

        while (pos < booksContent.length()) {
            size_t objStart = booksContent.find("{", pos);
            if (objStart == string::npos) break;

            size_t objEnd = booksContent.find("}", objStart);
            if (objEnd == string::npos) break;

            string bookObj = booksContent.substr(objStart, objEnd - objStart + 1);

            Book book;

            size_t idPos = bookObj.find("\"id\":");
            if (idPos != string::npos) {
                size_t numStart = bookObj.find_first_of("0123456789", idPos);
                size_t numEnd = bookObj.find_first_not_of("0123456789", numStart);
                book.id = stoi(bookObj.substr(numStart, numEnd - numStart));
            }

            size_t titlePos = bookObj.find("\"title\":");
            if (titlePos != string::npos) {
                size_t start = bookObj.find("\"", titlePos + 8) + 1;
                size_t end = bookObj.find("\"", start);
                book.title = unescapeJson(bookObj.substr(start, end - start));
            }

            size_t authorPos = bookObj.find("\"author\":");
            if (authorPos != string::npos) {
                size_t start = bookObj.find("\"", authorPos + 9) + 1;
                size_t end = bookObj.find("\"", start);
                book.author = unescapeJson(bookObj.substr(start, end - start));
            }

            size_t isbnPos = bookObj.find("\"isbn\":");
            if (isbnPos != string::npos) {
                size_t start = bookObj.find("\"", isbnPos + 7) + 1;
                size_t end = bookObj.find("\"", start);
                book.isbn = unescapeJson(bookObj.substr(start, end - start));
            }

            size_t qtyPos = bookObj.find("\"quantity\":");
            if (qtyPos != string::npos) {
                size_t numStart = bookObj.find_first_of("0123456789", qtyPos);
                size_t numEnd = bookObj.find_first_not_of("0123456789", numStart);
                book.quantity = stoi(bookObj.substr(numStart, numEnd - numStart));
            }

            books.push_back(book);
            bookCount++;

            pos = objEnd + 1;
        }

        if (bookCount > 0) {
            reindexBooks();
            cout << GREEN << "✅ Loaded " << bookCount << " books from memory!\n" << RESET;
        }
    }

    void run() {
        int choice;

        while (true) {
            printHeader();

            cout << GREEN << "1. Add Book\n";
            cout << YELLOW << "2. Edit Book\n";
            cout << RED << "3. Delete Book\n";
            cout << MAGENTA << "4. Search Book\n";
            cout << BLUE << "5. Show All Books\n";
            cout << CYAN << "6. Save to Memory \n";
            cout << RED << "7. Exit\n" << RESET;

            cout << CYAN << "\nEnter choice: " << RESET;

            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) addBook();
            else if (choice == 2) editBook();
            else if (choice == 3) deleteBook();
            else if (choice == 4) searchBook();
            else if (choice == 5) showAllBooks();
            else if (choice == 6) saveToMemory();
            else if (choice == 7) {
                cout << YELLOW << "\n💡 Don't forget to save before exiting!\n";
                cout << CYAN << "Save now? (y/n): " << RESET;
                char saveChoice;
                cin >> saveChoice;
                if (saveChoice == 'y' || saveChoice == 'Y') {
                    saveToMemory();
                }
                cout << GREEN << "Exiting...\n" << RESET;
                return;
            }
            else {
                cout << RED << "Invalid choice.\n" << RESET;
            }
        }
    }
};

int main() {
    LibrarySystem library;
    library.run();
    return 0;
}