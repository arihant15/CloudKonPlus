/**
 * Author: DFZ, dzhao8@iit.edu
 * Desc: This file includes the utility functions for FusionFS. They are 
 *			mainly for metadata management, but have other auxiliaries such
 *			as network protocols. 
 * History: 
 * DFZ, 1/22/2015: refactoring code
 *
 * DFZ, 07/14/2012: change zht_lookup() interface
 *
 * DFZ, 07/05/2012: added five ZHT based functions:
 * 		zht_init(), zht_insert(), zht_lookup(), zht_remove() and zht_free()
 *
 * DFZ, 06/27/2012: The hashtable is implemented with <search.h> for now.
 * 		This will be updated with more fancy ones (obsoleted)
 */

/*for net facilities*/
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "params.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/xattr.h>

/*for ZHT interface */
#include <stdbool.h>
#include <errno.h>
#include "../zht/c_zhtclient.h"

#include "log.h"
#include "util.h"

/*
 * get the ip address of the local machine
 */
int net_getmyip(char *addr) 
{
	struct ifaddrs *ifAddrStruct = NULL;
	getifaddrs(&ifAddrStruct);
	
	struct ifaddrs *ifa = NULL;
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) 
	{
		/*skip the loopback*/
		if (!strcmp("lo", ifa->ifa_name))
			continue;
		
		// check it is IP4
		if (AF_INET == ifa->ifa_addr->sa_family) 
		{ 
			// is a valid IP4 Address
			void *tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
			
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			
			strcpy(addr, addressBuffer);
		}
		
		/*Don't bother to use IP6 for now*/
//        else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
//            // is a valid IP6 Address
//            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
//            char addressBuffer[INET6_ADDRSTRLEN];
//            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
//            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
//        }
	}
	
	if (ifAddrStruct != NULL)
		freeifaddrs(ifAddrStruct);

	return 0;
}

/**
 *******************************************************************
 ** The following five operations are imported from ZHT utilities **
 *******************************************************************
 */

int zht_init(const char* neighborconf, const char* zhtconf)
{
	c_zht_init(zhtconf, neighborconf);

	return 0;
}

int zht_free()
{
	c_zht_teardown();

	return 0;
}

int zht_insert(const char *key, const char *value)
{
	return c_zht_insert(key, value);
}

int zht_lookup(const char *key, char *val)
{
	int res = c_zht_lookup(key, val);

	return res;
}

int zht_remove(const char *key)
{
	return c_zht_remove(key);
}

/**
 ** DFZ: obsoleted:
 *****************************************************************************
 ** The following 3 functions are hashtable implementations from <search.h> **
 *****************************************************************************
 */

/**
 * Insert <key, val> into the global hash table
 *
 * return int:
 * 		0 - success
 * 		1 - failed to insert
 */
int ht_insert(const char *key, const char *val)
{
	e.key = (char *)key;
	e.data = (void *)val;

	ep = hsearch(e, ENTER);
	if (NULL == ep) {
		fprintf(stderr, "entry failed in hash table \n");
		exit(EXIT_FAILURE);
	}

	/*
	 * DFZ: test ht_search
	 */
	ep = ht_search("/tmpfile");
	if ((ENTRY*)0 != ep)
		log_msg("\n =====DFZ debug: ep->key = %s, ep->data = %s (after 'ht_search')\n", ep->key, ep->data);
	else
		log_msg("\n =====DFZ debug: not found '/tmpfile' \n ");

	return 0;
}

/**
 * Remove <key, val> from the global hash table
 *
 * return int:
 * 		0 - success
 * 		1 - <key> not found in the hash table
 */
int ht_remove(const char *key)
{
	e.key = (char *)key;
	e.data = (void *)0;

	ep = hsearch(e, ENTER);
	if (NULL == ep) {
		fprintf(stderr, "entry failed in hash table \n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

/**
 * Search <key> from the global hash table
 *
 * return *ENTRY:
 * 		0 - not found
 * 		Otherwise - the pointer of the first found entry
 */
ENTRY* ht_search(const char *key)
{
	e.key = (char*) key;
	ep = hsearch(e, FIND);

	if (NULL != ep) {
		log_msg("\n ========DFZ debug: in ht_search(): ep->key = %s, ep->data = %s", ep->key, ep->data);
		return ep;
	}

	log_msg("\n ========DFZ debug: in ht_search(): %s not found ", key);

	return (ENTRY*)0;
}
