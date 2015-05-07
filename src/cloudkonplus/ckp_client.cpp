#include "../zht/cpp_zhtclient.h"
#include "../zht/ZHTUtil.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <exception>
#include <fstream>

using namespace std;
ZHTClient zc;
FILE *fp;
ifstream infile;
int taskCount, taskKey;
string IP;
struct timeval t;

void openFile(const string &fileName);
void printUsage(char *argv_0);
void test_lookup();
void test_push();

void push(string task) 
{
	int rc;
	string key, result;
	char intstr[10];

	sprintf(intstr, "%d", taskKey);
	key = IP + "." + string(intstr);
	//key = getIp() + string(intstr);
	
	rc = zc.push(key, task, "q1", result);

	if (rc == 0)
		printf("PUSH OK, rc(%d)\n", rc);
	else
		printf("PUSH ERR, rc(%d)\n", rc);

	taskKey = taskKey + 1;
}

void *pushTask(void *keyID)
{
	int rc, count = 1;
	string key, task, read1, read2, read3;
	char intstr[10];

	key = (char *) keyID;

	cout << "******** pushTask Start ********" << endl;

	while(infile >> read1 >> read2)
	{
		sprintf(intstr, "%d", count);
		read3 = string(intstr);

		task = key + "," + read1 + "," + read2 + "," + read3;
		cout << "Task: " << task << endl;

		push(task);
		count = count + 1;
	}
	infile.close();
	cout << "******** pushTask End ********" << endl;

}

void *getResult(void *keyID)
{
	string key = (char *) keyID;
	string result;
	int rc, i = 0;

	while((i - 1000) != taskCount)
	{
		rc = zc.lookup(key, result);

		if (rc == 0)
		{
			printf("LOOKUP OK, rc(%d), value={%d}\n", rc, (atoi(result.c_str()) - 1000));
			i = atoi(result.c_str());

			if(strlen(result.c_str()) == 0)
				break;

			if((i - 1000) != taskCount)
				sleep(5);
		}
		else
			sleep(5);
	}
	
	rc = zc.remove(key);

	if (rc == 0)
		printf("REMOVE OK, rc(%d)\n", rc);
	else
		printf("REMOVE ERR, rc(%d)\n", rc);
}

void startClient()
{
	clock_t start, end;
	double s_t, e_t, t_t;
	pthread_t threads[2];
	int i, rc;
	string result, key;

	IP = getIp().c_str();
	key = getIp() + ".AAIS";

	//rc = zc.push("temp", "test", "q1", result);
	//rc = zc.pop("temp1", "q1", result);

	start = clock();
	gettimeofday(&t, NULL);
	s_t = t.tv_sec+(t.tv_usec/1000000.0);

	rc = pthread_create(&threads[0], NULL, pushTask, (void *)key.c_str());

	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&threads[1], NULL, getResult, (void *)key.c_str());

	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}

	for(i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}

	gettimeofday(&t, NULL);
	e_t = t.tv_sec+(t.tv_usec/1000000.0);
	t_t = e_t - s_t;
	end = clock();

	cout << "########################################################" << endl;
	//cout << "Wall Time: " << ((double)(end - start))/CLOCKS_PER_SEC << " sec" << endl;
	cout << "Wall Time: " << t_t << " sec" << endl;
	cout << "########################################################" << endl;
}

int main(int argc, char **argv) 
{
	extern char *optarg;

	int printHelp = 0;
	string zhtConf = "";
	string neighborConf = "";
	string fileName = "";

	int c;
	while ((c = getopt(argc, argv, "z:n:w:h")) != -1) 
	{
		switch (c) 
		{
		case 'z':
			zhtConf = string(optarg);
			break;
		case 'n':
			neighborConf = string(optarg);
			break;
		case 'w':
			fileName = string(optarg);
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

		if (!zhtConf.empty() && !neighborConf.empty() && fileName != "") {

			zc.init(zhtConf, neighborConf);

			openFile(fileName);

			//test_lookup();

			//test_push();

			startClient();

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

void openFile(const string &fileName)
{
	string read1, read2;
	int i = 0;
	cout << "Opening fileName: " << fileName.c_str() << endl;

	infile.open(fileName.c_str());
	
	if (infile == NULL)
	{
		printf("Invaild File\n");
		exit(1);
	}

	while(infile >> read1 >> read2)
		++i;

	infile.close();
	infile.open(fileName.c_str());

	taskCount = i;
	taskKey = 1;
}

void printUsage(char *argv_0) 
{
	fprintf(stdout, "Usage:\n%s %s\n", argv_0, "-z zht.conf -n neighbor.conf -w <fileName> [-h(help)]");
}

void test_lookup() 
{
	string key = "response";
	string result = "y";
	int rc;

	while(result != "q")
	{
		rc = zc.lookup(key, result);

		if (rc == 0)
		{
			printf("LOOKUP OK, rc(%d), value={%s}\n", rc, result.c_str());
			zc.remove(key);
		}
		else
			sleep(30);
	}	
}

void test_push()
{
	int i = 1;

	char intstr[10];
	sprintf(intstr, "%d", i);
	string uuid = string(intstr);

	string val = "y";
	string result;

	while(i <= 10000)
	{
		//printf("Enter the value to be pushed,(Enter \"q\" Quit) Value = ");
		//cin>>val;
		cout << "UUID: " << uuid << endl;
		int rc = zc.push(uuid, uuid,"q1", result);

		if (rc == 0)
		{
			i = i + 1;
			sprintf(intstr, "%d", i);
			uuid = string(intstr);

			printf("PUSH OK, rc(%d)\n", rc);
		}
		else
			printf("PUSH ERR, rc(%d)\n", rc);
	}
}

/*
int mac = system("ifconfig eth0 | grep -o -E '([[:xdigit:]]{1,2}:){5}[[:xdigit:]]{1,2}'");
char intstr[10];
sprintf(intstr, "%d", mac);
string key = exec("ifconfig eth0 | grep -o -E '([[:xdigit:]]{1,2}:){5}[[:xdigit:]]{1,2}'");

while ( getline(infile, task) )
{
	cout << "Task: " << task << "\n";

	system(task.c_str());
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
*/
