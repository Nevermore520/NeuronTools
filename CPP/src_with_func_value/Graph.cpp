/***********
//Code by Dayu Shi
//Oct. 2016
***********/
#include "Graph.h"

double computePointDistance(const Point3D& p1, const Point3D& p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}

double computeDistance(pair<double, double>& p1, pair<double, double>& p2)
{
	double deltaX, deltaY;
	deltaX = fabs(p1.first - p2.first);
	deltaY = fabs(p1.second - p2.second);
	return deltaX > deltaY ? deltaX : deltaY;
}

int compare(const void * a, const void * b)
{
	if (*(double *)a < *(double *)b)
		return -1;
	else if (*(double *)a > *(double *)b)
		return 1;
	else
		return 0;
}

void Graph::printGraph()
{
	int i;
	for (i = 0; i < this->numV; i++)
	{
		cout << "AdjList of Vertex " << i << endl;
		cout << "head";
		AdjList& vertex = this->aList[i];
		for (int k = 0; k < vertex.adjList.size(); ++k)
		{
			cout << " -> " << vertex.adjList[k].dest;
		}
		cout << endl;
		//cout << "Degree: " << this->aList[i].degree << endl << endl;
	}

}

void Graph::outputWeightedGraph(string filename)
{
	std::ofstream ofs(filename.c_str());
	ofs << numV << endl;
	for (int i = 0; i < this->numV; i++)
	{
		AdjList& vertex = aList[i];
		for (int k = 0; k < vertex.adjList.size(); ++k)
		{
			if (i < vertex.adjList[k].dest)
			{
				ofs << "(" << i << " , " << vertex.adjList[k].dest << ") --- " << vertex.adjList[k].edgeWeight << endl;
			}
		}
	}
	ofs.close();
	ofs.clear();
}

Graph::Graph(int numV, double delta, int numF) : dist(numV, vector<double>(numV, 987654321)), next(numV, vector<int>(numV, -1))
{
	this->numV = numV;
	this->numF = numF;
	this->numE = 0;
	this->iMaximum = 0;
	this->dAllEL = 0;
	this->dAvgEL = 0;
	this->zero = std::numeric_limits<double>::min();	//
	this->dMax = std::numeric_limits<double>::max();
	int i;
	for (i = 0; i < numV; i++)
	{
		AdjList a;
		this->aList.push_back(a);
	}
}

void Graph::setFuncVal(int v, double val)
{
	this->aList[v].heightVal = val;
	return;
}

void Graph::addEdge(int src, int dest, double edgeWeight)
{
	//no multiple edges
	AdjList& vertex = this->aList[src];
	for (int k = 0; k < vertex.adjList.size(); ++k)
	{
		if (dest == vertex.adjList[k].dest)
			return;
	}
	//no self-loops
	if (src == dest)
		return;
	//add dest to src
	this->dAllEL += edgeWeight;
	Vertex v(dest, edgeWeight);
	this->aList[src].adjList.push_back(v);
	//Since the graph is undirected, add src to dest
	Vertex v2(src, edgeWeight);
	this->aList[dest].adjList.push_back(v2);
	this->numE++;
}

bool compareHeight(const pair<int, double>& a, const pair<int, double>& b)
{
	if (a.second < b.second)
		return true;
	else if (a.second == b.second && a.first < b.first)
		return true;
	else
		return false;
}

//sort vertices from bottom to top, if two have the same height value, make the one with smaller index before the other. use quick sort
void Graph::sortVertices()
{
	vector< pair<int, double> > hvPair;
	vector< pair<int, double> >::iterator it;
	for (int i = 0; i < aList.size(); i++)
		hvPair.push_back(::make_pair(i, aList[i].heightVal));
	std::sort(hvPair.begin(), hvPair.end(), compareHeight);

	this->sortedList.clear();
	for (it = hvPair.begin(); it != hvPair.end(); it++)
		this->sortedList.push_back(it->first);

	/*if (aList[this->sortedList[0]].heightVal == -1)
		throw runtime_error("Unconnected Graph!");*/

	//computeMaximumPoint
	int max = this->sortedList[aList.size() - 1];
	int min = this->sortedList[0];
	this->fMaxHeight = this->aList[max].heightVal;
	this->fMinHeight = this->aList[min].heightVal;
}

