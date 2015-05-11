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
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
ZHTClient zc;
FILE *fp;
ifstream infile;
int taskCount, taskKey, messageCount = 0;
string IP;
struct timeval t;

void openFile(const string &fileName);
void printUsage(char *argv_0);
void test_lookup();
void test_push();
string exec(string cmd);
void error(const char *msg);

void push(string task) {
	int rc;
	string key, result;
	char intstr[10];

	sprintf(intstr, "%d", taskKey);
	key = IP + "." + string(intstr);

	rc = zc.push(key, task, "q1", result);

	if (rc == 0)
		printf("PUSH OK, rc(%d)\n", rc);
	else
		printf("PUSH ERR, rc(%d)\n", rc);

	taskKey = taskKey + 1;
}

void *pushTask(void *keyID) {
	int rc, count = 1;
	string key, task, read3;
	char intstr[10];

	key = (char *) keyID;

	cout << "******** pushTask Start ********" << endl;

	DIR *dpdf;
	struct dirent *epdf;

	dpdf = opendir("/home/ubuntu/CloudKonPlus/script/fusion_mount");
	if (dpdf != NULL) {
		while (epdf = readdir(dpdf)) {
			if (strcmp(epdf->d_name, ".") != 0
					&& strcmp(epdf->d_name, "..") != 0
					&& strncmp(epdf->d_name, "x", 0) == 0) {
				sprintf(intstr, "%d", count);
				read3 = string(intstr);
				//cout << "Filename: " << epdf->d_name << endl;
				task = key + "," + epdf->d_name + "," + read3;
				cout << "Task: " << task << endl;
				push(task);
				count = count + 1;
			}
		}
	}

	cout << "******** pushTask End ********" << endl;

}

void *getResult(void *keyID) {
	int sockfd, newsockfd, portno, n, rc;
	string cmd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 30000;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	while (messageCount != taskCount) {
		listen(sockfd, 5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0)
			error("ERROR on accept");

		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);

		int temp = atoi(buffer);
		messageCount += temp;
		printf("the count is %i\n", messageCount);
	}

	cout << "Merging the output... " << endl;
	chdir("/home/ubuntu/CloudKonPlus/script/fusion_mount");
	cmd = "/home/ubuntu/CloudKonPlus/src/wordcount/wc_merge.sh";

	rc = system(cmd.c_str());

	if (rc == 0) {
		rc = system("rm -rf x*");
		rc = system("rm -rf sim_*");
		cout << "Output File present in the location: CloudKonPlus/script/fusion_mount/output.txt" << endl;
	}

	close(newsockfd);
	close(sockfd);
}

void startClient() {
	clock_t start, end;
	double s_t, e_t, t_t;
	pthread_t threads[2];
	int i, rc;
	string result, key;

	IP = getIp().c_str();
	key = getIp();

	start = clock();
	gettimeofday(&t, NULL);
	s_t = t.tv_sec + (t.tv_usec / 1000000.0);

	rc = pthread_create(&threads[0], NULL, pushTask, (void *) key.c_str());

	if (rc) {
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}

	rc = pthread_create(&threads[1], NULL, getResult, (void *) key.c_str());

	if (rc) {
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}

	for (i = 0; i < 2; i++) {
		pthread_join(threads[i], NULL);
	}

	gettimeofday(&t, NULL);
	e_t = t.tv_sec + (t.tv_usec / 1000000.0);
	t_t = e_t - s_t;
	end = clock();

	cout << "########################################################" << endl;
	//cout << "Wall Time: " << ((double)(end - start))/CLOCKS_PER_SEC << " sec" << endl;
	cout << "Wall Time: " << t_t << " sec" << endl;
	cout << "########################################################" << endl;
}

int main(int argc, char **argv) {
	extern char *optarg;

	int printHelp = 0;
	string zhtConf = "";
	string neighborConf = "";
	string fileName = "";

	int c;
	while ((c = getopt(argc, argv, "z:n:w:h")) != -1) {
		switch (c) {
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

void openFile(const string &fileName) {
	string size, read, cmd;
	int i = 0, rc;

	cout << "fileName: " << fileName.c_str() << endl;
	cout << "Splitting files ..." << endl;

	cmd = "split -l 1000 " + fileName;
	//rc = system("mkdir /home/arihant/Documents/Workspace/DIC/Code/CloudKonPlus/script/fusion_mount/inputs");
	chdir("/home/ubuntu/CloudKonPlus/script/fusion_mount");

	rc = system(cmd.c_str());

	read = exec("ls | wc -l");

	taskCount = atoi(read.c_str());
	taskKey = 1;
}

void printUsage(char *argv_0) {
	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"-z zht.conf -n neighbor.conf -w <fileName> [-h(help)]");
}

void error(const char *msg) {
	perror(msg);
	exit(1);
}

void test_lookup() {
	string key = "response";
	string result = "y";
	int rc;

	while (result != "q") {
		rc = zc.lookup(key, result);

		if (rc == 0) {
			printf("LOOKUP OK, rc(%d), value={%s}\n", rc, result.c_str());
			zc.remove(key);
		} else
			sleep(30);
	}
}

void test_push() {
	int i = 1;

	char intstr[10];
	sprintf(intstr, "%d", i);
	string uuid = string(intstr);

	string val = "y";
	string result;

	while (i <= 10000) {
		//printf("Enter the value to be pushed,(Enter \"q\" Quit) Value = ");
		//cin>>val;
		cout << "UUID: " << uuid << endl;
		int rc = zc.push(uuid, uuid, "q1", result);

		if (rc == 0) {
			i = i + 1;
			sprintf(intstr, "%d", i);
			uuid = string(intstr);

			printf("PUSH OK, rc(%d)\n", rc);
		} else
			printf("PUSH ERR, rc(%d)\n", rc);
	}
}

string exec(string cmd) {
	int i;
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe)
		return "ERROR";

	char buffer[10];
	string result = "";

	while (fgets(buffer, 10, pipe) != NULL)
		result += buffer;

	pclose(pipe);
	return result;
}
