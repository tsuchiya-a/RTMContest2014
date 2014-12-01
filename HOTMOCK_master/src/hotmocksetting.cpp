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
		std::ifstream infile(hmstFilename,std::ios::in); //hmstファイルを開く

		if(!infile){
			std::cerr << "hmst file can not open : " << hmstFilename << std::endl;
			return -1;
		}

		do{
			std::string str;
			std::getline(infile,str); //開いたファイルから一行ずつ読み込む

			std::string bom(str,0,3); //BOMを保存
			str.erase(0,3); //BOMを削除

			while(str.find(".xml")!=std::string::npos){ //".xml"に一致する文字列があるならば継続
        		configFilenameList.push_back(str);      //xmlファイルの名前一覧(FileList)を作成
				std::getline(infile,str);
			}

			int list_num=configFilenameList.size();

			for(int i=0;i<list_num;i++){
				if(str.find("<?xml")!=std::string::npos){ //XML宣言があればFileListにある名前の順にファイルを作成する
					std::ofstream outfile(configFilenameList[i],std::ios::out);

					if (!outfile) {
						std::cerr << "out file can not open" << std::endl;
						return -1;
					}

					outfile << bom; //文字コードをUFT-8にする
					outfile << str; //XML宣言をファイルに書き込む
					outfile << std::endl;
					std::getline(infile,str); //次の行を読み込む

					while(str.find("<?xml")==std::string::npos&&str.length()!=0){ //次のXML宣言の手前までファイルを書き込む
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
		CoInitialize(NULL); //COMライブラリの初期化

		for(int i=0;i<configFilenameList.size();i++){ //configFilenameListにあるファイルの順に設定
			HRESULT hr = Doc.CreateInstance(CLSID_DOMDocument);
			if(FAILED(hr)){
				 std::cerr << "can not createInstance" << std::endl;
				 return -1;
			}

			Doc->put_async(VARIANT_FALSE); //ロード完了を待つ

			VARIANT_BOOL varResult; 
			hr = Doc->load(_variant_t(configFilenameList[i].c_str()), &varResult); //ロード
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

				if(BDflag==1){ //ボード型の指定
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
		CoFreeUnusedLibraries(); //COMライブラリの解放
		CoUninitialize();
		return 0;
	}

	void HotmockSetting::finalize(){
		configFilenameList.clear();
		useIDFlagList.clear();
		return;
	}
};