void Graph::printHeightVal()
{
	for (int i = 0; i < aList.size(); i++)
	{
		cout << i << "'s height value: " << aList[i].heightVal << endl;
		if (i >= numV)
		{
			cout << "(" << aList[i].dsList[0] << ", " << aList[i].dsList[1] << ")" << endl;
		}
	}
	cout << endl;
	cout << "Max Height: " << this->fMaxHeight << endl << endl;
}

void Graph::classifyVertices()
{
	//compute up and down saddle lists
	double f, g;
	int dest;
	for (int i = 0; i < this->aList.size(); i++)
	{
		AdjList& src = aList[i];
		src.usList.clear();
		src.dsList.clear();
		src.ks = 0;
		f = src.heightVal;
		for (int k = 0; k < src.adjList.size(); ++k)
		{
			dest = src.adjList[k].dest;
			g = aList[dest].heightVal;
			if (f < g)
				src.usList.push_back(dest);
			else if (f > g)
				src.dsList.push_back(dest);
			else if (i < dest)
				src.usList.push_back(dest);
			else
				src.dsList.push_back(dest);
		}
	}
}

void Graph::pairingH0()
{
	pair<int, int> p;
	int i, j, k, l, m;
	vector<int> trus;

	vector<int> ufList;
	//initialize the union find set
	for (int i = 0; i < aList.size(); i++)
		ufList.push_back(i);
	this->SH0.clear();
	this->SH0i.clear();

	//pairing for SH0
	for (i = 1; i < this->sortedList.size(); i++)
	{
		k = this->sortedList[this->sortedList.size() - 1 - i];
		if (this->aList[k].usList.size() == 1)
		{
			int t = this->aList[k].usList[0];
			while (ufList[t] != t)
				t = ufList[t];
			ufList[k] = t;
		}
		else if (this->aList[k].usList.size() > 1)
		{
			for (m = 0; m < this->aList[k].usList.size(); m++)
			{
				//j is the upsaddle adjacent vertex to k
				j = this->aList[k].usList[m];

				while (ufList[j] != j)
					j = ufList[j];

				//first edge in upsaddle
				if (ufList[k] == k)
				{
					ufList[k] = j;
					continue;
				}

				//after first edge in upsaddle
				l = k;
				while (ufList[l] != l)
					l = ufList[l];

				if (j != l)		//pairing and update the number of times that k is paired in SH0, using "-f" value
				{
					this->aList[k].ks++;
					if (this->isSmaller(l,j))
					{
						this->SH0.push_back(::make_pair(aList[k].heightVal, aList[l].heightVal));
						this->SH0i.push_back(::make_pair(k, l));
						//mid point
						if (l >= numV && aList[l].dsList[0] != k && aList[l].dsList[1] != k)
							trus.push_back(k);
						//union operation
						ufList[k] = j;
						ufList[l] = j;
					}
					else
					{
						this->SH0.push_back(::make_pair(aList[k].heightVal, aList[j].heightVal));
						this->SH0i.push_back(::make_pair(k, j));
						//mid point
						if (j >= numV && aList[j].dsList[0] != k && aList[j].dsList[1] != k)
							trus.push_back(k);
						//union operation
						ufList[j] = l;
					}
				}
			}
		}
	}
	return;
}

//check if a is before b in sortedList
bool Graph::isSmaller(int a, int b)
{
	if (aList[a].heightVal < aList[b].heightVal || ((fabs(aList[a].heightVal - aList[b].heightVal) < this->zero) && a < b))
		return true;
	else
		return false;
}


