// -*- C++ -*-
/*!
 * @file  KobukiControllerByHMSwitches.cpp
 * @brief HOTMOCK Switches Control Kobuki
 * @date $Date$
 *
 * @author 土屋彩茜
 * ma14076@shibaura-it.ac.jp
 *
 * $Id$
 */

#include "KobukiControllerByHMSwitches.h"

// Module specification
// <rtc-template block="module_spec">
static const char* kobukicontrollerbyhmswitches_spec[] =
  {
    "implementation_id", "KobukiControllerByHMSwitches",
    "type_name",         "KobukiControllerByHMSwitches",
    "description",       "HOTMOCK Switches Control Kobuki",
    "version",           "1.0.1",
    "vendor",            "Ayaka Tsuchiya",
    "category",          "demonstration",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.Speed", "0.2",
    "conf.default.RotSpeed", "0.5",
    // Widget
    "conf.__widget__.Speed", "text",
    "conf.__widget__.RotSpeed", "text",
    // Constraints
    "conf.__constraints__.Speed", "x>=0",
    "conf.__constraints__.RotSpeed", "x>=0",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
KobukiControllerByHMSwitches::KobukiControllerByHMSwitches(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ForwardIn("Forward", m_Forward),
    m_BackIn("Back", m_Back),
    m_RightIn("Right", m_Right),
    m_LeftIn("Left", m_Left),
    m_VelocityOut("Velocity", m_Velocity)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
KobukiControllerByHMSwitches::~KobukiControllerByHMSwitches()
{
}



RTC::ReturnCode_t KobukiControllerByHMSwitches::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Forward", m_ForwardIn);
  addInPort("Back", m_BackIn);
  addInPort("Right", m_RightIn);
  addInPort("Left", m_LeftIn);
  
  // Set OutPort buffer
  addOutPort("Velocity", m_VelocityOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("Speed", m_Speed, "0.2");
  bindParameter("RotSpeed", m_RotSpeed, "0.5");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * 現在の速度を0に初期化する。
 */

RTC::ReturnCode_t KobukiControllerByHMSwitches::onActivated(RTC::UniqueId ec_id)
{
  command.vx=0;
  command.vy=0;
  command.va=0;
  return RTC::RTC_OK;
}

/*!
 * Kobukiを停止させる。
 */

RTC::ReturnCode_t KobukiControllerByHMSwitches::onDeactivated(RTC::UniqueId ec_id)
{
  command.vx=0;
  command.vy=0;
  command.va=0;

  m_Velocity.data=command;
  m_VelocityOut.write();
  return RTC::RTC_OK;
}

/*!
 * 押されたスイッチによって速度指令を出力する。
 */

RTC::ReturnCode_t KobukiControllerByHMSwitches::onExecute(RTC::UniqueId ec_id)
{
  bool output = false;

  if(m_ForwardIn.isNew()){ //前進指令がきたら
	m_ForwardIn.read();
	if(m_Forward.data==1){ //1(press)のとき走行する
		command.vx += m_Speed;
		output = true;
	}else if(m_Forward.data==2){ //2(release)のとき停止する
		command.vx = 0;
		output = true;
	}
	std::cout<<"forward"<<std::endl;
  }
  if(m_BackIn.isNew()){  //後退指令がきたら
	m_BackIn.read();
	if(m_Back.data==1){ //1(press)のとき走行する
		command.vx -= m_Speed;
		output = true;
	}else if(m_Back.data==2){ //2(release)のとき停止する
		command.vx = 0;
		output = true;
	}
	std::cout<<"back"<<std::endl;
  }
  if(m_RightIn.isNew()){  //右回転指令がきたら
	m_RightIn.read();
	if(m_Right.data==1){ //1(press)のとき回転する
		command.va -= m_RotSpeed;
		output = true;
	}else if(m_Right.data==2){ //2(release)のとき停止する
		command.va = 0;
		output = true;
	}
	std::cout<<"right"<<std::endl;
  }
  if(m_LeftIn.isNew()){  //左回転指令がきたら
	m_LeftIn.read();
	if(m_Left.data==1){ //1(press)のとき回転する
		command.va += m_RotSpeed;
		output = true;
	}else if(m_Left.data==2){ //2(release)のとき停止する
		command.va = 0;
		output = true;
	}
	std::cout<<"left"<<std::endl;
  }

  if(output){
    m_Velocity.data=command;
	m_VelocityOut.write();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiControllerByHMSwitches::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void KobukiControllerByHMSwitchesInit(RTC::Manager* manager)
  {
    coil::Properties profile(kobukicontrollerbyhmswitches_spec);
    manager->registerFactory(profile,
                             RTC::Create<KobukiControllerByHMSwitches>,
                             RTC::Delete<KobukiControllerByHMSwitches>);
  }
  
};


