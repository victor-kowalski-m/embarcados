#ifndef TASK_SWITCHER_H_INCLUDED
#define TASK_SWITCHER_H_INCLUDED

#include <Arduino.h>
#include "definicoes_sistema.h"

#define READY 1
#define WAIT  0

typedef struct {
  void          (*task)();
  int           interval;
  long          current_time;
  bool          status;
  char          execucoes;
  bool          ativa;
  void          (*inicioDaTask)();
  void          (*fimDaTask)();
} TaskControl;

class TaskSwitcher {
  public:
    TaskSwitcher();
    void begin(long timerInterruptInuSecs);
    char createTask(void (*t)(), int interval, char execucoes, bool ativa,  void (*inicio)(), void (*fim)());
    void ativaTask(char idxTask, int interval, char execucoes);
    void desativaTask(char idxTask);
    void runCurrentTask();
    void updateTickCounter();

  private:
    TaskControl taskList[MAX_TAREFAS];
    int taskCount;
};

extern TaskSwitcher TaskController;

#endif
