/***********
//Code by Yanjie Li
//Oct. 2016
***********/
#include "Graph.h"
#include "FileFun.h"

using namespace std;

class myComp3D
{
	bool reverse;
	Point3D p;
public:
	myComp3D(Point3D pp, const bool& revparam)
	{
		p = pp;
		reverse = revparam;
	}
	bool operator() (const std::pair<int, Point3D> & lhs, const std::pair<int, Point3D> &rhs) const
	{
		if (reverse) return (computePointDistance(p, lhs.second) > computePointDistance(p, rhs.second));
		else return (computePointDistance(p, lhs.second) < computePointDistance(p, rhs.second));
	}
};

//typedef std::priority_queue<std::pair<int, Point3D>, std::vector< std::pair<int, Point3D> >, myComp3D> mypq_type;


string extractFileName(string fileName){
	int slashIndex = fileName.find_last_of('\\');
	return fileName.substr(slashIndex+1);
}

void printPersistentDiagramsWithDiameter(vector<string> files, string outputFolder){
	unsigned int fileNum = files.size();
	std::string fileName;
	double delta;
	vector<Point3D> vP;
	for(size_t i=0;i<fileNum;i++){
		fileName = files[i];
		cout<<"process "<< i<<": "<< fileName << endl;
		delta = 0.1;
		std::ifstream ifs;
		double x, y, z;
		int i1, i2;
		double d;
		vP.clear();
		ifs.open(fileName.c_str());
		int countP, countF;
		ifs >> countP >> countF;
		for(int j = 0; j < countP; j++){
			ifs >> x >> y >> z;
			Point3D p;
			p.x = x;
			p.y = y;
			p.z = z;
			vP.push_back(p);
		}
		Graph ga(vP.size(), delta, countF);
		ga.setVP(vP);
		for(int j = 0; j < countF; j++){
			ifs >> i1 >> i2;
			ga.addEdge(i1, i2);
		}
		for(int j = 0; j < countP; j++){
			ifs >> d;
			ga.setFuncVal(j, d);
		}
		ifs.close();
		ifs.clear();
		vP.clear();
		ga.outputPersistenceDiagramsOnlyData(outputFolder+extractFileName(fileName));
	}
}

void getPersistentDiagrams(string filePath, string outputFolder){
	vector<string> files;
	getFiles(filePath, "swc", files);
	vector<string> filesFixIndex = getFixIndexFiles(files);
	printPersistentDiagramsWithDiameter(filesFixIndex, outputFolder);
}

int main(int argc, char **argv){
	if(argc != 3){
		cout << "usage: PDmain <input folder name> <output folder name>" << endl;
		return 0;
	}
	string euclideanWithFuncVal(argv[1]);
	string persistenceDiagramFolder(argv[2]);
	if(persistenceDiagramFolder.find_last_of('/') != persistenceDiagramFolder.size() - 1){
		persistenceDiagramFolder += '/';
	}
	getPersistentDiagrams(euclideanWithFuncVal, persistenceDiagramFolder);
}
