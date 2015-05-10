#include "restclient.h"
#include "mclient.h"
#include <stdio.h>

#include <thread>
#include <iostream>
#include <chrono>

using namespace std::chrono;
//using namespace std::literals;

ClientApi::workerD ClientApi::metricData;
ofstream ClientApi::workerFile;

void ClientApi::sendMetrics() {
  auto start = std::chrono::high_resolution_clock::now();
  thread t (&ClientApi::sendM, this);
  t.join();  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed1 = end-start;
  ClientApi::workerFile << ClientApi::metricData.counter << "," << elapsed1.count() << endl;
  ClientApi::metricData.counter = ClientApi::metricData.counter + 1;
}

void ClientApi::sendM() {
  string worker = ClientApi::metricData.workerName;
  //this->replaceSpace(worker);
  string workerId = ClientApi::metricData.workerId;
  //this->replaceSpace(workerId);
  string state = ClientApi::metricData.state;
  //this->replaceSpace(state);
  struct timeval tp;
  gettimeofday(&tp, NULL);
  string timestamp ("");
  long int ti = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  const int n = snprintf(NULL, 0, "%lu", ti);
  if (n > 0) {
    char buf[n+1];
    int c = snprintf(buf, n+1, "%lu", ti);
    for (int i = 0; i < c; i++) {
      timestamp = timestamp + buf[i];
    }
  }
  string tags("");
  for (int i = 0; i < ClientApi::metricData.tags.size(); i++) {
    tags = tags + "&tags="+ ClientApi::metricData.tags.at(i);
  }
  //this->replaceSpace(tags);
  
  RestClient::response r = RestClient::get("http://ec2-52-5-11-81.compute-1.amazonaws.com:8080/MonitoringService/api/mservice?name=" + worker + "&workerId=" + workerId + "&state=" + state + "&timeStamp=" + timestamp + tags);
   
  std::cout << "response received: '" << r.code << "'\n";

}

void ClientApi::replaceSpace(string &s) {
  for (size_t i=0; i<s.size(); i++) 
    if (isspace((unsigned char)s[i])) 
      s[i] = '%';
}

/*
int main() {
//  RestClient::response r = RestClient::get("http://localhost:10080/MonitoringService/api/mservice?name=worker2&workerId=workerId&state='in%20progress'&timeStamp=122212121&tags=shif&tags=come");
   
  //std::cout << "response received: '" << r.code << "'\n";
  ClientApi cm;
  ClientApi::metricData.counter = 1;
  ClientApi::metricData.workerName = "test1";
  ClientApi::metricData.workerId = "work1";
  ClientApi::metricData.state = "1";
  ClientApi::metricData.tags.push_back("monitor");
  ClientApi::workerFile.open(ClientApi::metricData.workerId + ".csv", ios::in);

  if (ClientApi::workerFile.is_open()) {
    ClientApi::workerFile.close();
    ClientApi::workerFile.open(ClientApi::metricData.workerId + ".csv", ios::app);

  } else {
    ClientApi::workerFile.open(ClientApi::metricData.workerId + ".csv", ios::app);
    ClientApi::workerFile << "count,latency" << endl;
  }
  std::cout << "hi" << endl;
  
  auto start = std::chrono::high_resolution_clock::now();
  cm.sendMetrics();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed1 = end-start;
  ClientApi::workerFile << ClientApi::metricData.counter << "," << elapsed1.count() << endl;
  ClientApi::metricData.counter = ClientApi::metricData.counter + 1;
  for (; ;) {
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    if (ClientApi::metricData.state == "1")
      ClientApi::metricData.state = "0";
    else {
      ClientApi::metricData.state = "1";
    }

    start = std::chrono::high_resolution_clock::now();
    cm.sendMetrics();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed2 = end-start;
    // std::cout  << elapsed.count();
    // long result = (elapsed2.cout() + elapsed1.count()) / 2;
    std::cout  << ClientApi::metricData.counter << "," << elapsed2.count() << endl;
    ClientApi::workerFile << ClientApi::metricData.counter << "," << elapsed2.count() << endl;
    ClientApi::metricData.counter = ClientApi::metricData.counter + 1;
    // cm.sendMetrics();  
  }
 
  return 0;
}
*/