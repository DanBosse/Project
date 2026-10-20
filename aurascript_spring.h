#ifndef AURASCRIPT_SPRING_H
#define AURASCRIPT_SPRING_H

void aurascript_spring_init(void);
int aurascript_register_spring(const char* name, float mass, float stiffness, float damping);

#endif
