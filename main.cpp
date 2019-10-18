#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>

using namespace std;

void readFileAndInitVector(vector<int>& pagesVector) {

	int value;
	ifstream myFile("entrada.txt");

	if (!myFile.is_open()) {
		cerr << "Unable to open file entrada.txt";
		exit(1);

	} else {
		while (myFile >> value) {
			pagesVector.push_back(value);
		}
	}

	myFile.close();

}


void FIFO_Algorithm(const vector<int>& pagesVector) {
	array<int, 4> frameVector = {-1,-1,-1,-1};
	array<int, 4>::iterator it;

	int positionFrameVector = 0;
	int pageFault = 0;

	for (int i = 0; i < pagesVector.size(); i++) {
		it = find(frameVector.begin(), frameVector.end(), pagesVector[i]);
		if (it == frameVector.end()) {
			frameVector[positionFrameVector] = pagesVector[i];
			positionFrameVector = (positionFrameVector + 1) % 4;
			pageFault++;
		}
	 /*
		for (int j = 0; j < frameVector.size(); j++) {
			cout << frameVector[j] << " ";
		}
		cout << endl;
	*/
	}
	

	cout << pageFault << endl;
}



int main() {

	vector<int> pagesVector;

	readFileAndInitVector(pagesVector);

	FIFO_Algorithm(pagesVector);

	/*
	for (int i = 0; i < pagesVector.size(); i++) {
		cout << pagesVector[i] << endl;
	}
	*/

	return 0;
}