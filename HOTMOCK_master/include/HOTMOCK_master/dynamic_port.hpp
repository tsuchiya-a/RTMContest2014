// -*- C++ -*-
/*!
 * @file  dynamic_port.hpp
 * @brief dynamic inport/outport
 * @date $Date$
 *
 * @author ç≤ÅXñÿãB (Takeshi SASAKI) 
 * sasaki-t(_at_)ieee.org
 *
 *
 */

#ifndef DYNAMIC_PORT_HPP
#define DYNAMIC_PORT_HPP

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <new>
#include <stdexcept>

//not necessary -- the following RTM files have already been included in the "ComponentName.h".
//#include <rtm/RTObject.h> 
//#include <rtm/DataInPort.h>
//#include <rtm/DataOutPort.h>

template <class DataType> class DynamicInPort;
template <class DataType> class DynamicOutPort;

/*!
 * @class PortDataVect
 * @brief PortDataVect class
 */
template <class DataType>
class PortDataVect{
	friend class DynamicInPort<DataType>;
	friend class DynamicOutPort<DataType>;

	std::vector< DataType* > DataPtr;

	int addData();
	int deleteData(unsigned int i);
	/*!
	 * @brief Delete last data
	 * @return 0 if no error
	 */
	int deleteData(){ //default: delete last data
		if(DataPtr.empty()){return 1;}
		return deleteData(DataPtr.size()-1);
	}

public:
	PortDataVect();
	//PortDataVect(const PortDataVect &cp);
	~PortDataVect();

	typename std::vector< DataType* >::size_type getSize(){return DataPtr.size();}
	DataType &operator [](int i) const {return *(DataPtr[i]);}
};

/*!
 * @class InPortVect
 * @brief InPortVect class
 */
template <class DataType>
class InPortVect{
	friend class DynamicInPort<DataType>;

	std::vector< RTC::InPort<DataType>* > InPortPtr;

	int addInPort(const char* name, DataType& data);
	int deleteInPort(unsigned int i);
	/*!
	 * @brief Delete last port
	 * @return 0 if no error
	 */
	int deleteInPort(){
		if(InPortPtr.empty()){return 1;}
		return deleteInPort(InPortPtr.size()-1);
	}

public:
	InPortVect();
	//InPortVect(const InPortVect &cp);
	~InPortVect();

	typename std::vector< RTC::InPort<DataType>* >::size_type getSize(){return InPortPtr.size();}
	RTC::InPort<DataType> &operator [](int i) const { return *(InPortPtr[i]); }
};

/*!
 * @class OutPortVect
 * @brief OutPortVect class
 */
template <class DataType>
class OutPortVect{
	friend class DynamicOutPort<DataType>;

	std::vector< RTC::OutPort<DataType>* > OutPortPtr;

	int addOutPort(const char* name, DataType& data);
	int deleteOutPort(unsigned int i);
	/*!
	 * @brief Delete last port
	 * @return 0 if no error
	 */
	int deleteOutPort(){
		if(OutPortPtr.empty()){return 1;}
		return deleteOutPort(OutPortPtr.size()-1);
	}

public:
	OutPortVect();
	//OutPortVect(const OutPortVect &cp);
	~OutPortVect();

	typename std::vector< RTC::OutPort<DataType> >::size_type getSize(){return OutPortPtr.size();}
	RTC::OutPort<DataType> &operator [](int i) const { return *(OutPortPtr[i]); }
};

/*!
 * @class DynamicInPort
 * @brief DynamicInPort class
 */
template <class DataType>
class DynamicInPort{
	unsigned int m_id_used;
	std::string m_name_base;
	std::vector<std::string> m_register_name;

public:
	PortDataVect< DataType > m_data;
	InPortVect< DataType > m_port;

	DynamicInPort(const char* name);
	//DynamicInPort(const DynamicInPort &cp);
	~DynamicInPort();

	int addPort();
	int resetPort(unsigned int i);
	int deletePort(unsigned int i);
	/*!
	 * @brief Delete last port
	 * @return 0 if no error
	 */
	int deletePort(){
		if(m_register_name.empty()){return 1;}
		return deletePort(getSize()-1);
	}

