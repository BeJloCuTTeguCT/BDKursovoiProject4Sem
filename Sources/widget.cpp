#include "widget.h"
#include "ui_widget.h"
#include "mfiltermodel.h"
#include <QMessageBox>
#include <QSqlDriver>
#include <QHeaderView>
#include <QFile>
#include <QByteArray>

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), _menubar(new QMenuBar), auth(nullptr),
      _succes_auth(false), simple_search(new Search()), search(new Search(SearchType::Advanced)),
      _editRow(new EditRow(this))
{
    ui->setupUi(this);
    QVBoxLayout *tab_lay = new QVBoxLayout();
    QVBoxLayout *tab2_lay = new QVBoxLayout();
    tab_lay->addWidget(simple_search);
    tab2_lay->addWidget(search);
    ui->tab->setLayout(tab_lay);
    ui->tab_2->setLayout(tab2_lay);
    connect(search, &Search::init_search, this, &Widget::querry_search);
    connect(simple_search, &Search::init_simple_search, this, &Widget::querry_simple_search);

    ui->verticalLayout->setMenuBar(_menubar);
    _menu = new QMenu("&Настройки", _menubar);
    _menu->setEnabled(false);
    QAction *connectBD = _menu->addAction("&Подключение к БД");
    QAction *authSetting = _menu->addAction("&Настройка авторизации");
    QAction *addRow = _menu->addAction("&Добавить книгу");
    _menubar->addMenu(_menu);
    _authMN =_menubar->addAction("Авторизация");
    connect(_authMN, &QAction::triggered, this, &Widget::authorization);
    connect(connectBD, &QAction::triggered, this, &Widget::set_connect);
    connect(authSetting, &QAction::triggered, this, &Widget::set_auth_message);
    connect(addRow, &QAction::triggered, this, &Widget::addRow);

    this->authorization();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::authorization()
{
    QFile file("..\\LibIS\\Setting.cfg");
    if(!file.open(QIODeviceBase::ReadOnly) || file.readAll().isEmpty()){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Ошибка конфигурации",
                "Ошибка при открытии конфигурационного\nфайла с параметрами соединения с БД",
                QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->show();
        return;
    }
    file.seek(0);
    configure_DB.append(QString(file.readLine()));
    configure_DB.append(QString(file.readLine()));
    configure_DB.append(QString(file.readLine()));
    configure_DB.append(QString(file.readLine()));
    configure_DB.append(QString(file.readLine()));
    configure_DB.append(QString(file.readLine()));
    file.close();
    if(configure_DB.value(ConfigurateFile::AuthActivate) == "true") {
        auth = new Authorization(this);
        auth->open();
        auth->authorization(configure_DB.value(ConfigurateFile::Login).chopped(2),
                            configure_DB.value(ConfigurateFile::Password).chopped(2));
        connect(auth, &Authorization::succes_authoriz, this, &Widget::succes_auth);
    }
    else
        succes_auth();
}

void Widget::succes_auth()
{
    if(auth != nullptr)
        delete auth;
    if(_authMN != nullptr)
        _menubar->removeAction(_authMN);
    _succes_auth = true;
    _menu->setEnabled(true);
    this->load_table(configure_DB);
}

void Widget::set_connect()
{
    this->_setConDb = new SetConnectDB(configure_DB,this);
    _setConDb->open();
    connect(_setConDb, &SetConnectDB::save_btn_clicked, this, &Widget::set_connect_apply);
    connect(_setConDb, &SetConnectDB::cancel_btn_clicked, this, &Widget::set_connect_cancel);
}

void Widget::set_connect_apply()
{
    configure_DB[ConfigurateFile::Host] = _setConDb->get_host() + "\r\n";
    configure_DB[ConfigurateFile::NameDB] = _setConDb->get_DB_name() + "\r\n";
    configure_DB[ConfigurateFile::Login] = _setConDb->get_login() + "\r\n";
    configure_DB[ConfigurateFile::Password] = _setConDb->get_passwd() + "\r\n";
    configure_DB[ConfigurateFile::Port] = _setConDb->get_port() + "\r\n";
    delete _setConDb;
    QFile file("..\\LibIS\\Setting.cfg");
    if(!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Ошибка конфигурации",
                "Ошибка при открытии конфигурационного\nфайла с параметрами соединения с БД",
                QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->show();
        return;
    }
    QString str;
    foreach(QString temp, configure_DB)
        str += temp;
    file.write(str.toUtf8());
    file.close();
}

void Widget::set_connect_cancel()
{
    delete this->_setConDb;
}

void Widget::set_auth_message()
{
    QString post;
    if(configure_DB.value(ConfigurateFile::AuthActivate) == "true")
        post = "Сейчас авторизация при загрузке программы\n"
               "включена. Вы уверены, что хотите её отключить?";
    else
        post = "Сейчас авторизация при загрузке программы\n"
               "выключена. Вы уверены, что хотите её включить?";
    _set_auth_msg = new QMessageBox(QMessageBox::Icon::Question, "Настройка авторизации", post,
            QMessageBox::StandardButton::Apply | QMessageBox::StandardButton::Cancel, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    _set_auth_msg->open();
    connect(_set_auth_msg, &QMessageBox::buttonClicked, this, &Widget::set_auth);
}

void Widget::set_auth(QAbstractButton *btn)
{
    QMessageBox::StandardButton button = _set_auth_msg->standardButton(btn);
    if(button == QMessageBox::StandardButton::Apply)
    {
        QFile file("..\\LibIS\\Setting.cfg");
        if(!file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)){
            QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Ошибка конфигурации",
                    "Ошибка при открытии конфигурационного\nфайла с параметрами соединения с БД",
                    QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
            msg->show();
            return;
        }
        QString str;
        foreach(QString temp, configure_DB)
            str += temp;
        if(configure_DB.value(ConfigurateFile::AuthActivate) == "true"){
            str = str.chopped(4);
            str += "false";
            configure_DB[ConfigurateFile::AuthActivate] = "fasle";
        }
        else {
            str = str.chopped(5);
            str += "true";
            configure_DB[ConfigurateFile::AuthActivate] = "true";
        }
        file.write(str.toUtf8());
        file.close();
    }
}

void Widget::load_table(const QStringList &configure_DB)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(configure_DB.value(ConfigurateFile::Host).chopped(2));
    db.setDatabaseName(configure_DB.value(ConfigurateFile::NameDB).chopped(2));
    db.setUserName(configure_DB.value(ConfigurateFile::Login).chopped(2));
    db.setPassword(configure_DB.value(ConfigurateFile::Password).chopped(2));
    db.setPort(configure_DB.value(ConfigurateFile::Port).chopped(2).toInt());
    if (!db.open()) {
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Ошибка подключения", db.lastError().databaseText(),
                                        QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->show();
        return;
    }
    _model = new QSqlTableModel(this, db);
    _model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    _model->setTable("full_info_of_book");
    _model->select();
    _model->setHeaderData(ColumnName::BookName, Qt::Horizontal, "Название");
    _model->setHeaderData(ColumnName::BookAuthor, Qt::Horizontal, "Автор");
    _model->setHeaderData(ColumnName::GenreName, Qt::Horizontal, "Жанр");
    _model->setHeaderData(ColumnName::BookPublishDate, Qt::Horizontal, "Год\nиздания");
    _model->setHeaderData(ColumnName::BookCount, Qt::Horizontal, "Количество\nкниг");
    QSortFilterProxyModel *model = new QSortFilterProxyModel(this);
    model->setSourceModel(_model);
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(ColumnName::BookID, true);
    ui->tableView->setColumnHidden(ColumnName::GenreID, true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSortingEnabled(true);

    QSqlTableModel *model_cb = new QSqlTableModel(this, db);
    model_cb->setTable("genre");
    model_cb->select();
    search->set_genre_list(model_cb);
}

void Widget::querry_simple_search(Accuracy accuracy, QString name)
{
    this->setCursor(Qt::BusyCursor);
    MFilterModel *proxyModel = new MFilterModel(this);
    proxyModel->setSourceModel(_model);
    proxyModel->setFilter(name, ColumnName::BookName, accuracy);
    ui->tableView->setModel(proxyModel);
    this->setCursor(Qt::ArrowCursor);
}

void Widget::querry_search()
{
    this->setCursor(Qt::BusyCursor);
    QList<Accuracy> intD = search->get_Accuracy();
    QList<QString> strD = search->get_DataSearch();
    QList<SearchColumnProp> columns;
    if(!strD.value(0).isEmpty())
        columns.push_back(SearchColumnProp(strD.value(0), ColumnName::BookName, intD.value(0)));
    if(!strD.value(1).isEmpty())
        columns.push_back(SearchColumnProp(strD.value(1), ColumnName::BookAuthor, intD.value(1)));
    if(!strD.value(2).isEmpty())
        columns.push_back(SearchColumnProp(strD.value(2), ColumnName::GenreName));
    if(!strD.value(3).isEmpty())
        columns.push_back(SearchColumnProp(strD.value(3), ColumnName::BookCount));
    if(!strD.value(4).isEmpty())
        columns.push_back(SearchColumnProp(strD.value(4), ColumnName::BookPublishDate));

    MFilterModel *proxyModel = new MFilterModel(this);
    proxyModel->setSourceModel(_model);
    proxyModel->setFilter(columns);
    ui->tableView->setModel(proxyModel);
    this->setCursor(Qt::ArrowCursor);
}

void Widget::addRow()
{
    return;
}

void Widget::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (dynamic_cast<QSqlTableModel*>(ui->tableView->model()) != NULL) {
        QSqlTableModel *temp = dynamic_cast<QSqlTableModel*>(ui->tableView->model());
        _editRow->setSave(temp->record(index.row()).value(ColumnName::BookID).toInt(),
                         temp->record(index.row()).value(ColumnName::GenreID).toInt(), &db);
    }
    else
        if (dynamic_cast<MFilterModel*>(ui->tableView->model()) != NULL) {
            MFilterModel *temp = dynamic_cast<MFilterModel*>(ui->tableView->model());
            _editRow->setSave(_model->record(temp->mapToSource(index).row()).value(ColumnName::BookID).toInt(),
                             _model->record(temp->mapToSource(index).row()).value(ColumnName::GenreID).toInt(), &db);
        }
        else
            if (dynamic_cast<QSortFilterProxyModel*>(ui->tableView->model()) != NULL) {
                QSortFilterProxyModel *temp = dynamic_cast<QSortFilterProxyModel*>(ui->tableView->model());
                _editRow->setSave(_model->record(temp->mapToSource(index).row()).value(ColumnName::BookID).toInt(),
                                 _model->record(temp->mapToSource(index).row()).value(ColumnName::GenreID).toInt(), &db);
            }
    _editRow->setModel(ui->tableView->model());
    _editRow->setCurentModelIndex(index);
    _editRow->show();
}

