#include "task_switcher.h"


TaskSwitcher::TaskSwitcher() : taskCount(0) {
}
    

void TaskSwitcher::begin(long timerInterruptInuSecs) {
  noInterrupts();

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  OCR2A = (16e6 / 256L * timerInterruptInuSecs) / 1e6 - 1;            

  TCCR2B |= (1 << WGM22);
  TCCR2B |= (1 << CS22); 
  TIMSK2 |= (1 << OCIE2A);

  interrupts();
}


char TaskSwitcher::createTask(void (*t)(), int interval, char execucoes, bool ativa, void (*inicio)(), void (*fim)()) {
  taskList[taskCount++] = {t, interval, 0, READY, execucoes, ativa, inicio, fim};
  return (taskCount-1);
}


void TaskSwitcher::ativaTask(char idxTask, int interval, char execucoes) {
  void (*task)();
  noInterrupts(); 
  taskList[idxTask].ativa = true;
  taskList[idxTask].status = READY;
  taskList[idxTask].execucoes = execucoes;
  if (interval != 0){
    taskList[idxTask].interval = interval;
  }
  interrupts();
  task = taskList[idxTask].inicioDaTask;
  (*task)();
}


void TaskSwitcher::desativaTask(char idxTask) {
  void (*task)();
  noInterrupts(); 
  taskList[idxTask].ativa = false;
  taskList[idxTask].status = WAIT;
  interrupts();
  task = taskList[idxTask].fimDaTask;
  (*task)();
}

    
void TaskSwitcher::runCurrentTask() {
  int i;
  void (*task)();
  for (i=0; i<  taskCount; i++) {
    if ((taskList[i].ativa) && (taskList[i].status == READY)) {
      noInterrupts();
      if (taskList[i].execucoes != 0){
        if(--taskList[i].execucoes == 0){
          taskList[i].ativa = false;
            taskList[i].status = WAIT;
            taskList[i].current_time = 0;
            task = taskList[i].fimDaTask;
            (*task)();
            continue;
        }
      }
      taskList[i].status = WAIT;
      taskList[i].current_time = 0;
      interrupts();
      task = taskList[i].task;
      (*task)();
    } 
  } 
}
    

void TaskSwitcher::updateTickCounter() {
  int i;
  for (i=0; i< taskCount; i++) {
    if ((taskList[i].ativa) && (taskList[i].status == WAIT)) {
      taskList[i].current_time++;
      if (taskList[i].current_time >= taskList[i].interval) {
        taskList[i].status = READY;
      }
    }
  } 
}


TaskSwitcher TaskController;


ISR(TIMER2_COMPA_vect) {
   TaskController.updateTickCounter();
   TaskController.runCurrentTask();
}