	/*!
	 * @brief Get # of ports
	 * @return How many ports are registered
	 */
	std::vector< std::string >::size_type getSize(){return m_register_name.size();}
	/*!
	 * @brief Get port name from port index
	 * @param i Port index
	 * @return Port name or NULL (if i >= port_size)
	 */
	const char* getName(unsigned int i){
		try{
			return m_register_name.at(i).c_str();
		}
		catch(std::out_of_range){
			return NULL;
		}
	}
};

/*!
 * @class DynamicOutPort
 * @brief DynamicOutPort class
 */
template <class DataType>
class DynamicOutPort{
	unsigned int m_id_used;
	std::string m_name_base;
	std::vector<std::string> m_register_name;

public:
	PortDataVect< DataType > m_data;
	OutPortVect< DataType > m_port;

	DynamicOutPort(const char* name);
	//DynamicOutPort(const DynamicOutPort &cp);
	~DynamicOutPort();

	int addPort();
	int resetPort(unsigned int i);
	int deletePort(unsigned int i);
	/*!
	 * @brief Delete last port
	 * @return 0 if no error
	 */
	int deletePort(){ //default: delete last port
		if(m_register_name.empty()){return 1;}
		return deletePort(getSize()-1);
	}

	/*!
	 * @brief Get # of ports
	 * @return How many ports are registered
	 */
	std::vector< std::string >::size_type getSize(){return m_register_name.size();}
	/*!
	 * @brief Get port name from port index
	 * @param i Port index
	 * @return Port name (or NULL if invalid index)
	 */
	const char* getName(std::vector< std::string >::size_type i){
		try{
			return m_register_name.at(i).c_str();
		}
		catch(std::out_of_range){
			return NULL;
		}
	}
};

/*!
 * @brief Constructor
 */
template <class DataType >
PortDataVect<DataType>::PortDataVect(){

}

/*!
 * @brief Destructor
 */
template <class DataType >
PortDataVect<DataType>::~PortDataVect(){
	int result;

	//delete all ports
	while(!DataPtr.empty()){
		result = deleteData();
		if(result != 0){
			std::cerr << "Error in PortDataVect::~PortDataVect()" << std::endl;
		}
	}
}

/****************************************************************/
/* PortDataVect<DataType>::PortDataVect(const PortDataVect &cp) */
/*  Copy constructor                                            */
/****************************************************************/
/*
template <class DataType> 
PortDataVect<DataType>::PortDataVect(const PortDataVect &cp){
	int size = cp.getSize();
	int i,result;

	for(i=0;i<size;i++){
		result = addData();
		if(result!=0){std::exit(1);}
		*(DataPtr[i]) = *(cp.DataPtr[i]);
	}
}
*/

/*!
 * @brief Add new data to DataPtr
 * @return 0 if no error, 1 if allocation error
 */
template <class DataType> 
int PortDataVect<DataType>::addData(){
	DataType* pt = NULL;

	//dynamic memory allocation
	try{
		pt = new DataType;
	}
	catch(std::bad_alloc){
		std::cerr << "Error in PortDataVect::addData(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	try{
		DataPtr.push_back(pt);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in PortDataVect::addData(): BAD ALLOC Exception" << std::endl;
		delete pt;
		return 1;
	}

	return 0;
}

/*!
 * @brief Delete data from the DataPtr
 * @param i Index of data to be deleted
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int PortDataVect<DataType>::deleteData(unsigned int i){
	//check argument
	if(i>=DataPtr.size()){
		std::cerr << "Error in PortDataVect::deleteData(): Invalid argument" << std::endl;
		return 1;
	}

	//delete data
	if(DataPtr[i] != NULL){
		delete DataPtr[i];
	}

	//delete pointer to data
	typename std::vector<DataType *>::iterator it = DataPtr.begin();
	std::advance(it,i);
	it = DataPtr.erase(it);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Constructor
 */
template <class DataType> 
InPortVect<DataType>::InPortVect(){

}

/*!
 * @brief Destructor
 */
