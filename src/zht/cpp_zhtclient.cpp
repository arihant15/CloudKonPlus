/*
 * Copyright 2010-2020 DatasysLab@iit.edu(http://datasys.cs.iit.edu/index.html)
 *      Director: Ioan Raicu(iraicu@cs.iit.edu)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is part of ZHT library(http://datasys.cs.iit.edu/projects/ZHT/index.html).
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname Tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname Xiaobingo,
 *      Ke Wang(kwang22@hawk.iit.edu) with nickname KWang,
 *      Dongfang Zhao(dzhao8@@hawk.iit.edu) with nickname DZhao,
 *      Ioan Raicu(iraicu@cs.iit.edu).
 *
 * cpp_zhtclient.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "cpp_zhtclient.h"

#include  <stdlib.h>
#include <string.h>

#include "zpack.pb.h"
#include "ConfHandler.h"
#include "Env.h"
#include "StrTokenizer.h"
#include <iostream>
#include "bigdata_transfer.h"
#include "tcp_proxy_stub.h"
#include "ZHTUtil.h"
#include <unistd.h>
#include "ZHTUtil.h"
#include "Util.h"
//#include "ipFinder.h"
using namespace iit::datasys::zht::dm;
//pthread_mutex_t mutex11 = PTHREAD_MUTEX_INITIALIZER;
//Duplicated from ip_proxy_stub.cpp
int loopedrecv(int sock, void *senderAddr, string &srecv) {

	ssize_t recvcount = -2;
//cout<<"In loopedrecv"<<endl;
	socklen_t addr_len = sizeof(struct sockaddr);

	BdRecvBase *pbrb = new BdRecvFromServer();

	char buf[Env::BUF_SIZE];

	while (1) {

		memset(buf, '\0', sizeof(buf));

		ssize_t count;
		if (senderAddr == NULL)
			count = ::recv(sock, buf, sizeof(buf), 0);
		else
			count = ::recvfrom(sock, buf, sizeof(buf), 0,
					(struct sockaddr *) senderAddr, &addr_len);

		if (count == -1 || count == 0) {

			recvcount = count;

			break;
		}

		bool ready = false;

		string bd = pbrb->getBdStr(sock, buf, count, ready);

		if (ready) {

			srecv = bd;
			recvcount = srecv.size();

			break;
		}

		memset(buf, '\0', sizeof(buf));
	}

	delete pbrb;
	pbrb = NULL;

	return recvcount;
}



ZHTClient::ZHTClient() :
		_proxy(0), _msg_maxsize(0) {

}

string ZHTClient::getStringToLocalServer(int len) {

	string str = HashUtil::randomString(len);
	ZHTUtil zu;
	HostEntity he = zu.keyToHost(str);
	string destHost = he.host;

	string localIP = getIp();

	while (0 != localIP.compare(destHost)) {
		str = HashUtil::randomString(len);
		he = zu.keyToHost(str);
		destHost = he.host;
		cout << "destHost:"<<destHost << endl;
		cout << "localIP"<<localIP << endl;
	}
	return str;
}

ZHTClient::ZHTClient(const string& zhtConf, const string& neighborConf) {

	init(zhtConf, neighborConf);
	//int len = 10;
	//this->stringToLocalServer = getStringToLocalServer(10);

}

ZHTClient::~ZHTClient() {

	if (_proxy != NULL) {

		delete _proxy;
		_proxy = NULL;
	}
}

int ZHTClient::init(const string& zhtConf, const string& neighborConf) {


	ConfHandler::initConf(zhtConf, neighborConf);

	_msg_maxsize = Env::get_msg_maxsize();

	_proxy = ProxyStubFactory::createProxy();

//	this->stringToLocalServer = getStringToLocalServer(10);

	if (_proxy == 0)
		return -1;
	else
		return 0;
}

//int ZHTClient::init(const char *zhtConf, const char *neighborConf) {
//
//	string szhtconf(zhtConf);
//	string sneighborconf(neighborConf);
//
//	int rc = init(szhtconf, sneighborconf);
//
//	return rc;
//}

int ZHTClient::commonOp(const string &opcode, const string &key,
		const string &val, const string &val2, string &result, int lease) {

	if (opcode != Const::ZSC_OPC_LOOKUP && opcode != Const::ZSC_OPC_REMOVE
			&& opcode != Const::ZSC_OPC_INSERT
			&& opcode != Const::ZSC_OPC_APPEND
			&& opcode != Const::ZSC_OPC_CMPSWP
			&& opcode != Const::ZSC_OPC_STCHGCB
			&& opcode != Const::ZSC_OPC_CREATE_QUEUE
			&& opcode != Const::ZSC_OPC_PUSH && opcode != Const::ZSC_OPC_POP
			&& opcode != Const::ZSC_OPC_DELETE_QUEUE
			&& opcode != Const::ZSC_OPC_FETCH_NODE
			&& opcode != Const::ZSC_OPC_ADD_NODE)
		return Const::toInt(Const::ZSC_REC_UOPC);

	string sstatus = commonOpInternal(opcode, key, val, val2, result, lease);

//	cout<<"commonopstatus="<<sstatus<<endl;

	if (sstatus == "100") {
//		cout<<"Add Entry to Metadata Queue"<<endl;
		const string opcode1 = Const::ZSC_OPC_ADD_NODE; // add node's first uuid to metadata queue
		const string value = key; // first uuid of a queue coming back from server side
		const string keys = "q1"; // this is hardcoded for one queue only
		sstatus = commonOpInternal(opcode1, keys, value, val2, result, lease);

		//sstatus="000";
	}

	if (sstatus == "200") {
		//cout << "Message Not Found" << endl;
		//node didnt had any message before but the user tried to fetch
		sstatus = "-92";

		// to be completed from here
		// 		1. call metadata queue and get a new uuid from it
		//		2. call pop using the new uuid that you got
		const string opcode1 = Const::ZSC_OPC_FETCH_NODE;
		const string opcode2 = Const::ZSC_OPC_POP;
		const string value;
		const string keys = "q1";
		string temp = commonOpInternal(opcode1, keys, value, val2, result,
				lease);
//			if (temp.empty()){
//				cout<<"temp empty"<<endl;
//			}
//			cout<<"after getting a new uuid for this queue"<<endl;
//			cout<<"returned this from metaQ="<<sstatus<<endl;
		// to be completed from here
		// 		1. call metadata queue and get a new uuid from it
		//		2. call pop using the new uuid that you got
		sstatus = commonOpInternal(opcode2, sstatus, value, val2, result,
				lease);
//			cout<<"	(iman) message got from second queue inside 200= "<<sstatus<<endl;
	}
	if (sstatus.size() > 3) { // this means the result was a uuid not just 100, 200, or 000 which are fail or normal success
		//so go and take it off metadata

		//node didnt have any message because it got emptied.

		//string opc=Const::getuuid();
//		cout<<"	Remove Entry to Metadata Queue="<<sstatus<<endl;

		const string opcode1 = Const::ZSC_OPC_FETCH_NODE;
		const string opcode2 = Const::ZSC_OPC_POP;
		const string value = sstatus;
		const string keys = "q1";
		sstatus = commonOpInternal(opcode1, keys, value, val2, result, lease);
		// to be completed from here
		// 		1. call metadata queue and get a new uuid from it
		//		2. call pop using the new uuid that you got
		sstatus = commonOpInternal(opcode2, sstatus, value, val2, result,
				lease);
		//	cout<<"	(iman) message got from second queue= "<<sstatus<<endl;

		//return sstatus;
		sstatus = "000";
		//cout<<" 2hops"<<endl; //enable this for popCount hopcount
	} else {
		//cout<<" 1hop"<<endl;//enable this for popCount hopcount
	}

	int status = Const::ZSI_REC_CLTFAIL;
	//cout<<"status="<<status<<endl;
	if (!sstatus.empty())
		status = Const::toInt(sstatus);

	return status;
}
int ZHTClient::push(const char *key, const char *val, const char *new_val,
		char *result) {

	return 0;
}
int ZHTClient::push(const string &key, const string &val, const string &new_val,
		string &result) {

	//string val;
	//string val2;
	int rc = commonOp(Const::ZSC_OPC_PUSH, key, val, new_val, result, 1);

	result = extract_value(result);
//	cout<<"result="<<result<<endl;
	return rc;

}

int ZHTClient::pop(const char *key, const char *val, char *result) {
	printf("POP WITH CHAR CALLED");
	return 0;
}
int ZHTClient::pop(const string &key, const string &val, string &result) {

	string new_val;
	int rc = commonOp(Const::ZSC_OPC_POP, key, val, new_val, result, 1);

	result = extract_value(result);

	return rc;

}

int ZHTClient::add_node(const char *key, char *result) {
	return 0;
}
int ZHTClient::add_node(const string &key, string &result) {
	return 0;
}
int ZHTClient::delete_queue(const char *key, const char *val) {
	return 0;
}
int ZHTClient::delete_queue(const string &key, const string &val) {
	return 0;
}

int ZHTClient::fetch_node(const char *key, char *result) {
	return 0;
}
int ZHTClient::fetch_node(const string &key, string &result) {
	return 0;
}

int ZHTClient::lookup(const string &key, string &result) {
	//pthread_mutex_lock( &mutex11 );
	string val;
	string val2;
	int rc = commonOp(Const::ZSC_OPC_LOOKUP, key, val, val2, result, 1);

	result = extract_value(result);
	//pthread_mutex_unlock( &mutex11 );
	return rc;
}

int ZHTClient::lookup(const char *key, char *result) {
	//pthread_mutex_lock( &mutex11 );
	string skey(key);
	string sresult;

	int rc = lookup(skey, sresult);

	strncpy(result, sresult.c_str(), sresult.size() + 1);
	//pthread_mutex_unlock( &mutex11 );
	return rc;
}

int ZHTClient::remove(const string &key) {

	string val;
	string val2;
	string result;
	int rc = commonOp(Const::ZSC_OPC_REMOVE, key, val, val2, result, 1);

	return rc;
}

int ZHTClient::remove(const char *key) {

	string skey(key);

	int rc = remove(skey);

	return rc;
}
int ZHTClient::create_queue(const string &key, const string &val) {	// value is always null
	string val2;
	string result;
	int rc = commonOp(Const::ZSC_OPC_CREATE_QUEUE, key, val, val2, result, 1);

	return rc;
}
int ZHTClient::create_queue(const char *key, const char *val) {
	string skey(key);
	string sval(val);

	int rc = insert(skey, sval);

	return rc;
}
int ZHTClient::insert(const string &key, const string &val) {
	//pthread_mutex_lock( &mutex11 );
	string val2;
	string result;
        //cout<<key<<endl;
	int rc = commonOp(Const::ZSC_OPC_INSERT, key, val, val2, result, 1);
	//pthread_mutex_unlock( &mutex11 );
	return rc;
}

int ZHTClient::insert(const char *key, const char *val) {
	//pthread_mutex_lock( &mutex11 );
	string skey(key);
	string sval(val);

	int rc = insert(skey, sval);
	//pthread_mutex_unlock( &mutex11 );
	return rc;
}

int ZHTClient::append(const string &key, const string &val) {
	//pthread_mutex_lock( &mutex11 );
	string val2;
	string result;
	int rc = commonOp(Const::ZSC_OPC_APPEND, key, val, val2, result, 1);
	//pthread_mutex_unlock( &mutex11 );
	return rc;
}

int ZHTClient::append(const char *key, const char *val) {
	//pthread_mutex_lock( &mutex11 );
	string skey(key);
	string sval(val);

	int rc = append(skey, sval);
	//pthread_mutex_unlock( &mutex11 );
	return rc;
}

string ZHTClient::extract_value(const string &returnStr) {

	string val;

	StrTokenizer strtok(returnStr, ":");
	/*
	 * hello,zht:hello,ZHT ==> zht:ZHT
	 * */

	if (strtok.has_more_tokens()) {

		while (strtok.has_more_tokens()) {

			ZPack zpack;
			zpack.ParseFromString(strtok.next_token());

			if (zpack.valnull())
				val.append("");
			else
				val.append(zpack.val());

			val.append(":");
		}

		size_t found = val.find_last_of(":");
		val = val.substr(0, found);

	} else {

		ZPack zpack;
		zpack.ParseFromString(returnStr);

		if (zpack.valnull())
			val = "";
		else
			val = zpack.val();
	}

	return val;
}

