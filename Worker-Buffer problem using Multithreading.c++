#include <iostream> 
#include <vector> 
#include <thread> 
#include<random> 
#include <mutex> 
#include <condition_variable> 
using namespace std;

vector<int> BufferState = { 0,0,0,0 };
mutex m1;
condition_variable_any waitForSpace;
condition_variable_any waitForParts;

void PrintUpdatedPlaceRequest(vector<int>& v, int partWorkerId) {
	cout << "Updated Buffer State: " << "(" << BufferState[0] << ", " << BufferState[1] << ", " << BufferState[2] << ", " << BufferState[3] << ")" << endl;
	cout << "Updated Place Request: " << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << endl;
}

void PrintPlaceRequest(vector<int>& v, int partWorkerId, int it) {
	cout << endl << "Part Worker ID: " << partWorkerId << endl;
	cout << "Iteration: " << it << endl;
	cout << "Buffer State: " << "(" << BufferState[0] << ", " << BufferState[1] << ", " << BufferState[2] << ", " << BufferState[3] << ")" << endl;
	cout << "Place Request: " << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << endl;
}

void PrintUpdatedPickupRequest(vector<int>& v, int productWorkerId) {
	cout << "Updated Buffer State: " << "(" << BufferState[0] << ", " << BufferState[1] << ", " << BufferState[2] << ", " << BufferState[3] << ")" << endl;
	cout << "Updated Pickup Request: " << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << endl;
}

void PrintPickupRequest(vector<int>& v, int productWorkerId, int iteration) {
	cout << endl << "Product Worker ID: " << productWorkerId << endl;
	cout << "Iteration: " << iteration << endl;
	cout << "Buffer State: " << "(" << BufferState[0] << ", " << BufferState[1] << ", " << BufferState[2] << ", " << BufferState[3] << ")" << endl;
	cout << "Pickup Request: " << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")" << endl;
}



void PlaceRequest(vector<int>& Pl, int partWorkerId, int it) {
	m1.lock();
	
	bool retryCase1 = false;
	bool retryCase2 = false;
	bool retryCase3 = false;
	bool retryCase4 = false;
	int spaceIn1;
	int spaceIn2;
	int spaceIn3;
	int spaceIn4;
	while (Pl[0] != 0 || Pl[1] != 0 || Pl[2] != 0 || Pl[3] != 0 || retryCase1 || retryCase2 || retryCase3 || retryCase4) {

		PrintPlaceRequest(Pl, partWorkerId, it);
		if (BufferState[0] < 6 && Pl[0] != 0)
		{
			spaceIn1 = 6 - BufferState[0];
			
			if (spaceIn1 >= Pl[0]) {
				BufferState[0] = BufferState[0] + Pl[0];
				Pl[0] = 0;
				spaceIn1 = 6 - BufferState[0];
				retryCase1 = false;

			}
			else
			{
				Pl[0] = Pl[0] - spaceIn1;
				BufferState[0] = 6;
				spaceIn1 = 0;
				retryCase1 = true;
			}
		}

		if (BufferState[1] < 5 && Pl[1] != 0) {
			spaceIn2 = 5 - BufferState[1];
			
			if (spaceIn2 >= Pl[1]) {
				BufferState[1] = BufferState[1] + Pl[1];
				spaceIn2 = 5 - BufferState[1];
				Pl[1] = 0;
				retryCase2 = false;
			}
			else 
			{
				Pl[1] = Pl[1] - spaceIn2;
				spaceIn2 = 0;
				BufferState[1] = 5;
				retryCase2 = true;
			}
		}

		if (BufferState[2] < 4 && Pl[2] != 0) {
			spaceIn3 = 4 - BufferState[2];
			
			if (spaceIn3 >= Pl[2]) {
				BufferState[2] = BufferState[2] + Pl[2];
				Pl[2] = 0;
				spaceIn3 = 4 - BufferState[2];
				retryCase3 = false;

			}
			else 
			{
				Pl[2] = Pl[2] - spaceIn3;
				BufferState[2] = 4;
				spaceIn3 = 0;
				retryCase3 = true;
			}
		}

		if (BufferState[3] < 3 && Pl[3] != 0) {
			spaceIn4 = 3 - BufferState[3];
			
			if (spaceIn4 >= Pl[3]) {
				BufferState[3] = BufferState[3] + Pl[3];
				Pl[3] = 0;
				spaceIn4 = 3 - BufferState[3];
				retryCase4 = false;
			}
			else
			{
				Pl[3] = Pl[3] - spaceIn4;
				spaceIn4 = 0;
				BufferState[3] = 3;
				retryCase4 = true;
			}
		}

		PrintUpdatedPlaceRequest(Pl, partWorkerId);

		if (retryCase1 || retryCase2 || retryCase3 || retryCase4) {
			cout << "DEADLOCK" << endl;
			cout << "ABORT" << it << endl;
			
			waitForSpace.wait(m1);
		}

	}
	waitForParts.notify_all(); m1.unlock();

}



