#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define BUFFER_MAX 100

void keyboard_init();

typedef struct keyboard_buffer
{
	int pointer;
	char buffer[BUFFER_MAX];
} keyboard_buffer_t;


#endif
/*

*/