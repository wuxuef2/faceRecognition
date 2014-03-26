#ifndef _FILE_HELPER_
#define _FILE_HELPER_

#include <windows.h>
#include <stdio.h>
#include "Config.h"

class FileHelper {
public:
	FileHelper(){};
	~FileHelper(){};

	void getImgFilesPath(string path, vector<string>& imgFilesHolder, string suffix);
	void getOpenCvLibFilesName(string openCvPath);
	bool isImgFile(string uri);
private:
	void findFIels(vector<string>&, string);
};

#endif