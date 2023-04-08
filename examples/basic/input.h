#ifndef __INPUT_H__
#define __INPUT_H__

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void);
int getch();

#endif