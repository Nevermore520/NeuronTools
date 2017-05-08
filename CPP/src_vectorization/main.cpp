
#include "Graph.h"
#include "FileFun.h"
#include "mathFun.h"

using namespace std;

double getMaxPersistentRange(vector<string> files){
	double maxRange = 0;
	std::string fileName;
	double delta;
	vector<Point3D> vP;
	unsigned int fileNum = files.size();
	for(size_t i=0;i<fileNum;i++){
		fileName = files[i];
		delta = 0.1;
		std::ifstream ifs;
		double x, y, z;
		int i1, i2;
		double d;
		vP.clear();
		ifs.open(fileName.c_str());
		int countP, countF;
		ifs >> countP >> countF;
		//reading first graph
		while (countP--)
		{
			ifs >> x >> y >> z;
			Point3D p;
			p.x = x;
			p.y = y;
			p.z = z;
			vP.push_back(p);
		}
		Graph ga(vP.size(), delta, countF);
		ga.setVP(vP);
		while (countF--)
		{
			ifs >> i1 >> i2 >> d;
			ga.addEdge(i1, i2, d);
		}
		ifs.close();
		ifs.clear();
		vP.clear();
		maxRange = max(ga.getPersistentRange(), maxRange);
	}
	return maxRange;
}

vector<vector<double> > computeVectorizedPersistDiagDistanceMatrix(vector<string> files, int vectLength, double range, double Sigma){
	vector<vector<double> > distanceMat;
	unsigned int fileNum = files.size();
	distanceMat.resize(fileNum);
	for (size_t i = 0; i < fileNum; ++i){
		distanceMat[i].resize(fileNum);
	}

	double maxRange = 0;
	if(range >= 0){ // use user input range
		maxRange = range;
	}else{ // user max range
		maxRange = getMaxPersistentRange(files);
	}
	cout<<"max range is: "<<maxRange<<endl;
	vector<vector<double > > vectors;
	int vectLen = vectLength;
	std::string fileName;
	double delta;
	vector<Point3D> vP;
	double sigma = Sigma;
	for(size_t i=0;i<fileNum;i++){
		fileName = files[i];
		delta = 0.1;
		std::ifstream ifs;
		double x, y, z;
		int i1, i2;
		double d;
		vP.clear();
		ifs.open(fileName.c_str());
		int countP, countF;
		ifs >> countP >> countF;
		//reading first graph
		while (countP--)
		{
			ifs >> x >> y >> z;
			Point3D p;
			p.x = x;
			p.y = y;
			p.z = z;
			vP.push_back(p);
		}
		Graph ga(vP.size(), delta, countF);
		ga.setVP(vP);
		while (countF--)
		{
			ifs >> i1 >> i2 >> d;
			ga.addEdge(i1, i2, d);
		}
		ifs.close();
		ifs.clear();
		vP.clear();
		vectors.push_back(ga.getPersisDiagFunGaussianVector(vectLen, sigma, maxRange));
	}

	for(size_t i=0;i<fileNum;i++){
		for(size_t j=i;j<fileNum;j++){
			vector<double > v1 = vectors[i];
			vector<double > v2 = vectors[j];
			double result = getDistanceL1(v1, v2);
			distanceMat[i][j] = result;
			distanceMat[j][i] = result;
		}
	}
	for(size_t i=0;i<vectors.size();i++){
		vectors[i].clear();
	}
	vectors.clear();
	return distanceMat;
}

void getVectorizedPersistDiagDistanceMatrixFixIndex(string filePath, string outputFile, int vectLength, double range, double Sigma){
	//file name start with its index eg. 12_name.swc, this index start from 1
	vector<string> files;
	getFiles(filePath, "swc", files);
	vector<string> filesFixIndex = getFixIndexFiles(files);
	vector<vector<double> > distanceMat = computeVectorizedPersistDiagDistanceMatrix(filesFixIndex, vectLength, range, Sigma);
	printMat(outputFile,distanceMat);
}

int main(int argc, char **argv){ // testing
	if(argc < 4 || argc > 5){
		cout << "usage: PDmain <input folder name> <output file name> <vector size> [<persistent range>]" << endl;
		return 0;
	}
	string inputFolder(argv[1]);
	string outputFile(argv[2]);
	int vectorSize = atoi(argv[3]);
	double range = -1;
	if(argc == 5){
		range = atof(argv[4]);
	}
	double Sigma = 50;
	getVectorizedPersistDiagDistanceMatrixFixIndex(inputFolder, outputFile, vectorSize, range, Sigma);
}
