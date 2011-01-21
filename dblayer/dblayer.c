/************************************************************************
 * Author by Jin Du
 ***********************************************************************/

#include <d_string.h>

int init_db_layer(){
    
    D_STRING ds;
    ds = init_d_string(10);
    strcpy_d_string(ds, "test for d_string");
    fprintf(stdout, "contents of ds: %s\n",get_buffd_string(ds));
    return 0;
}