void Graph::pairingH1()
{
	this->EH1.clear();
	this->EH1i.clear();
	for (int i = 0; i < this->sortedList.size() - 1; i++)		//no need to handle the last vertex, which is the maximum vertex
	{
		int s = sortedList[i];	//current vertex index
		int c = aList[s].usList.size() - aList[s].ks - 1;	//# unhandled up branches
		if (c <= 0)
			continue;
		if (B_ONE_RING_NEIGHBOR)
		{
			//check if c is equal to the number of up triangles of current vertex s
			//first find all up neighbors of current vertex s
			vector<int> us;
			for (int i = 0; i < aList[s].usList.size(); i++)
			{
				int u = aList[s].usList[i];
				us.push_back(u);
			}
			//find all up triangles of current vertex s
			vector<int> ufs = us;
			int countUT = 0;
			for (int i = 0; i < us.size() - 1; ++i)
			{
				for (int j = i + 1; j < us.size(); ++j)
				{
					if (isNeighbor(us[i], us[j]))
					{
						int m = i, n = j;
						while (ufs[m] != us[m])
						{
							for (int k = 0; k < us.size(); ++k)
							{
								if (us[k] == ufs[m])
									m = k;
							}
						}
						while (ufs[n] != us[n])
						{
							for (int k = 0; k < us.size(); ++k)
							{
								if (us[k] == ufs[n])
									n = k;
							}
						}
						if (m != n)
						{
							++countUT;
							ufs[n] = us[m];
						}
					}
				}
			}
			//trivial case
			if (countUT == c)
				continue;
		}
		//otherwise still run the normal pairing H1 process
		int slsize = this->sortedList.size();
		int ufsize = aList[s].usList.size() + slsize + 1;
		vector<int> uf(ufsize, -1);
		uf[s] = s;		//root of the union find set
		//add representitives to the union find set
		for (int k = 1; k <= aList[s].usList.size(); k++)
		{
			uf[slsize + k] = slsize + k;
		}
		//go from bottom up to merge classes and find cycles, stop right after c becomes zero
		for (int l = i + 1; l < this->sortedList.size() && c > 0; l++)
		{
			int o = sortedList[l];
			if (aList[o].dsList.size() == 1)	//only has one down saddle.
			{
				if (this->isSmaller(aList[o].dsList[0], s))
				{
					uf[o] = o;
				}
				else
				{
					int r = aList[o].dsList[0];
					if (r == s)	//look for a representitive r of s
					{
						for (int p = 1; p <= this->aList[s].usList.size(); p++)
						{
							if (o == this->aList[s].usList[p - 1])
							{
								r = slsize + p;
								break;
							}
						}
					}
					while (uf[r] != r)
						r = uf[r];
					uf[o] = r;
				}
			}
			else if (aList[o].dsList.size() > 1)
			{
				int r1, r2; //last down branch of o
				int i;
				for (i = 0; i < aList[o].dsList.size(); ++i)
				{
					r1 = aList[o].dsList[i];
					if (!isSmaller(r1, s))
						break;
				}
				if (i < aList[o].dsList.size())		//find a down branch which is after s in sortedlist
				{
					if (r1 == s)		//look for a representitive r of s
					{
						for (int p = 1; p <= this->aList[s].usList.size(); p++)
						{
							if (o == this->aList[s].usList[p - 1])
							{
								r1 = slsize + p;
								break;
							}
						}
					}
					while (uf[r1] != r1)
						r1 = uf[r1];
					uf[o] = r1;
					for (++i; i < aList[o].dsList.size(); ++i)
					{
						r2 = aList[o].dsList[i];
						//if current down branch goes to some point before s in the sortedList, ignore it
						if (isSmaller(r2, s))
						{
							r1 = uf[o];
							continue;
						}
						else
						{
							if (r2 == s)		//look for a representitive r of s
							{
								for (int p = 1; p <= this->aList[s].usList.size(); p++)
								{
									if (o == this->aList[s].usList[p - 1])
									{
										r2 = slsize + p;
										break;
									}
								}
							}
							while (uf[r2] != r2)
								r2 = uf[r2];
						}
						if (r1 > slsize && r2 > slsize)	//find a cycle
						{
							if (r1 != r2)
							{
								if (r1 < r2)
								{
									uf[o] = r1;
									uf[r2] = r1;
								}
								else
								{
									uf[o] = r2;
									uf[r1] = r2;
									r1 = r2;
								}
								this->EH1.push_back(std::make_pair(aList[s].heightVal, aList[o].heightVal));
								this->EH1i.push_back(std::make_pair(s, o));
								c -= 1;		//remaining cycle starting from s is reduced by one
								if (c <= 0)
									break;
							}
							else
							{
								uf[o] = r1;
							}
						}
						else if (r1 > slsize)
						{
							uf[r2] = r1;
							uf[aList[o].dsList[i]] = r1;
							uf[o] = r1;
						}
						else if (r2 > slsize)
						{
							uf[r1] = r2;
							uf[aList[o].dsList[i]] = r2;
							uf[o] = r2;
							r1 = r2;
						}
						else if (r1 < r2)
						{
							uf[r2] = r1;
							uf[aList[o].dsList[i]] = r1;
							uf[o] = r1;
						}
						else
						{
							uf[r1] = r2;
							uf[aList[o].dsList[i]] = r2;
							uf[o] = r2;
							r1 = r2;
						}
					}
				}
				else	//no down branch is after s in the sortedlist
				{
					uf[o] = o;
				}
			}
			else
			{
				//found a node with down degree 0;
				uf[o] = o;
			}
		}
	}
	return;
}

