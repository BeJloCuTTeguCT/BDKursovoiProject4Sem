#include "stylesheet.h"



QString StyleSheet::getStyleForSaveBtn()
{
    return "QPushButton {"
           "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 173, 136, 255), stop:1 rgba(0, 236, 75, 255));"
           "    border-radius: 14px;"
            "}";
}

QString StyleSheet::getStyleForCancelBtn()
{
    return "QPushButton {"
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
           "    border-radius: 10px;"
           "    padding-left: 5px;"
           "}";
}

//QString StyleSheet::getStyleForMenuBar()
//{
//    return "QMenuBar {"
//           "    color: rgb(255, 255, 255);"
//           "    background-color: rgb(86, 185, 209);"
//           "}";
//}

//QString StyleSheet::getStyleForMenu()
//{
//    return "QMenu {"
//           "    color: rgb(245, 245, 245);"
//           "    background-color: rgb(31, 143, 171);"
//           "}";
//}

QString StyleSheet::getStyleForSearchLine()
{
    return "QLineEdit {"
           "    background-color: rgb(225, 225, 225);"
           "    border-radius: 10px;"
           "    padding-left: 5px;"
           "}";
}

QString StyleSheet::getStyleForComboBox()
{
    return "QComboBox {"
           "border-radius: 10px;"
            "background: rgb(225, 225, 225);"
            "padding-left: 5px;"
        "}"

        "QComboBox::drop-down {"
        "subcontrol-origin: padding;"
            "subcontrol-position: top right;"
           "width: 20px;"
            "border-top-right-radius: 3px;"
           "border-bottom-right-radius: 3px;"
        "}"

        "QComboBox::down-arrow {"
            "image: url(:/icons/combobox);"
            "width: 16px;"
            "height: 16px;"
       "}";
}