#include "FileHelper.h"

void FileHelper::getImgFilesPath(string path, vector<string>& imgFilesHolder, string suffix) {

	WIN32_FIND_DATAA wfd;
    path = path + "\\*.*";

    HANDLE hFile = FindFirstFile(path.c_str(), &wfd);

    if(INVALID_HANDLE_VALUE == hFile) {
		return;
    }

	do {
		string fileName = wfd.cFileName;
		if ((suffix != "" && fileName.find(suffix) < fileName.length()) ||
			suffix == "" && isImgFile(fileName)) {
			imgFilesHolder.push_back(fileName);
		}
    } while(FindNextFile(hFile, &wfd));
}


bool FileHelper::isImgFile(string uri) {
	string imgSuffix[] = {"JPG", "jpg", "JPEG", "jpeg", "gif", "GIF",
		"png", "PNG", "TIFF", "tiff", "bmp", "BMP", "ico", "ICO"}; 

	for (int i = 0; i < 14; i++) {
		if (uri.find(imgSuffix[i]) < uri.length()) 
			return true;
	}
	return false;
}

void FileHelper::findFIels(vector<string>& sFileNames,string sPath) {

    WIN32_FIND_DATAA wfd;
    sPath = sPath + "\\*.*";//查找指定目录下的所有格式的文件。"*.xml":表示查找指定目录册xml文件

    HANDLE hFile = FindFirstFile(sPath.c_str(),&wfd);

    if(INVALID_HANDLE_VALUE == hFile) {
		return;
    }

	do {
		if (strcmp(wfd.cFileName, "."))
		sFileNames.push_back(wfd.cFileName);
    } while(FindNextFile(hFile, &wfd));

}

void FileHelper::getOpenCvLibFilesName(string openCvPath) {
	vector<string> openCvFilesNameHolder;
	ofstream outfile;

	string outFilePath = "D:\\output.txt";
	string debugFileName = "";
	string releaseFileName = "";

	outfile.open(outFilePath);

	if (openCvPath == "") {
		openCvPath = "C:\\opencv";
	}

	openCvPath += "\\build\\x86\\vc11\\lib";
	findFIels(openCvFilesNameHolder, openCvPath);

	for (int i = 0; i < openCvFilesNameHolder.size(); i++) {
		if (openCvFilesNameHolder[i].find(".lib") < openCvFilesNameHolder[i].length()) {
			if (openCvFilesNameHolder[i].find("d.lib") < openCvFilesNameHolder[i].length()) {
				debugFileName += openCvFilesNameHolder[i] + '\n';
			}
			else {
				releaseFileName += openCvFilesNameHolder[i] + '\n';
			}
		}
	}

	outfile << debugFileName << "\n\n" << releaseFileName << endl;
}