void Graph::computeDiagram(bool bSuperLevelSet)
{
	std::ofstream ofs;
	std::ifstream ifs;
	if (BTIMED)
		ofs.open("DiagramComputing.txt");
	clock_t timer = clock();
	clock_t timer1, timer2, timer3, timer4, timer5, timer6;
	vector< pair<double, double> > Dg;
	int iH0;
	if (!bSuperLevelSet)	//sub level set filtration
	{
		for (int i = 0; i < this->aList.size(); ++i)
			this->aList[i].heightVal = -this->aList[i].heightVal;
	}
	sortVertices();
	if (BTIMED)
	{
		timer2 = clock();
		ofs << "sortVertices: " << ((double)(timer2 - timer1)) / CLOCKS_PER_SEC << "s" << endl;
	}
	classifyVertices();
	if (BTIMED)
	{
		timer3 = clock();
		ofs << "classifyVertices: " << ((double)(timer3 - timer2)) / CLOCKS_PER_SEC << "s" << endl;
	}
	//printClassifiedPoints();
	pairingH0();
	if (BTIMED)
	{
		timer4 = clock();
		ofs << "pairingH0: " << ((double)(timer4 - timer3)) / CLOCKS_PER_SEC << "s" << endl;
	}
	pairingH1();
	if (BTIMED)
	{
		timer5 = clock();
		ofs << "pairingH1: " << ((double)(timer5 - timer4)) / CLOCKS_PER_SEC << "s" << endl;
	}
	Dg.clear();
	for (int i = 0; i < this->SH0.size(); i++)
	{
		/*if (fabs(this->SH0[i].first - this->SH0[i].second) < CUTOFF * this->dAvgEL)
			continue;*/
		Dg.push_back(this->SH0[i]);
	}
	Dg.push_back(std::make_pair(this->fMinHeight, this->fMaxHeight));
	iH0 = Dg.size();
	for (int i = 0; i < this->EH1.size(); i++)
	{
		/*if (fabs(this->EH1[i].first - this->EH1[i].second) < CUTOFF * this->dAvgEL)
			continue;*/
		Dg.push_back(this->EH1[i]);
	}
	if (!bSuperLevelSet)	//sub level set, reverse the heightVal and pairs in diagrams
	{
		for (int i = 0; i < this->aList.size(); ++i)
			this->aList[i].heightVal = -this->aList[i].heightVal;
		for (int i = 0; i < Dg.size(); ++i)
		{
			pair<double, double> p(-Dg[i].second, -Dg[i].first);
			this->DgSub.push_back(p);
		}
		this->iH0Sub = iH0;
	}
	else
	{
		for (int i = 0; i < Dg.size(); ++i)
		{
			pair<double, double> p(Dg[i].second, Dg[i].first);
			this->DgSuper.push_back(p);
		}
		this->iH0Super = iH0;
	}
	if (BTIMED)
	{
		ofs.close();
		ofs.clear();
	}
	return;
}

void Graph::addDirectedEdge(int src, int dest)
{
	//add dest to src
	Vertex v(dest);
	this->aList[src].adjList.push_back(v);
	this->numE++;
}

void Graph::inverseDirectedEdge(int src, int dest)
{
	this->deleteDirectedEdge(src, dest);
	this->addDirectedEdge(dest, src);
}

void Graph::deleteDirectedEdge(int src, int dest)
{
	AdjList& vertex = aList[src];
	for (int k = 0; k < vertex.adjList.size(); ++k)
	{
		if (vertex.adjList[k].dest == dest)
		{
			vertex.adjList.erase(vertex.adjList.begin() + k);
			break;
		}
	}
	this->numE--;
	return;
}

