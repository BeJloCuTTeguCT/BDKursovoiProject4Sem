#include "widget.h"
#include "ui_widget.h"
#include "mfiltermodel.h"
#include "accountsetting.h"
#include <qpushbutton.h>
#include <qsqlerror.h>


Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), _menubar(new QMenuBar(this)), auth(nullptr),
      _succes_auth(false), simple_search(new Search()), search(new Search(SearchType::Advanced))
{
    _FJson = new FileRW("..\\LibIS\\Configure.dat", this);
    _userRole = UserRole::Guest;

    ui->setupUi(this);
    QVBoxLayout *tab_lay = new QVBoxLayout(this);
    QVBoxLayout *tab2_lay = new QVBoxLayout(this);
    tab_lay->addWidget(simple_search);
    tab2_lay->addWidget(search);
    ui->tab->setLayout(tab_lay);
    ui->tab_2->setLayout(tab2_lay);
    connect(search, &Search::init_search, this, &Widget::querry_search);
    connect(simple_search, &Search::init_simple_search, this, &Widget::querry_simple_search);

    ui->verticalLayout->setMenuBar(_menubar);
    _authMN = _menubar->addAction("Авторизация");

    _updTable = (new QMenu(this))->addAction("");
    _updTable->setIconVisibleInMenu(true);
    _updTable->setIcon(QPixmap(":/icons/update"));
    _updTable->setShortcut(Qt::Key_F5);
    _menubar->addAction(_updTable);

    _menuPersAcc = new QMenu("&Личный кабинет", _menubar);
    _takenBook = _menuPersAcc->addAction("&Взятые книги");
    _takenBook->setCheckable(true);
    _returnBook = _menuPersAcc->addAction("&Прочитанные книги");
    _returnBook->setCheckable(true);
    QAction *PersSet = _menuPersAcc->addAction("&Настройки аккаунта");
    QAction *logout = _menuPersAcc->addAction("&Выйти из аккаунта");
    _menubar->addMenu(_menuPersAcc);

    _menuSet = new QMenu("&Настройки", _menubar);
    QAction *connectBD = _menuSet->addAction("&Подключение к БД");
    QAction *authSetting = _menuSet->addAction("&Настройка авторизации");
    _menubar->addMenu(_menuSet);

    _menuAdmin = new QMenu("&Редактирование", _menubar);
    _editMod = _menuAdmin->addAction("&Режим редактирования");
    _editMod->setCheckable(true);
    _editMod->setChecked(false);
    _editMod->setShortcut(Qt::CTRL | Qt::Key_E);
    QAction *addRow = _menuAdmin->addAction("&Добавить &книгу");
    QAction *createGenre = _menuAdmin->addAction("&Добавить &жанр");
    QAction *editGenre = _menuAdmin->addAction("&Отредактировать &жанр");
    QAction *rmRow = _menuAdmin->addAction("&Удалить &книгу");
    QAction *rmGenre = _menuAdmin->addAction("&Удалить &жанр");
    _menubar->addMenu(_menuAdmin);

    _updTable->setEnabled(false);
    _menuPersAcc->setEnabled(false);
    _menuSet->setEnabled(false);
    _menuAdmin->setEnabled(false);
    ui->tableView->setHidden(true);

    connect(ui->tableView, &QTableView::doubleClicked, this, &Widget::on_doubleClicked_tableView);
    connect(_authMN, &QAction::triggered, this, &Widget::authorization);
    connect(_updTable, &QAction::triggered, this, &Widget::update_table);
    connect(_takenBook, &QAction::triggered, this, &Widget::on_clicked_menu_takenBook);
    connect(_returnBook, &QAction::triggered, this, &Widget::on_clicked_menu_returnBook);
    connect(PersSet, &QAction::triggered, this, &Widget::on_clicked_menu_personSetting);
    connect(logout, &QAction::triggered, this, &Widget::on_clicked_menu_logout);
    connect(createGenre, &QAction::triggered, this, &Widget::on_clicked_menu_createGenre);
    connect(editGenre, &QAction::triggered, this, &Widget::on_clicked_menu_editGenre);
    connect(rmRow, &QAction::triggered, this, &Widget::on_clicked_menu_removeRow);
    connect(rmGenre, &QAction::triggered, this, &Widget::removeGenre);
    connect(connectBD, &QAction::triggered, this, &Widget::set_connect);
    connect(authSetting, &QAction::triggered, this, &Widget::set_auth_message);
    connect(addRow, &QAction::triggered, this, &Widget::addRow);
    connect(_editMod, &QAction::triggered, this, [this] () {if(ui->tableView->isHidden()){
                                                            ui->tableView->setHidden(false);
                                                            ui->start_image->setHidden(true);}});

    this->authorization();
}

