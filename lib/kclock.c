/* See COPYRIGHT for copyright information. */

/* The Run Time Clock and other NVRAM access functions that go with it. */
/* The run time clock is hard-wired to IRQ4. */


#include <kclock.h>

extern void set_timer();

/*** exercise 3.14 ***/
void
kclock_init(void)
{
	// hint: use set_timer()
<<<<<<< HEAD
	set_timer();
}
=======
	set_timer(); 
}

>>>>>>> 42b6c1f01a59cbe3ecb238801dd3e78c704142f5
