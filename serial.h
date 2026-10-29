#ifndef SERIAL_H
#define SERIAL_H

void serial_init(void);
void serial_write(char a);
void serial_print(const char* str);
char serial_read(void);
int serial_received(void);

#endif