Widget::~Widget()
{
    delete _FJson;
    delete search;
    delete simple_search;
    delete ui;
}

void Widget::authorization(bool reauth)
{
    auth = new Authorization(this);
    QStringList tmp(_FJson->getAuthPair(UserRole::Guest));
    auth->setConfigDB(_FJson->getConfigDB());
    auth->authorization(tmp.value(AuthPair::Login), tmp.value(AuthPair::Password));

        connect(auth, &Authorization::succes_authoriz, this, &Widget::succes_auth);
        connect(auth, &Authorization::closed, this, [this] () {delete this->auth;});

    if(_FJson->getBootAuthState() == "true" || reauth == true)
        auth->open();
    else
        auth->getLastUserRole(_FJson->getAuthPair(UserRole::Last));
}

void Widget::succes_auth(UserRole role, QStringList authPair)
{
    _userRole = role;
    _userLogin = authPair.value(AuthPair::Login);
    _FJson->setAuthPair(UserRole::Last, authPair);
    if (_FJson->getBootAuthState() == "false")
        _FJson->writeFile();
    if(auth != nullptr)
        delete auth;
    _succes_auth = true;

    _authMN->setVisible(false);
    _updTable->setEnabled(true);
    _menuSet->setEnabled(true);
    _menuPersAcc->setEnabled(true);
    if(_userRole == Admin)
        _menuAdmin->setEnabled(true);
    this->load_table();
}

void Widget::set_connect()
{
    QStringList mes(_FJson->getConfigDB());
    mes += _FJson->getAuthPair(_userRole);
    this->_setConDb = new SetConnectDB(mes, this);
    _setConDb->open();
    connect(_setConDb, &SetConnectDB::save_btn_clicked, this, &Widget::set_connect_apply);
    connect(_setConDb, &SetConnectDB::closed_window, this, [this] () {delete _setConDb;});
}

void Widget::set_connect_apply()
{
    QStringList configDB;
    configDB.append(_setConDb->get_host());
    configDB.append(_setConDb->get_port());
    configDB.append(_setConDb->get_DB_name());
    QStringList authPair;
    authPair.insert(AuthPair::Login, _setConDb->get_login());
    authPair.insert(AuthPair::Password, _setConDb->get_passwd());
    _FJson->setAuthPair(_userRole, authPair);
    _FJson->setConfigDB(configDB);
    _FJson->writeFile();
    delete _setConDb;
}

