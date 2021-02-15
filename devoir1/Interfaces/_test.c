/* srl info for test */

#include "/usr/local/lib/sr/srmulti.h"
#include "/usr/local/lib/sr/sr.h"

static char SR_version[] = 
	"SR version 2.3.3";

int sr_max_co_stmts = 1000000;
int sr_max_classes = 1000000;
int sr_max_loops = 10000;
int sr_max_operations = 1000000;
int sr_max_processes = 1000000;
int sr_max_rmt_reqs = 1000000;
int sr_max_resources = 1000000;
int sr_max_semaphores = 1000000;
int sr_stack_size = 40000;
int sr_async_flag = 0;
char sr_exec_path[] = "/usr/local/lib/sr/srx";

int N_test = 0;  extern void R_test(), F_test();

Rpat sr_rpatt[] = {
    { "test", R_test, F_test },
};
int sr_num_rpats = 1;
