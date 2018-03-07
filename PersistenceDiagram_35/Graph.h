/***********
//Code by Dayu Shi
//Oct. 2016
***********/

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
#include <math.h>
#include <iomanip>      //std::setprecision
#include <limits>
#include <utility>


#define BDEBUG false
#define BTIMED false	//time analysis for computing diagrams
#define PREFIX ""
#define FTF 0
#define FTS 1
#define STF 2
#define STS 3
#define BNORMALIZE false
#define CUTOFF 1.1
#define B_ONE_RING_NEIGHBOR false

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
private:
	int numV;
	int numE;
	int numF;
	vector<AdjList> aList;
	vector<Point3D> vP;
	vector<int> sortedList;		//including all vertices

	double fMaxHeight;
	double fMinHeight;
	int iMaximum;
	double dAllEL;
	double dAvgEL;
	double zero;
	double dMax;
	vector< pair<double, double> > SH0;
	vector< pair<double, double> > EH1;
	vector< pair<double, double> > DgSuper;
	vector< pair<double, double> > DgSub;
	//pairing with indices
	vector< pair<int, int> > SH0i;
	vector< pair<int, int> > EH1i;
	vector< pair<int, int> > Dgi;
	int iH0Super;
	int iH0Sub;

	void classifyVertices();
	friend bool compareFirst(pair<double, double>& a, pair<double, double>& b);
	friend bool compareSecond(pair<double, double>& a, pair<double, double>& b);

public:

	//constuctor
	Graph(int numV, double delta = 1.0 / 18, int numF = 0);
	//get and set
	double getAvgEL() { return this->dAvgEL; }
	int getNumE() {return this->numE;}
	int getNumV() {return this->numV;}
	int getNumF() { return this->numF; }
	void setVP(vector<Point3D> vp) { this->vP = vp;}
	//basic graph operations
	void addEdge(int src, int dest, double edgeWeight = 0);
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
	bool isSmaller(int a, int b);
	void sortVertices();
	//compute persistence diagrams
	void pairingH0();
	void pairingH1();
	void computeDiagram(bool bSuperLevelSet);
	//vector< pair<double, double> > getDiagram(){ return this->Dg; }
	//compute bottleneck distance
	double computeBottleneckDistanceDiagram(vector< pair<double, double> >& p, vector< pair<double, double> >& q);
	void computeBottleneckDistance(Graph& g2, string filename);
	int DFS(int dest, vector<int> &path);
	bool compareDiagrams(vector< pair<double, double> >& a, vector< pair<double, double> >& b) const;

	void outputPersistenceDiagrams(string filename);
	void outputPersistenceDiagramsOnlyData(string filename);
	void setFuncVal(int v, double val);
};

#endif /* GRAPH_H_ */
