 #include "filerw.h"
#include <QMessageBox>
#include <QtWidgets>

void FileRW::errorRead()
{
    QMessageBox msg(QMessageBox::Icon::Critical, "Ошибка конфигурации",
                   "Ошибка при открытии конфигурационного\nфайла с параметрами соединения с БД",
                   QMessageBox::Ok, _parent, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg.show();
}

FileRW::FileRW(const QString &pathFile, QWidget *parent) :
    _pathFile(pathFile), _parent(parent)
{
    readFile();
}

void FileRW::readFile()
{
    this->_ConfigDB.clear();
    this->_admin.clear();
    this->_user.clear();
    this->_guest.clear();

    QFile file(_pathFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        errorRead();
        return;
    }
    file.seek(0);
    QString val = file.readAll();
    file.close();
    QByteArray temp = val.toUtf8();
    QJsonParseError jsonErr;
    QJsonDocument JDoc(QJsonDocument::fromJson(temp, &jsonErr));
    qDebug() << jsonErr.errorString();
    if(JDoc.isEmpty()) {
        errorRead();
        return;
    }

    QJsonObject jObj = JDoc.object();
    this->_BootAuthState = jObj.value("BootAuthState").toString();
    this->_ConfigDB.push_back(jObj.value("ConfigDB").toObject().value("host").toString());
    this->_ConfigDB.push_back(jObj.value("ConfigDB").toObject().value("port").toString());
    this->_ConfigDB.push_back(jObj.value("ConfigDB").toObject().value("nameDB").toString());
    this->_admin.push_back(jObj.value("AdminAuthPair").toObject().value("login").toString());
    this->_admin.push_back(jObj.value("AdminAuthPair").toObject().value("password").toString());
    this->_user.push_back(jObj.value("UserAuthPair").toObject().value("login").toString());
    this->_user.push_back(jObj.value("UserAuthPair").toObject().value("password").toString());
    this->_guest.push_back(jObj.value("GuestAuthPair").toObject().value("login").toString());
    this->_guest.push_back(jObj.value("GuestAuthPair").toObject().value("password").toString());
}

void FileRW::writeFile()
{
    QJsonObject jObj;
    QJsonObject *jObj_t = new QJsonObject;
    jObj.insert("BootAuthState", _BootAuthState);
    jObj_t->insert("host", _ConfigDB.value(0));
    jObj_t->insert("port", _ConfigDB.value(1));
    jObj_t->insert("nameDB", _ConfigDB.value(2));
    jObj.insert("ConfigDB", *jObj_t);
    delete jObj_t; jObj_t = new QJsonObject;
    jObj_t->insert("login", _admin.value(0));
    jObj_t->insert("password", _admin.value(1));
    jObj.insert("AdminAuthPair", *jObj_t);
    delete jObj_t; jObj_t = new QJsonObject;
    jObj_t->insert("login", _user.value(0));
    jObj_t->insert("password", _user.value(1));
    jObj.insert("UserAuthPair", *jObj_t);
    delete jObj_t; jObj_t = new QJsonObject;
    jObj_t->insert("login", _guest.value(0));
    jObj_t->insert("password", _guest.value(1));
    jObj.insert("GuestAuthPair", *jObj_t);
    delete jObj_t;

    QJsonDocument JDoc;
    JDoc.setObject(jObj);
    QFile file(_pathFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        errorRead();
        return;
    }
    file.write(JDoc.toJson());
    file.close();
}

QStringList FileRW::getAuthPair(UserRole role)
{
    switch (role) {
    case UserRole::Admin:
        return _admin;
        break;
    case UserRole::User:
        return _user;
        break;
    default:
        return _guest;
        break;
    }
}

void FileRW::setBootAuthState(const QString &state)
{
    _BootAuthState = state;
}

QString FileRW::getBootAuthState()
{
    return _BootAuthState;
}

QStringList FileRW::getConfigDB()
{
    return _ConfigDB;
}

void FileRW::setConfigDB(const QStringList &configuration)
{
    _ConfigDB.clear();
    _ConfigDB.append(configuration.value(0));
    _ConfigDB.append(configuration.value(1));
    _ConfigDB.append(configuration.value(2));
}

void FileRW::setAuthPair(UserRole role, const QStringList &pair)
{
    switch (role) {
    case UserRole::Admin:
        _admin.clear();
        _admin.append(pair.value(0));
        _admin.append(pair.value(1));
        break;
    case UserRole::User:
        _user.clear();
        _user.append(pair.value(0));
        _user.append(pair.value(1));
        break;
    default:
        _guest.clear();
        _guest.append(pair.value(0));
        _guest.append(pair.value(1));
        break;
    }
}
