/*
 * FileFun.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: Yanjie
 */
#include "FileFun.h"

// get all file names with path from {path}. and save them to {files}. exd is the file extension (.txt, .swc)
void getFiles( string path, string exd, vector<string>& files )
{

	long   hFile   =   0;

	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(),&fileinfo)) !=  -1)
	{
		do
		{

			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( pathName.assign(path).append("\\").append(fileinfo.name), exd, files );
			}
			else
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

string extractFileName(string fileName){
	int slashIndex = fileName.find_last_of('\\');
	return fileName.substr(slashIndex+1);
}

void printMat(string outputFile, vector<vector<double> > &distanceMat){
	size_t rows = distanceMat.size();
	if(rows==0){
		return;
	}
	size_t columns = distanceMat[0].size();
	std::ofstream ofs;
	ofs.open(outputFile.c_str());
	ofs<<rows<<" "<<columns<<"\n";
	for(size_t i=0;i<rows;i++){
		for(size_t j=0;j<columns;j++){
			ofs<<distanceMat[i][j]<<" ";
		}
		ofs<<"\n";
	}
	ofs.close();
	ofs.clear();
}

void appendMatToFile(string outputFile, vector<vector<double> > &distanceMat){
	int rows = distanceMat.size();
	int columns = distanceMat[0].size();
	std::ofstream ofs;
	ofs.open(outputFile.c_str(),ios::app);
	for(unsigned int i=0;i<rows;i++){
		for(unsigned int j = 0;j<columns;j++){
			ofs<<distanceMat[i][j]<<" ";
		}
		ofs<<"\n";
	}
}

int getIndex(string fileName){
	int slashIndex = fileName.find_last_of('\\');
	string name = fileName.substr(slashIndex+1);
	int underscoreIndex = name.find_first_of('_');
	int index = atoi(name.substr(0,underscoreIndex).c_str());
	return index;
}

vector<string> getFixIndexFiles(vector<string> files){
	vector<string> filesFixIndex;
	filesFixIndex.resize(files.size());
	for(unsigned int i=0;i<files.size();i++){
		string next = files[i];
		int index = getIndex(next);
		filesFixIndex[index-1] = next;
	}
	for(size_t i=0;i<files.size();i++){
		cout<<filesFixIndex[i]<<endl;
	}
	return filesFixIndex;
}

void readMat(string fileName,vector<vector<double> > &distanceMat){
	std::ifstream ifs;
	ifs.open(fileName.c_str());
	unsigned int size;
	ifs>>size;
	ifs>>size;
	distanceMat.resize(size);
	for (size_t i = 0; i < size; ++i){
		distanceMat[i].resize(size);
	}
	double next;
	for(size_t i = 0;i<size;i++){
		for(size_t j = 0;j<size;j++){
			ifs>>next;
			distanceMat[i][j] = next;
		}
	}
	ifs.close();
	ifs.clear();
}



