#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <cmath>
#include <iomanip>      //std::setprecision
#include <boost/heap/fibonacci_heap.hpp>
#include <io.h>
#include <float.h>


#define BDEBUG false
#define BTIMED false	//time analysis for computing diagrams
#define PREFIX ""
#define FTF 0
#define FTS 1
#define STF 2
#define STS 3
#define BNORMALIZE false //control normalization
#define CUTOFF 0		//control cutoff value
#define B_ONE_RING_NEIGHBOR true
#define B_SUMMER_BN true // true is sum version

#define cell      double

typedef int ssize_t;
extern "C"
{
	ssize_t** kuhn_match(cell** table, size_t n, size_t m);
	void print(cell** t, size_t n, size_t m, ssize_t** assignment);
}

using namespace std;

typedef struct MidPoint
{
	int first, second;
	double height;
} MidPoint;

typedef struct
{
	double x, y, z;
} Point3D;

double computePointDistance(const Point3D& p1, const Point3D& p2);

typedef struct node
{
	int id;
	double weight;
	node(int i, double w) : id(i), weight(w){};
} node;

struct mycomparison
{
	bool operator() (const node & lhs, const node &rhs) const
	{
		return lhs.weight > rhs.weight;
	}
};

class Vertex
{
private:
	Vertex* next;
	int dest;
	double edgeWeight;
public:
	Vertex(int dest, double edgeWeight = 1)
	{
		this->dest = dest;
		this->next = NULL;
		this->edgeWeight = edgeWeight;
	}
	friend class AdjList;
	friend class Graph;
};

class AdjList
{
private:
	vector<Vertex> adjList;
	int ks;		//number of times paired in SH0
	double heightVal;
	bool isVisited;
	vector<int> usList;
	vector<int> dsList;

	friend class Graph;

public:
	AdjList()
	{
		//this->head = NULL;
		this->ks = 0;
		this->heightVal = -1;
		this->usList.clear();
		this->dsList.clear();
		this->isVisited = false;
	}
};

class Graph
{
public:
	vector< vector<double> > dist;
	vector< vector<int> > next;
	vector<int> sampleBP;		//contains the indicies of sample base points
private:
	int numV;
	int numE;
	int numF;
	vector<AdjList> aList;
	vector<Point3D> vP;
	vector<int> sortedList;		//including all vertices

	int baseP;
	int diamP;
	double fMaxHeight;
	int iMaximum;
	double dDiameter;
	double dAllEL;
	double dAvgEL;
	double delta;
	double zero;
	double dMax;

	vector<int> mSampleIndicies;	//map the indicies of sample base points to their indicies in sampleBP
	vector< pair<double, double> > SH0;
	vector< pair<double, double> > EH1;
	vector< pair<double, double> > Dg;
	//pairing with indices
	vector< pair<int, int> > SH0i;
	vector< pair<int, int> > EH1i;
	vector< pair<int, int> > Dgi;
	int iH0;
	map<int, int> graphMatching;			//store an approximated graph matching between two graphs

	void classifyVertices();
	friend bool compareFirst(pair<double, double>& a, pair<double, double>& b);
	friend bool compareSecond(pair<double, double>& a, pair<double, double>& b);

public:
	vector< vector< pair<double, double> > > vDiagram;		//store the current persistence diagram
	vector< vector< pair<double, double> > > vDiagram2;		//store the other graph's persistence diagrams
	vector< vector< pair<int, int> > > vDiagrami;		//store the pair indices of current persistence diagram
	vector< vector< pair<int, int> > > vDiagram2i;		//store the pair indices of the other graph's diagram
	vector<int> v1H0;	//store # H0 for each base point of the current graph
	vector<int> v2H0;	//store # H0 for each base point of the other graph

	//constuctor
	Graph(int numV, double delta = 1.0 / 18, int numF = 0);
	//get and set
	double getAvgEL() { return this->dAvgEL; }
	int getNumE() {return this->numE;}
	int getNumV() {return this->numV;}
	int getNumF() { return this->numF; }
	void setDiameter(double d) {this->dDiameter = d;}
	double computeDiameter();
	double getDiameter() {return this->dDiameter;}
	void setVP(vector<Point3D> vp) { this->vP = vp;}
	void setBasePoint(int n){ this->baseP = n; this->aList[n].heightVal = 0; }
	int getBasePoint(){ return this->baseP; }
	//basic graph operations
	void addEdge(int src, int dest, double edgeWeight = 1);
	void addDirectedEdge(int src, int dest);
	void inverseDirectedEdge(int src, int dest);
	void deleteDirectedEdge(int src, int dest);
	bool isNeighbor(int i, int j)
	{
		pair<int, int> p(-1, -1);
		AdjList& vertex = aList[i];
		for (int k = 0; k < vertex.adjList.size(); ++k)
		{
			if(vertex.adjList[k].dest == j)
				return true;
		}
		return false;
	}
	//print graph info
	void printGraph();
	void printHeightVal();
	void outputWeightedGraph(string filename);
	//different random sub-sample methods
	void sampleBasePoints(int b);		//use input b as the first sample base point
	void sampleBasePoints(string filename);		//sample from file
	void sampleBasePoints();		//random sample
	void sampleBasePointsWODeg2();		//random sample without degree-2 vertices
	vector<int> computeNeighborInRange(int base);
	//collect basic info for computing persistence diagrams
	void heightFunc(int base);
	void computeShortestDistanceDj();
	bool isSmaller(int a, int b);
	void sortVertices();
	//compute persistence diagrams
	void pairingH0();
	void pairingH1();
	void computeDiagram(string folder, int base);
	double computeDiagramDistToDiagnal(int base); 	// ---Yanjie 03/01/2016
	double getDistToDiagnalSum();					// ---Yanjie 03/01/2016
	void preparePersistentDiagram();		// ---Yanjie 03/08/2016
	void computePersistentDiagram(int base);		// ---Yanjie 03/08/2016
	double getPersistentRange();					// ---Yanjie 03/13/2016
	vector<pair<double, double> > getPersistentDiagram();	// ---Yanjie 04/01/2016
	vector< double > getPersisDiagFunGaussianVector(int vectLen, double sigma, double range);		// ---Yanjie 03/08/2016
	vector< pair<double, double> > getDiagram(){ return this->Dg; }
	//compute bottleneck distance
	double computeBottleneckDistanceDiagram(vector< pair<double, double> >& p, vector< pair<double, double> >& q);
	double computeBottleneckDistanceDiagramSumVersion(vector< pair<double, double> >& p, vector< pair<double, double> >& q);
	double computeBottleneckDistance(int base1, int base2, int mode, Graph& g2);
	int DFS(int dest, vector<int> &path);
	bool compareDiagrams(vector< pair<double, double> >& a, vector< pair<double, double> >& b) const;
	//compute PD distance
	double computePersistenceDistortionDistance(Graph& g2, string& fileResult);
};

#endif /* GRAPH_H_ */
