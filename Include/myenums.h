#ifndef MYENUMS_H
#define MYENUMS_H
#include <QString>

enum ConfigurateJson {
     Host,
    Port,
    NameDB
};

enum ColumnName {
    BookName,
    BookAuthor,
    GenreName,
    BookPublishDate,
    BookCount,
    BookID,
    GenreID
};

enum Accuracy {
    Same,
    Contains
};

enum SearchType{
    Advanced,
    Simple
};

enum TypeEditRow {
    AddingRow,
    EditingRow,
    RemovindRow
};

enum UserRole {
    Admin,
    User,
    Guest
};

enum AuthPair {
    Login,
    Password
};

enum TypeEditGenre {
    CreateGenre,
    EditingGenre,
    RemoveGenre
};

#endif // MYENUMS_H
