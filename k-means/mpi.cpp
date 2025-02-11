#include <iostream> 
#include <vector> 
#include <math.h> 
#include <algorithm> 
#include "Point.h" 
#include "Node.h" 
#include <stddef.h> 
#include <mpi.h> 
#include <fstream> 
#include <sstream> 
#define MAX_DIM 100 using namespace std;
int main (int argc, char *argv[]) 
{ 
	srand(time (NULL)); 
	int numNodes, rank;
	const int tag = 13; 
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &numNodes);
 
	int total_values; 
	int total_points;
	int K, max_iterations; 
	int lastIteration; 
	vector<Point> dataset;
	Node node (rank, MPI_COMM_WORLD); 
	node.createDataset();
	node.readDataset(); 
	node.scatterDataset(); 
	node.extractCluster(); 
	lastIteration = 0;
	for (int it = 0; it < node.getMaxIterations(); it++) { 
		if (rank == 0) {
			cout << "Iteration: "; 
			if (it % 3 == 0) {
				cout << "/ " << it << "\r" << flush;
			} 
			else if(it % 3 == 1){ 
				cout << "- " << it << "\r" << flush;
				else { 
				} 
 
			} 
			cout << "\\ " << it << "\r" << flush;
		} 
 
		int notChanged = node.run(it); 
		if(rank == 0){
		} 
		if(notChanged == numNodes){ 
			lastIteration = it;
			break;
		} 
		lastIteration = it;
	} 
 
	node.setLastIteration(lastIteration); 
	node.computeGlobalMembership(); 
	if (rank == 0){
		int* gm;
		gm = node.getGlobalMemberships(); 
		int numPoints = node.getNumPoints(); 
		string doWrite;
		cout << "Do you want to save points membership? (y/n)" << endl; 
		getline(cin, doWrite);
		if(doWrite == "y") { 
			string outFilename = "membershipsFilename"; 
			cout << "Specify output filename: \n"; 
			getline(cin, outFilename);
			node.writeClusterMembership(outFilename);
		} 
	} 
	node.getStatistics(); 
	MPI_Finalize();
}
