#include "cpp_zhtclient.h"
#include "ZHTUtil.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <time.h>
#include <string>
#include <exception>
#include <fstream>

using namespace std;
ZHTClient zc;
int threadCount = 0;

void printUsage(char *argv_0);
void test_insert();
void test_pop();

void *execTask(void *popTask)
{
	string result, task, key, cmd, data[4] = "";
	int i, k = 0, rc;
	char intstr[10];

	task = (char *) popTask;

	for(i = 0; i < strlen(task.c_str()); i++)
	{
		if (task[i] == ',')
			k = k + 1;
		else
			data[k] = data[k] + task[i];
		//key = key + task[i];
	}

	key = data[0];
	cmd = data[1] + " " + data[2];

	rc = system(cmd.c_str());

	if ( rc == 0)
	{
		rc = zc.lookup(key, result);

		if ( rc == 0)
		{
			int data = atoi(result.c_str());
			data = data + 1;

			sprintf(intstr, "%d", data);
			result = string(intstr);

			rc = zc.insert(key, result);

			if (rc == 0)
				printf("INSERT OK, rc(%d)\n", rc);
		}
		else
		{
			rc = zc.insert(key, "1");
		}
	}
	threadCount = threadCount - 1;
}

void startWorker(int numThrds)
{
	int i, rc, id = 1;
	char intstr[10];
	pthread_t threads[numThrds];
	string key, key1, result;

	key1 = getIp();

	sprintf(intstr, "%d", id);
	key = key1 + string(intstr);

	rc = zc.push("temp", "test", "1", result);
	rc = zc.pop("temp1", "1", result);

	while (true)
	{
		try
		{
			if(threadCount < numThrds)
			{
				rc = zc.pop(key, "1", result);

				if (rc == 0)
				{
					id = id + 1;
					sprintf(intstr, "%d", id);
					key = key1 + string(intstr);

					if ( result.c_str() != NULL)
					{
						printf("POP OK, rc(%d), value={%s}\n", rc, result.c_str());

						rc = pthread_create(&threads[threadCount], NULL, execTask, (void *)result.c_str());

						if (rc)
						{
							cout << "Error:unable to create thread," << rc << endl;
							exit(-1);
						}

						threadCount = threadCount + 1;
					}
				}
				else
					sleep(5);
			}
		}
		catch(...)
		{
			sleep(10);
		}
	}

	cout << "Worker completed" << endl;

}

int main(int argc, char **argv) 
{
	extern char *optarg;

	int printHelp = 0, numThrds = 0;
	string zhtConf = "";
	string neighborConf = "";

	int c;
	while ((c = getopt(argc, argv, "z:n:t:h")) != -1) 
	{
		switch (c) 
		{
		case 'z':
			zhtConf = string(optarg);
			break;
		case 'n':
			neighborConf = string(optarg);
			break;
		case 't':
			numThrds = atoi(optarg);
			break;
		case 'h':
			printHelp = 1;
			break;
		default:
			fprintf(stderr, "Illegal argument \"%c\"\n", c);
			printUsage(argv[0]);
			exit(1);
		}
	}

	int helpPrinted = 0;
	if (printHelp) {
		printUsage(argv[0]);
		helpPrinted = 1;
	}

	try {

		if (!zhtConf.empty() && !neighborConf.empty() && numThrds != 0) {

			zc.init(zhtConf, neighborConf);
			
			//test_insert();

			//test_pop();

			startWorker(numThrds);

			zc.teardown();

		} else {

			if (!helpPrinted)
				printUsage(argv[0]);
		}
	} catch (exception& e) {

		fprintf(stderr, "%s, exception caught:\n\t%s", "ZHTServer::main",
				e.what());
	}

}

void printUsage(char *argv_0) 
{
	fprintf(stdout, "Usage:\n%s %s\n", argv_0, "-z zht.conf -n neighbor.conf -t #threads [-h(help)]");
}

void test_insert() 
{
	string key = "response";
	string val = "y";

	while(val != "q")
	{
		printf("Enter the value to be inserted,(Enter \"q\" Quit) Value = ");
		cin>>val;

		int rc = zc.insert(key, val);

		if (rc == 0)
			printf("INSERT OK, rc(%d)\n", rc);
		else
			printf("INSERT ERR, rc(%d)\n", rc);
	}
}

void test_pop() 
{
	int i = 11111;
	char intstr[10];
	sprintf(intstr, "%d", i);
	string uuid = string(intstr);

	//string uuid = getIp();
	string result = "y";
	int rc;

	while(result != "q")
	{
		cout << "UUID" << uuid << endl;
		try
		{
			rc = zc.pop(uuid, "1", result);

			if (rc == 0)
			{
				i = i + 1;
				sprintf(intstr, "%d", i);
				uuid = string(intstr);

				printf("POP OK, rc(%d), value={%s}\n", rc, result.c_str());

			}
		}
		catch(...)
		{
			sleep(10);
		}
	}	
}