#include<Arduino.h>
///////////////////////Funções do Filtro/////////////////////////////////////
typedef struct {
  double *historyNum;
  const double *tapsNum;
  unsigned int lastIndexNum;
  unsigned int filterOrderNum;
  double *historyDen;
  const double *tapsDen;
  unsigned int lastIndexDen;
  unsigned int filterOrderDen;
} DigitalFilter;
   
void DigitalFilter_init(DigitalFilter* f,const double *filterTapsNum,unsigned int filterOrderNum, const double *filterTapsDen,unsigned int filterOrderDen) {
  int i;

  f->lastIndexNum = 0;  
  f->tapsNum = filterTapsNum;
  f->filterOrderNum = filterOrderNum;
  f->historyNum = (double *) malloc(f->filterOrderNum*sizeof(double));
  for(i = 0; i < f->filterOrderNum; ++i) f->historyNum[i] = 0;

  f->lastIndexDen = 0;
  f->tapsDen = filterTapsDen;
  f->filterOrderDen =  filterOrderDen;
  f->historyDen = (double *) malloc(f->filterOrderDen*sizeof(double));
  for(i = 0; i < f->filterOrderDen; ++i) f->historyDen[i] = 0; 
}
  
double DigitalFilter_put(DigitalFilter* f, double input) {
  double acc = 0.0;
  unsigned int index, i;
  
  f->historyNum[f->lastIndexNum++] = input;
  if(f->lastIndexNum == f->filterOrderNum) f->lastIndexNum = 0;

  index= f->lastIndexNum;
  for(i = 0; i < f->filterOrderNum; ++i) {
    index = index != 0 ? index-1 : f->filterOrderNum-1;
    acc += f->historyNum[index] * f->tapsNum[i];
  }

  index = f->lastIndexDen;
  for(i = 0; i < f->filterOrderDen; ++i) {
    index = index != 0 ? index-1 : f->filterOrderDen-1;
    acc -= f->historyDen[index] * f->tapsDen[i];
  }

  f->historyDen[f->lastIndexDen++] = acc;
  if(f->lastIndexDen == f->filterOrderDen) f->lastIndexDen = 0;
 
  return acc;
}
  
double DigitalFilter_get(DigitalFilter* f) {
  return f->historyDen[f->lastIndexDen];
}