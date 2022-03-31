#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


LEAF(_my_getchar)

lui t1 ,0x1000
lui t2 ,0
loop:
lb t0 ,t1
beq t0 t2 loop
sw t0 ,t1
li t0 ,10
sw t ,t1
move v0 ,t0

END(my_getchar)

LEAF(_my_putchar)
lui t1 ,0x1000
sw a0 t1
END(_my)putchar)

LEAF(_my_exit)
lui t1 ,0x1000
add t1 ,t1 0x10
sw t0 ,t1
END(_my_exit)
