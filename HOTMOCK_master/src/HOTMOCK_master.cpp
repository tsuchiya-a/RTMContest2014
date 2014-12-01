// -*- C++ -*-
/*!
 * @file  HOTMOCK_master.cpp
 * @brief Connect HOTMOCK devices with RTC
 * @date $Date$
 *
 * @author 土屋彩茜
 * ma14076@shibaura-it.ac.jp
 *
 * $Id$
 */

#include "HOTMOCK_master.h"

// Module specification
// <rtc-template block="module_spec">
static const char* hotmock_master_spec[] =
  {
    "implementation_id", "HOTMOCK_master",
    "type_name",         "HOTMOCK_master",
    "description",       "Connect HOTMOCK devices with RTC",
    "version",           "1.2.0",
    "vendor",            "Ayaka Tsuchiya",
    "category",          "tool",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.SettingFilename", "*.hmst",
    "conf.default.IPAddress", "127.0.0.1",
    "conf.default.PortNumber", "8888",
    "conf.default.GetDataType", "0,0,0,0",
    // Widget
    "conf.__widget__.SettingFilename", "text",
    "conf.__widget__.IPAddress", "text",
    "conf.__widget__.PortNumber", "text",
    "conf.__widget__.GetDataType", "ordered_list",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
HOTMOCK_master::HOTMOCK_master(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_DOIn("DO"),
    m_AOIn("AO"),
    m_Reset_PIIn("Reset_PI"),
    m_DIOut("DI"),
    m_AIOut("AI"),
    m_PIOut("PI"),
    m_TSOut("TS", m_TS),
    m_GSOut("GS", m_GS)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
HOTMOCK_master::~HOTMOCK_master()
{
}


/*!
 * DynamicPortのRTCへの登録を行う。
 */
RTC::ReturnCode_t HOTMOCK_master::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set Port buffers

  // HOTMOCKSettingで設定できる最大数のポートをRTCへ登録する
  for(int i=0;i<16;i++){
	m_DOIn.addPort();
	lastFlagList_DO.push_back(0);
  }

  for(int i=0;i<2;i++){
	m_AOIn.addPort();
	lastFlagList_AO.push_back(0);
  }

  for(int i=0;i<2;i++){
	m_Reset_PIIn.addPort();
	m_PIOut.addPort();
	lastFlagList_PI.push_back(0);
  }

  for(int i=0;i<32;i++){
	m_DIOut.addPort();
	lastFlagList_DI.push_back(0);
  }

  for(int i=0;i<6;i++){
	m_AIOut.addPort();
	lastFlagList_AI.push_back(0);
  }

  addOutPort("TS", m_TSOut);
  addOutPort("GS", m_GSOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("SettingFilename", m_SettingFilename, "*.hmst");
  bindParameter("IPAddress", m_IPAddress, "127.0.0.1");
  bindParameter("PortNumber", m_PortNumber, "8888");
  bindParameter("GetDataType", m_GetDataType, "0,0,0,0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*!
 * DynamicPortの削除を行う。
 */

RTC::ReturnCode_t HOTMOCK_master::onFinalize()
{
  // 現在使用しているポートを削除する
  for(int i=0;i<connectIDList_DO.size();i++){
	  removeInPort(m_DOIn.m_port[portNameList_DO[i]]);
  }
  for(int i=0;i<connectIDList_AO.size();i++){
	  removeInPort(m_AOIn.m_port[connectIDList_AO[i]]);
  }
  for(int i=0;i<connectIDList_PI.size();i++){
	  removeInPort(m_Reset_PIIn.m_port[connectIDList_PI[i]]);
	  removeOutPort(m_PIOut.m_port[connectIDList_PI[i]]);
  }
  for(int i=0;i<connectIDList_DI.size();i++){
	  removeOutPort(m_DIOut.m_port[connectIDList_DI[i]]);
  }
  for(int i=0;i<connectIDList_AI.size();i++){
	  removeOutPort(m_AIOut.m_port[portNameList_AI[i]]);
  }

  // RTCから登録したポートを削除する
  while(m_DOIn.getSize()>0){
	  m_DOIn.deletePort(m_DOIn.getSize()-1);
  }
  while(m_AOIn.getSize()>0){
	  m_AOIn.deletePort(m_AOIn.getSize()-1);
  }
  while(m_Reset_PIIn.getSize()>0){
	  m_Reset_PIIn.deletePort(m_Reset_PIIn.getSize()-1);
  }
  while(m_DIOut.getSize()>0){
	  m_DIOut.deletePort(m_DIOut.getSize()-1);
  }
  while(m_AIOut.getSize()>0){
	  m_AIOut.deletePort(m_AIOut.getSize()-1);
  }
  while(m_PIOut.getSize()>0){
	  m_PIOut.deletePort(m_PIOut.getSize()-1);
  }

  lastFlagList_DO.clear();
  lastFlagList_AO.clear();
  lastFlagList_DI.clear();
  lastFlagList_AI.clear();
  lastFlagList_PI.clear();

  connectIDList_DO.clear();
  connectIDList_AO.clear();
  connectIDList_DI.clear();
  connectIDList_AI.clear();
  connectIDList_PI.clear();

  portNameList_DO.clear();
  portNameList_AI.clear();

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HOTMOCK_master::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HOTMOCK_master::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * HOTMOCKデバイスの設定ファイルを読み込み、データポートの生成・削
 * 除を行う。その後ソケット通信を開始する。
 */

RTC::ReturnCode_t HOTMOCK_master::onActivated(RTC::UniqueId ec_id)
{
  int res;
  // .hmstファイルを読み込む
  res=hmst.initialize(m_SettingFilename);
  if(res!=0){
	  std::cerr << "---SettingFile is fault or make XML file error---" << std::endl;
	  return RTC::RTC_ERROR;
  }

  // 使用しているボードの型を保存、デバイスにフラグを立てる
  res=hmst.setPort();
  if(res!=0){
	  std::cerr << "---XML file is fault---" << std::endl;
	  return RTC::RTC_ERROR;
  }

  // 使用しているポートの初期化
  connectIDList_DO.clear();
  connectIDList_AO.clear();
  connectIDList_DI.clear();
  connectIDList_AI.clear();
  connectIDList_PI.clear();
	
  portNameList_DO.clear();
  portNameList_AI.clear();

  // 使用しているコネクタ位置（デバイス本体の表示）に対応する名前のポートを生成
  // 　　ex) デバイス表示名:DI01 ---> ポート名:DI1
  // 使用しているコネクタ位置をconnectIDList_**に保存
  // 　　ex) connectIDList_DO[i]=j ---> i番目のポートのポート名はDOj
  //
  // ! DO,AIのみデバイス本体の表示とHOTMOCKSettingの表示が違うため、デバイス本体に表示する名前をportNameList_**に保存

  for(int i=0;i<hmst.configFilenameList.size();i++){
	if(hmst.configFilenameList[i]=="DO_Config.xml"){ //DO Portの設定を行う
		for(int j=0;j<hmst.useIDFlagList[i].size();j++){
			if(lastFlagList_DO[j]==0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していなかったポートを今回使用する
				connectIDList_DO.push_back(j+1);
				if(hmst.boardType=="digital"){
					portNameList_DO.push_back(j+1);
					addInPort(m_DOIn.getName(j+1), m_DOIn.m_port[j+1]);
				}
				else if(hmst.boardType=="analog"){ //!アナログボードの場合、デバイス表示名はDO7から開始する(ソケット通信用のIDはDO06から)
					portNameList_DO.push_back(j+2);
					addInPort(m_DOIn.getName(j+2), m_DOIn.m_port[j+2]);
				}
			}
			else if(lastFlagList_DO[j]!=0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していたポートを今回も使用する
				connectIDList_DO.push_back(j+1);
				if(hmst.boardType=="digital"){
					portNameList_DO.push_back(j+1);
				}
				else if(hmst.boardType=="analog"){ //!アナログボードの場合、デバイス表示名はDO7から開始する(ソケット通信用のIDはDO06から)
					portNameList_DO.push_back(j+2);
				}
			}
			else if(lastFlagList_DO[j]!=0&&hmst.useIDFlagList[i][j]==0){ //前に使用していたポートを今回は使用しない
				if(hmst.boardType=="digital"){
					removeInPort(m_DOIn.m_port[j+1]);
					m_DOIn.resetPort(j+1);
				}
				else if(hmst.boardType=="analog"){ //!アナログボードの場合、デバイス表示名はDO7から開始する(ソケット通信用のIDはDO06から)
					removeInPort(m_DOIn.m_port[j+2]);
					m_DOIn.resetPort(j+2);
				}
			}
			else ; //前に使用していなかったポートを今回も使用しない--->何もしない

			// 今回使用しているポートを記録する
			lastFlagList_DO[j]=hmst.useIDFlagList[i][j];
		}
//		std::cout<<"add DO Port"<<std::endl;
	}
	else if(hmst.configFilenameList[i]=="AO_Config.xml"){ //AO Portの設定を行う
		for(int j=0;j<hmst.useIDFlagList[i].size();j++){
			if(lastFlagList_AO[j]==0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していなかったポートを今回使用する
				connectIDList_AO.push_back(j+1);
				addInPort(m_AOIn.getName(j+1), m_DOIn.m_port[j+1]);
			}
			else if(lastFlagList_AO[j]!=0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していたポートを今回も使用する
				connectIDList_AO.push_back(j+1);
			}
			else if(lastFlagList_AO[j]!=0&&hmst.useIDFlagList[i][j]==0){ //前に使用していたポートを今回は使用しない
				removeInPort(m_AOIn.m_port[j+1]);
				m_AOIn.resetPort(j+1);
			}
			else ; //前に使用していなかったポートを今回も使用しない--->何もしない

			// 今回使用しているポートを記録する
			lastFlagList_AO[j]=hmst.useIDFlagList[i][j];
		}
//		std::cout<<"add AO Port"<<std::endl;
	}
	else if(hmst.configFilenameList[i]=="PI_Config.xml"){ //Reset_PI Port,PI Portの設定を行う
		for(int j=0;j<hmst.useIDFlagList[i].size();j++){
			if(lastFlagList_PI[j]==0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していなかったポートを今回使用する
				connectIDList_PI.push_back(j+1);
				addInPort(m_Reset_PIIn.getName(j+1), m_Reset_PIIn.m_port[j+1]);
				addOutPort(m_PIOut.getName(j+1), m_PIOut.m_port[j+1]);
			}
			else if(lastFlagList_PI[j]!=0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していたポートを今回も使用する
				connectIDList_PI.push_back(j+1);
			}
			else if(lastFlagList_PI[j]!=0&&hmst.useIDFlagList[i][j]==0){ //前に使用していたポートを今回は使用しない
				removeInPort(m_Reset_PIIn.m_port[j+1]);
				m_Reset_PIIn.resetPort(j+1);
				removeOutPort(m_PIOut.m_port[j+1]);
				m_PIOut.resetPort(j+1);
			}
			else ; //前に使用していなかったポートを今回も使用しない--->何もしない

			// 今回使用しているポートを記録する
			lastFlagList_PI[j]=hmst.useIDFlagList[i][j];
		}
//		std::cout<<"add PI Port"<<std::endl;
	}
	else if(hmst.configFilenameList[i]=="DI_Config.xml"){ //DI Portの設定を行う
		for(int j=0;j<hmst.useIDFlagList[i].size();j++){
			if(lastFlagList_DI[j]==0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していなかったポートを今回使用する
				connectIDList_DI.push_back(j+1);
				addOutPort(m_DIOut.getName(j+1), m_DIOut.m_port[j+1]);
			}
			else if(lastFlagList_DI[j]!=0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していたポートを今回も使用する
				connectIDList_DI.push_back(j+1);
			}
			else if(lastFlagList_DI[j]!=0&&hmst.useIDFlagList[i][j]==0){ //前に使用していたポートを今回は使用しない
				removeOutPort(m_DIOut.m_port[j+1]);
				m_DIOut.resetPort(j+1);
			}
			else ; //前に使用していなかったポートを今回も使用しない--->何もしない

			// 今回使用しているポートを記録する
			lastFlagList_DI[j]=hmst.useIDFlagList[i][j];
		}
//		std::cout<<"add DI Port"<<std::endl;
	}
	else if(hmst.configFilenameList[i]=="AI_Config.xml"){ //AI Portの設定を行う
		for(int j=0;j<hmst.useIDFlagList[i].size();j++){
			if(lastFlagList_AI[j]==0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していなかったポートを今回使用する
				connectIDList_AI.push_back(j+1);
				if(hmst.boardType=="digital"){
					portNameList_AI.push_back(j+1);
					addOutPort(m_AIOut.getName(j+1), m_AIOut.m_port[j+1]);
				}
				else if(hmst.boardType=="analog"){ //!アナログボードの場合、デバイス表示名はAI1から開始する(ソケット通信用のIDはAI02から)
					portNameList_AI.push_back(j);
					addOutPort(m_AIOut.getName(j), m_AIOut.m_port[j]);
				}
			}
			else if(lastFlagList_AI[j]!=0&&hmst.useIDFlagList[i][j]!=0){ //前に使用していたポートを今回も使用する
				connectIDList_AI.push_back(j+1);
				if(hmst.boardType=="digital"){
					portNameList_AI.push_back(j+1);
				}
				else if(hmst.boardType=="analog"){ //!アナログボードの場合、デバイス表示名はAI1から開始する(ソケット通信用のIDはAI02から)
					portNameList_AI.push_back(j);
				}
			}
			else if(lastFlagList_AI[j]!=0&&hmst.useIDFlagList[i][j]==0){ //前に使用していたポートを今回は使用しない
				if(hmst.boardType=="digital"){
					removeOutPort(m_AIOut.m_port[j+1]);
					m_AIOut.resetPort(j+1);
				}
				else if(hmst.boardType=="analog"){ //!アナログボードの場合、デバイス表示名はAI1から開始する(ソケット通信用のIDはAI02から)
					removeOutPort(m_AIOut.m_port[j]);
					m_AIOut.resetPort(j);
				}
			}
			else ; //前に使用していなかったポートを今回も使用しない--->何もしない

			// 今回使用しているポートを記録する
			lastFlagList_AI[j]=hmst.useIDFlagList[i][j];
		}
//		std::cout<<"add AI Port"<<std::endl;
	}
  }

  std::cout << "add Port finished" << std::endl;

  if(hmst.boardType=="digital"){
	std::cout << "hotmock board type = digital" << std::endl;
	res=hmc.initialize(hotmock::HotmockBoardType::Digital,m_IPAddress.c_str(),m_PortNumber);
	if(res != 0){
		std::cerr << "---can not connected---" << std::endl;
		return RTC::RTC_ERROR;
	}
  }
  else if(hmst.boardType=="analog"){
	std::cout << "hotmock board type = analog" << std::endl;
	hmc.initialize(hotmock::HotmockBoardType::Analog,m_IPAddress.c_str(),m_PortNumber);
	if(res != 0){
		std::cerr << "---can not disconnected---" << std::endl;
		return RTC::RTC_ERROR;
	}
  }

  std::cout << "connecting..." << std::endl << std::endl;

  return RTC::RTC_OK;
}

/*!
 * ソケット通信を終了する。
 */

RTC::ReturnCode_t HOTMOCK_master::onDeactivated(RTC::UniqueId ec_id)
{
  hmst.finalize();
  //ソケット通信を終了する
  hmc.finalize();
  std::cout << "---disconnected---" << std::endl << std::endl;
  return RTC::RTC_OK;
}

/*!
 * ソケット通信によりHOTMOCKデバイスとデータのやり取りを行う。
 */

RTC::ReturnCode_t HOTMOCK_master::onExecute(RTC::UniqueId ec_id)
{
  short DO;
  boolean RPI;
//!  double AO;
  hotmock::Vector3d GS;
  std::vector<int> param;

  //InPort(Port:DO,AO,Reset_PI)に入力された値をHOTMOCKデバイスに送信する
  for(int i=0;i<connectIDList_DO.size();i++){
	if(m_DOIn.m_port[portNameList_DO[i]].isNew()){
		m_DOIn.m_port[portNameList_DO[i]].read();
		DO = m_DOIn.m_data[portNameList_DO[i]].data;
		std::cout << "DO Port" << portNameList_DO[i] << " : " << DO << std::endl << std::endl;
		if(DO==1){
			on_or_off = hotmock::DO_ON;
		}
		else if(DO==2){
			on_or_off = hotmock::DO_OFF;
		}
		hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::OUTPUT,hotmock::HotmockConnectorType::DO,connectIDList_DO[i],on_or_off);
	}
  }

  // ! AOに対応するデバイスはHOTMOCK側で未実装
/*  for(int i=0;i<connectIDList_AO.size();i++){
	if(m_AOIn.m_port[connectIDList_AO[i]].isNew()){
		m_AOIn.m_port[connectIDList_AO[i]].read();
		AO = m_AOIn.m_data[connectIDList_AO[i]].data;
//		std::cout << "AO Port" << connectIDList_AO[i] << " : " << AO << std::endl << std::endl;
		hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::OUTPUT,hotmock::HotmockConnectorType::AO,connectIDList_AO[i],AO);
	}
  }*/

  // PIの積算値をリセットする
  for(int i=0;i<connectIDList_PI.size();i++){
	if(m_Reset_PIIn.m_port[connectIDList_PI[i]].isNew()){
		m_Reset_PIIn.m_port[connectIDList_PI[i]].read();
		RPI = m_Reset_PIIn.m_data[connectIDList_PI[i]].data;
		std::cout << "Reset_PI Port" << connectIDList_PI[i] << " : " << RPI << std::endl << std::endl;
		if(RPI){
			on_or_off = hotmock::DO_ON;
		}
		else {
			on_or_off = hotmock::DO_OFF;
		}
		hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::INIT,hotmock::HotmockConnectorType::PI,connectIDList_PI[i],on_or_off);
	}
  }

  //HOTMOCKデバイスから受信したデータをOutPort(Port:DI,AI,TS,GS)に出力する
  int res = hmc.recvDataFromHotmock();
  if(res < 0){ //server disconnected
	std::cout << "---Server disconnected---" << std::endl << std::endl;
	return RTC::RTC_ERROR;
  }

  //DIの値を出力する
  for(int i=0;i<connectIDList_DI.size();i++){
	if(hmc.DIData.isNew(connectIDList_DI[i])){
		m_DIOut.m_data[connectIDList_DI[i]].data = hmc.DIData.getLatestData(connectIDList_DI[i]);
		std::cout << "DI Port " << connectIDList_DI[i] << " : " << m_DIOut.m_data[connectIDList_DI[i]].data << std::endl<<std::endl; 
		m_DIOut.m_port[connectIDList_DI[i]].write();
	}
  }

  //AI,PI,TS,GSに要求するデータの種類を設定する
  //Configurationで指定した配列の要素数が4より大きい場合、前4つを使用する
  if(m_GetDataType.size()>=4){
	for(int i=0;i<4;i++){ //AI:param[0],PI:param[1],TS:param[2],GS:param[3]
		if(m_GetDataType[i]==0){
			param.push_back(hotmock::REQUEST_RAW);
		}
		else if(m_GetDataType[i]==1){
			param.push_back(hotmock::REQUEST_PROCESSED);
		}
		else{
			std::cerr << "---GetDataType only 1 or 0---" << std::endl;
			return RTC::RTC_ERROR;
		}
	}
  }
  //Configurationで指定した配列の要素数が4より小さい場合、0を補填する
  else{
//	std::cout<<"!!! Configuration:DataType not enough !!!"<<std::endl;
	for(int i=0;i<m_GetDataType.size();i++){ //AI:param[0],PI:param[1],TS:param[2],GS:param[3]
		if(m_GetDataType[i]==0){
			param.push_back(hotmock::REQUEST_RAW);
		}
		else if(m_GetDataType[i]==1){
			param.push_back(hotmock::REQUEST_PROCESSED);
		}
		else{
			std::cout<<"---GetDataType only 1 or 0---"<<std::endl;
			return RTC::RTC_ERROR;
		}
	}
	for(int i=m_GetDataType.size();i<4;i++){
		param.push_back(hotmock::REQUEST_RAW); //指定されていない場合は生値を要求する
	}
  }

  //AI,PI,TS,GSの値をHOTMOCKに要求し、値を出力する
  if(connectIDList_AI.size()!=0){
	for(int i=0;i<connectIDList_AI.size();i++){
		hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::REQUEST,hotmock::HotmockConnectorType::AI,connectIDList_AI[i],param[0]);
		if(hmc.AIData.isNew(connectIDList_AI[i])){
			m_AIOut.m_data[portNameList_AI[i]].data = hmc.AIData.getLatestData(connectIDList_AI[i]);
			std::cout << "AI Port" << portNameList_AI[i] << " : " << m_AIOut.m_data[portNameList_AI[i]].data << std::endl << std::endl;
			m_AIOut.m_port[portNameList_AI[i]].write(); 
		}
	}
  }

  if(connectIDList_PI.size()!=0){
	for(int i=0;i<connectIDList_PI.size();i++){
		hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::REQUEST,hotmock::HotmockConnectorType::PI,connectIDList_PI[i],param[1]);
		if(hmc.PIData.isNew(connectIDList_PI[i])){
			m_PIOut.m_data[connectIDList_PI[i]].data = hmc.PIData.getLatestData(connectIDList_PI[i]);
			std::cout << "PI Port" << connectIDList_PI[i] << " : " << m_PIOut.m_data[connectIDList_PI[i]].data << std::endl << std::endl;
			m_PIOut.m_port[connectIDList_PI[i]].write(); 
		}
	}
  }

  hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::REQUEST,hotmock::HotmockConnectorType::TS,1,param[2]);
  if(hmc.TSData.isNew(1)){
	m_TS.data = hmc.TSData.getLatestData(1);
	std::cout << "TS data: " << m_TS.data << std::endl << std::endl; 
	m_TSOut.write();
  }

  hmc.sendCommandToHotmock(hotmock::HotmockClientCommand::REQUEST,hotmock::HotmockConnectorType::GS,1,param[3]);
  if(hmc.GSData.isNew(1)){
	GS=hmc.GSData.getLatestData(1);
	m_GS.data.length(3);
	m_GS.data[0]=GS.x;
	m_GS.data[1]=GS.y;
	m_GS.data[2]=GS.z;
	std::cout << "GS data: x=" << m_GS.data[0] << " y=" << m_GS.data[1] << " z=" << m_GS.data[2] << std::endl << std::endl;
	m_GSOut.write();
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HOTMOCK_master::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HOTMOCK_master::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * 終了処理を行う。
 */

RTC::ReturnCode_t HOTMOCK_master::onReset(RTC::UniqueId ec_id)
{
  hmst.finalize();
  hmc.finalize();
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t HOTMOCK_master::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t HOTMOCK_master::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void HOTMOCK_masterInit(RTC::Manager* manager)
  {
    coil::Properties profile(hotmock_master_spec);
    manager->registerFactory(profile,
                             RTC::Create<HOTMOCK_master>,
                             RTC::Delete<HOTMOCK_master>);
  }
  
};


