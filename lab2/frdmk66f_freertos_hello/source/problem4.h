#ifndef PROBLEM4_H
#define PROBLEM4_H

#define LEFT_BIT (1 << 0)
#define RIGHT_BIT (1 << 1)
#define UP_BIT (1 << 2)
#define DOWN_BIT (1 << 3)

typedef struct {
	SemaphoreHandle_t* sems;
	char* dir;
} Task_Input;

// TODO: ADD FUNC PROTOTYPES

#endif /* PROBLEM4_H */
