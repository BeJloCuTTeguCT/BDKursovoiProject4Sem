 #include "stylesheet.h"

QString StyleSheet::getStyleForSaveBtn()
{
    return "QPushButton {"
           "    min-height: 28px;"
           "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 173, 136, 255), stop:1 rgba(0, 236, 75, 255));"
           "    border-radius: 14px;"
           "}";
}

QString StyleSheet::getStyleForCancelBtn()
{
    return "QPushButton {"
           "    min-height: 28px;"
           "    background-color: rgb(225, 225, 225);"
           "    border-radius: 14px;"
           "}";
}

QString StyleSheet::getStyleForSearchBtn()
{
    return getStyleForSaveBtn();
}

QString StyleSheet::getStyleForEditLine()
{
    return "QLineEdit {"
           "    min-height: 28px;"
           "    border-radius: 10px;"
           "    padding-left: 5px;"
           "}";
}

QString StyleSheet::getStyleForErrorLineEdit()
{
    return "QLineEdit {"
           "    min-height: 28px;"
           "    color: rgb(255, 0, 0);"
           "    border-radius: 10px;"
           "    padding-left: 5px;"
           "}";
}

QString StyleSheet::getStyleForSearchLine()
{
    return "QLineEdit {"
           "    background-color: rgb(225, 225, 225);"
           "    border-radius: 10px;"
           "    padding-left: 5px;"
           "}";
}

QString StyleSheet::getStyleForSearchComboBox()
{
    return "QComboBox {"
        "    border-radius: 10px;"
        "    background: rgb(225, 225, 225);"
        "    padding-left: 5px;"
        "}"

        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-top-right-radius: 3px;"
        "    border-bottom-right-radius: 3px;"
        "}"

        "QComboBox::down-arrow {"
        "    image: url(:/icons/combobox);"
        "    width: 16px;"
        "    height: 16px;"
        "}";
}

QString StyleSheet::getStyleForComboBox()
{
    return "QComboBox {"
           "    min-height: 28px;"
           "    border-radius: 10px;"
           "    background: rgb(255, 255, 255);"
           "    padding-left: 5px;"
           "}"

           "QComboBox::drop-down {"
           "    subcontrol-origin: padding;"
           "    subcontrol-position: top right;"
           "    width: 20px;"
           "    border-top-right-radius: 3px;"
           "    border-bottom-right-radius: 3px;"
           "    }"

           "    QComboBox::down-arrow {"
           "    image: url(:/icons/combobox);"
           "    width: 16px;"
           "    height: 16px;"
           "}";
}
