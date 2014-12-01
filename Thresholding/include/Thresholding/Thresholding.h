// -*- C++ -*-
/*!
 * @file  Thresholding.h
 * @brief Send Command by Thresholding the Configuration
 * @date  $Date$
 *
 * @author 土屋彩茜
 * ma14076@shibaura-it.ac.jp
 *
 * $Id$
 */

#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>
#include "VectorConvert.h"

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class Thresholding
 * @brief Send Command by Thresholding the Configuration
 *
 * 入力された値と閾値を比較して、二値化した値を出力するコンポーネン
 * ト。
 * 閾値と出力する値はConfigurationで設定する。
 * 設定された値が不適切な場合はエラーになる。
 *
 * InPort:<name>/<datatype>/<documentation>
 * DoubleInData/TimedDouble/double型のデータを入力するポート。
 * ShortInData/TimedShort/short型のデータを入力するポート。
 * DoubleThreshold/TimedDouble/閾値となるdouble型を入力するポート。
 * ShortThreshold/TimedShort/閾値となるshort型を入力するポート。
 * OutPort:<name>/<datatype>/<documentation>
 * OutData/TimedShort/二値化された値を出力するポート。
 * Configuration:<name>/<datatype>/<default>
 * /<widget>/<documentation>
 * Threshold/double/50.0/text/閾値。
 * Threshold_Mode/int/0/radio/どの閾値を使用するか選択する。0ならば
 * Configuration：Threshold、1ならばInPort：DoubleInData、2ならばI
 * nPort：ShortInDataが閾値として用いられる。
 * OutDataValue/std::vector<short>/0,0,1/出力する値。値は要素数0か
 * ら順に閾値より小さい、閾値と等しい、閾値より大きいとなる。
 *
 */
class Thresholding
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  Thresholding(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~Thresholding();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
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
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

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
   * ポートを初期化する。
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
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
   * 入力された値と閾値を比較して、二値化した値を出力する。
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
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
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
   * 閾値。
   * - Name: Threshold Threshold
   * - DefaultValue: 50.0
   */
  double m_Threshold;
  /*!
   * どの閾値を使用するか選択する。
   * 0ならばConfiguration：Threshold、1ならばInPort：DoubleInData、
   * 2ならばInPort：ShortInData
   * が閾値として用いられる。
   * - Name: Threshold_Mode Threshold_Mode
   * - DefaultValue: 0
   * - Constraint: (0,1,2)
   */
  int m_Threshold_Mode;
  /*!
   * 出力する値。
   * 値は要素数0から順に閾値より小さい、閾値と等しい、閾値より大き
   * いとなる。
   * - Name: OutDataValue OutDataValue
   * - DefaultValue: 0,0,1
   */
  std::vector<short> m_OutDataValue;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedDouble m_DoubleInData;
  /*!
   * double型のデータを入力するポート。
   * - Type: TimedDouble
   */
  InPort<RTC::TimedDouble> m_DoubleInDataIn;
  RTC::TimedShort m_ShortInData;
  /*!
   * short型のデータを入力するポート。
   * - Type: TimedShort
   */
  InPort<RTC::TimedShort> m_ShortInDataIn;
  RTC::TimedDouble m_DoubleThreshold;
  /*!
   * 閾値となるdouble型を入力するポート。
   * - Type: TimedDouble
   */
  InPort<RTC::TimedDouble> m_DoubleThresholdIn;
  RTC::TimedShort m_ShortThreshold;
  /*!
   * 閾値となるshort型を入力するポート。
   * - Type: TimedShort
   */
  InPort<RTC::TimedShort> m_ShortThresholdIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedShort m_OutData;
  /*!
   * 二値化された値を出力するポート。
   * - Type: TimedShort
   */
  OutPort<RTC::TimedShort> m_OutDataOut;
  
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
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void ThresholdingInit(RTC::Manager* manager);
};

#endif // THRESHOLDING_H
