// -*- C++ -*-
/*!
 * @file  HOTMOCK_master.h
 * @brief Connect HOTMOCK devices with RTC
 * @date  $Date$
 *
 * @author 土屋彩茜
 * ma14076@shibaura-it.ac.jp
 *
 * $Id$
 */

#ifndef HOTMOCK_MASTER_H
#define HOTMOCK_MASTER_H
#include "hotmockclient.h"
#include "hotmocksetting.h"

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>
#include "dynamic_port.hpp"
#include "VectorConvert.h"

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class HOTMOCK_master
 * @brief Connect HOTMOCK devices with RTC
 *
 * HOTMOCKSettingとソケット通信を行い、HOTMOCKデバイスを使用するた
 * めのコンポーネント。
 * 初期化時にデバイス設定ファイルを読み込み、ポートが生成される。
 *
 * InPort:<name>/<datatype>/<documentation>
 * DO/TimedShort/
 * HOTMOCKデバイスのDO*に送信するデジタル出力値を取得するポート
 * AO/TimedDouble/
 * HOTMOCKデバイスのAO*に送信するアナログ出力値を取得するポート
 * Reset_PI/
 * TimedBoolean/HOTMOCKデバイスのPI*につながっているパルス入力の積
 * 算値をリセットするためのフラグを取得するポート
 * OutPort:<name>/<datatype>/<documentation>
 * DI/TimedShort/
 * HOTMOCKデバイスのDI*から受信したデジタル入力値を送るポート。
 * AI/TimedDouble/
 * HOTMOCKデバイスのAI*から受信したアナログ入力値を送るポート。
 * PI/TimedDouble/
 * HOTMOCKデバイスのPI*から受信したパルス入力値を送るポート。
 * TS/TimedDouble/HOTMOCKデバイス内蔵の温度センサから受信した値を送
 * るポート。
 * GS/TimedDoubleSeq/HOTMOCKデバイス内蔵の加速度センサから受信した
 * 値を送るポート。値は要素数0から順にx,y,zの配列として送られる。
 * Configuration:<name>/<datatype>/<default>
 * /<widget>/<documentation>
 * SettingFilename/string/*.hmst/text/HOTMOCKSettingによって作成さ
 * れたHOTMOCKデバイスの設定ファイル名。
 * IPAddress/string/127.0.0.1/text/HOTMOCKSettingのIPアドレス。
 * PortNumber/int/8888/text/HOTMOCKSettingのPort番号。
 * GetDataType/std::vector<int>/0,0,0,0/ordered_list/HOTMOCKデバイ
 * スからの入力を取得する際、0ならば生値を取得、1ならば工業変換値を
 * 取得する。配列0から順にAI,PI, TS,GSの設定をする。
 *
 */
