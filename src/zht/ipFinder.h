#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <string> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <iostream>


using namespace std;
class IpFinder {
  public:
    static string getIp ();
};
