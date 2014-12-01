// -*- C++ -*-
/*!
 * @file  KobukiControllerByHMSwitches.h
 * @brief HOTMOCK Switches Control Kobuki
 * @date  $Date$
 *
 * @author 土屋彩茜
 * ma14076@shibaura-it.ac.jp
 *
 * $Id$
 */

#ifndef KOBUKICONTROLLERBYHMSWITCHES_H
#define KOBUKICONTROLLERBYHMSWITCHES_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class KobukiControllerByHMSwitches
 * @brief HOTMOCK Switches Control Kobuki
 *
 * HOTMOCKのスイッチでKobukiを動かすためのコンポーネント。
 *
 * InPort:<name>/<datatype>/<documentation>
 * Forward/
 * TimedShort/前進指令を出すスイッチの値を取得するポート。
 * Back/ TimedShort/後退指令を出すスイッチの値を取得するポート。
 * Right/
 * TimedShort/右回りの指令を出すスイッチの値を取得するポート。
 * Left/
 * TimedShort/左回りの指令を出すスイッチの値を取得するポート。
 * OutPort:<name>/<datatype>/<documentation>
 * Velocity/TimedVelocity2D/kobukiへの速度指令値を出力するポート。
 * Configuration:<name>/<datatype>/<default>
 * /<widget>/<documentation>
 * Speed/double/0.2/text/Kobukiが前進・後退する時の速度。x>=0
 * RotSpeed/double/0.5/text/x>=0/Kobukiが右回転・左回転する時の速度
 * 。
 *
 */
class KobukiControllerByHMSwitches
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  KobukiControllerByHMSwitches(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~KobukiControllerByHMSwitches();

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
   * 現在の速度を0に初期化する。
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
   * Kobukiを停止させる。
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
   * 押されたスイッチによって速度指令を出力する。
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
   * Kobukiが前進・後退する時の速度。
   * - Name: speed Speed
   * - DefaultValue: 0.2
   * - Constraint: x>=0
   */
  double m_Speed;
  /*!
   * Kobukiが右回転・左回転する時の速度。
   * - Name: rot_speed RotSpeed
   * - DefaultValue: 0.5
   * - Constraint: x>=0
   */
  double m_RotSpeed;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedShort m_Forward;
  /*!
   * 前進指令を出すスイッチの値を取得するポート。
   * - Type: TimedShort
   */
  InPort<RTC::TimedShort> m_ForwardIn;
  RTC::TimedShort m_Back;
  /*!
   * 後退指令を出すスイッチの値を取得するポート。
   * - Type: TimeShort
   */
  InPort<RTC::TimedShort> m_BackIn;
  RTC::TimedShort m_Right;
  /*!
   * 右回りの指令を出すスイッチの値を取得するポート。
   * - Type: TimedShort
   */
  InPort<RTC::TimedShort> m_RightIn;
  RTC::TimedShort m_Left;
  /*!
   * 左回りの指令を出すスイッチの値を取得するポート。
   * - Type: TimedShort
   */
  InPort<RTC::TimedShort> m_LeftIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedVelocity2D m_Velocity;
  /*!
   * kobukiへの速度指令値を出力するポート。
   * - Type: TimedVelocity2D
   */
  OutPort<RTC::TimedVelocity2D> m_VelocityOut;
  
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
  Velocity2D command;

  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void KobukiControllerByHMSwitchesInit(RTC::Manager* manager);
};

#endif // KOBUKICONTROLLERBYHMSWITCHES_H
