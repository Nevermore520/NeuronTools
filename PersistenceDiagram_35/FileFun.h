/*
 * FileFun.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Yanjie
 */

#ifndef FILEFUN_H_
#define FILEFUN_H_

#include <string.h>
#include <io.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

void getFiles( string path, string exd, vector<string>& files );

int getIndex(string fileName);

vector<string> getFixIndexFiles(vector<string> files);

void readMat(string fileName,vector<vector<double> > &distanceMat);

#endif /* FILEFUN_H_ */
