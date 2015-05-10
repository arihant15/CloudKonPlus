#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class ClientApi {
public:
  struct workerD {
    int counter;
    string workerName;
    string workerId;
    string state;
    vector<string> tags;
  };

  void sendMetrics();
  void sendM();

public:
  static struct workerD metricData;
  static ofstream workerFile;
 
private: 
  void replaceSpace(string &s);
};
