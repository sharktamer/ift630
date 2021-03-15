// kernel.cl
// test pour obtenir les valeurs et faire la correspondance
// Device code.
  
 
// OpenCL Kernel


__kernel void

obtenir_valeur(__global int* A , __global int* B, __global int * flag, __global int * test, int c)
{
    global int *p;
    int val1 = (int)get_global_id(0);
    int val2 = (int)get_global_id(1);
    int val3 = (int)get_global_id(2);
    int val4;
    A[0] = (int)get_work_dim();
    A[1] = (int)get_global_id(0);
    A[2] = (int)get_global_id(1);
    A[3] = (int)get_global_id(2);
    A[4] = (int)get_global_size(0);
    A[5] = (int)get_global_size(1);
    A[6] = (int)get_global_size(2);
    A[7] = (int)get_global_offset(0);
    A[8] = (int)get_global_offset(1);
    A[9] = (int)get_global_offset(2);
    A[10] = (int)get_group_id(0);
    A[11] = (int)get_group_id(1);
    A[12] = (int)get_group_id(2);
    A[13] = (int)get_local_id(0);
    A[14] = (int)get_local_id(1);
    A[15] = (int)get_local_id(2);
    A[16] = (int)get_local_size(0);
    A[17] = (int)get_local_size(1);
    A[18] = (int)get_local_size(2);
    A[19] = (int)get_num_groups(0);
    A[20] = (int)get_num_groups(1);
    A[21] = (int)get_num_groups(2);
    A[22] = c;
    int nb=0;
    val4 = flag[0];
  	while (val4 != 2)
    { 
       nb++;
       if (val1 == 2) 
       {
           atomic_cmpxchg(flag, 9, 2); 
       }
       val4 = atomic_cmpxchg(flag, 99, 99);
    }
    atomic_inc(test);
    atomic_inc(test);
    A[23] = flag[0]+6; 
    A[24] = *test;
 
}