class HOTMOCK_master
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  HOTMOCK_master(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~HOTMOCK_master();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   * DynamicPortのRTCへの登録を行う。
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   * DynamicPortの削除を行う。
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   * HOTMOCKデバイスの設定ファイルを読み込み、データポートの生成・
   * 削除を行う。その後ソケット通信を開始する。
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * @pre HOTMOCKSettingが起動・シュミレーションモードになっている
   * こと。
   * @post HOTMOCKSettingが通信中になっていること。
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   * ソケット通信を終了する。
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   * ソケット通信によりHOTMOCKデバイスとデータのやり取りを行う。
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   * 終了処理を行う。
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * HOTMOCKSettingによって作成されたHOTMOCKデバイスの設定ファイル
   * 名。
   * - Name: SettingFilename SettingFilename
   * - DefaultValue: *.hmst
   */
  std::string m_SettingFilename;
  /*!
   * HOTMOCKSettingのIPアドレス。
   * - Name: IPAddress IPAddress
   * - DefaultValue: 127.0.0.1
   */
  std::string m_IPAddress;
  /*!
   * HOTMOCKSettingのPort番号。
   * - Name: PortNumber PortNumber
   * - DefaultValue: 8888
   */
  int m_PortNumber;
  /*!
   * HOTMOCKデバイスからの入力を取得する際、
   * 0ならば生値を取得、1ならば工業変換値を取得する。
   * 配列0から順にAI,PI,TS,GSの設定をする。
   * - Name: GetDataType GetDataType
   * - DefaultValue: 0,0,0,0
   */
  std::vector<int> m_GetDataType;

   // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  //RTC::TimedShort m_DO;
  /*!
   * HOTMOCKデバイスのDO*に送信するデジタル出力値を取得するポート。
   * - Type: TimedShort
   */
  //InPort<RTC::TimedShort> m_DOIn;
  DynamicInPort<TimedShort> m_DOIn;

  //RTC::TimedDouble m_AO;
  /*!
   * HOTMOCKデバイスのAO*に送信するアナログ出力値を取得するポート。
   * - Type: TimedDouble
   */
  //InPort<RTC::TimedDouble> m_AOIn;
  DynamicInPort<TimedDouble> m_AOIn;

  //RTC::TimedBoolean m_Reset_PI;
  /*!
   * HOTMOCKデバイスのPI01につながっているパルス入力の積算値をリセ
   * ットするためのフラグを取得するポート。
   * - Type: TimedBoolean
   */
  //InPort<RTC::TimedBoolean> m_Reset_PIIn;
  DynamicInPort<TimedBoolean> m_Reset_PIIn;
  
  // </rtc-template>

  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  //RTC::TimedShort m_DI;
  /*!
   * HOTMOCKデバイスのDI*から受信したデジタル入力値を送るポート。
   * - Type: TimedShort
   */
  //OutPort<RTC::TimedShort> m_DIOut;
  DynamicOutPort<TimedShort> m_DIOut;

  //RTC::TimedDouble m_AI;
  /*!
   * HOTMOCKデバイスのAI*から受信したアナログ入力値を送るポート。
   * - Type: TimedDouble
   */
  //OutPort<RTC::TimedDouble> m_AIOut;
  DynamicOutPort<TimedDouble> m_AIOut;

  //RTC::TimedDouble m_PI;
  /*!
   * HOTMOCKデバイスのPI*から受信したパルス入力値を送るポート。
   * - Type: TimedDouble
   */
  //OutPort<RTC::TimedDouble> m_PIOut;
  DynamicOutPort<TimedDouble> m_PIOut;

  RTC::TimedDouble m_TS;
  /*!
   * HOTMOCKデバイス内蔵の温度センサから受信した値を送るポート。
   * - Type: TimedDouble
   */
  OutPort<RTC::TimedDouble> m_TSOut;

  RTC::TimedDoubleSeq m_GS;
  /*!
   * HOTMOCKデバイス内蔵の加速度センサから受信した値を送るポート。
   * 値は要素数0から順にx,y,zの配列として送られる。
   * - Type: TimedDoubleSeq
   */
  OutPort<RTC::TimedDoubleSeq> m_GSOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
	hotmock::HotmockSetting hmst;
	hotmock::HotmockClient hmc;

	std::vector<unsigned short> lastFlagList_DO;
	std::vector<unsigned short> lastFlagList_AO;
	std::vector<unsigned short> lastFlagList_DI;
	std::vector<unsigned short> lastFlagList_AI;
	std::vector<unsigned short> lastFlagList_PI;

	std::vector<unsigned short> connectIDList_DO;
	std::vector<unsigned short> connectIDList_AO;
	std::vector<unsigned short> connectIDList_DI;
	std::vector<unsigned short> connectIDList_AI;
	std::vector<unsigned short> connectIDList_PI;

	//!AI,DOはコネクタ位置がデバイス本体の表示とHOTMOCKSettingの表示が異なっている!
	std::vector<unsigned short> portNameList_AI;
	std::vector<unsigned short> portNameList_DO;

	int on_or_off;
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void HOTMOCK_masterInit(RTC::Manager* manager);
};

#endif // HOTMOCK_MASTER_H
