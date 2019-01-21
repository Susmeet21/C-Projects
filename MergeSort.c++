//Threaded MergeSort
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
using namespace std;

void merge_sort(vector<int> &V, int i, int j); //sort elements of vector V from position i to position j
void merge(vector<int> &V, int i1, int i2, int j1, int j2); //Merge vector elments from position i1 to position i2 with vector elements
															//from position j1 to position j2.
								//Note that elements from i1 to i2 are already sorted, and elements from j1 to j2 are
								//also sorted already.



int main() {
	int n = 100000;
	vector<int> V(n);//V has n elements.
	for (int i = 0; i < 100000; i++) V[i] = rand() % 5000;
	//You need to crate four threads to perform sorting
	//You need to then save the sorted result to output.txt
	
	thread t1(merge_sort,std::ref(V),0,24999);
	thread t2(merge_sort,std::ref(V),25000,49999);
	thread t3(merge_sort,std::ref(V),50000,74999);
	thread t4(merge_sort,std::ref(V),75000,99999);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	merge(V, 0, 24999, 25000, 49999);
	merge(V, 50000, 74999, 75000, 99999);
	merge(V, 0, 49999, 50000, 99999);

	ofstream out1("HW_6output.txt");
		for (int q = 0; q < 100000; q++)
		{
			out1 << V[q] << "  ";
		}
		out1.close();
	return 0;
}
void merge(vector<int> &V, int i1, int i2, int j1, int j2) {
	int i = i1;
	int j = j1;
	vector<int> Res;
	while (i <= i2 && j <= j2) {

		if (V[i] < V[j]) 
		{ 
			Res.push_back(V[i]);
			i++;
		}
		else
		{
			Res.push_back(V[j]);
			j++; }
	}
	while (i <= i2) {
		Res.push_back(V[i]);
		i++;
	}
	
	while (j <= j2) {
		Res.push_back(V[j]);
		j++;
	}
	for (int k = 0; k < Res.size(); k++) {
		V[k + i1] = Res[k];

	}
}
void merge_sort(vector<int> &V, int i, int j) {

	if (i < j) {
		int mid_point = (i + j) / 2;
		merge_sort(V, i, mid_point);
		merge_sort(V, mid_point + 1, j);
		merge(V, i, mid_point, mid_point + 1, j);
	}

}
