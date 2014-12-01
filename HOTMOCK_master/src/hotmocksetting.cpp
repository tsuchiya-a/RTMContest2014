#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#import "msxml6.dll" raw_interfaces_only

#include "hotmocksetting.h"

namespace hotmock{

	HotmockSetting::HotmockSetting(){
	}

	HotmockSetting::~HotmockSetting(){
	}

	int HotmockSetting::initialize(std::string hmst){
		hmstFilename=hmst;
		std::ifstream infile(hmstFilename,std::ios::in); //hmst�t�@�C�����J��

		if(!infile){
			std::cerr << "hmst file can not open : " << hmstFilename << std::endl;
			return -1;
		}

		do{
			std::string str;
			std::getline(infile,str); //�J�����t�@�C�������s���ǂݍ���

			std::string bom(str,0,3); //BOM��ۑ�
			str.erase(0,3); //BOM���폜

			while(str.find(".xml")!=std::string::npos){ //".xml"�Ɉ�v���镶���񂪂���Ȃ�Όp��
        		configFilenameList.push_back(str);      //xml�t�@�C���̖��O�ꗗ(FileList)���쐬
				std::getline(infile,str);
			}

			int list_num=configFilenameList.size();

			for(int i=0;i<list_num;i++){
				if(str.find("<?xml")!=std::string::npos){ //XML�錾�������FileList�ɂ��閼�O�̏��Ƀt�@�C�����쐬����
					std::ofstream outfile(configFilenameList[i],std::ios::out);

					if (!outfile) {
						std::cerr << "out file can not open" << std::endl;
						return -1;
					}

					outfile << bom; //�����R�[�h��UFT-8�ɂ���
					outfile << str; //XML�錾���t�@�C���ɏ�������
					outfile << std::endl;
					std::getline(infile,str); //���̍s��ǂݍ���

					while(str.find("<?xml")==std::string::npos&&str.length()!=0){ //����XML�錾�̎�O�܂Ńt�@�C������������
						outfile << str;
						outfile << std::endl;
						std::getline(infile,str);
					}		
					outfile.close();
				}
			}

		}while(!infile.eof());
		infile.close();

		return 0;
	}

	int HotmockSetting::setPort(){
		CoInitialize(NULL); //COM���C�u�����̏�����

		for(int i=0;i<configFilenameList.size();i++){ //configFilenameList�ɂ���t�@�C���̏��ɐݒ�
			HRESULT hr = Doc.CreateInstance(CLSID_DOMDocument);
			if(FAILED(hr)){
				 std::cerr << "can not createInstance" << std::endl;
				 return -1;
			}

			Doc->put_async(VARIANT_FALSE); //���[�h������҂�

			VARIANT_BOOL varResult; 
			hr = Doc->load(_variant_t(configFilenameList[i].c_str()), &varResult); //���[�h
			if((hr!=S_OK)||!varResult){
				std::cerr << "config file can not load" << std::endl;
				return -1;
			}

			MSXML2::IXMLDOMNodePtr node_deviceName=NULL;
			MSXML2::IXMLDOMNodePtr node_ID=NULL;
			MSXML2::IXMLDOMNodePtr node=NULL;
			MSXML2::IXMLDOMNodeListPtr nodeList_deviceName=NULL;
			MSXML2::IXMLDOMNodeListPtr nodeList_ID=NULL;
			_variant_t var1,var2,var3;
			long node_num = 0;
			std::string str;
			std::vector<unsigned short> useIDflag;
			int BDflag=0;

			Doc->getElementsByTagName(L"device_name",&nodeList_deviceName);
			nodeList_deviceName->get_length( &node_num );
			Doc->getElementsByTagName(L"id",&nodeList_ID);

			if(configFilenameList[i]=="BD_Config.xml") BDflag=1;

			for(int i=0;i<node_num;i++){
				nodeList_deviceName->get_item( i, &node_deviceName);
				node_deviceName->get_text(&var1.bstrVal);
				nodeList_ID->get_item( i, &node_ID);
				node_ID->get_text(&var2.bstrVal);

				if(BDflag==1){ //�{�[�h�^�̎w��
					if(_bstr_t(var1.bstrVal)==_bstr_t("DigitalBoard")){
						boardType="digital";
					}
					else if(_bstr_t(var1.bstrVal)==_bstr_t("AnalogBoard")){
						boardType="analog";
					}
					else{ //error
						std::cerr<<"??? board type ???"<<std::endl;
						return -1;
					}
				}

				if(_bstr_t(var1.bstrVal)!=_bstr_t("")){
					useIDflag.push_back(1);
				}
				else{
					useIDflag.push_back(0);
				}
			}

			useIDFlagList.push_back(useIDflag);

		}

		Doc=NULL;
		CoFreeUnusedLibraries(); //COM���C�u�����̉��
		CoUninitialize();
		return 0;
	}

	void HotmockSetting::finalize(){
		configFilenameList.clear();
		useIDFlagList.clear();
		return;
	}
};
