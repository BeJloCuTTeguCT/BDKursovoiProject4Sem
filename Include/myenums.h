#ifndef MYENUMS_H
#define MYENUMS_H

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
    BookID
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
    Guest,
    Last
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
