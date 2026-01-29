#include <iostream>
#include <string>
#include "sqlite3.h" // Local header

using namespace std;

// Function to handle database errors
void checkError(int rc, char* errMsg) {
    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

int main() {
    sqlite3* DB;
    char* errMsg = 0;
    
    // 1. Open/Create Database
    int exit = sqlite3_open("library.db", &DB);
    
    // 2. Create Table automatically if it doesn't exist
    string sql = "CREATE TABLE IF NOT EXISTS BOOKS("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "TITLE TEXT NOT NULL, "
                 "AUTHOR TEXT NOT NULL, "
                 "STATUS TEXT DEFAULT 'Available');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    checkError(exit, errMsg);

    int choice;
    while (true) {
        cout << "\n--- VS Code Library System ---\n";
        cout << "1. Add Book\n2. View Books\n3. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string title, author;
            cout << "Title: "; cin.ignore(); getline(cin, title);
            cout << "Author: "; getline(cin, author);
            
            string insertSql = "INSERT INTO BOOKS (TITLE, AUTHOR) VALUES ('" + title + "', '" + author + "');";
            exit = sqlite3_exec(DB, insertSql.c_str(), NULL, 0, &errMsg);
            if(exit == SQLITE_OK) cout << "Saved to database!\n";
        } 
        else if (choice == 2) {
            // Simple callback to print data
            auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
                for (int i = 0; i < argc; i++) {
                    cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
                }
                cout << endl;
                return 0;
            };
            sqlite3_exec(DB, "SELECT * FROM BOOKS;", callback, NULL, NULL);
        }
        else if (choice == 3) break;
    }

    sqlite3_close(DB);
    return 0;
}