bool compareFirst(pair<double, double>& a, pair<double, double>& b)
{
	if (a.first < b.first)
		return true;
	else
		return false;
}

bool compareSecond(pair<double, double>& a, pair<double, double>& b)
{
	if (a.second < b.second)
		return true;
	else
		return false;
}

bool Graph::compareDiagrams(vector<pair <double, double> >& a, vector< pair<double, double> >& b) const
{
	int i = a.size();
	int j = b.size();
	if (i != j)
		return false;
	for (int k = 0; k < i; k++)
	{
		if (!(fabs(a[k].first - b[k].first) < this->zero) || !(fabs(a[k].second - b[k].second) < this->zero))
			return false;
	}
	return true;
}
void Graph::outputPersistenceDiagramsOnlyData(string filename){
	this->computeDiagram(true);
	this->computeDiagram(false);
	ofstream ofs(filename.c_str());
	int ind;
	for (ind = 0; ind < this->iH0Super; ind++){
		ofs << this->DgSuper[ind].first << ' ' << this->DgSuper[ind].second << endl;
	}
	for (ind = 0; ind < this->iH0Sub; ind++){
		ofs << this->DgSub[ind].first << ' ' << this->DgSub[ind].second << endl;
	}
	ofs.close();
	ofs.clear();
}

void Graph::outputPersistenceDiagrams(string filename)
{
	this->computeDiagram(true);
	this->computeDiagram(false);
	ofstream ofs(filename.c_str());
	int ind;
	ofs << "-----------------------------------------" << endl;
	ofs << "Superlevel-set filtration: " << endl;
	ofs << "-----------------------------------------" << endl;
	ofs << "Diagram size: " << this->DgSuper.size() << endl;
	ofs << "--------------" << endl;
	ofs << "0th diagram: " << endl;
	for (ind = 0; ind < this->iH0Super; ind++)
	{
		ofs << this->DgSuper[ind].first << ' ' << this->DgSuper[ind].second;
		if (ind == this->iH0Super - 1)
			ofs << " (max-min pair)";
		ofs << endl;
	}
	ofs << "--------------" << endl;
	ofs << "1st diagram: " << endl;
	for (; ind < this->DgSuper.size(); ind++)
	{
		ofs << this->DgSuper[ind].first << ' ' << this->DgSuper[ind].second << endl;
	}
	ofs << endl;

	ofs << "-----------------------------------------" << endl;
	ofs << "Sublevel-set filtration: " << endl;
	ofs << "-----------------------------------------" << endl;
	ofs << "Diagram size: " << this->DgSub.size() << endl;
	ofs << "--------------" << endl;
	ofs << "0th diagram: " << endl;
	for (ind = 0; ind < this->iH0Sub; ind++)
	{
		ofs << this->DgSub[ind].first << ' ' << this->DgSub[ind].second;
		if (ind == this->iH0Sub - 1)
			ofs << " (min-max pair)";
		ofs << endl;
	}
	ofs << "--------------" << endl;
	ofs << "1st diagram: " << endl;
	for (; ind < this->DgSub.size(); ind++)
	{
		ofs << this->DgSub[ind].first << ' ' << this->DgSub[ind].second << endl;
	}
	ofs << endl;
	ofs.close();
	ofs.clear();
	return;
}

