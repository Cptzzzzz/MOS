/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>

struct my_struct{
	int size;
	char c;
	int array[3];
};
int main()
{
	printf("main.c:\tmain is start ...\n");
	struct my_struct pp;
	pp.size=3;
	pp.c='Q';
	pp.array[0]=5;
	pp.array[1]=-7;
	pp.array[2]=-13;
	printf("%-4T",&pp);
	mips_init();
	panic("main is over is error!");

	return 0;
}
