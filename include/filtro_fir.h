#include <Arduino.h>
///////////////////////Funções do Filtro/////////////////////////////////////
typedef struct
{
  double *history;
  const double *taps;
  unsigned int last_index;
  unsigned int filterOrder;
} DigitalFilter;

void DigitalFilter_init(DigitalFilter *f, const unsigned int filterOrder, const double *const filterTaps)
{
  f->history = (double *)malloc(filterOrder * sizeof(double));
  for (unsigned int i = 0; i < filterOrder; ++i)
    f->history[i] = 0;
  f->taps = filterTaps;
  f->filterOrder = filterOrder;
  f->last_index = 0;
}

void DigitalFilter_put(DigitalFilter *f, const double input)
{
  f->history[f->last_index++] = input;
  if (f->last_index == f->filterOrder)
    f->last_index = 0;
}

double DigitalFilter_get(DigitalFilter *f)
{
  double acc = 0;
  unsigned int index = f->last_index, i;
  for (i = 0; i < f->filterOrder; ++i)
  {
    index = index != 0 ? index - 1 : f->filterOrder - 1;
    acc += f->history[index] * f->taps[i];
  };
  return acc;
}