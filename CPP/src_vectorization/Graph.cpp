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
	//return deltaX+deltaY; //delta L1
	return deltaX > deltaY ? deltaX : deltaY; //delta Linfty
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

Graph::Graph(int numV, double delta, int numF) : dist(numV, vector<double>(numV, 987654321)), next(numV, vector<int>(numV, -1)), mSampleIndicies(numV, -1)
{
	this->numV = numV;
	this->numF = numF;
	this->delta = delta;
	this->numE = 0;
	this->iMaximum = 0;
	this->dAllEL = 0;
	this->dAvgEL = 0;
	this->zero = 0.00000000001;
	this->iH0 = 0;
	this->dMax = std::numeric_limits<double>::max();
	this->dDiameter = 0;
	int i;
	for (i = 0; i < numV; i++)
	{
		AdjList a;
		this->aList.push_back(a);
	}
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

void Graph::heightFunc(int base)
{
	this->fMaxHeight = 0;
	int index = this->mSampleIndicies[base];
	for (int i = 0; i < this->numV; i++)
	{
		aList[i].heightVal = this->dist[index][i];
		if (aList[i].heightVal > this->fMaxHeight)
			this->fMaxHeight = aList[i].heightVal;
	}
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

	if (aList[this->sortedList[0]].heightVal == -1)
		throw runtime_error("Unconnected Graph!");

	//computeMaximumPoint
	this->iMaximum = this->sortedList[aList.size() - 1];
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
					if (this->aList[j].heightVal > this->aList[l].heightVal || fabs(this->aList[j].heightVal - this->aList[l].heightVal) < this->zero && j > l)
					{
						this->SH0.push_back(::make_pair(aList[k].heightVal, aList[l].heightVal));
						this->SH0i.push_back(::make_pair(k, l));
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
	if (aList[a].heightVal < aList[b].heightVal || ((aList[a].heightVal == aList[b].heightVal) && a < b))
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

void Graph::computeDiagram(string folder, int base)
{
	std::ofstream ofs;
	std::ifstream ifs;
	string f = PREFIX;
	f += folder;
	f += "/";
	int n = base;
	f += n / 10000 + '0';
	n = n % 10000;
	f += n / 1000 + '0';
	n = n % 1000;
	f += n / 100 + '0';
	n = n % 100;
	f += n / 10 + '0';
	n = n % 10;
	f += n + '0';
	f += ".txt";
	if (BTIMED)
	{
		ofs.open(f.c_str());
	}
	clock_t timer = clock();
	clock_t timer1, timer2, timer3, timer4, timer5, timer6;

	this->baseP = base;
	heightFunc(this->baseP);
	if (BTIMED)
	{
		timer1 = clock();
		ofs << "heightFunc: " << ((double)(timer1 - timer)) / CLOCKS_PER_SEC << "s" << endl;
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
	this->Dg.clear();
	this->Dgi.clear();
	for (int i = 0; i < this->SH0.size(); i++)
	{
		if (fabs(this->SH0[i].first - this->SH0[i].second) < CUTOFF * this->dAvgEL)
			continue;
		this->Dg.push_back(this->SH0[i]);
		this->Dgi.push_back(this->SH0i[i]);
	}
	this->Dg.push_back(std::make_pair(0, this->fMaxHeight));
	this->Dgi.push_back(std::make_pair(base, this->iMaximum));
	this->iH0 = this->Dg.size();
	for (int i = 0; i < this->EH1.size(); i++)
	{
		if (fabs(this->EH1[i].first - this->EH1[i].second) < CUTOFF * this->dAvgEL)
			continue;
		this->Dg.push_back(this->EH1[i]);
		this->Dgi.push_back(this->EH1i[i]);
	}
	if (BTIMED)
	{
		timer6 = clock();
		ofs << "make this->Dg: " << ((double)(timer6 - timer5)) / CLOCKS_PER_SEC << "s" << endl;

		timer = clock() - timer;
		ofs << "Total time: " << ((double)timer) / CLOCKS_PER_SEC << "s" << endl;
		ofs << "Diagram size: " << this->Dg.size() << endl;
		for (int i = 0; i < this->Dg.size(); i++)
		{
			ofs << this->Dg[i].first << ' ';
		}
		ofs << endl << endl << endl;
		for (int i = 0; i < this->Dg.size(); i++)
		{
			ofs << this->Dg[i].second << ' ';
		}
		ofs << endl << endl << endl;
		for (int i = 0; i < this->Dg.size(); i++)
		{
			ofs << this->Dgi[i].first << ' ';
		}
		ofs << endl << endl << endl;
		for (int i = 0; i < this->Dg.size(); i++)
		{
			ofs << this->Dgi[i].second << ' ';
		}
		ofs << endl << endl << endl;
		for (int i = 0; i < this->Dg.size(); i++)
		{
			ofs << 0 << ' ';
		}
		ofs << endl << endl << endl;
		for (int i = 0; i < this->Dg.size(); i++)
		{
			ofs << 1 << ' ';
		}
		ofs << endl;
		ofs.close();
		ofs.clear();
	}

	return;
}

double Graph::computeDiagramDistToDiagnal(int base){
	this->baseP = base;
	heightFunc(this->baseP);

	sortVertices();

	classifyVertices();

	pairingH0();

	pairingH1();

	this->Dg.clear();
	this->Dgi.clear();
	for (int i = 0; i < this->SH0.size(); i++)
	{
		if (fabs(this->SH0[i].first - this->SH0[i].second) < CUTOFF * this->dAvgEL)
			continue;
		this->Dg.push_back(this->SH0[i]);
		this->Dgi.push_back(this->SH0i[i]);
	}
	this->Dg.push_back(std::make_pair(0, this->fMaxHeight));
	this->Dgi.push_back(std::make_pair(base, this->iMaximum));
	this->iH0 = this->Dg.size();

	double distSum = 0;
	for(int i = 0; i < this->iH0; i++){
		distSum += this->Dg[i].second - this->Dg[i].first;
	}

	return distSum;
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

double Graph::computeDiameter()
{
	this->sampleBP.push_back(0);
	this->mSampleIndicies[0] = 0;

	this->computeShortestDistanceDj();
	this->sampleBP.clear();
	this->sampleBP.push_back(this->diamP);
	this->mSampleIndicies[this->diamP] = 0;

	this->computeShortestDistanceDj();
	return this->dDiameter;
}


double Graph::computePersistenceDistortionDistance(Graph& g2, string& fileResult)
{
	//cout<<"test point 1"<<endl;
	std::ofstream ofs;
	std::ifstream ifs;
	string f = PREFIX;
	f += fileResult;
	ofs.open(f.c_str());
	int p1, p2, px1, px2, py1, py2, pp1, pp2;

	double a, b, c;
	double maxX = -1, minY;
	double maxY = -1, minX;
	int sizeX = this->numV, sizeY = g2.numV;

	//sample base points  //need to modify later

	this->computeDiameter();

	g2.computeDiameter();
	//TO DO here only need sample one base point. Input from file. Use sampleBasePoints(String filename)
//	srand(time(NULL));
//	int t = rand() % this->numV;
	string t = "data/basePoint.txt";
	//cout<<"test point 2"<<endl;
	this->sampleBasePoints(t);
//	t = rand() % g2.numV;
	g2.sampleBasePoints(t);


	//cout<<"test point 3"<<endl;
	//compute shortest distance for all sample points
	this->computeShortestDistanceDj();
	g2.computeShortestDistanceDj();

	//clear vDiagram and vDiagram2
	this->vDiagram.clear();
	this->vDiagram2.clear();

	//first compute all the persistence diagrams of g1 and g2
	for (int i = 0; i < this->sampleBP.size(); i++)
	{
		this->computeDiagram("1", this->sampleBP[i]); // get diagram here, need first computeShortestDistanceDJ() for this --yanjie
		this->vDiagram.push_back(this->Dg);
		this->vDiagrami.push_back(this->Dgi);
		this->v1H0.push_back(this->iH0);
	}
	for (int j = 0; j < g2.sampleBP.size(); j++)
	{
		g2.computeDiagram("2", g2.sampleBP[j]);
		this->vDiagram2.push_back(g2.Dg);
		this->vDiagram2i.push_back(g2.Dgi);
		this->v2H0.push_back(g2.iH0);
	}
	//cout<<"test point 4"<<endl;
	double bottleNeckDistance = computeBottleneckDistance(0, 0, FTS, g2);


/************************************************************************
	int it1, it2;
	sizeX = this->sampleBP.size();
	sizeY = g2.sampleBP.size();
	vector<double> dHDArray(sizeX * sizeY, -1);

	//from X to Y
	//first compute the bottleneck distance from the first base point in X to each base point in Y
	int ii, ij;
	minY = dMax;
	it1 = 0;
	for (it2 = 0; it2 < g2.sampleBP.size(); ++it2)
	{
		int i = this->sampleBP[it1], j = g2.sampleBP[it2];
		c = this->computeBottleneckDistance(i, j, FTS, g2);
		ii = this->mSampleIndicies[i];
		ij = g2.mSampleIndicies[j];
		dHDArray[ii*sizeY + ij] = c;
		if (c < minY)
		{
			minY = c;
			pp1 = i;
			pp2 = j;
		}
	}
	maxX = minY;

	px1 = pp1;
	px2 = pp2;

	//continue to the second base point in X
	for (it1 += 1; it1 < this->sampleBP.size(); ++it1)
	{
		int i = this->sampleBP[it1];
		it1 -= 1;
		int last = this->sampleBP[it1];
		it1 += 1;
		minY = dMax;
		a = this->computeBottleneckDistance(i, last, FTF, g2);
		for (it2 = 0; it2 < g2.sampleBP.size(); ++it2)
		{
			int j = g2.sampleBP[it2];
			ii = this->mSampleIndicies[last];
			ij = g2.mSampleIndicies[j];
			b = dHDArray[ii*sizeY + ij];
			if (b < 0)		//b has not been computed, then compute c
			{
				c = this->computeBottleneckDistance(i, j, FTS, g2);
				ii = this->mSampleIndicies[i];
				ij = g2.mSampleIndicies[j];
				dHDArray[ii*sizeY + ij] = c;
				if (c < maxX || fabs(c - maxX) < this->zero)
					break;
				if (c < minY)
				{
					minY = c;
					pp1 = i;
					pp2 = j;
				}
			}
			else if (minY > a - b && minY > b - a)	//triangle inequality
			{
				if (b < this->zero)
					c = a;
				else
					c = this->computeBottleneckDistance(i, j, FTS, g2);
				ii = this->mSampleIndicies[i];
				ij = g2.mSampleIndicies[j];
				dHDArray[ii*sizeY + ij] = c;
				if (c < maxX || fabs(c - maxX) < this->zero)
					break;
				if (c < minY)
				{
					minY = c;
					pp1 = i;
					pp2 = j;
				}
			}
		}
		if (it2 < g2.sampleBP.size())
			continue;
		if (minY > maxX)
		{
			maxX = minY;
			px1 = pp1;
			px2 = pp2;
		}
	}

	//from Y to X
	for (it2 = 0; it2 < g2.sampleBP.size(); ++it2)
	{
		int j = g2.sampleBP[it2];
		int last;
		minX = dMax;
		if (it2 == 0)
		{
			a = 0;
			last = g2.sampleBP[it2];
		}
		else
		{
			it2 -= 1;
			last = g2.sampleBP[it2];
			it2 += 1;
			a = this->computeBottleneckDistance(j, last, STS, g2);
			if (a < this->zero)
				continue;
		}
		for (it1 = 0; it1 < this->sampleBP.size(); ++it1)
		{
			int i = this->sampleBP[it1];
			ii = this->mSampleIndicies[i];
			ij = g2.mSampleIndicies[last];
			b = dHDArray[ii*sizeY + ij];
			ii = this->mSampleIndicies[i];
			ij = g2.mSampleIndicies[j];
			c = dHDArray[ii*sizeY + ij];
			if (c < 0)		//c has not been computed
			{
				if (b < 0)		//b has not been computed, then compute c
				{
					c = this->computeBottleneckDistance(i, j, FTS, g2);
					ii = this->mSampleIndicies[i];
					ij = g2.mSampleIndicies[j];
					dHDArray[ii*sizeY + ij] = c;
					if (c < maxY || fabs(c - maxY) < this->zero)
						break;
					if (c < minX)
					{
						minX = c;
						pp1 = i;
						pp2 = j;
					}
				}
				else if (minX > a - b && minX > b - a)
				{
					if (b < this->zero)
						c = a;
					else
						c = this->computeBottleneckDistance(i, j, FTS, g2);
					if (c < this->zero)
						this->graphMatching[i] = j;
					ii = this->mSampleIndicies[i];
					ij = g2.mSampleIndicies[j];
					dHDArray[ii*sizeY + ij] = c;
					if (c < maxY || fabs(c - maxY) < this->zero)
						break;
					if (c < minX)
					{
						minX = c;
						pp1 = i;
						pp2 = j;
					}
				}
			}
			else
			{
				if (c < maxY || fabs(c - maxY) < this->zero)
					break;
				if (c < minX)
				{
					minX = c;
					pp1 = i;
					pp2 = j;
				}
			}
		}
		if (it1 < this->sampleBP.size())
			continue;
		if (minX > maxY)
		{
			maxY = minX;
			py1 = pp1;
			py2 = pp2;
		}
	}

	double hd;
	if (maxX > maxY)
	{
		p1 = px1;
		p2 = px2;
		hd = maxX;
	}
	else
	{
		p1 = py1;
		p2 = py2;
		hd = maxY;
	}
	//output result
	cout << "Persistence-distortion Distance: " << hd << endl;
	ofs << "Persistence-distortion Distance: " << hd << endl;
	ofs << "PD pair: " << p1 << " --- " << p2 << endl;
	ofs << "V1: " << this->numV << endl;
	ofs << "E1: " << this->numE << endl;
	ofs << "F1: " << this->numF << endl;
	ofs << "V2: " << g2.numV << endl;
	ofs << "E2: " << g2.numE << endl;
	ofs << "F2: " << g2.numF << endl;
	ofs << "Diameter of G1: " << this->dDiameter << endl;
	ofs << "Diameter of G2: " << g2.dDiameter << endl;
	ofs << "Average edge length of G1: " << this->dAvgEL << endl;
	ofs << "Average edge length of G2: " << g2.dAvgEL << endl;
	ofs << endl;
	ofs << "Graph1 Sample Base Points(" << this->sampleBP.size() << "):" << endl;
	for (int i = 0; i < this->sampleBP.size(); i++)
		ofs << this->sampleBP[i] << ' ';
	ofs << endl << endl;
	ofs << "Graph2 Sample Base Points(" << g2.sampleBP.size() << "):" << endl;
	for (int i = 0; i < g2.sampleBP.size(); i++)
		ofs << g2.sampleBP[i] << ' ';
	ofs << endl;
/**********************************************************/
	ofs.close();
	ofs.clear();

	//return hd;
	return bottleNeckDistance;
}

double Graph::getDistToDiagnalSum(){

		//sample base points  //need to modify later

		this->computeDiameter();
		//TO DO here only need sample one base point. Input from file. Use sampleBasePoints(String filename)
	//	srand(time(NULL));
	//	int t = rand() % this->numV;
		string t = "data/basePoint.txt";
		//cout<<"test point 2"<<endl;
		this->sampleBasePoints(t);
	//	t = rand() % g2.numV;


		//cout<<"test point 3"<<endl;
		//compute shortest distance for all sample points
		this->computeShortestDistanceDj();
		//clear vDiagram
		this->vDiagram.clear();
		this->vDiagram2.clear();
		return this->computeDiagramDistToDiagnal(this->sampleBP[0]); // get diagram here, need first computeShortestDistanceDJ() for this --yanjie
}

void  Graph::preparePersistentDiagram(){
	this->computeDiameter();
	// TODO here only need sample one base point. Input from file. Use sampleBasePoints(String filename)
	//string t = "data/basePoint.txt";
	//this->sampleBasePointCustom(0); // no better than original
	//compute shortest distance for all sample points
	this->computeShortestDistanceDj();
	//clear vDiagram
	this->vDiagram.clear();
	this->vDiagram2.clear();
}

void Graph::computePersistentDiagram(int base){
	this->baseP = this->sampleBP[0];
	heightFunc(this->baseP);

	sortVertices();

	classifyVertices();

	pairingH0();

	pairingH1();

	this->Dg.clear();
	this->Dgi.clear();
	for (int i = 0; i < this->SH0.size(); i++)
	{
		if (fabs(this->SH0[i].first - this->SH0[i].second) < CUTOFF * this->dAvgEL)
			continue;
		this->Dg.push_back(this->SH0[i]);
		this->Dgi.push_back(this->SH0i[i]);
	}
	this->Dg.push_back(std::make_pair(0, this->fMaxHeight));
	this->Dgi.push_back(std::make_pair(this->baseP, this->iMaximum));
	this->iH0 = this->Dg.size();

//	double distSum = 0;
//	for(int i = 0; i < this->iH0; i++){
//		distSum += this->Dg[i].second - this->Dg[i].first;
//	}
}
double Graph::getPersistentRange(){
	preparePersistentDiagram();
	computePersistentDiagram(this->sampleBP[0]);
	double range = 0;
	for(int i = 0; i < this->iH0; i++){
		range = max(this->Dg[i].first, range);
	}
	return range;
}

vector<pair<double, double> > Graph::getPersistentDiagram(){
	preparePersistentDiagram();
	computePersistentDiagram(this->sampleBP[0]);

	vector<pair<double, double> > diagramVector;
	for(int i = 0; i < this->iH0; i++){
		diagramVector.push_back(::make_pair(this->Dg[i].first, this->Dg[i].second));
	}
	return diagramVector;
}

vector< double > Graph::getPersisDiagFunGaussianVector(int vectLen, double sigma, double range){
	// TODO  Done compute vector representation by using sum of Gaussian function on each point
	preparePersistentDiagram();
	computePersistentDiagram(this->sampleBP[0]);

	vector<double> Y;
	double upBound = range;
	double lowBound = 0;
	double interval = (upBound - lowBound) / (vectLen-1);
	sigma = 2*sigma*sigma;
	for(double pos = lowBound; pos <= upBound; pos+=interval){
		double funValue = 0;
		for(int i = 0; i < this->iH0; i++){
			double alpha = this->Dg[i].second;
			double Mu = this->Dg[i].first;
			double dist = Mu-pos;
			funValue += alpha*exp(-(dist*dist) / sigma);
		}

		Y.push_back(funValue);
	}
	return Y;
}

//mode: FTF: first graph to first; FTS: first to second; STF: second to first; STS: second to second
double Graph::computeBottleneckDistance(int base1, int base2, int mode, Graph& g2)
{
	//TO DO only need H_0, which means this graph is a tree. //H_1 means circle
	int k, i1, i2;
	vector< pair<double, double> > p, q, d1, d2;
	double bn1, bn2;

	if (mode == FTS || mode == FTF)
	{
		k = this->mSampleIndicies[base1];
		p = this->vDiagram[k];
		i1 = this->v1H0[k];
	}
	else
	{
		k = g2.mSampleIndicies[base1];
		p = this->vDiagram2[k];
		i1 = this->v2H0[k];
	}
	if (mode == FTS || mode == STS)
	{
		k = g2.mSampleIndicies[base2];
		q = this->vDiagram2[k];
		i2 = this->v2H0[k];
	}
	else
	{
		k = this->mSampleIndicies[base2];
		q = this->vDiagram[k];
		i2 = this->v1H0[k];
	}
	//Bottleneck distance for H0 // here H0 is need.
	for (int i = 0; i < i1; ++i)
	{
		d1.push_back(p[i]);
	}
	for (int j = 0; j < i2; ++j)
	{
		d2.push_back(q[j]);
	}
	if (B_SUMMER_BN)
		bn1 = this->computeBottleneckDistanceDiagramSumVersion(d1, d2);
	else
		bn1 = this->computeBottleneckDistanceDiagram(d1, d2);
	//Bottleneck distance for H1 //here H1 is not needed.
//	d1.clear();
//	d2.clear();
//	for (int i = i1; i < p.size(); ++i)
//	{
//		d1.push_back(p[i]);
//	}
//	for (int j = i2; j < q.size(); ++j)
//	{
//		d2.push_back(q[j]);
//	}
//	if (B_SUMMER_BN)
//		bn2 = this->computeBottleneckDistanceDiagramSumVersion(d1, d2);
//	else
//		bn2 = this->computeBottleneckDistanceDiagram(d1, d2);
//	return bn1 > bn2 ? bn1 : bn2;
	return bn1;
}

double Graph::computeBottleneckDistanceDiagramSumVersion(vector< pair<double, double> >& p, vector< pair<double, double> >& q)
{
	int pSize = p.size();
	int qSize = q.size();
	pair<double, double> ii;
	//ui: dummy points corresponding to qi
	for (int i = 0; i < qSize; i++)
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

	size_t i, j;
	size_t n = p.size(), m = q.size();
	cell** cost = new cell*[n];
	for (int i = 0; i < n; ++i)
		cost[i] = new cell[m];
	cell** table = new cell*[n];
	for (int i = 0; i < n; ++i)
		table[i] = new cell[m];

	for (int i = 0; i < pSize; i++)
	{
		for (int j = 0; j < qSize; ++j)
		{
			table[i][j] = cost[i][j] = computeDistance(p[i], q[j]);
		}
	}
	//between pi and wi
	for (int i = 0; i < pSize; i++)
	{
		for (int j = qSize; j < q.size(); j++)
		{
			if (i == j - qSize)
			{
				table[i][j] = cost[i][j] = computeDistance(p[i], q[j]);
			}
			else
				table[i][j] = cost[i][j] = dMax;
		}
	}
	//between qj and uj
	for (int i = pSize; i < p.size(); i++)
	{
		for (int j = 0; j < qSize; j++)
		{
			if (i - pSize == j)
			{
				table[i][j] = cost[i][j] = computeDistance(p[i], q[j]);
			}
			else
				table[i][j] = cost[i][j] = dMax;
		}
	}
	//between uj and wi, dummy points on the diagonal
	for (int i = pSize; i < p.size(); i++)
	{
		for (int j = qSize; j < q.size(); j++)
		{
			table[i][j] = cost[i][j] = 0;
		}
	}

	ssize_t** assignment = kuhn_match(table, n, m);

	double bnDistance = 0;
	for (int i = 0; i < n; ++i)
	{
		int ai = assignment[i][0];
		int aj = assignment[i][1];
		if (ai < pSize)
		{
			bnDistance += (double)cost[ai][aj];
		}
		else if (aj < qSize)
		{
			bnDistance += (double)cost[ai][aj];
		}
	}

    for (i = 0; i < n; i++)
    {
	free(*(assignment + i));
	free(*(table + i));
	free(*(cost + i));
    }
    	free(assignment);
    	free(table);
    	free(cost);
	if (bnDistance < this->zero)
		bnDistance = 0;
	return bnDistance;
}

/**
 *	this function is the max version of compute bottleneck distance
 */
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

void Graph::sampleBasePoints()
{
	for (int i = 0; i < this->numV; i++)
	{
		this->sampleBP.push_back(i);
		this->mSampleIndicies[i] = i;
	}
}

void Graph::sampleBasePointsWODeg2()
{
	for (int i = 0; i < this->numV; i++)
	{
		if (aList[i].adjList.size() > 2)
		{
			this->sampleBP.push_back(i);
			this->mSampleIndicies[i] = this->sampleBP.size() - 1;
		}
	}
}

vector<int> Graph::computeNeighborInRange(int base)
{
	vector<int> vNIR;
	boost::heap::fibonacci_heap< node, boost::heap::compare<mycomparison> > q;
	boost::heap::fibonacci_heap< node, boost::heap::compare<mycomparison> >::handle_type handle;
	vector<boost::heap::fibonacci_heap< node, boost::heap::compare<mycomparison> >::handle_type> vecIH(numV);
	vector<double> dist(numV, dMax);
	vector<bool> isInQ(numV, false);
	vecIH[base] = q.push(node(base, 0));
	dist[base] = 0;
	isInQ[base] = true;
	int i;
	while (!q.empty())
	{
		//make sure to call pop function right after top function because update fibbq/pq may change the top element
		node p = q.top();
		q.pop();
		i = p.id;
		if (dist[i] / this->dDiameter < delta)
		{
			vNIR.push_back(i);
			AdjList& vertex = aList[i];
			for (int k = 0; k < vertex.adjList.size(); ++k)
			{
				int j = vertex.adjList[k].dest;
				double edgeWeight = vertex.adjList[k].edgeWeight;
				double newEdgeWeight = dist[i] + edgeWeight;
				if (newEdgeWeight < dist[j])
				{
					if (isInQ[j])
					{
						handle = vecIH[j];
						(*handle).weight = newEdgeWeight;
						q.decrease(handle);
					}
					else
					{
						vecIH[j] = q.push(node(j, newEdgeWeight));
						isInQ[j] = true;
					}
					dist[j] = dist[i] + edgeWeight;
				}
			}
		}
		else
			break;
	}
	return vNIR;
}

void Graph::sampleBasePoints(int b)
{
	set<int> indicies;
	for (int i = 0; i < this->numV; i++)
		indicies.insert(i);
	int base = b;

	srand(time(NULL));

	while (!indicies.empty())
	{
		this->sampleBP.push_back(base);
		this->mSampleIndicies[base] = this->sampleBP.size() - 1;
		vector<int> vNIR = computeNeighborInRange(base);
		for (int i = 0; i < vNIR.size(); ++i)
		{
			indicies.erase(vNIR[i]);
		}

		if (!indicies.empty())
		{
			int t = rand() % indicies.size();
			set<int>::iterator it;
			for (it = indicies.begin(); it != indicies.end(); it++, t--)
			{
				if (t == 0)
				{
					base = (*it);
					break;
				}
			}
			if (it == indicies.end())
				base = (*indicies.rbegin());
		}
		else
			break;
	}
}

//This is needed. file contains the index of base points
void Graph::sampleBasePoints(string filename)
{
	ifstream ifs(filename.c_str());
	if (!ifs.is_open())
		cout << "can't open sample points file" << endl;
	int num;
	int index;
	ifs >> num;
	while (num--)
	{
		cout << "in sample base points" << endl;
		ifs >> index;
		this->sampleBP.push_back(index);
		this->mSampleIndicies[index] = this->sampleBP.size() - 1;
	}
	cout << "base point size: " << this->sampleBP.size() << endl;
	ifs.close();
	ifs.clear();
}

void Graph::sampleBasePointCustom(int b)
{
	this->sampleBP.clear();
	this->mSampleIndicies.clear();
	this->sampleBP.push_back(b);
	this->mSampleIndicies[b] = this->sampleBP.size() - 1;
}

void Graph::computeShortestDistanceDj()
{
	boost::heap::fibonacci_heap< node, boost::heap::compare<mycomparison> > q;
	boost::heap::fibonacci_heap< node, boost::heap::compare<mycomparison> >::handle_type handle;
	vector<boost::heap::fibonacci_heap< node, boost::heap::compare<mycomparison> >::handle_type> vecIH(numV);

	for (int k = 0; k < this->sampleBP.size(); ++k)
	{
		q.clear();
		vector<int> isInQ(numV, 0);		//{0,1,2}
		int p = this->sampleBP[k];
		dist[k][p] = 0;
		vecIH[p] = q.push(node(p, 0));
		isInQ[p] = 1;
		int count = numV;
		int i;

		while (true)
		{
			//make sure to call pop function right after top function because update fibbq/pq may change the top element

			node p = q.top();
			q.pop();
			count -= 1;
			i = p.id;
			dist[k][i] = p.weight;
			isInQ[i] = 2;
			if (count == 0)
			{
				if (p.weight > this->dDiameter)
				{
					this->dDiameter = p.weight;
					this->diamP = p.id;
				}

				break;
			}

			AdjList& vertex = aList[i];

			for (int m = 0; m < vertex.adjList.size(); ++m)
			{

				int j = vertex.adjList[m].dest;
				if (isInQ[j] != 2)
				{
					double edgeWeight = vertex.adjList[m].edgeWeight;
					double newEdgeWeight = dist[k][i] + edgeWeight;
					if (isInQ[j] == 1)
					{
						handle = vecIH[j];
						if (newEdgeWeight < (*handle).weight)
						{
							(*handle).weight = newEdgeWeight;
							q.decrease(handle);
						}
					}
					else
					{
						vecIH[j] = q.push(node(j, newEdgeWeight));
						isInQ[j] = true;
					}
				}
			}

			if (count > 0 && count < numV - 1 && q.empty())
			{
				cout << "not connected!" << endl;
				exit(-1);
			}

		}
	}

	//resize
	if (BNORMALIZE)
		for (int k = 0; k < this->sampleBP.size(); ++k)
		{
		for (int p = 0; p < numV; ++p)
		{
			dist[k][p] /= this->dDiameter;
		}
		}
	//compute average edge length
	if (BNORMALIZE)
		this->dAvgEL = this->dAllEL / this->numE / this->dDiameter;
	else
		this->dAvgEL = this->dAllEL / this->numE;
	//compute this->zero
	this->zero = this->dAvgEL / 1000000;
}

