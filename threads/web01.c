#include "unpthread.h"
#include <thread.h>  /*solaris threads */

#define MAXFILES 20
#define SERV "80"  /*port number or service name*/

struct file {
    char *f_name;       /*filename*/
    char *f_host;       /*hostname or IP address */
    int f_fd;           /*descriptor*/
    
}
