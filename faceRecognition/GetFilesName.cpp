#include "GetFilesName.h"

void GetFilesName::findFIels(vector<string>& sFileNames,string sPath) {

    WIN32_FIND_DATAA wfd;
    sPath = sPath + "\\*.*";//����ָ��Ŀ¼�µ����и�ʽ���ļ���"*.xml":��ʾ����ָ��Ŀ¼��xml�ļ�

    HANDLE hFile = FindFirstFile(sPath.c_str(),&wfd);

    if(INVALID_HANDLE_VALUE == hFile) {
		return;
    }

	do {
		sFileNames.push_back(wfd.cFileName);
    } while(FindNextFile(hFile, &wfd));

}

void GetFilesName::getOpenCvLibFilesName(string openCvPath) {
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