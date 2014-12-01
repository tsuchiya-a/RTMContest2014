// -*- C++ -*-
/*!
 * @file  hotmockclient.cpp
 * @brief hotmock client class used to connect to hotmock server
 * @date $Date$
 *
 * @author ç≤ÅXñÿãB (Takeshi SASAKI) 
 * sasaki-t(_at_)ieee.org
 *
 *
 */

#include <iostream>
#include <iomanip> //needed for setfill and setw
#include <sstream> //needed for ostringstream
#include <string>
#include <cstring> //needed for memcpy

#include "hotmockclient.h"

namespace hotmock{

	/*!
	 * @brief Constuctor
	 */
	HotmockConnectorInformation::HotmockConnectorInformation(HotmockBoardType type){
		switch(type){
			case Legacy:
			case Digital:
				DIFirstConnectorID=1;
				DOFirstConnectorID=1;
				AIFirstConnectorID=1;
				AOFirstConnectorID=0;
				PIFirstConnectorID=1;
				GSFirstConnectorID=1;
				TSFirstConnectorID=1;
				DIConnectorNum=15;
				DOConnectorNum=5;
				AIConnectorNum=1;
				AOConnectorNum=0;
				PIConnectorNum=2;
				GSConnectorNum=1;
				TSConnectorNum=1;
				break;

			case Analog:
				DIFirstConnectorID=16;
				DOFirstConnectorID=6;
				AIFirstConnectorID=1;
				AOFirstConnectorID=1;
				PIFirstConnectorID=0;
				GSFirstConnectorID=1;
				TSFirstConnectorID=1;
				DIConnectorNum=10;
				DOConnectorNum=5;
				AIConnectorNum=5;
				AOConnectorNum=2;
				PIConnectorNum=0;
				GSConnectorNum=1;
				TSConnectorNum=1;
				break;

			default:
				DIFirstConnectorID=0;
				DOFirstConnectorID=0;
				AIFirstConnectorID=0;
				AOFirstConnectorID=0;
				PIFirstConnectorID=0;
				GSFirstConnectorID=0;
				TSFirstConnectorID=0;
				DIConnectorNum=0;
				DOConnectorNum=0;
				AIConnectorNum=0;
				AOConnectorNum=0;
				PIConnectorNum=0;
				GSConnectorNum=0;
				TSConnectorNum=0;
				std::cerr << "Error in HotmockConnectorInformation::HotmockConnectorInformation(): invalid board type" << std::endl;
		}
	}

	/*!
	 * @brief Constuctor
	 */
	HotmockClient::HotmockClient(){
		sock = INVALID_SOCKET;
		wsa_set = false;
	}

	/*!
	 * @brief Destructor
	 */
	HotmockClient::~HotmockClient(){
		finalize();
	}

	/*!
	 * @brief Initialization - set data buffer and connect to Hotmock server
	 * @param hmtype Hotmock type
	 * @param ip IP of the server
	 * @param port port number of the server
	 * @return 0 if no error
	 */
	int HotmockClient::initialize(HotmockBoardType hmtype, const char *ip, unsigned short port){
		finalize();

		HotmockConnectorInformation info(hmtype);

		connector_info = info;

		//Set buffer depending on the hotmock type
		DIData.initialize(connector_info.DIConnectorNum, connector_info.DIFirstConnectorID);
		PIData.initialize(connector_info.PIConnectorNum, connector_info.PIFirstConnectorID);
		AIData.initialize(connector_info.AIConnectorNum, connector_info.AIFirstConnectorID);
		GSData.initialize(connector_info.GSConnectorNum, connector_info.GSFirstConnectorID);
		TSData.initialize(connector_info.TSConnectorNum, connector_info.TSFirstConnectorID);

		//Set flag to check if connector accepts request or not
		PIRequestAcceptable.resize(connector_info.AIConnectorNum,true);
		AIRequestAcceptable.resize(connector_info.AIConnectorNum,true);
		GSRequestAcceptable.resize(connector_info.GSConnectorNum,true);
		TSRequestAcceptable.resize(connector_info.TSConnectorNum,true);

		// Initialize Winsock
		int res = WSAStartup(MAKEWORD(2,2), &wsaData);
		if(res != 0) {
			WSACleanup();
			std::cerr << "WSAStartup failed with error:" << res << std::endl;
			return res;
		}
		wsa_set = true;

		res = connectToHotmock(ip, port);
		if(res != 0){
			finalize();
		}

		return res;
	}