void PickupRequest(vector<int>& T, int productWorkerId, int it) {
	m1.lock();
	bool needFrom1 = false;
	bool needFrom2 = false;
	bool needFrom3 = false;
	bool needFrom4 = false;

	PrintPickupRequest(T, productWorkerId, it);

	while (needFrom1 || needFrom2 || needFrom3 || needFrom4) {
		if (T[0] != 0) {

			if (BufferState[0] >= T[0]) {
				BufferState[0] = BufferState[0] - T[0];
				T[0] = 0;
				needFrom1 = false;
			}
			else
			{
				T[0] = T[0] - BufferState[0];
				BufferState[0] = 0;
				needFrom1 = true;
			}
		}

		if (T[1] != 0) {
			
			if (BufferState[1] >= T[1]) {
				BufferState[1] = BufferState[1] - T[1];
				T[1] = 0;
				needFrom2 = false;
			}
			else
			{
				T[1] = T[1] - BufferState[1];
				BufferState[1] = 0;
				needFrom2 = true;
			}
		}

		if (T[2] != 0) {
			
			if (BufferState[2] >= T[2]) {
				BufferState[2] = BufferState[2] - T[2];
				T[2] = 0;
				needFrom3 = false;
			}
			else 
			{
				T[2] = T[2] - BufferState[2];
				BufferState[2] = 0;
				needFrom3 = true;
			}
		}

		if (T[3] != 0) {
			
			if (BufferState[3] >= T[3]) {
				BufferState[3] = BufferState[3] - T[3];
				T[3] = 0;
				needFrom4 = false;
			}
			else 
			{
				T[3] = T[3] - BufferState[3];
				BufferState[3] = 0;
				needFrom4 = true;
			}
		}

		PrintUpdatedPickupRequest(T, productWorkerId);

		if (needFrom1 || needFrom2 || needFrom3 || needFrom4) {
			cout << "DEADLOCK" << endl;
			cout << "ABORT" << it << endl;
			waitForParts.wait(m1);
		}
	}
	waitForSpace.notify_all();
	m1.unlock();
}
vector<int> PartWorker(int i) {
	int id = i + 1;
	int it = 1;
	random_device random;
	uniform_int_distribution<> zeroToFour(0, 4);
	uniform_int_distribution<> zeroToNine(0, 9);
	vector<int> v;
	
	for (int j = 0; j < 1; j++) {
		int maxVal = 4;
		int first, second, third, fourth;
		
		first = zeroToFour(random);
		second = zeroToNine(random) % (maxVal + 1 - first);
		third = maxVal - first - second;
		fourth = 0;
		
		v = { first, second, third, fourth };
		random_shuffle(v.begin(), v.end());
		
		PlaceRequest(v, id, it);
		it++;
	}
	return v;

}
vector<int> ProductWorker(int i) {
	int id = i + 1;
	int it = 1;
	vector<int> v;
	for (int j = 0; j < 5; j++) {
		bool invalid = true;
		
		while (invalid) {
			int maxVal = 5;
			int first, second, third, fourth;
			
			first = rand() % (maxVal + 1);
			second = rand() % (maxVal + 1 - first);
			third = maxVal - first - second;
			fourth = 0;
			v = { first, second, third, fourth };

			random_shuffle(v.begin(), v.end());
			
			if (count(v.begin(), v.end(), 0) == 1) {
				invalid = true;
			}
			else
			{
				invalid = false;
			}
		}
		PlaceRequest(v, id, it);
		it++;
	}
	return v;
}

int main() {
	const int m = 20, n = 16; //m: number of Part Workers 
	//n: number of Product Workers 
	//m>n 
	thread partW[m];
	thread prodW[n];
	for (int i = 0; i < n; i++) {
		partW[i] = thread(PartWorker, i);
		prodW[i] = thread(ProductWorker, i);
	}
	for (int i = n; i < m; i++) {
		partW[i] = thread(PartWorker, i);
	}
	/*Join the threads to the main threads */
	for (int i = 0; i < n; i++) {
		partW[i].join();
		prodW[i].join();
	}
	for (int i = n; i < m; i++) {
		partW[i].join();
	}
	cout << "Finish!" << endl;
	getchar();
	getchar();
	return 0;
}
