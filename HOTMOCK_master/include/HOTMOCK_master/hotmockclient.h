// -*- C++ -*-
/*!
 * @file  hotmockclient.h
 * @brief hotmock client class which connects to hotmock server
 * @date $Date$
 *
 * @author ç≤ÅXñÿãB (Takeshi SASAKI) 
 * sasaki-t(_at_)ieee.org
 *
 *
 */
#ifndef HOTMOCKCLIENT_H
#define HOTMOCKCLIENT_H

//needed for socket
#include <winsock2.h> //must be included before #include <windows.h>
#include <ws2tcpip.h> //must be included before #include <windows.h>
#include <windows.h>

#include <string>
#include <vector>
#include <deque>

namespace hotmock{

	/*!
	 * @name digital input states
	 * Values used in digital input (especially for tact switch)
	 */
	/* @{ */
	const int DI_PRESS = 1; /*!< Switch pressed */
	const int DI_RELEASE = 2; /*!< Switch released */
	const int DI_LONGPRESS = 3; /*!< Switch pressed for a certain period of time */
	/* @} */

	/*!
	 * @name command parameters
	 * Parameters used in sending command to Hotmock
	 */
	/* @{ */
	const int DO_OFF = 0; /*!< "OFF" for digital output (ex. turn off LED) */
	const int DO_ON = 1; /*!< "ON" for digital output (ex. turn on LED) */

	const int REQUEST_RAW = 0; /*!< Request raw data (for AI, PI, TS or GS) */
	const int REQUEST_PROCESSED = 1; /*!< Request processed data (for AI, PI, TS or GS) */
	/* @} */

	/*!
	 * @class Vector3d
	 * @brief 3D vector data for onboard accelerometer
	 */
	class Vector3d{
	public:
		double x;
		double y;
		double z;
	};

	/*!
	 * @enum HotmockBoardType
	 * @brief Hotmock board type
	 */
	enum HotmockBoardType{
		Legacy, /*!< Legacy Kit */
		Digital, /*!< Digital Kit */
		Analog /*!< Analog Kit */
	};

	/*!
	 * @enum HotmockConnectorType
	 * @brief Connector type used in message
	 */
	enum HotmockConnectorType{
		DI, /*!< Digital Input */
		DO, /*!< Digital Output */
		AI, /*!< Analog Input */
		//AO, /*!< Analog Ouput (not implemented yet) */
		PI, /*!< Pulse Input */
		GS, /*!< Accelerometer (onboard) */
		TS /*!< Temperature Sensor (onboard) */
	};

	/*!
	 * @enum HotmockClientCommand
	 * @brief Command used to send message to hotmock
	 */
	enum HotmockClientCommand{
		REQUEST, /*!< Request current data */
		OUTPUT, /*!< Output to device */
		INIT /*!< Initialize */
	};

	/*!
	 * @class HotmockConnectorInformation
	 * @brief HotmockConnectorInformation class
	 */
	class HotmockConnectorInformation{
		public:
		unsigned short DIFirstConnectorID; /*!< ID number assigned to first digital input connector */
		unsigned short DOFirstConnectorID; /*!< ID number assigned to first digital output connector */
		unsigned short AIFirstConnectorID; /*!< ID number assigned to first analog input connector */
		unsigned short AOFirstConnectorID; /*!< ID number assigned to first analog output connector */
		unsigned short PIFirstConnectorID; /*!< ID number assigned to first pulse input connector */
		unsigned short GSFirstConnectorID; /*!< ID number assigned to first accelerometer */
		unsigned short TSFirstConnectorID; /*!< ID number assigned to first temperature sensor */

		unsigned int DIConnectorNum; /*!< # of digital inputs */
		unsigned int DOConnectorNum; /*!< # of digital outputs */
		unsigned int AIConnectorNum; /*!< # of analog inputs */
		unsigned int AOConnectorNum; /*!< # of analog outputs */
		unsigned int PIConnectorNum; /*!< # of pulse inputs */
		unsigned int GSConnectorNum; /*!< # of accelerometers */
		unsigned int TSConnectorNum; /*!< # of temperature sensors */

		HotmockConnectorInformation(HotmockBoardType type = Digital);
	};

	/*!
	 * @class HotmockData
	 * @brief HotmockData class
	 */
	template <class DataType>
	class HotmockData{
		std::vector< std::deque<DataType> > data; //ring buffer
		int index_offset;

	public:
		static const int max_data_store = 20;

		HotmockData(typename std::vector<DataType>::size_type size=0, unsigned short firstConnectorID=0);
		~HotmockData();
		int initialize(typename std::vector<DataType>::size_type size=0, unsigned short firstConnectorID=0);
		//void finalize();

