#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <random>
#include <dirent.h>
#include <string.h>

#include "bottleneck.h"

using namespace std;

// any container of pairs of doubles can be used,
// we use vector in this example.

typedef std::vector<std::pair<double, double>>  PairVector;

/********Helper functions***************************/
void listFiles( const char* path, const char* exd, std::vector<std::string>& files)
{
   DIR* dirFile = opendir( path );
   if ( dirFile ) 
   {
      struct dirent* hFile;
      //errno = 0;
      while (( hFile = readdir( dirFile )) != NULL ) 
      {
         if ( !strcmp( hFile->d_name, "."  )) continue;
         if ( !strcmp( hFile->d_name, ".." )) continue;

         // in linux hidden files all start with '.'
         //if ( gIgnoreHidden && ( hFile->d_name[0] == '.' )) continue;

         // dirFile.name is the name of the file. Do whatever string comparison 
         // you want here. Something like:
         if ( strstr( hFile->d_name, exd )){
		string fileName(hFile->d_name);
		//cout<<path+fileName<<endl;
		files.push_back(path + fileName);
	 }            
		//printf( "%s\n", hFile->d_name );
      } 
      closedir( dirFile );
   }
}

int getIndex(std::string fileName){
	int slashIndex = fileName.find_last_of('/');
	std::string name = fileName.substr(slashIndex+1);
	int underscoreIndex = name.find_first_of('_');
	int index = atoi(name.substr(0,underscoreIndex).c_str());
	return index;
}