void Graph::computeBottleneckDistance(Graph& g2, string filename)
{
	int k, i1, i2;
	vector< pair<double, double> > d1, d2;
	double bn1, bn2, bn1Sub, bn2Sub;

	//superlevel-set filtration
	//Bottleneck distance for H0
	for (int i = 0; i < this->iH0Super; ++i)
		d1.push_back(this->DgSuper[i]);
	for (int j = 0; j < g2.iH0Super; ++j)
		d2.push_back(g2.DgSuper[j]);
	bn1 = this->computeBottleneckDistanceDiagram(d1, d2);
	//Bottleneck distance for H1
	d1.clear();
	d2.clear();
	for (int i = this->iH0Super; i < this->DgSuper.size(); ++i)
		d1.push_back(this->DgSuper[i]);
	for (int j = g2.iH0Super; j < g2.DgSuper.size(); ++j)
		d2.push_back(g2.DgSuper[j]);
	bn2 = this->computeBottleneckDistanceDiagram(d1, d2);
	ofstream ofs(filename.c_str());
	ofs << "-----------------------------------------" << endl;
	ofs << "Superlevel-set filtration: " << endl;
	ofs << "-----------------------------------------" << endl;
	ofs << "0th bottleneck distance: " << bn1 << endl;
	ofs << "1st bottleneck distance: " << bn2 << endl;
	ofs << endl;

	cout << "-----------------------------------------" << endl;
	cout << "Superlevel-set filtration: " << endl;
	cout << "-----------------------------------------" << endl;
	cout << "0th bottleneck distance: " << bn1 << endl;
	cout << "1st bottleneck distance: " << bn2 << endl;
	cout << endl;

	//sublevel-set filtration
	//Bottleneck distance for H0
	d1.clear();
	d2.clear();
	for (int i = 0; i < this->iH0Sub; ++i)
		d1.push_back(this->DgSub[i]);
	for (int j = 0; j < g2.iH0Sub; ++j)
		d2.push_back(g2.DgSub[j]);
	bn1Sub = this->computeBottleneckDistanceDiagram(d1, d2);
	//Bottleneck distance for H1
	d1.clear();
	d2.clear();
	for (int i = this->iH0Sub; i < this->DgSub.size(); ++i)
		d1.push_back(this->DgSub[i]);
	for (int j = g2.iH0Sub; j < g2.DgSub.size(); ++j)
		d2.push_back(g2.DgSub[j]);
	bn2Sub = this->computeBottleneckDistanceDiagram(d1, d2);

	ofs << "-----------------------------------------" << endl;
	ofs << "Sublevel-set filtration: " << endl;
	ofs << "-----------------------------------------" << endl;
	ofs << "0th bottleneck distance: " << bn1Sub << endl;
	ofs << "1st bottleneck distance: " << bn2Sub << endl;
	ofs << endl;

	cout << "-----------------------------------------" << endl;
	cout << "Sublevel-set filtration: " << endl;
	cout << "-----------------------------------------" << endl;
	cout << "0th bottleneck distance: " << bn1Sub << endl;
	cout << "1st bottleneck distance: " << bn2Sub << endl;
	cout << endl;

	ofs << "-----------------------------------------" << endl;
	ofs << "Final result: " << endl;
	ofs << "-----------------------------------------" << endl;
	ofs << "0th bottleneck distance: " << std::max(bn1, bn1Sub) << endl;
	ofs << "1st bottleneck distance: " << std::max(bn2, bn2Sub) << endl;
	ofs << endl;

	cout << "-----------------------------------------" << endl;
	cout << "Final result: " << endl;
	cout << "-----------------------------------------" << endl;
	cout << "0th bottleneck distance: " << std::max(bn1, bn1Sub) << endl;
	cout << "1st bottleneck distance: " << std::max(bn2, bn2Sub) << endl;
	cout << endl;

	ofs.close();
	ofs.clear();

	return;
}

