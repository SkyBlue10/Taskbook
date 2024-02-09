#pragma once
#include <map>
#include <QWidget>
#include "ui_taskbook.h"
#include "task.h"
#include "completetaskswindow.h"

class TaskBook : public QWidget
{
    Q_OBJECT
public:
    ~TaskBook();

    static TaskBook* getInstance();

    void addComplete(Task* cTask, bool isNew = false);
    void addFromComplete(Task* for_all, Task* for_category, QString endDate);
    void delTask(int);
    void editTask(int);

    void writeInDB(Task* task);
    void delFromDB(Task* task);

public slots:
    void addNewTask();
    void addCategory();
    void onTabCloseRequested(int index);
    //void openSettings(); 
    void openCompleteTasks();

private:
    TaskBook(QWidget* parent = nullptr);
    static TaskBook* instance;
    CompleteTasksWindow* complete_tasks;

    Ui::TaskBookClass ui;
    std::map<int, Task*> tasks;

    void addTaskInForm(Task* task);
    void addTask(Task* task, Task* pairTask = nullptr);

    void loadData();
    void reindexing();

    //void debug_foo(Task* task); //метод для отладки, где я буду просматривать значения task. Позже убрать
};