std::vector<std::string> getFixIndexFiles(std::vector<std::string> files){
	std::vector<string> filesFixIndex;
	filesFixIndex.resize(files.size());
	for(unsigned int i=0;i<files.size();i++){
		std::string next = files[i];
		int index = getIndex(next);
		filesFixIndex[index-1] = next;
	}
	for(size_t i=0;i<files.size();i++){
		//std::cout<<filesFixIndex[i]<<endl;
	}
	return filesFixIndex;
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

void splitPDFile(string indexFile, string tmp, string tmpFlip){
    std::ifstream ifs;
    vector<double> X;
    vector<double> Y;
    vector<double> XFlip;
    vector<double> YFlip;
    double x, y;
    ifs.open(indexFile.c_str());
    while(ifs >> x){
        ifs >> y;
	if(x < y){ // above diagnal
	    X.push_back(x);
	    Y.push_back(y);
	}else if(x > y){ // below diagnal
	    XFlip.push_back(y);
	    YFlip.push_back(x);
	}
    }
    ifs.close();
    ifs.clear();
    std::ofstream ofs;
    ofs.open(tmp.c_str());
    for(int i = 0; i < X.size(); i++){
	ofs << X[i] << " " << Y[i] << endl;
    }
    ofs.close();
    ofs.clear();
    ofs.open(tmpFlip.c_str());
    for(int i = 0; i < XFlip.size(); i++){
	ofs << XFlip[i] << " " << YFlip[i] << endl;
    }
    ofs.close();
    ofs.clear();
}
/***************************************************/

int main_separate(int argc, char* argv[])
{
    if(argc != 4){
        cout<<"usage: wasserstein_dist <distance_diagrams_path> <output_distance_matrix_file_path> <tmp_working_folder>"<<endl;
	std::exit(1);
    }
    std::string filePath = argv[1];
    std::string output = argv[2];
    std::string tmpFolder = argv[3];
    
    std::string tmpPrefix = "/tmp_";
    std::string tmpFlipPrefix = "/tmpFlip_";
    std::string fileA, fileB, tmpA, tmpAFlip, tmpB, tmpBFlip;
    //std::string filePath = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/Giorgio_1268_inter_principal/output_diagrams/";
    //std::string filePath = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/output_diagrams_withDiameter/";
    //std::string filePath = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/output_test/";

    std::vector<std::string> files;
    listFiles(filePath.c_str(), ".swc", files);
    std::vector<std::string> indexFiles;
    indexFiles = getFixIndexFiles(files);
	
	std::map<string, string> tmpMap;
	std::map<string, string> tmpFlipMap;
	for(int i = 0; i < indexFiles.size(); i++){
	    string tmp, tmpFlip;
	    std::string filename;
	    filename = indexFiles[i];
	    size_t last_slash_idx = filename.find_last_of("\\/");
	    if (std::string::npos != last_slash_idx){
    		filename.erase(0, last_slash_idx + 1);
	    }
    	    tmp.append(tmpFolder).append(tmpPrefix).append(filename);
    	    tmpFlip.append(tmpFolder).append(tmpFlipPrefix).append(filename);
	    tmpMap[filename] = tmp;
	    tmpFlipMap[filename] = tmpFlip;
	    splitPDFile(indexFiles[i], tmp, tmpFlip);
	}


	vector<vector<double>> distMatrix;
	distMatrix.resize(indexFiles.size());
	for(int i=0;i<indexFiles.size();i++){
		distMatrix[i].resize(indexFiles.size());
	}
    	for(int i=0;i<indexFiles.size();i++){
		cout<<indexFiles[i]<<endl;
		for(int j=i;j<indexFiles.size();j++){
	        	fileA = indexFiles[i];
	        	size_t last_slash_idx = fileA.find_last_of("\\/");
	    		if (std::string::npos != last_slash_idx){
    			    fileA.erase(0, last_slash_idx + 1);
		        }
			fileB = indexFiles[j];
	        	last_slash_idx = fileB.find_last_of("\\/");
	    		if (std::string::npos != last_slash_idx){
    			    fileB.erase(0, last_slash_idx + 1);
		        }
 	   		PairVector diagramA, diagramAFlip, diagramB, diagramBFlip;
			//cout<<j<<endl;				
			//cout<<"tmpA"<<" ";				
			if (!geom_bt::readDiagramPointSet(tmpMap[fileA], diagramA)) {
				std::exit(1);
			}
			//cout<<"tmpAFlip"<<" ";
			if(!geom_bt::readDiagramPointSet(tmpFlipMap[fileA], diagramAFlip)) {
				std::exit(1);
			}
			//cout<<"tmpB"<<" ";
			if (!geom_bt::readDiagramPointSet(tmpMap[fileB], diagramB)) {
				std::exit(1);
			}
			//cout<<"tmpBFlip"<<" ";
			if (!geom_bt::readDiagramPointSet(tmpFlipMap[fileB], diagramBFlip)) {
				std::exit(1);
			}
			double res = geom_bt::bottleneckDistExact(diagramA, diagramB);
			//cout<<"diagramAFlip--diagramBFlip"<<endl;
			double resFlip = geom_bt::bottleneckDistExact(diagramAFlip, diagramBFlip);
			distMatrix[i][j] = max(res, resFlip);
			distMatrix[j][i] = distMatrix[i][j];
			//std::cout << std::setprecision(15) << res << std::endl;
			diagramA.clear();
			diagramB.clear();
		}	
	} 
	//string output = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/output/distMatrix.txt";
	//string output = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/Giorgio_1268_inter_principal/wasserstein.txt";
		
	printMat(output, distMatrix);	
   return 0;
}


int main(int argc, char* argv[])
{
    if(argc != 3){
        cout<<"usage: wasserstein_dist <distance_diagrams_path> <output_distance_matrix_file_path>"<<endl;
	std::exit(1);
    }
    std::string filePath = argv[1];
    std::string output = argv[2];
    //std::string filePath = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/Giorgio_1268_inter_principal/output_diagrams/";
    //std::string filePath = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/output_diagrams_withDiameter/";
    //std::string filePath = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/output_test/";
        

    std::vector<std::string> files;
    listFiles(filePath.c_str(), ".swc", files);
    std::vector<std::string> indexFiles;
    indexFiles = getFixIndexFiles(files);
	
	vector<vector<double>> distMatrix;
	distMatrix.resize(indexFiles.size());
	for(int i=0;i<indexFiles.size();i++){
		distMatrix[i].resize(indexFiles.size());
		distMatrix[i][i] = 0; // set diagnal to 0
	}
	cout<<indexFiles.size()<<endl;
    	for(int i=0;i<indexFiles.size();i++){
		cout<<indexFiles[i]<<endl;
		for(int j=i + 1;j<indexFiles.size();j++){
			cout<<indexFiles[j]<<"end"<<endl;
 	   		PairVector diagramA, diagramB;
			if (!geom_bt::readDiagramPointSet(indexFiles[i], diagramA)) {
				std::exit(1);
			}

			if (!geom_bt::readDiagramPointSet(indexFiles[j], diagramB)) {
				std::exit(1);
			}
			double res = geom_bt::bottleneckDistExact(diagramA, diagramB);
			distMatrix[i][j] = res;
			distMatrix[j][i] = res;
			//std::cout << std::setprecision(15) << res << std::endl;
			diagramA.clear();
			diagramB.clear();
		}	
	} 
	//string output = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/output/distMatrix.txt";
	//string output = "/home/louis/Research/Research2016/grey_narn-geom_matching-f6306648de3b/wasserstein/data/Giorgio_1268_inter_principal/wasserstein.txt";
		
	printMat(output, distMatrix);	
   return 0;
}

int main_original(int argc, char* argv[])
{
    if (argc < 3 ) {
        std::cerr << "Usage: " << argv[0] << " file1 file2 [relative_error]. Without relative_error calculate the exact distance." << std::endl;
        return 1;
    }
    
    std::string file1 = "/home/louis/Research/Research2016/grey_narn-hera-5226e1038a0f/geom_bottleneck/data/PD_func_val/testing/1_wasserstein_test_GA.swc";
    std::string file2 = "/home/louis/Research/Research2016/grey_narn-hera-5226e1038a0f/geom_bottleneck/data/PD_func_val/testing/2_wasserstein_test_GB.swc";
    PairVector diagramA, diagramB;
    if (!geom_bt::readDiagramPointSet(file1, diagramA)) {
        std::exit(1);
    }

    if (!geom_bt::readDiagramPointSet(file2, diagramB)) {
        std::exit(1);
    }

    double res;
    if (argc >= 4) {
        // the third parameter is epsilon,
        // return approximate distance (faster)
        double approxEpsilon =  atof(argv[3]);
        if (approxEpsilon > 0.0) {
            res = geom_bt::bottleneckDistApprox(diagramA, diagramB, approxEpsilon);
        } else if (approxEpsilon == 0.0) {
            res = geom_bt::bottleneckDistExact(diagramA, diagramB);
        } else {
            std::cerr << "The third parameter (relative error) must be positive!" << std::endl;
            std::exit(1);
        }
    } else {
        // only filenames have been supplied, return exact distance
        res = geom_bt::bottleneckDistExact(diagramA, diagramB);
    }
    std::cout << std::setprecision(15) << res << std::endl;

    // Alternative could be to construct DiagramPointSet
    // using the constructor with iterators.
    // May be useful if the same diagram is used multiple times
    // to avoid copying data from user's container each time.
    
    //geom_bt::DiagramPointSet dA(diagramA.begin(), diagramA.end());
    //geom_bt::DiagramPointSet dB(diagramB.begin(), diagramB.end());
    //double result1 = geom_bt::bottleneckDistExact(dA, dB);
    //std::cout << std::setprecision(15) << result1 << std::endl;

    return 0;
}
