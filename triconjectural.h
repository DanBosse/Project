#ifndef TRICONJECTURAL_H
#define TRICONJECTURAL_H

#include <stdint.h>

void triconjectural_init(void);
void triconjectural_search_data(const char* query);
void triconjectural_search_net(const char* query);
void triconjectural_search_os(const char* query);

#endif // TRICONJECTURAL_H
