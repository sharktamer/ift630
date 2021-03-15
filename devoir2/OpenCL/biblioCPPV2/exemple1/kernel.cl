// kernel.cl
// test pour obtenir les valeurs et faire la correspondance
// Device code.
  
 
__kernel void

obtenir_valeur(__global int* A , __global int* B, int c)
{
    int val1 = (int)get_global_id(0);
    int val2 = (int)get_global_id(1);
    int val3 = (int)get_global_id(2);

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
    A[23] = B[3];
   
}