template <class DataType> 
InPortVect<DataType>::~InPortVect(){
	int result;

	//delete all ports
	while(!InPortPtr.empty()){
		result = deleteInPort();
		if(result != 0){
			std::cerr << "Error in InPortVect::~InPortVect()" << std::endl;
		}
	}
}

/**********************************************************/
/* InPortVect<DataType>::InPortVect(const InPortVect &cp) */
/*  Copy constructor                                      */
/**********************************************************/
/*
template <class DataType> 
InPortVect<DataType>::InPortVect(const InPortVect &cp){

}
*/

/*!
 * @brief Add data to InPortPtr
 * @param name Name (with index) of InPort
 * @param data Reference of a data variable (input data will be stored here)
 * @return 0 if no error, 1 if allocation error
 */
template <class DataType> 
int InPortVect<DataType>::addInPort(const char* name, DataType& data){
	RTC::InPort<DataType>* pt = NULL;

	//dynamic memory allocation
	try{
		pt = new RTC::InPort < DataType >(name, data);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in InPortVect::addInPort(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	try{
		InPortPtr.push_back(pt);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in InPortVect::addInPort(): BAD ALLOC Exception" << std::endl;
		delete pt;
		return 1;
	}

	return 0;
}

/*!
 * @brief Delete data from InPortPtr
 * @param i Index of port to be deleted
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int InPortVect<DataType>::deleteInPort(unsigned int i){
	//check argument
	if(i>=InPortPtr.size()){
		std::cerr << "Error in InPortVect::deleteInPort(): Invalid argument" << std::endl;
		return 1;
	}

	//delete port
	if(InPortPtr[i] != NULL){
		delete InPortPtr[i];
	}

	//delete pointer to port
	typename std::vector< RTC::InPort<DataType>* >::iterator it=InPortPtr.begin();
	std::advance(it,i);
	it = InPortPtr.erase(it);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Constructor
 */
template <class DataType> 
OutPortVect<DataType>::OutPortVect(){

}

/*!
 * @brief Destructor
 */
template <class DataType> 
OutPortVect<DataType>::~OutPortVect(){
	int result;

	//delete all ports
	while(!OutPortPtr.empty()){
		result = deleteOutPort();
		if(result != 0){
			std::cerr << "Error in OutPortVect::~OutPortVect()" << std::endl;
		}
	}
}

/*************************************************************/
/* OutPortVect<DataType>::OutPortVect(const OutPortVect &cp) */
/*  Copy constructor                                         */
/*************************************************************/
/*
template <class DataType> 
OutPortVect<DataType>::OutPortVect(const OutPortVect &cp){

}
*/

/*!
 * @brief Add data to InPortPtr
 * @param name Name (with index) of OutPort
 * @param data Reference of a data variable (output data will be stored here)
 * @return 0 if no error, 1 if allocation error
 */
template <class DataType> 
int OutPortVect<DataType>::addOutPort(const char* name, DataType& data){
	RTC::OutPort<DataType>* pt = NULL;

	//dynamic memory allocation
	try{
		pt = new RTC::OutPort < DataType >(name, data);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in OutPortVect::addOutPort(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	try{
		OutPortPtr.push_back(pt);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in OutPortVect::addOutPort(): BAD ALLOC Exception" << std::endl;
		delete pt;
		return 1;
	}

	return 0;
}

/*!
 * @brief Delete data from OutPortPtr
 * @param i Index of port to be deleted
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int OutPortVect<DataType>::deleteOutPort(unsigned int i){
	//check argument
	if(i>=OutPortPtr.size()){
		std::cerr << "Error in OutPortVect::deleteOutPort(): Invalid argument" << std::endl;
		return 1;
	}

	//delete port
	if(OutPortPtr[i] != NULL){
		delete OutPortPtr[i];
	}

	//delete pointer to port
	typename std::vector< RTC::OutPort<DataType>* >::iterator it=OutPortPtr.begin();
	std::advance(it,i);
	it = OutPortPtr.erase(it);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////


/*!
 * @brief Constructor
 * @param name Base name (name without index) of InPort
 */
template <class DataType> 
DynamicInPort<DataType>::DynamicInPort(const char* name){
	//set id used
	m_id_used = 0;

	//set name
	m_name_base = name;

}

/*!
 * @brief Destructor
 */
template <class DataType> 
DynamicInPort<DataType>::~DynamicInPort(){
	int result;

	//delete all ports
	while(!getSize()==0){
		result = deletePort();
		if(result != 0){
			std::cerr << "Error in DynamicInPort::~DynamicInPort()" << std::endl;
		}
	}
}

/*******************************************************************/
/* DynamicInPort<DataType>::DynamicInPort(const DynamicInPort &cp) */
/*  Copy constructor                                               */
/*******************************************************************/
/*
template <class DataType> 
DynamicInPort<DataType>::DynamicInPort(const DynamicInPort &cp){
	int size = cp.getSize();
	int i, result;
	std::string name;

	m_id_used = cp.m_id_used;
	m_name_base = cp.m_name_base;

	for(i=0;i<size;i++){
		name = cp.getName(i);
		m_register_name.push_back(name);

		//create data
		result = m_data.addData();
		if(result != 0){
			std::exit(1);
		}

		//create port
		result = m_port.addInPort(m_register_name[m_register_name.size()-1].c_str(), m_data[m_data.getSize()-1]);
		if(result != 0){
			std::exit(1);
		}
	}

}
*/

/*!
 * @brief Add new port
 * @return 0 if no error, 1 if allocation error, 2 error in m_data.addData(), 3 error in m_port.addInPort()
 */
template <class DataType> 
int DynamicInPort<DataType>::addPort(){
	int result;
	std::string name;
	std::ostringstream oss;

	//set name id
	oss << m_name_base.c_str() << m_id_used;
	name = oss.str();

	try{
		m_register_name.push_back(name);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in DynamicInPort<DataType>::addPort(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	//create data
	result = m_data.addData();
	if(result != 0){
		m_register_name.pop_back();
		return 2;
	}
	//create port
	result = m_port.addInPort(m_register_name[m_register_name.size()-1].c_str(), m_data[m_data.getSize()-1]);
	if(result != 0){
		m_register_name.pop_back();
		m_data.deleteData();
		return 3;
	}
	m_id_used++;

	return 0;
}

/*!
 * @brief Reset port
 * @param i Index of port to be reset
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int DynamicInPort<DataType>::resetPort(unsigned int i){
	//check argument
	if(i>=getSize()){
		std::cerr << "Error in DynamicInPort::resetPort(): Invalid argument" << std::endl;
		return 1;
	}

	//reset data
	//delete data
	if(m_data.DataPtr[i]!=NULL){
		delete m_data.DataPtr[i];
		m_data.DataPtr[i]=NULL;
	}
	//dynamic memory allocation
	try{
		m_data.DataPtr[i] = new DataType;
	}
	catch(std::bad_alloc){
		std::cerr << "Error in PortDataVect::addData(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	//reset port
	//delete port
	if(m_port.InPortPtr[i] != NULL){
		delete m_port.InPortPtr[i];
		m_port.InPortPtr[i] = NULL;
	}
	//dynamic memory allocation
	try{
		m_port.InPortPtr[i] = new RTC::InPort < DataType >(m_register_name[i].c_str(), m_data[i]);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in InPortVect::addInPort(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	return 0;
}

/*!
 * @brief Delete port
 * @param i Index of port to be deleted
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int DynamicInPort<DataType>::deletePort(unsigned int i){
	int result;

	//check argument
	if(i>=getSize()){
		std::cerr << "Error in DynamicInPort::deletePort(): Invalid argument" << std::endl;
		return 1;
	}

	//delete data
	result = m_data.deleteData(i);
	if(result != 0){
		return 1;
	}
	//delete port
	result = m_port.deleteInPort(i);
	if(result != 0){
		return 1;
	}

	//delete name
	std::vector< std::string >::iterator it=m_register_name.begin();
	std::advance(it,i);
	it = m_register_name.erase(it);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////


/*!
 * @brief Constructor
 * @param name Base name (name without index) of OutPort
 */
template <class DataType> 
DynamicOutPort<DataType>::DynamicOutPort(const char* name){
	//set id used
	m_id_used = 0;

	//set name
	m_name_base = name;

}

/*!
 * @brief Destructor
 */
template <class DataType> 
DynamicOutPort<DataType>::~DynamicOutPort(){
	int result;

	//delete all ports
	while(!getSize()==0){
		result = deletePort();
		if(result != 0){
			std::cerr << "Error in DynamicOutPort::~DynamicOutPort()" << std::endl;
		}
	}
}

/**********************************************************************/
/* DynamicOutPort<DataType>::DynamicOutPort(const DynamicOutPort &cp) */
/*  Copy constructor                                                  */
/**********************************************************************/
/*
template <class DataType> 
DynamicOutPort<DataType>::DynamicOutPort(const DynamicOutPort &cp){
	int size = cp.getSize();
	int i, result;
	std::string name;

	m_id_used = cp.m_id_used;
	m_name_base = cp.m_name_base;

	for(i=0;i<size;i++){
		name = cp.getName(i);
		m_register_name.push_back(name);

		//create data
		result = m_data.addData();
		if(result != 0){
			std::exit(1);
		}

		//create port
		result = m_port.addOutPort(m_register_name[m_register_name.size()-1].c_str(), m_data[m_data.getSize()-1]);
		if(result != 0){
			std::exit(1);
		}
	}

}
*/

/*!
 * @brief Add new port
 * @return 0 if no error, 1 if allocation error, 2 error in m_data.addData(), 3 error in m_port.addOutPort()
 */
template <class DataType> 
int DynamicOutPort<DataType>::addPort(){
	int result;
	std::string name;
	std::ostringstream oss;

	//set name id
	oss << m_name_base.c_str() << m_id_used;
	name = oss.str();

	try{
		m_register_name.push_back(name);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in DynamicOutPort<DataType>::addPort(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	//create data
	result = m_data.addData();
	if(result != 0){
		m_register_name.pop_back();
		return 2;
	}
	//create port
	result = m_port.addOutPort(m_register_name[m_register_name.size()-1].c_str(), m_data[m_data.getSize()-1]);
	if(result != 0){
		m_register_name.pop_back();
		m_data.deleteData();
		return 3;
	}
	m_id_used++;

	return 0;
}

/*!
 * @brief Reset port
 * @param i Index of port to be reset
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int DynamicOutPort<DataType>::resetPort(unsigned int i){
	//check argument
	if(i>=getSize()){
		std::cerr << "Error in DynamicOutPort::resetPort(): Invalid argument" << std::endl;
		return 1;
	}

	//reset data
	//delete data
	if(m_data.DataPtr[i]!=NULL){
		delete m_data.DataPtr[i];
		m_data.DataPtr[i]=NULL;
	}
	//dynamic memory allocation
	try{
		m_data.DataPtr[i] = new DataType;
	}
	catch(std::bad_alloc){
		std::cerr << "Error in PortDataVect::addData(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	//reset port
	//delete port
	if(m_port.OutPortPtr[i] != NULL){
		delete m_port.OutPortPtr[i];
		m_port.OutPortPtr[i] = NULL;
	}
	//dynamic memory allocation
	try{
		m_port.OutPortPtr[i] = new RTC::OutPort < DataType >(m_register_name[i].c_str(), m_data[i]);
	}
	catch(std::bad_alloc){
		std::cerr << "Error in OutPortVect::addOutPort(): BAD ALLOC Exception" << std::endl;
		return 1;
	}

	return 0;
}


/*!
 * @brief Delete port
 * @param i Index of port to be deleted
 * @return 0 if no error, 1 if invalid index
 */
template <class DataType> 
int DynamicOutPort<DataType>::deletePort(unsigned int i){
	int result;

	//check argument
	if(i>=getSize()){
		std::cerr << "Error in DynamicOutPort::deletePort(): Invalid argument" << std::endl;
		return 1;
	}

	//delete data
	result = m_data.deleteData(i);
	if(result != 0){
		return 1;
	}
	//delete port
	result = m_port.deleteOutPort(i);
	if(result != 0){
		return 1;
	}

	//delete name
	std::vector< std::string >::iterator it=m_register_name.begin();
	std::advance(it,i);
	it = m_register_name.erase(it);

	return 0;
}

#endif
