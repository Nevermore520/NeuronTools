/*
 
Copyright (c) 2015, M. Kerber, D. Morozov, A. Nigmetov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 

You are under no obligation whatsoever to provide any bug fixes, patches, or
upgrades to the features, functionality or performance of the source code
(Enhancements) to anyone; however, if you choose to make your Enhancements
available either publicly, or directly to copyright holder,
without imposing a separate written license agreement for such Enhancements,
then you hereby grant the following license: a  non-exclusive, royalty-free
perpetual license to install, use, modify, prepare derivative works, incorporate
into other computer software, distribute, and sublicense such enhancements or
derivative works thereof, in binary and source code form.

  */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <dirent.h>
#include <string.h>

#include "wasserstein.h"

using namespace std;

// any container of pairs of doubles can be used,
// we use vector in this example.

typedef std::vector<std::pair<double, double>>  PairVector;

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
	}
    	for(int i=0;i<indexFiles.size();i++){
		cout<<indexFiles[i]<<endl;
		for(int j=i;j<indexFiles.size();j++){
 	   		PairVector diagramA, diagramB;
			if (!geom_ws::readDiagramPointSet(indexFiles[i], diagramA)) {
				std::exit(1);
			}

			if (!geom_ws::readDiagramPointSet(indexFiles[j], diagramB)) {
				std::exit(1);
			}
 	   		double wasserPower = 1.0;

 	   		//default relative error:  1%
 	   		double delta = 0.01;
		
			// default for internal metric is l_infinity
			double internal_p = std::numeric_limits<double>::infinity();

			double res = geom_ws::wassersteinDist(diagramA, diagramB, wasserPower, delta, internal_p);
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
