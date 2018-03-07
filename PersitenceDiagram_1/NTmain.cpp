
#include "Graph.h"
#include "FileFun.h"

using namespace std;

double computeEuclideanDiameter(string fileName){
	std::ifstream ifs;
	ifs.open(fileName.c_str());
	vector<Point3D> vp;
	double x, y, z;
	int countP, countF;
	ifs >> countP >> countF;
	while (countP--)
	{
		ifs >> x >> y >> z;
		Point3D p;
		p.x = x;
		p.y = y;
		p.z = z;
		vp.push_back(p);
	}
	ifs.close();
	ifs.clear();
	double diameter = 0;
	for(size_t i=0;i<vp.size();i++){
		for(size_t j=i+1;j<vp.size();j++){
			double tmpDistance = computePointDistance(vp[i],vp[j]);
			if(diameter<tmpDistance){
				diameter = tmpDistance;
			}
		}
	}
	return diameter;
}

void printDiagram(vector<pair<double, double> > diagram, string file){
	std::ofstream ofs;
	ofs.open(file.c_str());
	for(size_t i=0;i<diagram.size();i++){
		ofs<<diagram[i].first<<" "<<diagram[i].second<<"\n";
	}
	ofs.close();
	ofs.clear();
}

// Compute Persistent Diagrams and write the results to outputFolder
void printPersistentDiagrams(vector<string> files, string outputFolder){
	unsigned int fileNum = files.size();
	std::string fileName, fileResult = "data/dummyFile.txt";
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
		// maxRange = max(ga.getPersistentRange(), maxRange);
		vector<pair<double, double> > diagram =ga.getPersistentDiagram();

		//add diameter to persistent diagram
		//double diameter = computeEuclideanDiameter(fileName);
		//diagram.push_back(::make_pair(0, diameter));


		printDiagram(diagram, outputFolder+extractFileName(fileName));
	}
}

// Read all files from filePath, compute their Persistent Diagrams, and write the results to outputFolder.
void getPersistentDiagrams(string filePath, string outputFolder){
	vector<string> files;
	getFiles(filePath, "swc", files);
	vector<string> filesFixIndex = getFixIndexFiles(files);
	printPersistentDiagrams(filesFixIndex, outputFolder);
}

int main(int argc, char **argv){ // testing
//	if(argc != 3){
//		cout << "usage: PDmain <input folder name> <output folder name>" << endl;
//		return 0;
//	}
//	string inputFolder(argv[1]);
//	string outputFolder(argv[2]);

	string inputFolder("input\\1_data_1268\\");
	string outputFolder("output\\1_data_1268\\");
	cout << "??" << endl;
	if(outputFolder.find_last_of('/') != outputFolder.size() - 1){
		outputFolder += '/';
	}
	getPersistentDiagrams(inputFolder, outputFolder);
}
