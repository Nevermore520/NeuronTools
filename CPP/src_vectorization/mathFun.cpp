/*
 * mathFun.cpp
 *
 *  Created on: May 13, 2016
 *      Author: Yanjie
 */

#include "mathFun.h"

using namespace std;

double getDistanceL2(vector<double> v1, vector<double> v2){
	double dist = 0;
	for(int i = 0; i < v1.size(); i++){
		dist += (v1[i] - v2[i])*((v1[i] - v2[i]));
	}
	return sqrt(dist);
}

double getDistanceL1(vector<double> v1, vector<double> v2){
	double dist = 0;
	for(int i = 0; i < v1.size(); i++){
		dist += abs(v1[i] - v2[i]);
	}
	return dist;
}



