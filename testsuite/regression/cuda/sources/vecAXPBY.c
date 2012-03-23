void VecAXPBY(int n, double a, double *x, double b, double *y) {

    register int i;

    /*@ begin Loop (
          transform CUDA(threadCount=16, cacheBlocks=True, pinHostMem=False, streamCount=2)
        for (i=0; i<=n-1; i++)
          y[i]=a*x[i]+b*y[i];
    ) @*/

    for (i=0; i<=n-1; i++)
        y[i]=a*x[i]+b*y[i];

    /*@ end @*/
}