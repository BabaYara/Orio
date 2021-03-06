/*@ begin PerfTuning (
 def build {
   arg build_command = 'gcc -O3';
   arg libs = '-lrt';
 } 
 def performance_counter {
   #arg method = 'bgp counter';
   arg repetitions = 3;
 }
 def performance_params {  
   param UF[] = range(1,11);
   param VEC[] = [False,True];
   #constraint divisible_by_two = (UF % 2 == 0);
 }
 def input_params {
   param N[] = [100000];
 }
 def input_vars {
   decl dynamic double x1[N] = random;
   decl dynamic double x2[N] = random;
   decl dynamic double x3[N] = random;
   decl dynamic double x4[N] = random;
   decl dynamic double x5[N] = random;
   decl dynamic double y[N] = 0;
   decl double a1 = random;
   decl double a2 = random;
   decl double a3 = random;
   decl double a4 = random;
   decl double a5 = random;
 }
 def search {
   arg algorithm = 'Exhaustive';
 }
) 
@*/
/**-- (Generated by Orio) 
Best performance cost: 
  [0.000354274, 0.000351704, 0.000351125] 
Tuned for specific problem sizes: 
  N = 100000 
Best performance parameters: 
  UF = 9 
  VEC = False 
--**/



register int i;

/*@ begin Loop ( 
    transform Composite(
      unrolljam = (['i'],[UF]),
      vector = (VEC, ['ivdep','vector always'])
     )
  for (i=0; i<=N-1; i++)
    y[i]=y[i]+a1*x1[i]+a2*x2[i]+a3*x3[i]+a4*x4[i]+a5*x5[i];
) @*/
{
  int i;
  for (i=0; i<=N-9; i=i+9) {
    y[i]=y[i]+a1*x1[i]+a2*x2[i]+a3*x3[i]+a4*x4[i]+a5*x5[i];
    y[(i+1)]=y[(i+1)]+a1*x1[(i+1)]+a2*x2[(i+1)]+a3*x3[(i+1)]+a4*x4[(i+1)]+a5*x5[(i+1)];
    y[(i+2)]=y[(i+2)]+a1*x1[(i+2)]+a2*x2[(i+2)]+a3*x3[(i+2)]+a4*x4[(i+2)]+a5*x5[(i+2)];
    y[(i+3)]=y[(i+3)]+a1*x1[(i+3)]+a2*x2[(i+3)]+a3*x3[(i+3)]+a4*x4[(i+3)]+a5*x5[(i+3)];
    y[(i+4)]=y[(i+4)]+a1*x1[(i+4)]+a2*x2[(i+4)]+a3*x3[(i+4)]+a4*x4[(i+4)]+a5*x5[(i+4)];
    y[(i+5)]=y[(i+5)]+a1*x1[(i+5)]+a2*x2[(i+5)]+a3*x3[(i+5)]+a4*x4[(i+5)]+a5*x5[(i+5)];
    y[(i+6)]=y[(i+6)]+a1*x1[(i+6)]+a2*x2[(i+6)]+a3*x3[(i+6)]+a4*x4[(i+6)]+a5*x5[(i+6)];
    y[(i+7)]=y[(i+7)]+a1*x1[(i+7)]+a2*x2[(i+7)]+a3*x3[(i+7)]+a4*x4[(i+7)]+a5*x5[(i+7)];
    y[(i+8)]=y[(i+8)]+a1*x1[(i+8)]+a2*x2[(i+8)]+a3*x3[(i+8)]+a4*x4[(i+8)]+a5*x5[(i+8)];
  }
  for (i=N-((N-(0))%9); i<=N-1; i=i+1) 
    y[i]=y[i]+a1*x1[i]+a2*x2[i]+a3*x3[i]+a4*x4[i]+a5*x5[i];
}
/*@ end @*/
/*@ end @*/

