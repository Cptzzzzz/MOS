#include <stdio.h>


#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))



void FCFS (int number_of_jobs,const int job_submitted_time [],const int job_required_time [],int job_sched_start [])
{
    int s=0;
    for(int i=0;i<number_of_jobs;i++){
        s=max(s,job_submitted_time[i]);
        job_sched_start[i]=s;
        s+=job_required_time[i];
    }
}

int heap[2005];
int heapl=0;
int arr1[2005],arr2[2005];
int greater(int a,int b)
{
    if(arr2[b]<arr2[a])return 1;
    if(arr2[b]==arr2[a]&&b<a)return 1;
    return 0;
}
int getn()
{
    int res=heap[1];
    heap[1]=heap[heapl];
    heapl--;
    int go=1,nextt=1;
    while(1){
        if(2*go<=heapl&&greater(heap[go],heap[2*go]))nextt=2*go;
        if(2*go+1<=heapl&&greater(heap[nextt],heap[2*go+1]))nextt=2*go+1;
        if(nextt==go)break;
        heap[go]^=heap[nextt]^=heap[go]^=heap[nextt];
        go=nextt;
    }
    // printf("%d\n",res);
    return res;
}
int addn(int now)
{
    heap[++heapl]=now;
    int go=heapl;
    while(go>1&&greater(heap[go/2],heap[go])){
        heap[go]^=heap[go/2]^=heap[go]^=heap[go/2];
        go>>=1;
    }
}
void SJF (int number_of_jobs,const int job_submitted_time [],const int job_required_time [],int job_sched_start [])
{
    int s=0;
    for(int i=0;i<number_of_jobs;i++){
        arr1[i]=job_submitted_time[i];
        arr2[i]=job_required_time[i];
    }
    for(int i=0;i<number_of_jobs;i++){
        while(job_submitted_time[i]>s){
            if(heapl==0){
                s=job_submitted_time[i];
            }else{
                int now=getn();
                job_sched_start[now]=s;
                s+=job_required_time[now];
            }
        }
        addn(i);
    }
    while(heapl){
        int now=getn();
        job_sched_start[now]=s;
        s+=job_required_time[now];
    }
}