		bool isNew(unsigned short connectorID);
		int setData(unsigned short connectorID, DataType value);

		DataType getNextData(unsigned short connectorID);
		DataType getLatestData(unsigned short connectorID);
	};

	/*!
	 * @class HotmockClient
	 * @brief HotmockClient class
	 */
	class HotmockClient{
		HotmockConnectorInformation connector_info;

		//communication
		SOCKET sock;
		WSADATA wsaData;
		bool wsa_set;
		std::string message; //message

		int connectToHotmock(const char *ip, unsigned short port);
		void parseMessage();

		std::string HMConnectorTyepe2String(HotmockConnectorType type);
		std::string HMClientCommand2String(HotmockClientCommand command);
	public:
		HotmockClient();
		~HotmockClient();
		int initialize(HotmockBoardType hmtype, const char *ip="127.0.0.1", unsigned short port=8888);
		void finalize();

		//bool isNewData(HotmockConnectorType type, unsigned short connectorID);
		//int getData(HotmockConnectorType type, unsigned short connectorID);

		int sendCommandToHotmock(HotmockClientCommand cmd, HotmockConnectorType type, unsigned short connectorID, int param=-1);
		int recvDataFromHotmock();

		//data
		HotmockData<unsigned char> DIData; /*< data from digital inputs */
		HotmockData<double> PIData; /*< data from pulse inputs */
		HotmockData<double> AIData; /*< data from analog inputs */
		HotmockData<Vector3d> GSData; /*< data from accelerometers */
		HotmockData<double> TSData; /*< data from temperature sensors */

		//request acceptable or not
		std::vector<bool> PIRequestAcceptable;
		std::vector<bool> AIRequestAcceptable;
		std::vector<bool> GSRequestAcceptable;
		std::vector<bool> TSRequestAcceptable;
	};

	/*!
	 * @brief Constuctor
	 * @param size buffer size
	 * @param firstconnectorID ID number assigned to first connector installed on the device
	 */
	template <class DataType>
	HotmockData<DataType>::HotmockData(typename std::vector<DataType>::size_type size, unsigned short firstConnectorID){
		initialize(firstConnectorID);
	}

	/*!
	 * @brief Destructor
	 */
	template <class DataType>
	HotmockData<DataType>::~HotmockData(){
	}

	/*!
	 * @brief Initialization
	 * @param size buffer size
	 * @param firstconnectorID ID number assigned to first connector installed on the device
	 * @return 0 if no error
	 */
	template <class DataType>
	int HotmockData<DataType>::initialize(typename std::vector<DataType>::size_type size, unsigned short firstConnectorID){
		index_offset = (int)firstConnectorID;
		data.clear();
		data.resize(size);
		return 0;
	}

	/*!
	 * @brief Check if Add data to InPortPtr
	 * @param connectorID Connector ID to be checked
	 * @return true if new data stored, false if new data not arrived or invalid connector ID
	 */
	template <class DataType>
	bool HotmockData<DataType>::isNew(unsigned short connectorID){
		int index = connectorID-index_offset;

		//out of range
		if(index<0 || index >= data.size()){
			std::cerr << "Error in HotmockData::isNew(): out of range (" << connectorID << ")" << std::endl;
			return false;
		}

		if(!data[index].empty()){
			return true;
		}else{
			return false;
		}
	}

	/*!
	 * @brief Get data which is not read yet (should be used after checking if new data arrived using isNew())
	 * @param connectorID Connector ID to be read
	 * @return stored data
	 */
	template <class DataType>
	DataType HotmockData<DataType>::getNextData(unsigned short connectorID){
		int index = connectorID-index_offset;

		DataType value = data[index].front();
		data[index].pop_front();
		return value;
	}

	/*!
	 * @brief Get the latest data which is not read and old data is erased (should be used after checking if new data arrived using isNew())
	 * @param connectorID Connector ID to be read
	 * @return stored data
	 */
	template <class DataType>
	DataType HotmockData<DataType>::getLatestData(unsigned short connectorID){
		int index = connectorID-index_offset;
		DataType value = data[index].back();
		data[index].clear();
		return value;
	}

	/*!
	 * @brief Set data to buffer
	 * @param connectorID Connector ID cooresponding to the buffer
	 * @param value Data to be set
	 * @return 0 if no error, -1 out of range
	 */
	template <class DataType>
	int HotmockData<DataType>::setData(unsigned short connectorID, DataType value){
		int index = connectorID-index_offset;

		if(index<0 || index >= data.size()){
			std::cerr << "Error in HotmockData::setData(): out of range (" << connectorID << ")" << std::endl;
			return -1;
		}

		while(data[index].size()>=max_data_store){
			data[index].pop_front();
		}
		data[index].push_back(value);
		return 0;
	}
};
#endif