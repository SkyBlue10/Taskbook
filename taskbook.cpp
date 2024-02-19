//my code
#include "taskbook.h"
#include "addtaskwindow.h"
#include "addcategorywindow.h"
#include "category.h"
//#include "menuwindow.h"
#include "LogSystem.h"

//std libs
#include <Windows.h>
#include <fstream>
#include <string>

//QtWidgets
#include <QPushButton>
#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>

//QtSql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>


TaskBook::TaskBook(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    LogSystem::Open();
    LogSystem::Write("Launching the program");
    ui.qpbSettings->hide(); //прячем кнопку настроек, она для информатички не понадобится
    ui.categories->currentWidget()->objectName();
    complete_tasks = new CompleteTasksWindow(this);

    connect(ui.qpbAddTask, SIGNAL(clicked()), this, SLOT(addNewTask())); //соединение кнопки добавления задачи с слотом addTask
    connect(ui.qpbAddCategory, SIGNAL(clicked()), this, SLOT(addCategory())); //соединение кнопки добавления категории с слотом addCategory
    //connect(ui.qpbSettings, SIGNAL(clicked()), this, SLOT(openSettings())); //соединение кнопки открытия настроек с слотом openSettings
    connect(ui.categories, &QTabWidget::tabCloseRequested, this, &TaskBook::onTabCloseRequested); //соединения события закрытия вкладки категории с слотом onTabCloseRequested
    connect(ui.qpbCompleteTasks, SIGNAL(clicked()), this, SLOT(openCompleteTasks())); //соединение кнопки открытия окна завершённых задач с слотом openCompleteTasks

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); //добавляем базу
    db.setDatabaseName("./data/tasks.sqlite"); //ставим имя базе (путь к бд)

    loadData(); //загрузка данных из БД 
}

void TaskBook::writeInDB(Task* task)
{
    LogSystem::Write("Call TaskBook::writeInDB(Task* task)");
    LogSystem::Write(task, "Saving a task to the database");
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    if (task->isComplete()) {
        bool i = query.exec("INSERT INTO Complete VALUES('" + QString::number(task->getId()) + "', '" + QString::number(task->getPairId()) + "', '"
            + task->getName() + "', '" + task->getText() + "', '" + task->getDate() + "', '" + task->getImportant()
            + "', '" + task->getCategory() + "', '" + task->getEndDate() + "');");
        std::string str = i ? "true" : "false";
        LogSystem::Write("Saving a completed task to a database: " + str);
    }
    else {
        bool i = query.exec("INSERT INTO Tasks VALUES('" + QString::number(task->getId()) + "', '" + QString::number(task->getPairId()) + "', '"
            + task->getName() + "', '" + task->getText() + "', '" + task->getDate() + "', '" + task->getImportant()
            + "', '" + task->getCategory() + "');");
        std::string str = i ? "true" : "false";
        LogSystem::Write("Saving a noncompleted task to a database: " + str);
    }
    db.close();
    LogSystem::Write("End of the call TaskBook::writeInDB(Task* task)\n");
}

void TaskBook::delFromDB(Task* task)
{
    LogSystem::Write("Call TaskBook::delFromDB(Task* task)");
    LogSystem::Write(task, "Deleting a task from the database");
    //debug_foo(task);
    QString table = task->isComplete() ? "Complete" : "Tasks";
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    bool i = query.exec("DELETE FROM " + table + " WHERE id = '" + QString::number(task->getId()) + "';");
    std::string str = i ? "true" : "false";
    LogSystem::Write("Deleting a task from a database: " + str);
    db.close();
    LogSystem::Write("End of the call TaskBook::delFromDB(Task* task)\n");
}

void TaskBook::loadData()
{
    LogSystem::Write("Start loading categories from file");
    Category* new_category;
    std::fstream file;
    file.open("./data/categories", std::ios::in);
    if (!(file.peek() == EOF)) 
    {
        std::string str;
        while (!file.eof())
        {
            std::getline(file, str);
            if (str.empty() || str == "All")
                continue;
            new_category = new Category((QString)str.c_str());
            ui.categories->addTab(new_category, (QString)str.c_str());
        }
    }
    file.close();
    LogSystem::Write("The categories were uploaded successfully");
    LogSystem::Write("Start loading data from the database");

    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    query.exec("SELECT * FROM Tasks;");
    QSqlRecord rec = query.record();

    while (query.next())
    {
        Task* task_for_category = nullptr;
        Task* task_for_all = new Task(query, rec);
        tasks.emplace(task_for_all->getId(), task_for_all);
        LogSystem::Write(task_for_all, "The task has been loaded: ");
        //debug_foo(task_for_all);

        if (task_for_all->getPairId() != -1) 
        {
            query.next();
            task_for_category = new Task(query, rec);
            tasks.emplace(task_for_category->getId(), task_for_category);
            LogSystem::Write(task_for_category, "The task has been loaded: ");
            //debug_foo(task_for_category);
        }
        addTaskInForm(task_for_all);
    }

    query.exec("SELECT * FROM Complete;");
    rec = query.record();
    while (query.next())
    {
        Task* task = new Task(query, rec);
        task->setEndDate(query.value(rec.indexOf("endDate")).toString());
        task->prepare_complete_task();
        LogSystem::Write(task, "The completed task has been loaded: ");
        addComplete(task);
        //debug_foo(task);
    }
    db.close();
    LogSystem::Write("The task upload was successful\n");
}

