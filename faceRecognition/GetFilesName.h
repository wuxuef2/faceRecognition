#ifndef _GET_FILES_NAME_
#define _GET_FILES_NAME_

#include "Config.h"

class GetFilesName {
public:
	GetFilesName(){};
	~GetFilesName(){};

	void getOpenCvLibFilesName(string openCvPath);

private:
	void findFIels(vector<string>&, string);
};

#endif