double Graph::computeBottleneckDistanceDiagram(vector< pair<double, double> >& p, vector< pair<double, double> >& q)
{
	vector< pair<int, int> > matching;
	vector<int> path;
	bool binaryFlag;
	double leftVal, rightVal;
	pair<double, double> ii;
	int pSize = p.size();
	int qSize = q.size();
	//ui: dummy points corresponding to qi
	for (int i = 0; i < q.size(); i++)
	{
		ii = q[i];
		ii.second = ii.first;
		p.push_back(ii);
	}
	//wi: dummy points corresponding to pi
	for (int i = 0; i < pSize; i++)
	{
		ii = p[i];
		ii.second = ii.first;
		q.push_back(ii);
	}

	double* distance = new double[p.size() * q.size()];
	bool* isMatched = new bool[p.size() + q.size()];		//indicate whether a vertex is alredy matched
	int mCount;		//count of original vertices (non-dummy ones) in the matching

	//nearest neighbor check
	memset(isMatched, false, (p.size() + q.size()) * sizeof(bool));
	double nnMax = 0;		//lower bound of bottleneck distance
	vector< pair< pair<int, int>, double> > nnMatching;
	for (int i = 0; i < pSize; i++)
	{
		double min = dMax;
		int iMin = -1;
		double d;
		for (int j = 0; j < qSize; j++)
		{
			d = computeDistance(p[i], q[j]);
			distance[i * p.size() + j] = d;
			if (d - min < this->zero)
			{
				min = d;
				if (isMatched[p.size() + j] == false)
					iMin = j;
			}
		}
		d = p[i].second - p[i].first;
		if (d < min)
		{
			min = d;
			iMin = qSize + i;
		}
		if (iMin != -1)
		{
			isMatched[p.size() + iMin] = true;
			nnMatching.push_back(::make_pair(make_pair(i, iMin), min));
		}
	}
	int nnCount = 0;
	for (vector< pair<pair<int, int>, double> >::iterator it = nnMatching.begin(); it != nnMatching.end(); ++it)
	{
		if (it->second > nnMax)
			nnMax = it->second;
		if (it->first.first < pSize)
			nnCount++;
		if (it->first.second < qSize)
			nnCount++;
	}
	if (nnCount == pSize + qSize)
	{
		return nnMax;
	}

	//between pi and wi
	for (int i = 0; i < pSize; i++)
	{
		for (int j = qSize; j < q.size(); j++)
		{
			if (i == j - qSize)
			{
				distance[i * p.size() + j] = computeDistance(p[i], q[j]);
			}
			else
				distance[i * p.size() + j] = dMax;
		}
	}
	//between qj and uj
	for (int i = pSize; i < p.size(); i++)
	{
		for (int j = 0; j < qSize; j++)
		{
			if (i - pSize == j)
			{
				distance[i * p.size() + j] = computeDistance(p[i], q[j]);
			}
			else
				distance[i * p.size() + j] = dMax;
		}
	}
	//between uj and wi, dummy points on the diagonal
	for (int i = pSize; i < p.size(); i++)
	{
		for (int j = qSize; j < q.size(); j++)
		{
			distance[i * p.size() + j] = 0;
		}
	}

	double * sortedDistance = new double[p.size() * q.size()];
	memcpy(sortedDistance, distance, p.size() * q.size() * sizeof(double));

	qsort(sortedDistance, p.size() * q.size(), sizeof(double), ::compare);
	leftVal = nnMax;
	rightVal = sortedDistance[p.size() * q.size() - 1];

	double delta;
	int low, high, middle;
	low = 0;
	high = p.size() * q.size() - 1;
	middle = (low + high) / 2;
	int BMCount = 0;
	//binary search to find the bottleneck distance
	while (middle != low)
	{
		delta = sortedDistance[middle];
		if (delta < leftVal)		//current delta is smaller than the lower bound of bottleneck distance, so there is no need to check
		{
			binaryFlag = false;
		}
		else if (delta >= rightVal)
		{
			binaryFlag = true;
		}
		else
		{
			matching.clear();
			memset(isMatched, false, (p.size() + q.size()) * sizeof(bool));
			mCount = 0;
			//constuct a bipartiteGraph
			Graph bg(1 + p.size() + q.size());
			int s = p.size() + q.size();	//add an additional source vertex s to the bipartite graph to be the source of DFS later
			for (int i = 0; i < p.size(); i++)
			{
				bg.addDirectedEdge(s, i);	//add a directed edge from s to each vertex in P
				for (int j = q.size() - 1; j >= 0; j--)
				{
					if (distance[i * p.size() + j] < delta || fabs(distance[i * p.size() + j] - delta) < this->zero)
					{
						//add a directed edge from left to right in the bipartite graph
						bg.addDirectedEdge(i, p.size() + j);
					}
				}
			}
			//use Simple Greedy Matching(SGM) to initialize matching. This can be improved with more heuristics.
			for (int i = 0; i < p.size(); i++)
			{
				int dest;
				AdjList& vertex = bg.aList[i];
				for (int k = 0; k < vertex.adjList.size(); ++k)
				{
					dest = vertex.adjList[k].dest;
					if (isMatched[dest] == false)
					{
						bg.deleteDirectedEdge(s, i);
						bg.inverseDirectedEdge(i, dest);
						matching.push_back(pair<int, int>(i, dest));
						isMatched[i] = true;
						if (i < pSize)
							mCount++;
						isMatched[dest] = true;
						if (dest >= p.size() && dest < p.size() + qSize)
							mCount++;
						break;
					}
				}
			}
			//check if the initial matching already contains all the original vertices for SGM, no need to check if using NN-Matching
			if (mCount == pSize + qSize)
			{
				binaryFlag = true;
				rightVal = delta;
			}
			else
			{			//give a bipartite graph bg and an initial matching, compute a maximum cardinality bipartite matching by DFS
				++BMCount;
				while (true)
				{
					//clear path and isVisited for each node at the beginning
					path.clear();
					for (int i = 0; i < bg.aList.size(); i++)
					{
						bg.aList[i].isVisited = false;
					}
					bool isIncreased = false;		//check if the matching is increased
					int vp = 0;
					AdjList& vertex = bg.aList[s];
					while (vp != vertex.adjList.size())
					{
						int dest = vertex.adjList[vp].dest;
						if (bg.aList[dest].isVisited)
						{
							vp += 1;
							continue;
						}
						path.push_back(dest);
						bg.aList[dest].isVisited = true;
						int result = bg.DFS(dest, path);
						if (result == 1)		//found an augmenting path
						{
							//inverse the edge s and vp so that vp won't appear next time because it is matched
							bg.deleteDirectedEdge(s, dest);
							if (isMatched[path[0]] == false && (path[0] < pSize || (path[0] >= p.size() && path[0] < p.size() + qSize)))	//found a new original vertex in the matching
							{
								isMatched[path[0]] = true;
								mCount++;
							}
							int end = path.size() - 1;
							if (isMatched[path[end]] == false && (path[end] < pSize || (path[end] >= p.size() && path[end] < p.size() + qSize)))	//found a new original vertex in the matching
							{
								isMatched[path[end]] = true;
								mCount++;
							}
							for (int i = 1; i < path.size(); i += 2)
							{
								//inverse the direction for each edge in the augmenting path
								bg.inverseDirectedEdge(path[i - 1], path[i]);
								if (i + 1 < path.size())
									bg.inverseDirectedEdge(path[i], path[i + 1]);
								vector< pair<int, int> >::iterator it;
								for (it = matching.begin(); it != matching.end(); ++it)
								{
									if (it->first == path[i - 1])
									{
										it->second = path[i];
										break;
									}
								}
								if (it == matching.end())
								{
									matching.push_back(pair<int, int>(path[i - 1], path[i]));
								}
							}
							isIncreased = true;
							//each time found an augmenting path, means finish one DFS, so clear isVisited for each node
							for (int i = 0; i < bg.aList.size(); i++)
							{
								bg.aList[i].isVisited = false;
							}
							path.clear();
							if (mCount == pSize + qSize)
							{
								break;
							}
						}
						else
						{
							if (bg.aList[dest].isVisited)
							{
								path.pop_back();
							}
							vp++;
						}
					}
					if (mCount == pSize + qSize)
					{
						binaryFlag = true;
						rightVal = delta;
						break;
					}
					if (!isIncreased)
					{
						binaryFlag = false;
						leftVal = delta;
						break;
					}
				}
			}
		}
		if (binaryFlag == true)
		{
			//find first middle which distance[middle] isn't equal to delta to accelerate binary search
			do
			{
				high = middle;
				middle = (low + high) / 2;
				if (low == middle)
					break;
			} while (delta == sortedDistance[middle]);
		}
		else
		{
			//find first middle which distance[middle] isn't equal to delta to accelerate binary search
			do
			{
				low = middle;
				middle = (low + high) / 2;
				if (low == middle)
					break;
			} while (delta == sortedDistance[middle]);
		}
	}
	double bottleneck = sortedDistance[high];
	delete[] isMatched;
	delete[] distance;
	delete[] sortedDistance;
	return bottleneck;
}

//return value: 0, ends at a matched vertex on the left, dead end; 1, found an unmatched vertex on the right, thus found an augmenting path
int Graph::DFS(int dest, vector<int> &path)
{
	if (aList[dest].adjList.empty())
	{
		if (dest < this->numV / 2)
			return 0;
		else
			return 1;
	}
	AdjList& vertex = aList[dest];
	for (int k = 0; k < vertex.adjList.size(); ++k)
	{
		int dest = vertex.adjList[k].dest;
		if (aList[dest].isVisited)
			continue;
		path.push_back(dest);
		aList[dest].isVisited = true;
		int result = DFS(dest, path);
		if (result == 1)
			return 1;
		else
		{
			if (aList[dest].isVisited)
			{
				path.pop_back();
			}
		}
	}
	return 0;
}