void TaskBook::reindexing()
{
    LogSystem::Write("The start of reindexing tasks");
    std::string true_or_false;
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    int i = 0;
    for (auto iter = tasks.begin(); iter != tasks.end(); iter++) //меняем значение тасков в БД
    {
        if (iter->second->getPairId() == -1) {
            bool a = query.exec("UPDATE Tasks SET id = '" + QString::number(i) + "' WHERE id = '" + QString::number(iter->second->getId()) + "';");
            true_or_false = a ? "true" : "false";
            LogSystem::Write("Has the task been updated inside the database: " + true_or_false);
            LogSystem::Write(iter->second, "A new task: ");
        }
        else {
            LogSystem::Write(iter->second, "Updating a task inside the database");
            bool a = query.exec("UPDATE Tasks SET id = '" + QString::number(i) + "', pair_id = '" + QString::number(i + 1)
                + "' WHERE id = '" + QString::number(iter->second->getId()) + "';");
            true_or_false = a ? "true" : "false";
            LogSystem::Write("Has the task been updated inside the database: " + true_or_false);
            LogSystem::Write(iter->second, "A new task: ");
            iter++;
            a = query.exec("UPDATE Tasks SET id = '" + QString::number(i + 1) + "', pair_id = '" + QString::number(i)
                + "' WHERE id = '" + QString::number(iter->second->getId()) + "';");
            true_or_false = a ? "true" : "false";
            LogSystem::Write("Has the task been updated inside the database: " + true_or_false);
            LogSystem::Write(iter->second, "A new task: ");
            i++;
        }
        i++;
    }
    db.close();
    LogSystem::Write("The reindexation of the tasks was successful\n");
}

TaskBook::~TaskBook()
{
    LogSystem::Write("The start of closing the program");
    reindexing(); //переназначение индексов внутри БД
    QSqlDatabase::removeDatabase("./data/tasks.sqlite");
    for (auto pair : tasks) {
        LogSystem::Write(pair.second, "Deleting a task from memory");
        delete pair.second;
        LogSystem::Write("Deleting the task from memory was successful");
    }
    tasks.clear();
    LogSystem::Write("The memory release was successful");
    LogSystem::Write("End of the program");
    LogSystem::Close();
}

TaskBook* TaskBook::getInstance()
{
    if (!instance)
        instance = new TaskBook();
    return instance;
}

void TaskBook::addTaskInForm(Task* task)
{
    LogSystem::Write("Call TaskBook::addTaskInForm(Task* task)");
    LogSystem::Write(task, "Adding a task to a window");
    LogSystem::Write(task, "Adding a task to the All category");
    //debug_foo(task);
    //if (task->getPairId() != -1)
        //debug_foo(tasks[task->getPairId()]);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
    layout->insertWidget(0, task);
    LogSystem::Write(task, "Adding a task to the All category was successful");

    if (task->getPairId() != -1) 
    {
        for (int i = 0; i < ui.categories->count(); i++)
        {
            if (ui.categories->widget(i)->objectName() == tasks[task->getPairId()]->getCategory()) {
                LogSystem::Write(task, "Adding a task to the category " + ui.categories->widget(i)->objectName().toStdString());
                Category* currentCategory = qobject_cast<Category*>(ui.categories->widget(i));
                currentCategory->addTask(tasks[task->getPairId()]);
                LogSystem::Write(task, "Adding a task to the category " + ui.categories->widget(i)->objectName().toStdString() + " was successful");
                break;
            }
        }
    }
    LogSystem::Write("End of the call TaskBook::addTaskInForm(Task* task)\n");
}

void TaskBook::addTask(Task* task, Task* pairTask)
{
    LogSystem::Write("Call TaskBook::addTask(Task* task, Task* pairTask)");
    //debug_foo(task);
    //debug_foo(pairTask);
    LogSystem::Write(task, "Adding a task to memory");
    tasks.empty() ? task->setId(0) : task->setId((--tasks.end())->first + 1); //если в tasks нет задач, то задаём задаче id = 0
    task->setPairId(-1);
    tasks.emplace(task->getId(), task);
    LogSystem::Write(task, "Adding a task to memory was successful");
    if (pairTask != nullptr) 
    {
        LogSystem::Write(pairTask, "Adding a task to memory");
        pairTask->setId((--tasks.end())->first + 1);
        tasks.emplace(pairTask->getId(), pairTask);

        task->setPairId(pairTask->getId());
        pairTask->setPairId(task->getId());
        LogSystem::Write(pairTask, "Adding a task to memory was successful");
    }
    LogSystem::Write("End of the call TaskBook::addTask(Task* task, Task* pairTask)\n");
}

