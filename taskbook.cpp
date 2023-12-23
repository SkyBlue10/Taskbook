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


TaskBook::TaskBook(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.categories->currentWidget()->objectName();
    connect(ui.qpbAddTask, SIGNAL(clicked()), this, SLOT(addTask()));
    connect(ui.qpbAddCategory, SIGNAL(clicked()), this, SLOT(addCategory()));
    connect(ui.qpbSettings, SIGNAL(clicked()), this, SLOT(openSettings()));
    connect(ui.categories, &QTabWidget::tabCloseRequested, this, &TaskBook::onTabCloseRequested);
    size = 0;
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

void TaskBook::addCategory()
{
    AddCategoryWindow dlg(ui.categories, this);
    switch (dlg.exec())
    {
    case QDialog::Accepted: {
        Category* newCategory = new Category();
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
            task->setIdPair(NULL);
            add(task);

            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());
            layout->insertWidget(0, task);
        }
        else {
            Task* task_for_current_category = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), nameWidget
            );
            task_for_current_category->setId(TaskBook::getSize());
            add(task_for_current_category);

            Task* task_for_category_All = new Task(
                dlg.getQLENameTask(), dlg.getQLETextTask(), dlg.getDatePeriodEnding(), dlg.getCBImportant(), nameWidget
            );
            task_for_category_All->setId(TaskBook::getSize());
            add(task_for_category_All);


            task_for_category_All->setIdPair(task_for_current_category->getId());
            task_for_current_category->setIdPair(task_for_category_All->getId());

            Category* currentCategory = qobject_cast<Category*>(ui.categories->currentWidget());
            QVBoxLayout* categoryAll = qobject_cast<QVBoxLayout*>(ui.qsaArea->widget()->layout());

            currentCategory->addTask(task_for_current_category);
            categoryAll->insertWidget(0, task_for_category_All);
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
    if (idPair != NULL) {
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