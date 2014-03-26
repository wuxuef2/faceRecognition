#include "Helper.h"
#include "FileHelper.h"

int main()
{
	Helper myHelper;

	//myHelper.preprocess();
	string path = "C:\\opencv";
	
	FileHelper myFileHelper;
	myFileHelper.getOpenCvLibFilesName(path);

	return 0;
}





