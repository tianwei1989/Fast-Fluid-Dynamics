///////////////////////////////////////////////////////////////////////////////
//
// Filename ffd.h
//
// Task: Header file of ffd.c
//
// Modification history:
// 7/20/2013 by Wangda Zuo: First implementation
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _FFD_H
#define _FFD_H
#endif

#ifndef _DATA_STRUCTURE_H
#define _DATA_STRUCTURE_H
#include "data_structure.h"
#endif

DWORD WINAPI ffd(PVOID p);

int allocate_data (void);

static void display_func(void);

static void idle_func(void);

static void key_func(unsigned char key, int x, int y);

static void motion_func(int x, int y);

static void mouse_func(int button, int state, int x, int y);

static void reshape_func(int width, int height);