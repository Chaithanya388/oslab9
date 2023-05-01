#include "console.h"
#include "portmap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>	
#include <stdio.h>	
int terminal_position = 0;
char* const VGA_BUFFER = (char*) 0xb8000;

#include <stdio.h>
#include <stdlib.h>

void itoa(char* buffer, int toPrint);

void print_integer(int toPrint) {
    char buffer[12]; // buffer size is enough for 32-bit integers
    itoa(buffer, toPrint);
    printf("%s", buffer);
}

void itoa(char* buffer, int toPrint) {
    int i = 0;
    int isNegative = 0;
    if (toPrint < 0) {
        isNegative = 1;
        toPrint = -toPrint;
    }
    do {
        buffer[i++] = toPrint % 10 + '0';
        toPrint /= 10;
    } while (toPrint > 0);
    if (isNegative) {
        buffer[i++] = '-';
    }
    buffer[i] = '\0';
    // Reverse the string
    int j = 0;
    if (isNegative) {
        j = 1;
    }
    for (; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}


void update_cursor(int terminal_position)
{	
	uint16_t cursor_position = terminal_position >> 1;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (cursor_position));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) (cursor_position >> 8));
}

uint16_t get_cursor()
{
	uint16_t cursor_position = 0;
	outb(0x3D4, 0x0F);
	cursor_position |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	cursor_position |= ((uint16_t)inb(0x3D5)) << 8;
	return cursor_position;

}
void clear_screen()
{
	for(int i=0; i< VGA_TOTAL_BYTES; i+=2)
		{
			VGA_BUFFER[i] = 0;
			VGA_BUFFER[i+1] = 0x07;
		}
	terminal_position = 0;

}

void print_character(char c)
{
	if(c == '\n')
	{ terminal_position = (VGA_BYTES_PER_ROW - (terminal_position % VGA_BYTES_PER_ROW)) + terminal_position;

		return;
	}
	VGA_BUFFER[terminal_position] = c;
	VGA_BUFFER[++terminal_position] = 0x07;
	terminal_position = terminal_position + 1;

	

}

void print_string(char* str)
{
	for (int i=0; str[i] != '\0'; i++)
	{
		print_character(str[i]);
		update_cursor(terminal_position);
		get_cursor();
		
	}
	
}


void print_line(char* str)
{
	print_string(str);
	print_character('\n');	

}

int len(int toPrint)
{
	if (toPrint == 0)
	return 1;
	
	else
	{
		int num_digits =0;
		while (toPrint > 0)
			{
				toPrint /= 10;
				num_digits ++;
			}
		return num_digits;
	}
	
}


void strrev(char *arr, int start, int end)
{
    char temp;

    if (start >= end)
        return;

    temp = *(arr + start);
    *(arr + start) = *(arr + end);
    *(arr + end) = temp;

    start++;
    end--;
    strrev(arr, start, end);
}

char* itoa(int number, char *arr, int base)
{
    int i = 0, r, negative = 0;

    if (number == 0)
    {
        arr[i] = '0';
        arr[i + 1] = '\0';
        return arr;
    }

    if (number < 0 && base == 10)
    {
        number *= -1;
        negative = 1;
    }

    while (number != 0)
    {
        r = number % base;
        arr[i] = (r > 9) ? (r - 10) + 'a' : r + '0';
        i++;
        number /= base;
    }


    strrev(arr, 0, i - 1);

    arr[i] = '\0';

    return arr;
}
void print_integer(int toPrint)

{	
	
	int l = len(toPrint);
	char buffer [l];
	for (int i=l; i==0; i--)
	{
		buffer[i]= toPrint/10;
		toPrint /= 10;
	}
	char* x = itoa(toPrint,buffer,10);
	print_string(x);
}








































