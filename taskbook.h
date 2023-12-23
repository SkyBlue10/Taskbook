#pragma once
#include <map>
#include <QWidget>
#include "ui_taskbook.h"
#include "task.h"

class TaskBook : public QWidget
{
    Q_OBJECT
public:
    ~TaskBook();

    static TaskBook* getInstance();
    int getSize();

    void add(Task* task);
    void delTask(int);
    void editTask(int);




public slots:
    void addTask();
    void addCategory();
    void onTabCloseRequested(int index);
    void openSettings();

//signals:
//    void taskIsComplete(int);
//    void taskIsNotComplete(int);

private:
    TaskBook(QWidget* parent = nullptr);
    static TaskBook* instance;

    Ui::TaskBookClass ui;
    std::map<int, Task*> tasks;
    int size;//кол-во задач
};