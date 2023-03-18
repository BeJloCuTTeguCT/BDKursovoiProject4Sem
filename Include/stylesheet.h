 #ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>

class StyleSheet
{
public:
    static QString getStyleForSaveBtn();
    static QString getStyleForCancelBtn();
    static QString getStyleForSearchBtn();
    static QString getStyleForEditLine();
//    static QString getStyleForMenuBar();
    static QString getStyleForErrorLineEdit();
    static QString getStyleForSearchLine();
    static QString getStyleForSearchComboBox();
    static QString getStyleForComboBox();
};

#endif // STYLESHEET_H
