#include "../zht/cpp_zhtclient.h"
#include "../zht/ZHTUtil.h"
#include "../monitoring-client/mclient.h"
#include "../monitoring-client/restclient.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <time.h>
#include <string>
#include <exception>
// #include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
// #include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

ZHTClient zc;
int threadCount = 0, id, job_count;
string zht_key = "xxx";
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

// monitoring client
ClientApi cm;

void printUsage(char *argv_0);
void test_insert();
void test_pop();
void error(const char *msg);
string exec(string cmd);

void *execTask(void *popTask)
{
	ClientApi::metricData.state = "1";
	cm.sendMetrics();

	string result, task, key, filename, cmd, dataTask[3] = "";
	int i, k = 0, rc;
	char intstr[10];

	task = (char *) popTask;

	for(i = 0; i < strlen(task.c_str()); i++)
	{
		if (task[i] == ',')
			k = k + 1;
		else
			dataTask[k] = dataTask[k] + task[i];
	}

	key = dataTask[0];
	filename = dataTask[1];

	chdir("/home/arihant/Documents/Workspace/DIC/Code/CloudKonPlus/script/fusion_mount");

	cmd = "/home/arihant/Documents/Workspace/DIC/Code/CloudKonPlus/src/wordcount/wc_script.sh " + filename +" sim_" +filename;

	rc = system(cmd.c_str());
	zht_key = key;
	//pthread_mutex_lock( &mutex1 );
	if ( rc == 0)
	{
		job_count = job_count + 1;
		cout << "Job Count Completed: " << (job_count - 1000) << endl;
	}
	else
	{
		cout << "Job Failed execution: " << task << endl;
		cout << "Pushing the failed job back to queue" << endl;
		id = id + 1;
		sprintf(intstr, "%d", id);
		rc = zc.push((key + "." + string(intstr) + dataTask[3]), task, "q1", result);

		if (rc == 0)
			printf("PUSH OK, rc(%d)\n", rc);
		else
			printf("PUSH ERR, rc(%d)\n", rc);
	}
	threadCount = threadCount - 1;
	chdir("/home/arihant/Documents/Workspace/DIC/Code/CloudKonPlus/script");
	ClientApi::metricData.state = "0";
	cm.sendMetrics();
	//pthread_mutex_unlock( &mutex1 );
}

void sendResult()
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = 30000;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    //char ip[strlen(zht_key.c_str())];
    //sprintf(ip, "%s", zht_key.c_str());
    //server->h_name = (char *)zht_key.c_str();
    server = gethostbyname(zht_key.c_str());
    
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    bzero(buffer,256);
    
    char tempBuff[10];
    sprintf(tempBuff,"%d", (job_count-1000));
    
    //fgets(buffer,255,tempBuff);

	n = write(sockfd,tempBuff,strlen(tempBuff));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    close(sockfd);
}

void startWorker(int numThrds)
{
	int i, rc;
	char intstr[10];
	pthread_t threads[numThrds];
	string key, key1, result;

	key1 = getIp();

	sprintf(intstr, "%d", id);
	key = string(intstr) + "." + key1;



	while (true)
	{
		try
		{
			if(threadCount < numThrds)
			{
				rc = zc.pop(key, "q1", result);

				if (rc == 0)
				{
					if ( result.c_str() != NULL && strlen(result.c_str()) > 0)
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
				{
					printf("POP ERR, rc(%d), value={%s}\n", rc, result.c_str());

					if(strcmp(zht_key.c_str(),"xxx") != 0)
					{
						pthread_mutex_lock( &mutex1 );
						sendResult();
						zht_key = "xxx";
						job_count = 1000;
						pthread_mutex_unlock( &mutex1 );
					}
					else
					{
						job_count = 1000;
						sleep(5);
					}
				}
			}
		}
		catch(...)
		{
			sleep(5);
		}
		
		id = id + 1;
        sprintf(intstr, "%d", id);
        key = string(intstr) + "." + key1;
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
			
			cout << "Initializing Worker" << endl;
			string result;
			zc.push("temp", "test", "q1", result);
			zc.pop("xxxx", "q1", result);
			id = 1000;
			job_count = 1000;

			ClientApi::metricData.counter = 1;
			ClientApi::metricData.workerName = getIp().c_str();
			ClientApi::metricData.workerId = getIp().c_str();
			ClientApi::metricData.state = "0";
			ClientApi::metricData.tags.push_back("monitor");
			ClientApi::workerFile.open(ClientApi::metricData.workerId + ".csv", ios::in);

			if (ClientApi::workerFile.is_open()) {
			  ClientApi::workerFile.close();
			  ClientApi::workerFile.open(ClientApi::metricData.workerId + ".csv", ios::app);
			  //cout << "hi pls create" << endl;
			} else {
			  ClientApi::workerFile.open(ClientApi::metricData.workerId + ".csv", ios::app);
			  ClientApi::workerFile << "count,latency" << endl;
			  //cout << "hi pls create na" << endl;
			}

			//cout << "hi hi" << endl;
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

void error(const char *msg)
{
    perror(msg);
    exit(0);
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
			rc = zc.pop(uuid, "q1", result);

			if (rc == 0)
			{
				i = i + 1;
				sprintf(intstr, "%d", i);
				uuid = string(intstr);

				printf("POP OK, rc(%d), value={%s}\n", rc, result.c_str());
			}
			else
				sleep(10);
		}
		catch(...)
		{
			sleep(10);
		}
	}	
}

string exec(string cmd)
{
	int i;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";

    char buffer[10];
    string result = "";

    while(fgets(buffer, 10, pipe) != NULL)
			result += buffer;

    pclose(pipe);
    return result;
}