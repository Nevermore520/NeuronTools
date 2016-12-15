/*
 * FileFun.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Yanjie
 */

#ifndef FILEFUN_H_
#define FILEFUN_H_

#include "Graph.h"

void getFiles( string path, string exd, vector<string>& files );

string extractFileName(string fileName);

void printMat(string outputFile, vector<vector<double> > &distanceMat);

void appendMatToFile(string outputFile, vector<vector<double> > &distanceMat);

int getIndex(string fileName);

vector<string> getFixIndexFiles(vector<string> files);

void readMat(string fileName,vector<vector<double> > &distanceMat);

#endif /* FILEFUN_H_ */
