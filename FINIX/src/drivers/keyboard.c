#include "keyboard.h"
#include <core/isr.h>
#include <core/ports.h>
#include <stdio.h>
#include <stdlib.h>
#include "sch/scheduler.h"

#define SCANCODE_MAX 57



char scancode_to_ascii(uint8_t scancode);


keyboard_buffer_t kb_buffer;


static void buffer_clear(void)
{
	kb_buffer.pointer = 0;
	for(int i =0 ;i<BUFFER_MAX;i++)
		kb_buffer.buffer[i] = '\0';
}

static void buffer_add(char c)
{
	if(kb_buffer.pointer < BUFFER_MAX-1)
		kb_buffer.buffer[kb_buffer.pointer++] = 'c';
}

static void buffer_show(void)
{
	for(int i = 0; i<kb_buffer.pointer;i++)
		printf("%c",kb_buffer.buffer[i]);
	printf("\n");
}

static void keyboard_callback(stack_t *stack)
{
    uint8_t scancode = port_byte_in(KEYBOARD_DATA_PORT);

    if (scancode > SCANCODE_MAX) {
        return;
    }
    if(scancode == 0x1c)
    {
    	buffer_clear();
    	printf("\n$:");
    }
    else if(scancode == 0x1d)
    {
	fork();
	sample_task();
    }
    else
    {
    	char c = scancode_to_ascii(scancode);
    	buffer_add(c);
    	printf("%c", c);
    }
    
	//printf("0x%x\n",scancode);
    UNUSED(*stack);
}

void keyboard_init()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
    buffer_clear();
}

char scancode_to_ascii(uint8_t scancode)
{
	char c;
	switch(scancode)
	{
		case 0x2:
		c = '1';
		break;

		case 0x3:
		c = '2';
		break;

		case 0x4:
		c = '3';
		break;

		case 0x5:
		c = '4';
		break;

		case 0x6:
		c = '5';
		break;

		case 0x7:
		c = '6';
		break;

		case 0x8:
		c = '7';
		break;

		case 0x9:
		c = '8';
		break;

		case 0xa:
		c = '9';
		break;

		case 0xb:
		c = '0';
		break;

		case 0x10:
		c = 'q';
		break;

		case 0x11:
		c = 'w';
		break;

		case 0x12:
		c = 'e';
		break;

		case 0x13:
		c = 'r';
		break;

		case 0x14:
		c = 't';
		break;

		case 0x15:
		c = 'y';
		break;

		case 0x16:
		c = 'u';
		break;

		case 0x17:
		c = 'i';
		break;

		case 0x18:
		c = 'o';
		break;

		case 0x19:
		c = 'p';
		break;

		case 0x1e:
		c = 'a';
		break;

		case 0x1f:
		c = 's';
		break;

		case 0x20:
		c = 'd';
		break;

		case 0x21:
		c = 'f';
		break;

		case 0x22:
		c = 'g';
		break;

		case 0x23:
		c = 'h';
		break;

		case 0x24:
		c = 'j';
		break;

		case 0x25:
		c = 'k';
		break;

		case 0x26:
		c = 'l';
		break;

		case 0x2c:
		c = 'z';
		break;

		case 0x2d:
		c = 'x';
		break;

		case 0x2e:
		c = 'c';
		break;

		case 0x2f:
		c = 'v';
		break;

		case 0x30:
		c = 'b';
		break;

		case 0x31:
		c = 'n';
		break;

		case 0x32:
		c = 'm';
		break;

		default:
		c = '?';
	}

	return c;
}