int ZHTClient::compare_swap(const string &key, const string &seen_val,
		const string &new_val, string &result) {

	int rc = commonOp(Const::ZSC_OPC_CMPSWP, key, seen_val, new_val, result, 1);

	result = extract_value(result);

	return rc;
}

int ZHTClient::compare_swap(const char *key, const char *seen_val,
		const char *new_val, char *result) {

	string skey(key);
	string sseen_val(seen_val);
	string snew_val(new_val);
	string sresult;

	int rc = compare_swap(skey, sseen_val, snew_val, sresult);

	strncpy(result, sresult.c_str(), sresult.size() + 1);

	return rc;
}

int ZHTClient::state_change_callback(const string &key,
		const string &expected_val, int lease) {

	string val2;
	string result;

	int rc = commonOp(Const::ZSC_OPC_STCHGCB, key, expected_val, val2, result,
			lease);

	return rc;
}

int ZHTClient::state_change_callback(const char *key, const char *expeded_val,
		int lease) {

	string skey(key);
	string sexpeded_val(expeded_val);

	int rc = state_change_callback(skey, sexpeded_val, lease);

	return rc;
}

string ZHTClient::commonOpInternal(const string &opcode, const string &key,
		const string &val, const string &val2, string &result, int lease) {

	ZPack zpack;
	zpack.set_opcode(opcode); //"001": lookup, "002": remove, "003": insert, "004": append, "005", compare_swap
	zpack.set_replicanum(3);

        //cout<<"Op Code"<<opcode<<"Key"<<key<<endl;

	if (key.empty())
		return Const::ZSC_REC_EMPTYKEY; //-1, empty key not allowed.
	else
		zpack.set_key(key);

	if (val.empty()) {

		zpack.set_val("^"); //coup, to fix ridiculous bug of protobuf! //to debug
		zpack.set_valnull(true);
	} else {

		zpack.set_val(val);
		zpack.set_valnull(false);
	}

	if (val2.empty()) {

		zpack.set_newval("?"); //coup, to fix ridiculous bug of protobuf! //to debug
		zpack.set_newvalnull(true);
	} else {

		zpack.set_newval(val2);
		zpack.set_newvalnull(false);
	}

	zpack.set_lease(Const::toString(lease));
//cout<<"lease "<<lease<<endl;
	string msg = zpack.SerializeAsString();
//cout<<msg<<endl;
	/*ZPack tmp;
	 tmp.ParseFromString(msg);
	 printf("{%s}:{%s,%s}\n", tmp.key().c_str(), tmp.val().c_str(),
	 tmp.newval().c_str());*/
/*

	char *buf = (char*) calloc(_msg_maxsize, sizeof(char));
	size_t msz = _msg_maxsize;

*/
	/*send to and receive from*/
	// this is old making large msg problem
	//_proxy->sendrecv(msg.c_str(), msg.size(), buf, msz);

	TCPProxy tcp;
		ZHTUtil zu;
		HostEntity he = zu.getHostEntityByKey(msg);
		int sock = tcp.getSockCached(he.host, he.port);
		tcp.sendTo(sock, (void*) msg.c_str(), msg.size());
		string res;

		int recvcount = loopedrecv(sock, NULL, res);

	/*...parse status and result*/
	string sstatus;
	//  srecv is no longer used due to a large msg fix. now instead of sendrecv,
	//we are using sendto and loopedrecv methods
/*
	string srecv(buf);
cout<<"res value is "<<res<<endl;
	if (srecv.empty()) {
		cout<<"empty"<<endl;
		sstatus = Const::ZSC_REC_SRVEXP;
	} else {

		result = srecv.substr(3); //the left, if any, is lookup result or second-try zpack
		//cout<<"srecv string size="<<srecv.size()<<endl;
		if (srecv.size() == 36) {
			sstatus = srecv.substr(0, 36);
			//sstatus=Const::ZSC_REC_REMOVEMETADATAQUEUE;
		} else {
			sstatus = srecv.substr(0, 3); //status returned, the first three chars, like 001, -98...
		}

	}
*/
//cout<<"res value is "<<res<<endl;
	if (res.empty()) {
		//cout<<"empty"<<endl;
		sstatus = Const::ZSC_REC_SRVEXP;
	} else {

		result = res.substr(3); //the left, if any, is lookup result or second-try zpack
		//cout<<"srecv string size="<<srecv.size()<<endl;
		if (res.size() == 36) {
			sstatus = res.substr(0, 36);
			//sstatus=Const::ZSC_REC_REMOVEMETADATAQUEUE;
		} else {
			sstatus = res.substr(0, 3); //status returned, the first three chars, like 001, -98...
		}

	}


//	free(buf);
	return sstatus;
}

int ZHTClient::teardown() {

	if (_proxy->teardown())
		return 0;
	else
		return -1;
}
