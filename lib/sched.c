#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
int next_index(int x)
{
    return (x+1)%3;
}
int weight_point(int x)
{
    if(x==0)return 1;
    if(x==1)return 2;
    return 4;
}
void sched_yield(void)
{
    // printf("hh\n");
    printf("\n");
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    struct Env *e;
    count--;
    int nextpoint;
    if(count<=0||curenv==NULL||curenv->env_status!=ENV_RUNNABLE){
        if(curenv!=NULL){
            if((curenv->env_pri)%2==1){
                nextpoint=next_index(point);
            }else{
                nextpoint=next_index(next_index(point));
            }
            LIST_INSERT_TAIL(&env_sched_list[nextpoint],curenv,env_sched_link);
        }
        int state=0;
        while(state==0){
            LIST_FOREACH(e,&env_sched_list[point],env_sched_link){
                if((e->env_status==ENV_RUNNABLE)&&((e->env_pri)>0)){
                    LIST_REMOVE(e,env_sched_link);
                    count=(e->env_pri)*weight_point(point);
                    env_run(e);
                    state=1;
                    break;
                }
            }
            point=next_index(point);
        }
    } 
    env_run(curenv);  
    
    /*
    count--;

    if(count<=0||curenv==NULL){
        if(curenv!=NULL){
            LIST_INSERT_HEAD(&env_sched_list[1-point],curenv,env_sched_link);
        }
        if(LIST_EMPTY(&env_sched_list[point])){
            point=1-point;
        }
        e=LIST_FIRST(&env_sched_list[point]);
        LIST_REMOVE(e,env_sched_link);
        count=e->env_pri;
        env_run(e);
    }
    env_run(curenv);
*/
    // if (count <= 0) {
	// 	do {
	// 		if (LIST_EMPTY(&env_sched_list[point])) {
	// 			point = 1 - point;
	// 		}

	// 		e = LIST_FIRST(&env_sched_list[point]);

	// 		if (e != NULL) {
	// 			LIST_REMOVE(e, env_sched_link);
	// 			LIST_INSERT_TAIL(&env_sched_list[1 - point], e, env_sched_link);
	// 			count = e->env_pri;
	// 		}
	// 	} while (e == NULL || e->env_status != ENV_RUNNABLE);
	// }
	// count --;
	// env_run(e);
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
}