	/*!
	 * @brief Close socket and clear message
	 */
	void HotmockClient::finalize(){
		if(sock != INVALID_SOCKET){
			closesocket(sock);
			sock = INVALID_SOCKET;
		}
		if(wsa_set){
			WSACleanup();
			wsa_set = false;
		}

		message.clear();
		PIRequestAcceptable.clear();
		AIRequestAcceptable.clear();
		GSRequestAcceptable.clear();
		TSRequestAcceptable.clear();
	}

	/*!
	 * @brief Connect to server
	 * @param ip IP of the server
	 * @param port port number of the server
	 * @return 0 if no error, -1 if error
	 */
	int HotmockClient::connectToHotmock(const char *ip, unsigned short port){
		struct sockaddr_in addr; //server address information

		//create socket
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock == INVALID_SOCKET){
			std::cerr << "error in socket" << std::endl;
			return -1;
		}

		addr.sin_family = AF_INET;    // host byte order
		addr.sin_port = htons(port);  // short, network byte order  / port -> each program
		addr.sin_addr.s_addr = inet_addr(ip);	// IP of the server
		memset(&(addr.sin_zero), '\0', 8);  // zero the rest of the struct

		std::cerr << "Connecting to the server:" << ip << " port:" << port << "..."  << std::endl;

		if(connect(sock, (sockaddr *)&addr, sizeof(sockaddr)) == -1){
			std::cerr << "error in connect" << std::endl;
			return -1;
		}
		std::cerr << "connected" << std::endl << std::endl;

		//set nonblocking mode
		u_long iMode = 1;
		ioctlsocket(sock, FIONBIO, &iMode);

