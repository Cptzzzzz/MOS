#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

void self_check() {
	int i, j;
	struct Env* root;
	env_alloc(&root, 0);
	printf(">>> ROOT ALLOC OK: %x\n", root->env_id);
	assert(lab3_get_sum(root) == 1);
	for (i = 0;i < 4;i++) {
	u_int son = fork(root);
	printf(">>> SON FORK OK: %x\n", son);
	assert(lab3_get_sum(son) == 1);
	for (j = 0;j < 2;j++) {
		struct Env* subsonparent;
		assert(envid2env(son, &subsonparent, 0) == 0);
		int subson = fork(subsonparent);
		printf(">>> SUB SON FORK OK: %x\n", subson);
	}
		assert(lab3_get_sum(son) == 3);
		lab3_output(son);
	}
	printf(">>> ROOT SUM: %d\n", lab3_get_sum(root));
	assert(lab3_get_sum(root->env_id) == 13);
	printf("【EXAM PASSED】");
}
void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();

	mips_vm_init();
	page_init();

	env_init();
	env_check();
	load_icode_check();
	
	/*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
	 * code_b.c*/
	/*** exercise 3.9 ***/
	/*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
	 * interesting, have fun please*/
	ENV_CREATE_PRIORITY(user_A, 2);
	ENV_CREATE_PRIORITY(user_B, 1);

	self_check();
	// trap_init();
	// kclock_init();
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}

void bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	max = dst + len;
	// copy machine words while possible
	while (dst + 3 < max)
	{
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len)
{
	void *max;

	max = b + len;

	//printf("init.c:\tzero from %x to %x\n",(int)b,(int)max);

	// zero machine words while possible

	while (b + 3 < max)
	{
		*(int *)b = 0;
		b+=4;
	}

	// finish remaining 0-3 bytes
	while (b < max)
	{
		*(char *)b++ = 0;
	}

}
