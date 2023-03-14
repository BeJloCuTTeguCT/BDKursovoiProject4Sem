#ifndef MYENUMS_H
#define MYENUMS_H
#include <QString>

enum ConfigurateFile {
    Host,
    NameDB,
    Login,
    Password,
    Port,
    AuthActivate
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

struct TempTable {

    const QString _BookName;
    const QString _BookAuthor;
    const QString _GenreName;
    const int _BookPublishDate;
    const int _BookCount;
    const int _BookID;
    const int _GenreID;
    TempTable(QString BookName, QString BookAuthor, QString GenreName, int BookPublishDate,
            int BookCount, int BookID, int GenreID) :
        _BookName(BookName), _BookAuthor(BookAuthor), _GenreName(GenreName), _BookPublishDate(BookPublishDate),
        _BookCount(BookCount), _BookID(BookID), _GenreID(GenreID) {};
};

#endif // MYENUMS_H