void TaskBook::addComplete(Task* cTask, bool isNew)
{
    LogSystem::Write("Call TaskBook::addComplete(Task* cTask, bool isNew)");
    LogSystem::Write(cTask, "Adding a task to the completed group");
    //debug_foo(cTask);
    if (isNew)
        writeInDB(cTask);

    complete_tasks->addTask(cTask);
    LogSystem::Write(cTask, "Adding a task to the completed group was successful");
    LogSystem::Write("End of the call TaskBook::addComplete(Task* cTask, bool isNew)\n");
}

void TaskBook::addFromComplete(Task* for_all, Task* for_category, QString endDate) //---------------------------------------------------
{
    LogSystem::Write("Call TaskBook::addFromComplete(Task* for_all, Task* for_category, QString endDate)");
    if (for_category == nullptr) {
        addTask(for_all);

        QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
        categoryAll->insertWidget(0, for_all);

        complete_tasks->del(endDate);
        writeInDB(for_all);
    }
    else {
        bool presenceOfCategory = false;
        for (int i = 0; i < ui.categories->count(); i++)
        {
            if (ui.categories->widget(i)->objectName() == for_category->getCategory()) { //проверка наличия категории задачи
                presenceOfCategory = true;
                LogSystem::Write("Category " + for_all->getCategory().toStdString() + " does not exist");
                break;
            }
        }
        if (!presenceOfCategory)  //если категория по какой то причине отсутствует
        {
            Category* currentCategory = new Category(for_all->getCategory(), true);
            ui.categories->addTab(currentCategory, for_all->getCategory());
        }
        addTask(for_all, for_category);
        for (int i = 0; i < ui.categories->count(); i++)
        {
            if (ui.categories->widget(i)->objectName() == for_category->getCategory()) {
                Category* currentCategory = qobject_cast<Category*>(ui.categories->widget(i));
                currentCategory->addTask(for_category);
                break;
            }
        }
        LogSystem::Write(for_all, "Adding a task to the All category");
        QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
        categoryAll->insertWidget(0, for_all);
        complete_tasks->del(endDate);
        
        writeInDB(for_all);
        writeInDB(for_category);
    }
    LogSystem::Write("End of the call TaskBook::addFromComplete(Task* for_all, Task* for_category, QString endDate)\n");
}

void TaskBook::addCategory()
{
    LogSystem::Write("Call TaskBook::addCategory()");
    LogSystem::Write("The user creates a new category");
    AddCategoryWindow dlg(ui.categories, this);
    switch (dlg.exec())
    {
    case QDialog::Accepted: {
        LogSystem::Write("The user has created a new category " + dlg.getQLEInputCategory().toStdString());
        Category* newCategory = new Category(dlg.getQLEInputCategory(), true);
        ui.categories->addTab(newCategory, dlg.getQLEInputCategory());
        break;
    }
    case QDialog::Rejected: {
        LogSystem::Write("The user canceled the creation of the category");
        break;
    }
    default:
        break;
    }
    LogSystem::Write("End of the call TaskBook::addCategory()\n");
}


void TaskBook::addNewTask() {
    LogSystem::Write("Call TaskBook::addNewTask()");
    LogSystem::Write("The user creates a new task");
    AddTaskWindow dlg(ui.categories->currentWidget()->objectName(), this);

    switch (dlg.exec())
    {
    case QDialog::Accepted: {
        QString nameCategory = ui.categories->currentWidget()->objectName();
        if (nameCategory == "All") {
            Task* task = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), "All"
            );
            LogSystem::Write(task, "The user has created a new task for the All category");
            addTask(task);

            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
            layout->insertWidget(0, task);
            writeInDB(task);
        }
        else {
            Task* task_for_category_All = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), nameCategory
            );
            Task* task_for_current_category = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), nameCategory
            );

            LogSystem::Write(task_for_category_All, "The user has created a new task for the category " + nameCategory.toStdString());

            addTask(task_for_category_All, task_for_current_category);

            QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
            Category* currentCategory = qobject_cast<Category*>(ui.categories->currentWidget());

            categoryAll->insertWidget(0, task_for_category_All);
            currentCategory->addTask(task_for_current_category);
            writeInDB(task_for_category_All);
            writeInDB(task_for_current_category);
        }
        break;
    }
    case QDialog::Rejected: {
        LogSystem::Write("The user rejected the creation of a new task");
        break;
    }
    default:
        break;
    }
    LogSystem::Write("End of the call TaskBook::addNewTask()\n");
}

