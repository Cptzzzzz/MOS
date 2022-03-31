#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


LEAF(_my_getchar)

lui t1 ,0x1000
li t2 ,0
loop:
lb t0 ,0(t1)
beq t0, t2, loop
sb t0 ,0(t1)
li t0 ,10
sb t0 ,0(t1)
add v0,t0,0
add v1,t0,0

END(_my_getchar)

LEAF(_my_putchar)
lui t1 ,0x1000
sb a0, 0(t1)
END(_my_putchar)

LEAF(_my_exit)
lui t1 ,0x1000
add t1 ,t1 ,0x10
sb t0 ,0(t1)
END(_my_exit)
