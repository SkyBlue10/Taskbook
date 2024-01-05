#include "taskbook.h"
#include "addtaskwindow.h"
#include "addcategorywindow.h"
#include "category.h"
#include "menuwindow.h"
#include <QPushButton>
#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include "QSqlDatabase"


TaskBook::TaskBook(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.categories->currentWidget()->objectName();
    size = 0;
    complete_tasks = new CompleteTasksWindow(this);

    connect(ui.qpbAddTask, SIGNAL(clicked()), this, SLOT(addTask()));
    connect(ui.qpbAddCategory, SIGNAL(clicked()), this, SLOT(addCategory()));
    connect(ui.qpbSettings, SIGNAL(clicked()), this, SLOT(openSettings()));
    connect(ui.categories, &QTabWidget::tabCloseRequested, this, &TaskBook::onTabCloseRequested);
    connect(ui.qpbCompleteTasks, SIGNAL(clicked()), this, SLOT(openCompleteTasks()));

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "tasks_db");
    loadTasks();
}

void TaskBook::loadTasks()
{
    QSqlDatabase db = QSqlDatabase::database("tasks_db");
    db.open();

}

TaskBook::~TaskBook()
{
    for (auto pair : tasks) {
        delete pair.second;
    }
}

TaskBook* TaskBook::getInstance()
{
    if (!instance)
        instance = new TaskBook();
    return instance;
}

int TaskBook::getSize()
{
    return size;
}

void TaskBook::add(Task* task)
{
    tasks.emplace(size, task);
    size++;
}

void TaskBook::addComplete(Task* cTask)
{
    complete_tasks->addTask(cTask);
}

void TaskBook::addFromComplete(Task* for_all, Task* for_category)
{
    if (for_category == nullptr) {
        for_all->setId(this->getSize());
        for_all->setIdPair(-1);
        add(for_all);

        QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
        categoryAll->insertWidget(0, for_all);

        complete_tasks->del(for_all->getDate());

        return;
    }
    else {
        for_all->setId(this->getSize());
        add(for_all);
        for_category->setId(this->getSize());
        add(for_category);

        for_all->setIdPair(for_category->getId());
        for_category->setIdPair(for_all->getId());

        for (int i = 0; i < ui.categories->count(); i++)
        {
            if (ui.categories->widget(i)->objectName() == for_category->getCategory()) {
                Category* currentCategory = qobject_cast<Category*>(ui.categories->widget(i));
                currentCategory->addTask(for_category);
                break;
            }
        }
        QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
        categoryAll->insertWidget(0, for_all);
        complete_tasks->del(for_all->getDate());
    }
}

void TaskBook::addCategory()
{
    AddCategoryWindow dlg(ui.categories, this);
    switch (dlg.exec())
    {
    case QDialog::Accepted: {
        Category* newCategory = new Category(dlg.getQLEInputCategory());
        int index = ui.categories->addTab(newCategory, dlg.getQLEInputCategory());
        ui.categories->widget(index)->setObjectName(dlg.getQLEInputCategory());
        break;
    }
    case QDialog::Rejected: {
        break;
    }
    default:
        break;
    }
}


void TaskBook::addTask() {//переписать, использовав map tasks
    AddTaskWindow dlg(ui.categories->currentWidget()->objectName(), this);

    switch (dlg.exec())
    {
    case QDialog::Accepted: {
        QString nameWidget = ui.categories->currentWidget()->objectName();
        if (nameWidget == "qwAll") {
            Task* task = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), "Without category"
            );

            task->setId(TaskBook::getSize());
            task->setIdPair(-1);
            add(task);

            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
            layout->insertWidget(0, task);
        }
        else {
            Task* task_for_category_All = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), nameWidget
            );
            task_for_category_All->setId(TaskBook::getSize());
            add(task_for_category_All);

            Task* task_for_current_category = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), nameWidget
            );
            task_for_current_category->setId(TaskBook::getSize());
            add(task_for_current_category);


            task_for_current_category->setIdPair(task_for_category_All->getId());
            task_for_category_All->setIdPair(task_for_current_category->getId());

            QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
            Category* currentCategory = qobject_cast<Category*>(ui.categories->currentWidget());

            categoryAll->insertWidget(0, task_for_category_All);
            currentCategory->addTask(task_for_current_category);
        }
        break;
    }
    case QDialog::Rejected: {
        break;
    }
    default:
        break;
    }
}

void TaskBook::onTabCloseRequested(int index)
{
    //добавить окно с упоминанием, что все задачи этой категории не сохранятся
    if (ui.categories->widget(index)->objectName() == "qwAll") {
        return;
    }

    QWidget* widget = ui.categories->widget(index);
    QString category = ui.categories->widget(index)->objectName();
    int size = tasks.size();

    if (widget != nullptr) {
        for (int i = 0; i < getSize(); i += 2)
        {
            if (tasks.find(i) != tasks.end()) {
                if (category == tasks.at(i)->getCategory()) {
                    delTask(i);
                }
            }
        }
        ui.categories->removeTab(index);
        delete widget;
    }
}

void TaskBook::openSettings()
{
    MenuWindow dlg(this);
    dlg.exec();
}

void TaskBook::openCompleteTasks()
{
    complete_tasks->exec();
}

//void TaskBook::taskIsComplete(int id)
//{
//    if (tasks[id]->getCategory() == "All") {
//        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qwArea->layout());
//
//    }
//    else {
//
//    }
//    //QWidget* task = qobject_cast<QWidget*>(ui.);
//}
//
//void TaskBook::taskIsNotComplete(int id)
//{
//
//}

void TaskBook::delTask(int id)
{
    QWidget* deleted;
    int idPair = tasks.at(id)->getIdPair();
    if (idPair != -1) {
        deleted = qobject_cast<QWidget*>(tasks[idPair]);
        delete deleted;
        tasks.erase(idPair);
    }

    deleted = qobject_cast<QWidget*>(tasks[id]);
    delete deleted;
    tasks.erase(id);
}

void TaskBook::editTask(int id)
{
    int idPair = tasks.at(id)->getIdPair();
    tasks[idPair]->setName(tasks[id]->getName());
    tasks[idPair]->setText(tasks[id]->getText());
    tasks[idPair]->setDate(tasks[id]->getDate());
    tasks[idPair]->setImportant(tasks[id]->getImportant());
}