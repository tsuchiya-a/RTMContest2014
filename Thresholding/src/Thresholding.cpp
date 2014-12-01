// -*- C++ -*-
/*!
 * @file  Thresholding.cpp
 * @brief Send Command by Thresholding the Configuration
 * @date $Date$
 *
 * @author 土屋彩茜
 * ma14076@shibaura-it.ac.jp
 *
 * $Id$
 */

#include "Thresholding.h"

// Module specification
// <rtc-template block="module_spec">
static const char* thresholding_spec[] =
  {
    "implementation_id", "Thresholding",
    "type_name",         "Thresholding",
    "description",       "Send Command by Thresholding the Configuration",
    "version",           "1.0.0",
    "vendor",            "Ayaka Tsuchiya",
    "category",          "tool",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.Threshold", "50.0",
    "conf.default.Threshold_Mode", "0",
    "conf.default.OutDataValue", "0,0,1",
    // Widget
    "conf.__widget__.Threshold", "text",
    "conf.__widget__.Threshold_Mode", "radio",
    "conf.__widget__.OutDataValue", "text",
    // Constraints
    "conf.__constraints__.Threshold_Mode", "(0,1,2)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
Thresholding::Thresholding(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_DoubleInDataIn("DoubleInData", m_DoubleInData),
    m_ShortInDataIn("ShortInData", m_ShortInData),
    m_DoubleThresholdIn("DoubleThreshold", m_DoubleThreshold),
    m_ShortThresholdIn("ShortThreshold", m_ShortThreshold),
    m_OutDataOut("OutData", m_OutData)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Thresholding::~Thresholding()
{
}



RTC::ReturnCode_t Thresholding::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("DoubleInData", m_DoubleInDataIn);
  addInPort("ShortInData", m_ShortInDataIn);
  addInPort("DoubleThreshold", m_DoubleThresholdIn);
  addInPort("ShortThreshold", m_ShortThresholdIn);
  
  // Set OutPort buffer
  addOutPort("OutData", m_OutDataOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("Threshold", m_Threshold, "50.0");
  bindParameter("Threshold_Mode", m_Threshold_Mode, "0");
  bindParameter("OutDataValue", m_OutDataValue, "0,0,1");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Thresholding::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Thresholding::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Thresholding::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * ポートを初期化する。
 */

RTC::ReturnCode_t Thresholding::onActivated(RTC::UniqueId ec_id)
{
  while(m_DoubleInDataIn.isNew()) m_DoubleInDataIn.read();
  while(m_ShortInDataIn.isNew()) m_ShortInDataIn.read();
  while(m_DoubleThresholdIn.isNew()) m_DoubleThresholdIn.read();
  while(m_ShortThresholdIn.isNew()) m_ShortThresholdIn.read();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Thresholding::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*!
 * 入力された値と閾値を比較して、二値化した値を出力する。
 */

RTC::ReturnCode_t Thresholding::onExecute(RTC::UniqueId ec_id)
{
  double threshold;
  int res;

  // 閾値の更新
  if(m_Threshold_Mode==0){ //0ならばConfiguration：Thresholdが閾値
	threshold=m_Threshold;
  }
  else if(m_Threshold_Mode==1){ //1ならばInPort：DoubleInDataが閾値
	m_DoubleThresholdIn.read();
	threshold=m_DoubleThreshold.data;
  }
  else if(m_Threshold_Mode==2){ //2ならばInPort：ShortInDataが閾値
	m_ShortThresholdIn.read();
	threshold=m_ShortThreshold.data;
  }

  if(m_DoubleInDataIn.isNew()){
	m_DoubleInDataIn.read();
	if(m_DoubleInData.data<threshold){ //閾値より小さい場合
		std::cout<<"InPut Data:"<<m_DoubleInData.data<<"< threshold:"<<threshold<<std::endl;
		m_OutData.data=m_OutDataValue[0];
	}
	else if(m_DoubleInData.data==threshold){ //閾値と等しい場合
		std::cout<<"InPut Data:"<<m_DoubleInData.data<<"= threshold:"<<threshold<<std::endl;
		m_OutData.data=m_OutDataValue[1];
	}
	else if(m_DoubleInData.data>threshold){ //閾値より大きい場合
		std::cout<<"InPut Data:"<<m_DoubleInData.data<<"> threshold:"<<threshold<<std::endl;
		m_OutData.data=m_OutDataValue[2];
	}
	std::cout<<"OutData :"<<m_OutData.data<<std::endl<<std::endl;
	m_OutDataOut.write();
  }
  if(m_ShortInDataIn.isNew()){
	m_ShortInDataIn.read();
	if(m_ShortInData.data<threshold){ //閾値より小さい場合
		std::cout<<"InPut Data:"<<m_ShortInData.data<<"< threshold:"<<threshold<<std::endl;
		m_OutData.data=m_OutDataValue[0];
	}
	else if(m_ShortInData.data==threshold){ //閾値と等しい場合
		std::cout<<"InPut Data:"<<m_ShortInData.data<<"= threshold:"<<threshold<<std::endl;
		m_OutData.data=m_OutDataValue[1];
	}
	else if(m_ShortInData.data>threshold){ //閾値より大きい場合
		std::cout<<"InPut Data:"<<m_ShortInData.data<<"> threshold:"<<threshold<<std::endl;
		m_OutData.data=m_OutDataValue[0];
	}
		std::cout<<"OutData :"<<m_OutData.data<<std::endl<<std::endl;;
		m_OutDataOut.write();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Thresholding::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Thresholding::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Thresholding::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Thresholding::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Thresholding::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void ThresholdingInit(RTC::Manager* manager)
  {
    coil::Properties profile(thresholding_spec);
    manager->registerFactory(profile,
                             RTC::Create<Thresholding>,
                             RTC::Delete<Thresholding>);
  }
  
};