void TaskBook::onTabCloseRequested(int index)
{
    LogSystem::Write("Call TaskBook::onTabCloseRequested(int index)");
    QMessageBox msgBox;
    if (ui.categories->widget(index)->objectName() == "All") {
        msgBox.setText("You cannot delete the \"All\" category");
        msgBox.setWindowTitle("Deleting the \"All\" category");
        msgBox.setIcon(QMessageBox::Icon::Warning);
        msgBox.exec();
        return;
    }

    msgBox.setWindowTitle("Are you sure?");
    msgBox.setIcon(QMessageBox::Icon::Question);
    msgBox.setText("Are you sure you want to delete this category?");
    msgBox.setInformativeText("Tasks in this category will be deleted");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No)
        return;
    LogSystem::Write("start deleting a category");
    QWidget* widget = ui.categories->widget(index);
    QString category = ui.categories->widget(index)->objectName();
    LogSystem::Write("the user deletes the category " + category.toStdString());

    if (widget != nullptr) {
        for (auto iter = tasks.begin(); iter != tasks.end();)
        {
            if (category == iter->second->getCategory()) 
            {
                LogSystem::Write(iter->second, "Deleting a task");
                delFromDB(iter->second);
                delete iter->second;
                iter = tasks.erase(iter);
                LogSystem::Write("Deleting a task was successful");
            }
            else
                ++iter;
        }
        LogSystem::Write("Removing a category from ui.categories");
        ui.categories->removeTab(index);
        delete widget;
        LogSystem::Write("Removing a category from ui.categories was successful");
    }

    LogSystem::Write("Deleting a category from a file categories");
    std::vector<std::string> buffer;
    std::string str;
    std::fstream file;
    file.open("./data/categories", std::ios::in);
    while (!file.eof())
    {
        getline(file, str);
        if (str != category.toStdString() && !str.empty())
            buffer.push_back(str);
    }
    file.close();
    file.open("./data/categories", std::ios::out);
    for (std::string line : buffer) {
        file << line << std::endl;
    }
    file.close();
    LogSystem::Write("Deleting a category from a file categories was successful");
    LogSystem::Write("End of the call TaskBook::onTabCloseRequested(int index)\n");
}

//void TaskBook::openSettings()
//{
//    LogSystem::Write("Open settings");
//    MenuWindow dlg(this);
//    dlg.exec();
//    LogSystem::Write("Close settings");
//}

void TaskBook::openCompleteTasks()
{
    LogSystem::Write("Call TaskBook::openCompleteTasks()");
    complete_tasks->exec();
    LogSystem::Write("End of the call TaskBook::openCompleteTasks()\n");
}

void TaskBook::delTask(int id)
{
    LogSystem::Write("Call TaskBook::delTask(int id), id = " + std::to_string(id));
    QWidget* deleted;
    int idPair = tasks.at(id)->getPairId();
    if (idPair != -1) {
        LogSystem::Write(tasks[idPair], "Deleting a task");
        delFromDB(tasks[idPair]);
        deleted = qobject_cast<QWidget*>(tasks[idPair]);
        delete deleted;
        tasks.erase(idPair);
        LogSystem::Write("Deleting a task was complete");
    }

    LogSystem::Write(tasks[id], "Deleting a task");
    delFromDB(tasks[id]);
    deleted = qobject_cast<QWidget*>(tasks[id]);
    delete deleted;
    tasks.erase(id);
    LogSystem::Write("Deleting a task was complete");
    LogSystem::Write("End of the call TaskBook::delTask(int id)\n");
}

void TaskBook::editTask(int id)
{
    LogSystem::Write("Call TaskBook::editTask(int id)");
    int idPair = tasks.at(id)->getPairId();
    tasks[idPair]->setName(tasks[id]->getName());
    tasks[idPair]->setText(tasks[id]->getText());
    tasks[idPair]->setDate(tasks[id]->getDate());
    tasks[idPair]->setImportant(tasks[id]->getImportant());
    delFromDB(tasks[idPair]);
    writeInDB(tasks[idPair]);
    LogSystem::Write(tasks[idPair], "Editing a task was successful");
    LogSystem::Write("End of the call TaskBook::editTask(int id)\n");
}

//void TaskBook::debug_foo(Task* task) {
//    if (task != nullptr) {
//        int id = task->getId();
//        int id_pair = task->getPairId();
//        QString name = task->getName();
//        QString text = task->getText();
//        QString date = task->getDate();
//        QString imp = task->getImportant();
//        QString category = task->getCategory();
//        bool isComplete = task->isComplete();
//    }
//}