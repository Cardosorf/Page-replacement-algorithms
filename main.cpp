#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
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
	array<int, 4> frameArray = {-1,-1,-1,-1};
	array<int, 4>::iterator it;

	int positionframeArray = 0;
	int pageFault = 0;

	for (int i = 0; i < pagesVector.size(); i++) {
		it = find(frameArray.begin(), frameArray.end(), pagesVector[i]);
		if (it == frameArray.end()) {
			frameArray[positionframeArray] = pagesVector[i];
			positionframeArray = (positionframeArray + 1) % frameArray.size();
			pageFault++;
		}
	 /*
		for (int j = 0; j < frameArray.size(); j++) {
			cout << frameArray[j] << " ";
		}
		cout << endl;
	*/
	}
	
	cout << pageFault << endl;
}


void LRU_Algorithm(const vector<int>& pagesVector) {
	array<int, 4> frameArray = { -1,-1,-1,-1};
	array<int, 4>::iterator it;
	array<int, 4> frameTimeArray = { 0,0,0,0 };
	int max;
	int positionFrameArray;
	int pageFault = 0;

	for (int i = 0; i < frameArray.size(); i++) {
		frameArray[i] = pagesVector[i];
		pageFault++;
		for (int j = 0; j <= i; j++) {
				frameTimeArray[j]++;
		}

	}

	for (int i = frameArray.size(); i < pagesVector.size(); i++) {
		max = -1;
		positionFrameArray = -1;

		it = find(frameArray.begin(), frameArray.end(), pagesVector[i]);
		if (it == frameArray.end()) {
			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (max < frameTimeArray[j]) {
					max = frameTimeArray[j];
					positionFrameArray = j;
				}
			}
			pageFault++;
			frameArray[positionFrameArray] = pagesVector[i];
			frameTimeArray[positionFrameArray] = 0;

			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (frameArray[j] != pagesVector[i]) {
					frameTimeArray[j]++;
				}

			}

		} else {
			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (frameArray[j] != pagesVector[i]) {
					frameTimeArray[j]++;
				}
				else {
					frameTimeArray[j] = 0;
				}
			}

		}

	}

	cout << pageFault << endl;

}


int main() {

	vector<int> pagesVector;

	readFileAndInitVector(pagesVector);

	FIFO_Algorithm(pagesVector);
	
	LRU_Algorithm(pagesVector);

	/*
	for (int i = 0; i < pagesVector.size(); i++) {
		cout << pagesVector[i] << endl;
	}
	*/

	return 0;
}
