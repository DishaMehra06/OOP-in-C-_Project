#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

// Book class
class Book {
private:
    string title;
    string author;
    string isbn;
    int totalCopies;
    int borrowedCopies;

public:
    Book(string t, string a, string i, int copies) 
        : title(t), author(a), isbn(i), totalCopies(copies), borrowedCopies(0) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    int getAvailableCopies() const { return totalCopies - borrowedCopies; }

    void borrowBook() {
        if (getAvailableCopies() > 0) {
            borrowedCopies++;
            cout << "Book borrowed successfully!" << endl;
        } else {
            cout << "No copies available!" << endl;
        }
    }

    void returnBook() {
        if (borrowedCopies > 0) {
            borrowedCopies--;
            cout << "Book returned successfully!" << endl;
        } else {
            cout << "No borrowed copies to return!" << endl;
        }
    }

    void display() const {
        cout << setw(20) << left << title
             << setw(20) << author
             << setw(15) << isbn
             << setw(10) << getAvailableCopies()
             << endl;
    }

    void saveToFile(ofstream& file) const {
        file << title << "," << author << "," << isbn << "," 
             << totalCopies << "," << borrowedCopies << endl;
    }

    static Book loadFromFile(string data) {
        string t, a, i;
        int copies, borrowed;
        size_t pos = 0;

        pos = data.find(",");
        t = data.substr(0, pos); data.erase(0, pos + 1);

        pos = data.find(",");
        a = data.substr(0, pos); data.erase(0, pos + 1);

        pos = data.find(",");
        i = data.substr(0, pos); data.erase(0, pos + 1);

        pos = data.find(",");
        copies = stoi(data.substr(0, pos)); data.erase(0, pos + 1);

        borrowed = stoi(data);

        return Book(t, a, i, copies);
    }
};

// Library class
class Library {
private:
    vector<Book> books;

public:
    void addBook() {
        string title, author, isbn;
        int copies;
        cout << "Enter title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        cin >> isbn;
        cout << "Enter total copies: ";
        cin >> copies;

        books.emplace_back(title, author, isbn, copies);
        cout << "Book added successfully!" << endl;
    }

    void viewBooks() {
        if (books.empty()) {
            cout << "No books in the library!" << endl;
            return;
        }

        cout << setw(20) << left << "Title"
             << setw(20) << "Author"
             << setw(15) << "ISBN"
             << setw(10) << "Available"
             << endl;
        cout << string(65, '-') << endl;

        for (const auto& book : books)
            book.display();
    }

    void borrowBook() {
        string title;
        cout << "Enter the title of the book to borrow: ";
        cin.ignore();
        getline(cin, title);

        for (auto& book : books) {
            if (book.getTitle() == title) {
                book.borrowBook();
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    void returnBook() {
        string title;
        cout << "Enter the title of the book to return: ";
        cin.ignore();
        getline(cin, title);

        for (auto& book : books) {
            if (book.getTitle() == title) {
                book.returnBook();
                return;
            }
        }
        cout << "Book not found!" << endl;
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Error saving to file!" << endl;
            return;
        }

        for (const auto& book : books)
            book.saveToFile(file);

        file.close();
        cout << "Library data saved successfully!" << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "No saved data found. Starting fresh!" << endl;
            return;
        }

        books.clear();
        string line;
        while (getline(file, line)) {
            books.push_back(Book::loadFromFile(line));
        }

        file.close();
        cout << "Library data loaded successfully!" << endl;
    }
};

// Main function
int main() {
    Library library;
    library.loadFromFile("library_data.txt");

    int choice;
    do {
        cout << "\n--- Library Management System ---" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. View Books" << endl;
        cout << "3. Borrow Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Save and Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: library.addBook(); break;
            case 2: library.viewBooks(); break;
            case 3: library.borrowBook(); break;
            case 4: library.returnBook(); break;
            case 5: library.saveToFile("library_data.txt"); break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 5);

    return 0;
}
