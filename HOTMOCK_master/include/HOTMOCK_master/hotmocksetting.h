#ifndef WIN32
#define WIN32
#endif

#ifndef HOTMOCKSETTING_H
#define HOTMOCKSETTING_H

#include <string>
#include <vector>

#import "msxml6.dll" raw_interfaces_only

namespace hotmock{

	class HotmockSetting{

	public:
		std::string hmstFilename; //hmst�t�@�C���̖��O
		std::vector<std::string> configFilenameList; //hmst�t�@�C���̐擪�ɋL�q����Ă���xml�t�@�C���̖��O�ꗗ
		std::vector<std::vector<unsigned short>> useIDFlagList; //�g�p����Ă���ID��1,�����łȂ����0
		MSXML2::IXMLDOMDocumentPtr Doc;
		std::string boardType;

	 public:
		HotmockSetting();
		~HotmockSetting();
		int initialize(std::string hmst);
		void finalize();
		int setPort();
	};

};

#endif



