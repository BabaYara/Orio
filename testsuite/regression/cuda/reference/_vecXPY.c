void VecXPY(int n, double *x, double *y) {

    register int i;
    int lb = 4;

    /*@ begin Loop (
          transform CUDA(threadCount=16, cacheBlocks=True, pinHostMem=False, streamCount=2)
        for (i=lb; i<=n-1; i++)
          y[i]=x[i]+y[i];
    ) @*/
    {
      /*declare variables*/
      double *dev_y, *dev_x;
      int nthreads=16;
      int nstreams=2;
      /*calculate device dimensions*/
      dim3 dimGrid, dimBlock;
      dimBlock.x=nthreads;
      dimGrid.x=(n+nthreads-1)/nthreads;
      /*create streams*/
      int istream, soffset;
      cudaStream_t stream[nstreams+1];
      for (istream=0; istream<=nstreams; istream++ ) 
        cudaStreamCreate(&stream[istream]);
      int chunklen=n/nstreams;
      int chunkrem=n%nstreams;
      /*allocate device memory*/
      int nbytes=n*sizeof(double);
      cudaMalloc((void**)&dev_y,nbytes);
      cudaHostRegister(y,nbytes,cudaHostRegisterPortable);
      cudaMalloc((void**)&dev_x,nbytes);
      cudaHostRegister(x,nbytes,cudaHostRegisterPortable);
      /*copy data from host to device*/
      for (istream=0; istream<nstreams; istream++ ) {
        soffset=istream*chunklen;
        cudaMemcpyAsync(dev_y+soffset,y+soffset,chunklen*sizeof(double),cudaMemcpyHostToDevice,stream[istream]);
        cudaMemcpyAsync(dev_x+soffset,x+soffset,chunklen*sizeof(double),cudaMemcpyHostToDevice,stream[istream]);
      }
      if (chunkrem!=0) {
        soffset=istream*chunklen;
        cudaMemcpyAsync(dev_y+soffset,y+soffset,chunkrem*sizeof(double),cudaMemcpyHostToDevice,stream[istream]);
        cudaMemcpyAsync(dev_x+soffset,x+soffset,chunkrem*sizeof(double),cudaMemcpyHostToDevice,stream[istream]);
      }
      cudaEvent_t start, stop;
      cudaEventCreate(&start);
      cudaEventCreate(&stop);
      cudaEventRecord(start,0);
      /*invoke device kernel*/
      int orcu_var1=lb;
      int blks4chunk=dimGrid.x/nstreams;
      if (dimGrid.x%nstreams!=0) 
        blks4chunk++ ;
      for (istream=0; istream<nstreams; istream++ ) {
        soffset=istream*chunklen;
        orcu_kernel4<<<blks4chunk,dimBlock,0,stream[istream]>>>(chunklen,orcu_var1,dev_y+soffset,dev_x+soffset);
      }
      if (chunkrem!=0) {
        soffset=istream*chunklen;
        orcu_kernel4<<<blks4chunk,dimBlock,0,stream[istream]>>>(chunkrem,orcu_var1,dev_y+soffset,dev_x+soffset);
      }
      cudaEventRecord(stop,0);
      cudaEventSynchronize(stop);
      cudaEventElapsedTime(&orcu_elapsed,start,stop);
      cudaEventDestroy(start);
      cudaEventDestroy(stop);
      /*copy data from device to host*/
      for (istream=0; istream<nstreams; istream++ ) {
        soffset=istream*chunklen;
        cudaMemcpyAsync(y+soffset,dev_y+soffset,chunklen*sizeof(double),cudaMemcpyDeviceToHost,stream[istream]);
      }
      if (chunkrem!=0) {
        soffset=istream*chunklen;
        cudaMemcpyAsync(y+soffset,dev_y+soffset,chunkrem*sizeof(double),cudaMemcpyDeviceToHost,stream[istream]);
      }
      for (istream=0; istream<=nstreams; istream++ ) 
        cudaStreamSynchronize(stream[istream]);
      cudaHostUnregister(y);
      cudaHostUnregister(x);
      for (istream=0; istream<=nstreams; istream++ ) 
        cudaStreamDestroy(stream[istream]);
      /*free allocated memory*/
      cudaFree(dev_y);
      cudaFree(dev_x);
    }
/*@ end @*/
}
__global__ void orcu_kernel4(int n, int orcu_var1, double* y, double* x) {
  int tid=blockIdx.x*blockDim.x+threadIdx.x+orcu_var1;
  __shared__ double shared_y[16];
  __shared__ double shared_x[16];
  if (tid<=n-1) {
    shared_y[threadIdx.x]=y[tid];
    shared_x[threadIdx.x]=x[tid];
    shared_y[threadIdx.x]=shared_x[threadIdx.x]+shared_y[threadIdx.x];
    y[tid]=shared_y[threadIdx.x];
  }
}
