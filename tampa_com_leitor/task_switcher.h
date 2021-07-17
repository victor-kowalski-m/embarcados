#ifndef TASK_SWITCHER_H_INCLUDED
#define TASK_SWITCHER_H_INCLUDED

#include "definicoes_sistema.h"

typedef struct {
void (*task)();
int          interval;
long          current_time;
bool          status;
char          execucoes;
bool          ativa;
} TaskControl;

class TaskSwitcher {
    public:
    TaskSwitcher();
    void begin(long timerInterruptInuSecs);
    TaskControl *createTask(void (*t)(), int interval, char execucoes, bool ativa);
    void ativaTask(TaskControl *task, int interval, char execucoes);
    void desativaTask(TaskControl *task);
    void runCurrentTask();
    void updateTickCounter();

    private:
    TaskControl taskList[MAX_TAREFAS];
    int taskCount;
};

extern TaskSwitcher TaskController;

#endif // TASK_SWITCHER_H_INCLUDED
