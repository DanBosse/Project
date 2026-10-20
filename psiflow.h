#ifndef PSIFLOW_H
#define PSIFLOW_H

#include <stdint.h>

void psiflow_init(void);
int psiflow_execute_pipeline(const char* psi_stream);

#endif