		return 0;
	}

	/*!
	 * @brief Convert HotmockConnectorType data to corresponding string used in communication message
	 * @param type HotmockConnectorType data to be converted
	 * @return corresponding string
	 */
	std::string HotmockClient::HMConnectorTyepe2String(HotmockConnectorType type){
		switch(type){
			case DI: //Digital Input
				return std::string("DI");
			case DO: //Digital Output
				return std::string("DO");
			case AI: //Analog Input
				return std::string("AI");
			case PI: //Pulse Input
				return std::string("PI");
			case GS: //Accelerometer (onboard)
				return std::string("GS");
			case TS: //Temperature Sensor (onboard)
				return std::string("TS");
			default:
				return std::string("");
		}
	}

	/*!
	 * @brief Convert HotmockClientCommand data to corresponding string used in communication message
	 * @param type HotmockClientCommand data to be converted
	 * @return corresponding string
	 */
	std::string HotmockClient::HMClientCommand2String(HotmockClientCommand command){
		switch(command){
			case REQUEST: //Request current data
				return std::string("REQUEST");
			case OUTPUT: //Output to device
				return std::string("OUTPUT");
			case INIT: //Initialize
				return std::string("INIT");
			default:
				return std::string("");
		}
	}

	/*!
	 * @brief Parse message received from Hotmock. The data value is stored corresponding HotmockData buffer
	 */
	void HotmockClient::parseMessage(){
		std::string cmd;
		std::string::size_type cmd_end_loc;
		std::string::size_type loc1;
		std::string::size_type loc2;
		std::vector<std::string> args;
		int connectorID;

		for(;;){
			cmd_end_loc = message.find_first_of('&');
			if(cmd_end_loc == std::string::npos){ //not contain command
				break;
			}

			//get all arguments devided by commas
			cmd = message.substr(0, cmd_end_loc);
			args.clear();
			loc1 = 0;
			int k=0;
			do{
				loc2 = cmd.substr(loc1).find_first_of(',');
				args.push_back(cmd.substr(loc1,loc2));
				loc1 += loc2+1;
//				std::cout << "args[" << k << "]:" << args.back() << std::endl; //for debug
				k++;
			}while(loc2!=std::string::npos);


			//set data depending on the connector type and ID
			if(args[0].size()>=3){
				//get connector ID
				connectorID = std::stoi(args[0].substr(2));

				//GS<connector ID>,<real>,<real>,<real>
				if(args[0].substr(0,2)=="GS"){ //+3 argments
					if(args.size()!=4){
						std::cerr << "Invalid message received: " << message.substr(0,cmd_end_loc+1) << std::endl;
					}else{
						Vector3d value;
						value.x = stof(args[1]);
						value.y = stof(args[2]);
						value.z = stof(args[3]);
						GSData.setData(connectorID, value);
						GSRequestAcceptable[connectorID-connector_info.GSFirstConnectorID] = true;
					}
				}else{ //+1 argment
					if(args.size()!=2){
						std::cerr << "Invalid message received: " << message.substr(0,cmd_end_loc+1) << std::endl;
					}else{
						//DI<connector ID>,<unsigned integer>
						if(message.substr(0,2)=="DI"){
							unsigned char value = (unsigned char)stoi(args[1]);
							DIData.setData(connectorID, value);
						}else{
							//{PI,AI,TS}<connector ID>,<real>
							double value = stof(args[1]);
							if(message.substr(0,2)=="PI"){
								PIData.setData(connectorID, value);
								PIRequestAcceptable[connectorID-connector_info.PIFirstConnectorID] = true;
							}else if(message.substr(0,2)=="AI"){
								AIData.setData(connectorID, value);
								AIRequestAcceptable[connectorID-connector_info.AIFirstConnectorID] = true;
							}else if(message.substr(0,2)=="TS"){
								TSData.setData(connectorID, value);
								TSRequestAcceptable[connectorID-connector_info.TSFirstConnectorID] = true;
							}else{
								std::cerr << "Invalid message received: " << message.substr(0,cmd_end_loc+1) << std::endl;
							}
						}
					}
				}
			}else{
				std::cerr << "Invalid message received: " << message.substr(0,cmd_end_loc+1) << std::endl;
			} //end if(args[0].size()<3)

			//erase processed message
			message.erase(0,cmd_end_loc+1);
		} //end for(;;)

		return;
	}

	/*!
	 * @brief Send command to Hotmock
	 * @param cmd Command
	 * @param type Connector type
	 * @param connectorID Connector ID
	 * @param param Parameter for the command
	 * @return number of bytes sent if on success, -1 if send error, <-2 if invalid arguments
	 */
	int HotmockClient::sendCommandToHotmock(HotmockClientCommand cmd, HotmockConnectorType type, unsigned short connectorID, int param){
		int res;
		std::ostringstream oss;
		std::string command = HMClientCommand2String(cmd);
		std::string connector = HMConnectorTyepe2String(type);
		unsigned short id;
		unsigned int num;
		std::vector<bool>::iterator it_ra_vect;

		//check command
		//check if connector ID is correct
		switch(type){
			case DI:
				id = connector_info.DIFirstConnectorID;
				num = connector_info.DIConnectorNum;
				break;
			case DO:
				id = connector_info.DOFirstConnectorID;
				num = connector_info.DOConnectorNum;
				break;
			case AI:
				id = connector_info.AIFirstConnectorID;
				num = connector_info.AIConnectorNum;
				break;
			/*
			case AO:
				id = connector_info.AOFirstConnectorID;
				num = connector_info.AOConnectorNum;
				break;
			*/
			case PI:
				id = connector_info.PIFirstConnectorID;
				num = connector_info.PIConnectorNum;
				break;
			case GS:
				id = connector_info.GSFirstConnectorID;
				num = connector_info.GSConnectorNum;
				break;
			case TS:
				id = connector_info.TSFirstConnectorID;
				num = connector_info.TSConnectorNum;
				break;
			default:
				std::cerr << "Error in HotmockClient::sendCommandToHotmock(): invalid connector type" << std::endl;
				return -2;
		}
		if(connectorID < id || connectorID >= id + num){
			std::cerr << "Error in HotmockClient::sendCommandToHotmock(): connectorID out of range: connector=" << connector << connectorID << std::endl;
			return -3;
		}

		//check if combination of command and type is correct
		if(cmd==REQUEST){
			switch(type){
				case PI:
					if(!PIRequestAcceptable[connectorID-connector_info.PIFirstConnectorID]){return 0;} //request not accepted
					it_ra_vect = PIRequestAcceptable.begin();
					advance(it_ra_vect, connectorID-connector_info.PIFirstConnectorID);
					break;
				case AI:
					if(!AIRequestAcceptable[connectorID-connector_info.AIFirstConnectorID]){return 0;} //request not accepted
					it_ra_vect = AIRequestAcceptable.begin();
					advance(it_ra_vect, connectorID-connector_info.AIFirstConnectorID);
					break;
				case GS:
					if(!GSRequestAcceptable[connectorID-connector_info.GSFirstConnectorID]){return 0;} //request not accepted
					it_ra_vect = GSRequestAcceptable.begin();
					advance(it_ra_vect, connectorID-connector_info.GSFirstConnectorID);
					break;
				case TS:
					if(!TSRequestAcceptable[connectorID-connector_info.TSFirstConnectorID]){return 0;} //request not accepted
					it_ra_vect = TSRequestAcceptable.begin();
					advance(it_ra_vect, connectorID-connector_info.TSFirstConnectorID);
					break;
				default:
					std::cerr << "Error in HotmockClient::sendCommandToHotmock(): invalid combination of command and connector type: command="  << command << ", connector type=" << connector << std::endl;
					return -4;
			}
			if(param!=0 && param!=1){
				std::cerr << "Error in HotmockClient::sendCommandToHotmock(): invalid parameter: command="  << command << ", connector type=" << connector << ", parameter=" << param << std::endl;		
				return -5;
			}
		}else if(((cmd==OUTPUT) && (type != DO)) || ((cmd==INIT) && (type != PI))){
			std::cerr << "Error in HotmockClient::sendCommandToHotmock(): invalid combination of command and connector type: command="  << command << ", connector type=" << connector << std::endl;
			return -4;
		}


		//set command
		oss << command << ',' << connector << std::setfill('0') << std::setw(2) << connectorID;
		if(param >= 0){
			oss << ',' << param;
		}
		oss << '&'; //add delimiter

		std::cout << "send command:" << oss.str() << " size:" << oss.str().size() << std::endl;
		res = send(sock,oss.str().c_str(),oss.str().size(),0);
		//set flag
		if(res >= 0 && cmd == REQUEST){
			*it_ra_vect = false;
		}
		return res;
	}

	/*!
	 * @brief Send command to Hotmock
	 * @param cmd Command
	 * @param type Target connector type of the command
	 * @param connectorID Target connector ID of the command
	 * @param param Parameter for the command
	 * @return received message size if no error, negative value if error
	 */
	int HotmockClient::recvDataFromHotmock(){
		const int bufsize = 256;
		int res;
		char buf[bufsize];

		std::memset(buf,0,bufsize-1);
		res = recv(sock,buf,bufsize-1,0);
		if(res > 0){
			std::cout << "received: " << buf << std::endl;
			message += buf;
			std::cout << "message in buffer: " << message << std::endl;

			parseMessage();
		}else{
			int nError = WSAGetLastError();
			if(nError!=WSAEWOULDBLOCK && nError!=0){
				std::cerr << "Error in receive message" << std::endl;
			}else{
				res = 0;
			}
		}
		
		return res;
	}

}
