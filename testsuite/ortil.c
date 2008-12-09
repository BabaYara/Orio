
//--------------------------------------------------
// 1-d Jacobi
//--------------------------------------------------
/*@ begin OrTil ((t,i):3) @*/

if (N >= 4) {
  for (t=0;t<=T-1;t++) {
    B[1]=0.33333*(A[1+1]+A[1]+A[1 -1]) ;
    for (i=2*t+2;i<=2*t+N-2;i++) {
      B[-2*t+i]=0.33333*(A[1+-2*t+i]+A[-2*t+i]+A[-2*t+i-1]) ;
      A[-2*t+i-1]=B[-2*t+i-1] ;
    }
    A[N-2]=B[N-2] ;
  }
 }
if (N == 3) {
  for (t=0;t<=T-1;t++) {
    B[1]=0.33333*(A[1+1]+A[1]+A[1 -1]) ;
    A[1]=B[1] ;
  }
 }

/*@ end @*/


//--------------------------------------------------
// LU Decomposition
//--------------------------------------------------
/*@ begin OrTil ((i,j,k):3) @*/

for (k=0;k<=N-2;k++) {
  for (j=k+1;j<=N-1;j++) {
    A[k][j]=A[k][j]/A[k][k] ;
    for (i=k+1;i<=N-1;i++) {
      A[i][j]=A[i][j]-A[i][k]*A[k][j] ;
    }
  }
 }

/*@ end @*/

//--------------------------------------------------
// 2-d FDTD
//--------------------------------------------------
/*@ begin OrTil ((t,i,j):1) @*/

if ((nx >= 2) && (ny >= 2)) {
  for (t=0;t<=tmax-1;t++) {
    ey[0][0]=t ;
    for (j=t+1;j<=t+nx-1;j++) {
      ey[-t+j][0]=ey[-t+j][0]-0.5*(hz[-t+j][0]-hz[-t+j-1][0]) ;
    }
    for (i=t+1;i<=t+ny-1;i++) {
      ex[0][-t+i]=ex[0][-t+i]-0.5*(hz[0][-t+i]-hz[0][-t+i-1]) ;
      ey[0][-t+i]=t ;
      for (j=t+1;j<=t+nx-1;j++) {
	hz[-t+j-1][-t+i-1]=hz[-t+j-1][-t+i-1]-0.7*(ey[1+-t+j-1][-t+i-1]+ex[-t+j-1][1+-t+i-1]-ex[-t+j-1][-t+i-1]-ey[-t+j-1][-t+i-1]) ;
	ey[-t+j][-t+i]=ey[-t+j][-t+i]-0.5*(hz[-t+j][-t+i]-hz[-t+j-1][-t+i]) ;
	ex[-t+j][-t+i]=ex[-t+j][-t+i]-0.5*(hz[-t+j][-t+i]-hz[-t+j][-t+i-1]) ;
      }
      hz[nx-1][-t+i-1]=hz[nx-1][-t+i-1]-0.7*(ey[1+nx-1][-t+i-1]+ex[nx-1][1+-t+i-1]-ex[nx-1][-t+i-1]-ey[nx-1][-t+i-1]) ;
    }
    for (j=t+1;j<=t+nx;j++) {
      hz[-t+j-1][ny-1]=hz[-t+j-1][ny-1]-0.7*(ey[1+-t+j-1][ny-1]+ex[-t+j-1][1+ny-1]-ex[-t+j-1][ny-1]-ey[-t+j-1][ny-1]) ;
    }
  }
 }
if ((nx >= 2) && (ny == 1)) {
  for (t=0;t<=tmax-1;t++) {
    ey[0][0]=t ;
    for (j=t+1;j<=t+nx-1;j++) {
      ey[-t+j][0]=ey[-t+j][0]-0.5*(hz[-t+j][0]-hz[-t+j-1][0]) ;
    }
    for (j=t+1;j<=t+nx;j++) {
      hz[-t+j-1][0]=hz[-t+j-1][0]-0.7*(ey[1+-t+j-1][0]+ex[-t+j-1][1+0]-ex[-t+j-1][0]-ey[-t+j-1][0]) ;
    }
  }
 }
if ((nx == 1) && (ny >= 2)) {
  for (t=0;t<=tmax-1;t++) {
    ey[0][0]=t ;
    for (i=t+1;i<=t+ny-1;i++) {
      ex[0][-t+i]=ex[0][-t+i]-0.5*(hz[0][-t+i]-hz[0][-t+i-1]) ;
      ey[0][-t+i]=t ;
      hz[0][-t+i-1]=hz[0][-t+i-1]-0.7*(ey[1+0][-t+i-1]+ex[0][1+-t+i-1]-ex[0][-t+i-1]-ey[0][-t+i-1]) ;
    }
    hz[0][ny-1]=hz[0][ny-1]-0.7*(ey[1+0][ny-1]+ex[0][1+ny-1]-ex[0][ny-1]-ey[0][ny-1]) ;
  }
 }
if ((nx == 1) && (ny == 1)) {
  for (t=0;t<=tmax-1;t++) {
    ey[0][0]=t ;
    hz[0][0]=hz[0][0]-0.7*(ey[1+0][0]+ex[0][1+0]-ex[0][0]-ey[0][0]) ;
  }
 }

/*@ end @*/
