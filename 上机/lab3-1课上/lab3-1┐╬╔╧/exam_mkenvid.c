u_int mkenvid(struct Env *e)
{
    /*Hint: lower bits of envid hold e's position in the envs array. */
    u_int idx = (u_int)e - (u_int)envs;
    idx /= sizeof(struct Env);

    /*Hint: avoid envid being zero. */
    return (1 << (LOG2NENV)) | idx;  //LOG2NENV=10
}