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
		std::string hmstFilename; //hmstファイルの名前
		std::vector<std::string> configFilenameList; //hmstファイルの先頭に記述されているxmlファイルの名前一覧
		std::vector<std::vector<unsigned short>> useIDFlagList; //使用されているIDは1,そうでなければ0
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