void Widget::set_auth_message()
{
    QString mes;
    if(_FJson->getBootAuthState() == "true")
        mes = "Сейчас авторизация при загрузке программы\n"
               "включена. Вы уверены, что хотите её отключить?";
    else
        mes = "Сейчас авторизация при загрузке программы\n"
               "выключена. Вы уверены, что хотите её включить?";
    _set_auth_msg = new QMessageBox(QMessageBox::Icon::Question, "Настройка авторизации", mes,
            QMessageBox::StandardButton::Apply | QMessageBox::StandardButton::Cancel, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    _set_auth_msg->show();
    _set_auth_msg->setAttribute(Qt::WA_DeleteOnClose);
    connect(_set_auth_msg, &QMessageBox::buttonClicked, this, &Widget::set_auth);
}

void Widget::set_auth(QAbstractButton *btn)
{
    QMessageBox::StandardButton button = _set_auth_msg->standardButton(btn);
    if(button == QMessageBox::StandardButton::Apply)
    {
        if(_FJson->getBootAuthState() == "true") {
            _FJson->setBootAuthState("false");
            _FJson->writeFile();
        }
        else {
            _FJson->setBootAuthState("true");
            QStringList temp = _FJson->getAuthPair(UserRole::Last);
            _FJson->setAuthPair(UserRole::Last, QStringList() << "" << "");
            _FJson->writeFile();
            _FJson->setAuthPair(UserRole::Last, temp);
        }
    }
}

void Widget::load_table()
{
    this->setCursor(Qt::BusyCursor);
    QStringList config = _FJson->getConfigDB();
    QStringList authPair = _FJson->getAuthPair(_userRole);
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(config.value(ConfigurateJson::Host));
    db.setPort(config.value(ConfigurateJson::Port).toInt());
    db.setDatabaseName(config.value(ConfigurateJson::NameDB));
    db.setUserName(authPair.value(AuthPair::Login));
    db.setPassword(authPair.value(AuthPair::Password));

    if (!db.open()) {
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Ошибка подключения", db.lastError().databaseText(),
                                        QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->setAttribute(Qt::WA_DeleteOnClose);
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
    _model_sort = new QSortFilterProxyModel(this);
    _model_sort->setSourceModel(_model);
    ui->tableView->setModel(_model_sort);
    ui->tableView->setColumnHidden(ColumnName::BookID, true);
    ui->tableView->horizontalHeader()->stretchLastSection();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSortingEnabled(true);

    _model_genre = new QSqlTableModel(this, db);
    _model_genre->setTable("genre");
    _model_genre->select();
    _model_genre->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    search->set_genre_list(_model_genre);
    this->setCursor(Qt::ArrowCursor);
}

void Widget::MesNoFind()
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Information, "Поиск",
            "Поиск по введеному запросу\nне дал результатов",
            QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->show();
}

void Widget::update_table()
{
    this->setCursor(Qt::BusyCursor);
    if(ui->tableView->isHidden()){
        ui->tableView->setHidden(false);
        ui->start_image->setHidden(true);
    }
    if(db.isOpen())
        _model->select();
    else
        load_table();
    this->setCursor(Qt::ArrowCursor);
}

void Widget::on_clicked_menu_takenBook()
{
    if (!_takenBook->isChecked()) {
        ui->tableView->setModel(_model_sort);
        ui->tableView->setColumnHidden(ColumnName::BookID, true);
        return;
    }
    if (_returnBook->isChecked()) {
        _returnBook->setChecked(false);
        ui->tableView->setColumnHidden(ColumnName::BookID, true);
    }

    QString query = QString("select * from print_taken_books_by_user_login('%1')").arg(_userLogin);
    QSqlQueryModel *takeBookModel = new QSqlQueryModel(this);
    takeBookModel->setQuery(query, db);
    takeBookModel->setHeaderData(0, Qt::Horizontal, "Название");
    takeBookModel->setHeaderData(1, Qt::Horizontal, "Автор");
    takeBookModel->setHeaderData(2, Qt::Horizontal, "Жанр");
    takeBookModel->setHeaderData(3, Qt::Horizontal, "Дата взятия");
    QSortFilterProxyModel *model = new QSortFilterProxyModel(this);
    model->setSourceModel(takeBookModel);
    ui->tableView->setModel(model);
    if(ui->tableView->isHidden()){
        ui->tableView->setHidden(false);
        ui->start_image->setHidden(true);
    }
}

void Widget::on_clicked_menu_returnBook()
{
    if (!_returnBook->isChecked()) {
        ui->tableView->setModel(_model_sort);
        ui->tableView->setColumnHidden(ColumnName::BookID, true);
        return;
    }
    if (_takenBook->isChecked()) {
        _takenBook->setChecked(false);
        ui->tableView->setColumnHidden(ColumnName::BookID, true);
    }

    QString query = QString("select * from print_retuns_books_by_user_login('%1')").arg(_userLogin);
    QSqlQueryModel *returnBookModel = new QSqlQueryModel(this);
    returnBookModel->setQuery(query, db);
    returnBookModel->setHeaderData(0, Qt::Horizontal, "Название");
    returnBookModel->setHeaderData(1, Qt::Horizontal, "Автор");
    returnBookModel->setHeaderData(2, Qt::Horizontal, "Жанр");
    returnBookModel->setHeaderData(3, Qt::Horizontal, "Дата взятия");
    returnBookModel->setHeaderData(4, Qt::Horizontal, "Дата возвращения");
    QSortFilterProxyModel *model = new QSortFilterProxyModel(this);
    model->setSourceModel(returnBookModel);
    ui->tableView->setModel(model);
    if(ui->tableView->isHidden()){
        ui->tableView->setHidden(false);
        ui->start_image->setHidden(true);
    }
}

void Widget::on_clicked_menu_personSetting()
{
    _accSet = new AccountSetting(db, _userLogin, this);
    _accSet->show();
    connect(_accSet, &AccountSetting::userLoginChanged, this, [this] (QString login) {this->_userLogin = login;});
    connect(_accSet, &AccountSetting::closed_window, this, [this] () {delete _accSet;});
}

void Widget::on_clicked_menu_logout()
{
    _succes_auth = false;
    this->db.close();
    ui->tableView->setHidden(true);
    ui->start_image->setHidden(false);
    _authMN->setVisible(true);
    _menuAdmin->setEnabled(false);
    _menuPersAcc->setEnabled(false);
    _menuSet->setEnabled(false);
    authorization(true);
}

void Widget::on_clicked_menu_createGenre()
{
    if (_userRole != Admin)
        return;
    if(_takenBook->isChecked() || _returnBook->isChecked())
        return;

    _editGenre = new EditGenre(TypeEditGenre::CreateGenre, this);
    _editGenre->setGenreList(_model_genre);
    _editGenre->show();
    connect(_editGenre, &EditGenre::closed_window, this, [this] () {delete _editGenre;});
}

void Widget::on_clicked_menu_editGenre()
{
    if (_userRole != Admin)
        return;
    if(_takenBook->isChecked() || _returnBook->isChecked())
        return;

    _editGenre = new EditGenre(TypeEditGenre::EditingGenre, this);
    _editGenre->setGenreList(_model_genre);
    _editGenre->show();
    connect(_editGenre, &EditGenre::closed_window, this, [this] () {delete _editGenre;});
}

void Widget::removeGenre()
{
    if (_userRole != Admin)
        return;
    _editGenre = new EditGenre(TypeEditGenre::RemoveGenre, this);
    _editGenre->setGenreList(_model_genre);
    _editGenre->show();
    connect(_editGenre, &EditGenre::closed_window, this, [this] () {delete _editGenre;});
}

void Widget::querry_simple_search(Accuracy accuracy, QString name)
{
    this->setCursor(Qt::BusyCursor);
    MFilterModel *proxyModel = new MFilterModel(this);
    proxyModel->setSourceModel(_model);
    proxyModel->setFilter(name, ColumnName::BookName, accuracy);
    ui->tableView->setModel(proxyModel);
    if(ui->tableView->isHidden()){
        ui->tableView->setHidden(false);
        ui->start_image->setHidden(true);
    }
    if(proxyModel->rowCount() == 0)
        MesNoFind();
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
    if(ui->tableView->isHidden()){
        ui->tableView->setHidden(false);
        ui->start_image->setHidden(true);
    }
    if(proxyModel->rowCount() == 0)
        MesNoFind();
    this->setCursor(Qt::ArrowCursor);
}

void Widget::addRow()
{
    if(_takenBook->isChecked() || _returnBook->isChecked())
        return; //обработка просмотра других табилиц

    _editRow = new EditRow(TypeEditRow::AddingRow, this);
    _editRow->setModel(ui->tableView->model());
    _editRow->setSave(&db);
    _editRow->setGenreList(_model_genre);
    _editRow->show();
    connect(_editRow, &EditRow::closed_window, this, [this] () {delete _editRow;});
}

void Widget::on_doubleClicked_tableView(const QModelIndex &index)
{
    if(!_editMod->isChecked() || _takenBook->isChecked() || _returnBook->isChecked())
        return; //обработка режима редактирования и просмотра других табилиц

    _editRow = new EditRow(TypeEditRow::EditingRow, this);
    if (dynamic_cast<QSqlTableModel*>(ui->tableView->model()) != NULL) {
        QSqlTableModel *temp = dynamic_cast<QSqlTableModel*>(ui->tableView->model());
        _editRow->setSave(&db, temp->record(index.row()).value(ColumnName::BookID).toString());
    }
    else
        if (dynamic_cast<MFilterModel*>(ui->tableView->model()) != NULL) {
            MFilterModel *temp = dynamic_cast<MFilterModel*>(ui->tableView->model());
            _editRow->setSave(&db, _model->record(temp->mapToSource(index).row()).value(ColumnName::BookID).toString());
        }
        else
            if (dynamic_cast<QSortFilterProxyModel*>(ui->tableView->model()) != NULL) {
                QSortFilterProxyModel *temp = dynamic_cast<QSortFilterProxyModel*>(ui->tableView->model());
                _editRow->setSave(&db, _model->record(temp->mapToSource(index).row()).value(ColumnName::BookID).toString());
            }
    _editRow->setModel(ui->tableView->model());
    _editRow->setCurentModelIndex(index);
    _editRow->setGenreList(_model_genre);
    _editRow->show();
    connect(_editRow, &EditRow::closed_window, this, [this] () {delete _editRow;});
}

void Widget::on_clicked_menu_removeRow()
{
    if (_userRole != Admin)
        return;
    if(_takenBook->isChecked() || _returnBook->isChecked())
        return;

    _editRow = new EditRow(TypeEditRow::RemovindRow, this);
    _editRow->setModel(ui->tableView->model());
    _editRow->setCurentModelIndex(ui->tableView->currentIndex());
    _editRow->setSave(&db);
    _editRow->show();
    connect(_editRow, &EditRow::closed_window, this, [this] () {delete _editRow; this->update_table();});
}

