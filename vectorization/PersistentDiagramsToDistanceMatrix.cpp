//============================================================================
// Name        : PersistentDiagramsToDistanceMatrix.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string.h>
#include <io.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

/*
 *get all file names with path from {path}. and save them to {files}. exd is the file extension (.txt, .swc)
 */
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

vector<string> readFiles(string filePath){
	vector<string> files;
	getFiles(filePath, "swc", files);
	return files;
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

vector<vector<vector<double> > > readDiagramsFixIndex(vector<string> files){
	vector<vector<vector<double> > > diagrams;
	for(size_t i = 0; i < files.size(); i++){
		string f = files[i];
		std::ifstream ifs;
		ifs.open(f.c_str());
		string line;
		double x, y;
		vector<vector<double> > diagram;
		vector<double> X; // x coordinate
		vector<double> Y; // y coordinate
		while(getline(ifs, line)){
			istringstream iss(line);
			if(iss >> x >> y){
				X.push_back(abs(x));
				Y.push_back(abs(y));
			}
		}
		diagram.push_back(X);
		diagram.push_back(Y);
		diagrams.push_back(diagram);

		ifs.close();
		ifs.clear();
	}
	return diagrams;
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

double maxRange(vector<vector<vector<double> > > diagrams){
	double range = 0;
	for(size_t i = 0; i < diagrams.size(); i++){
		vector<vector<double> > diagram = diagrams[i];
		for(size_t j = 0; j < diagram[0].size(); j++){
			range = max(range, diagram[0][j]);
		}
	}
	return range;
}

vector<vector<double> > vectorFromPersistentDiagramAbs(vector<vector<double> > persistentDiagram, int vectLen, double sigma, double range){
	vector<vector<double> > Y;
	vector<double> x;
	vector<double> y;
	double upBound = range;
	double lowBound = 0;
	double interval = (upBound - lowBound) / (vectLen-1);
	sigma = 2*sigma*sigma;
	for(double pos = lowBound; pos <= upBound; pos+=interval){
		double funValue = 0;
		for(int i = 0; i < persistentDiagram[0].size(); i++){
			double alpha = fabs(persistentDiagram[1][i] - persistentDiagram[0][i]);
			double Mu = persistentDiagram[0][i];
			double dist = Mu-pos;
			funValue += alpha*exp(-(dist*dist) / sigma);
		}
		x.push_back(pos);
		y.push_back(funValue);
	}
	Y.push_back(x);
	Y.push_back(y);
	return Y;
}

vector<vector<double> > vectorFromPersistentDiagram(vector<vector<double> > persistentDiagram, int vectLen, double sigma, double range){
	vector<vector<double> > Y;
	vector<double> x;
	vector<double> y;
	double upBound = range;
	double lowBound = 0;
	double interval = (upBound - lowBound) / (vectLen-1);
	sigma = 2*sigma*sigma;
	for(double pos = lowBound; pos <= upBound; pos+=interval){
		double funValue = 0;
		for(int i = 0; i < persistentDiagram[0].size(); i++){
			double alpha = persistentDiagram[1][i] - persistentDiagram[0][i];
			double Mu = persistentDiagram[0][i];
			double dist = Mu-pos;
			funValue += alpha*exp(-(dist*dist) / sigma);
		}
		x.push_back(pos);
		y.push_back(funValue);
	}
	Y.push_back(x);
	Y.push_back(y);
	return Y;
}

vector<vector<double> > diagramsToVectorsAbs(vector<vector<vector<double> > > diagrams, int vectLen, double sigma, double range){
	vector<vector<double> > vectors;
	for(size_t i = 0; i < diagrams.size(); i++){
		vector<vector<double> > diagram = diagrams[i];
		vectors.push_back(vectorFromPersistentDiagramAbs(diagram, vectLen, sigma, range)[1]);
	}
	return vectors;
}

vector<vector<double> > diagramsToVectors(vector<vector<vector<double> > > diagrams, int vectLen, double sigma, double range){
	vector<vector<double> > vectors;
	for(size_t i = 0; i < diagrams.size(); i++){
		vector<vector<double> > diagram = diagrams[i];
		vectors.push_back(vectorFromPersistentDiagram(diagram, vectLen, sigma, range)[1]);
	}
	return vectors;
}

double getDistanceL1(vector<double> v1, vector<double> v2){
	double dist = 0;
	for(int i = 0; i < v1.size(); i++){
		dist += abs(v1[i] - v2[i]);
	}
	return dist;
}

vector<vector<double> > computeDistanceMatrixFromVectors(vector<vector<double> > vectors){
	vector<vector<double> > distanceMatrix;
	int size = vectors.size();
	distanceMatrix.resize(size);
	for(size_t i = 0; i < size; i++){
		distanceMatrix[i].resize(size);
	}
	ofstream to;
	to.open("output/vector.txt");
	for(int i = 0; i<size; i++) {
		for(auto d : vectors[i]) {
			to << d << " ";
		}
		to << endl;
	}
	to.close();

	for(size_t i = 0; i < size; i++){
		for(size_t j = 0; j < i; j++){
			vector<double> v1 = vectors[i];
			vector<double> v2 = vectors[j];
			distanceMatrix[i][j] = getDistanceL1(v1, v2);
			distanceMatrix[j][i] = distanceMatrix[i][j];
		}
	}
	return distanceMatrix;
}

vector<vector<double> > computeDistanceMatrixFromVectorsCombineTwo(vector<vector<double> > vectors1, vector<vector<double> > vectors2){
	vector<vector<double> > distanceMatrix;
	int size = vectors1.size();
	distanceMatrix.resize(size);
	for(size_t i = 0; i < size; i++){
		distanceMatrix[i].resize(size);
	}
	for(size_t i = 0; i < size; i++){
		for(size_t j = 0; j < i; j++){
			distanceMatrix[i][j] = getDistanceL1(vectors1[i], vectors1[j]) + getDistanceL1(vectors2[i], vectors2[j]);
			distanceMatrix[j][i] = distanceMatrix[i][j];
		}
	}
	return distanceMatrix;
}

void computeDistanceMatrixFromDiagrams(string diagramsFolder, string distanceMatrix, bool twoVectors){
	double sigma = 50;
	int vectorLen = 100;
	vector<vector<vector<double> > > diagrams = readDiagramsFixIndex(getFixIndexFiles(readFiles(diagramsFolder)));
	cout << "maximum range: " << maxRange(diagrams) << endl;
	if(twoVectors){
		vector<vector<double> > vectors_1 = diagramsToVectorsAbs(diagrams, vectorLen, sigma, maxRange(diagrams));
		vector<vector<double> > vectors_2 = diagramsToVectors(diagrams, vectorLen, sigma, maxRange(diagrams));
		vector<vector<double> > matrix = computeDistanceMatrixFromVectorsCombineTwo(vectors_1, vectors_2);
		printMat(distanceMatrix, matrix);
	}else{
		vector<vector<double> > vectors = diagramsToVectorsAbs(diagrams, vectorLen, sigma, maxRange(diagrams));
		vector<vector<double> > matrix = computeDistanceMatrixFromVectors(vectors);
		printMat(distanceMatrix, matrix);
	}

}

int main(int argc, char **argv){
	string diagramsFolder = "input/data4_dendrites/";
	string distanceMatrix = "output/data4_dendrites/distances_5.txt";
	bool twoVectors = false;
	computeDistanceMatrixFromDiagrams(diagramsFolder, distanceMatrix, twoVectors);
}
