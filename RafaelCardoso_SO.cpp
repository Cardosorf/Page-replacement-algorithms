/*
	Developed by Rafael Pais Cardoso
	How to Compile:
		Compile using this command: g++ RafaelCardoso_SO.cpp -o main -O3
	How to Run:
		Use the following commands to run:
			./main 4 vsim-exemplo.txt
			./main 4 vsim-belady.txt
			./main 64 vsim-gcc.txt
			./main 256 vsim-gcc.txt
			./main 1024 vsim-gcc.txt
			./main 4096 vsim-gcc.txt
*/

#include <iostream>
#include <stdlib.h> 
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <fstream>

using namespace std;

void readFileAndInitVector(vector<int>& pagesVector, const string& fileName) {

	int value;
	ifstream myFile(fileName);

	if (!myFile.is_open()) {
		cerr << "Unable to open file: " << fileName << endl;
		exit(1);
	} else {
		while (myFile >> value) {
			pagesVector.push_back(value);
		}
	}

	myFile.close();
}

int FIFO_Algorithm(const vector<int>& pagesVector, int framesNumber) {
	vector<int> frameArray;
	vector<int>::iterator it;
	int positionframeArray = 0;
	int pageFault = 0;

	frameArray.assign(framesNumber, -1);


	for (int i = 0; i < pagesVector.size(); i++) {
		it = find(frameArray.begin(), frameArray.end(), pagesVector[i]);

		//Se a p�gina n�o se encontra no quadro, a nova p�gina � colocada na posi��o determinada pela positionframeArray e acrescenta-se uma unidade ao contator de falta de p�gina
		//A vari�vel positionframeArray � acrescida em uma unidade e o m�dulo � feito (para que ela s� varia entre 0 e o tamanho do quadro)
		if (it == frameArray.end()) {
			frameArray[positionframeArray] = pagesVector[i];
			positionframeArray = (positionframeArray + 1) % frameArray.size();
			pageFault++;
		}
	}

	return pageFault;
}


int LRU_Algorithm(const vector<int>& pagesVector, int framesNumber) {
	vector<int> frameArray;
	vector<int> frameTimeArray;
	vector<int>::iterator it;
	int maxTime;
	int positionFrameArray;
	int pageFault = 0;

	frameArray.assign(framesNumber, -1);
	frameTimeArray.assign(framesNumber, 0);

	for (int i = 0; i < pagesVector.size(); i++) {
		maxTime = -1;
		positionFrameArray = -1;

		//Verifica se a p�gina se encontra ou n�o nos quadros
		it = find(frameArray.begin(), frameArray.end(), pagesVector[i]);
		if (it == frameArray.end()) {

			//A p�gina n�o se encontra nos quadros.
			//Verifica se os quadros est�o com espa�os vazios 
			it = find(frameArray.begin(), frameArray.end(), -1);
			if (*it == -1) {
				//Coloca a nova p�gina no espa�o vazio do quadro
				*it = pagesVector[i];

				//Envelhece as p�ginas que j� estavam no quadro e a nova p�gina fica no tempo zero
				for (int j = 0; j < frameTimeArray.size(); j++) {
					if (frameArray[j] != pagesVector[i]) {
						frameTimeArray[j]++;
					}
					else {
						frameTimeArray[j] = 0;
					}
				}

				//Acrescenta-se uma unidade ao contator de falta de p�gina
				pageFault++;
				continue;
			}

			//Busca a p�gina do quadro que tem o maior tempo para ser substutu�da
			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (maxTime < frameTimeArray[j]) {
					maxTime = frameTimeArray[j];
					positionFrameArray = j;
				}
			}

			////Acrescenta-se uma unidade ao contator de falta de p�gina, substitui a p�gina de maior tempo pela nova p�gina e ajust ao seu tempo para zero.
			pageFault++;
			frameArray[positionFrameArray] = pagesVector[i];
			frameTimeArray[positionFrameArray] = 0;

			//Envelhece as p�ginas que j� estavam no quadro
			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (frameArray[j] != pagesVector[i]) {
					frameTimeArray[j]++;
				}
			}

		} 

		//J� existe a p�gina no quadro. Envelhece as demais e ajusta para o tempo zero essa p�gina
		else {
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

	return pageFault;
}


int OPT_Algorithm(vector<int>& pagesVector, int framesNumber) {
	vector<int> frameArray;
	vector<int>::iterator it;
	vector<int>::iterator it2;
	int dist;
	bool canEnter;
	int timeCounter = 0;
	int maxTime;
	int positionFrameArray;
	int pageFault = 0;

	frameArray.assign(framesNumber, -1);

	for (int i = 0; i < pagesVector.size(); i++) {

		it = find(frameArray.begin(), frameArray.end(), pagesVector[i]);

		//Se a nova p�gina n�o existir nos quadros, ser� verificado se o quadro ainda h� espa�os vazios
		if (it == frameArray.end()) {
			it = find(frameArray.begin(), frameArray.end(), -1);
			
			//Se o quadro tiver espa�os vazios, a nova p�gina ser� inserida nesse espa�o e o contator de falta de p�ginas ser� ascrescido em 1 unidade, assim como o contador de tempo total
			if (*it == -1) {
				*it = pagesVector[i];
				pageFault++;
				timeCounter++;
				continue;
			}

			for (int j = 0; j < frameArray.size(); j++) {
				it2 = find(pagesVector.begin() + timeCounter, pagesVector.end(), frameArray[j]);

				//Se a p�gina que est� no quadro n�o ir� aparecer mais como uma nova p�gina a ser inserida, ent�o essa sai do quadro e a nova p�gina � colocada em seu lugar
				if (it2 == pagesVector.end()) {
					frameArray[j] = pagesVector[i];
					canEnter = false;
					break;
				}

				//Busca-se pela p�gina que ir� demorar mais tempo a aparecer para ser inserida no futuro
				else {
					canEnter = true;
					int tempDist = distance(pagesVector.begin() + timeCounter, it2);
					if (tempDist > dist) {
						dist = tempDist;
						positionFrameArray = j;
					}
				}
			}

			//Assim, insere-se a nova p�gina, no quadro, no local da p�gina que demorar� mais tempo a ser inserida no futuro
			if (canEnter) {
				frameArray[positionFrameArray] = pagesVector[i];
				dist = -1;
				canEnter = false;
			}

			//Contator de falta de p�ginas � ascrescido em 1 unidade
			pageFault++;
		}

		//Contator de tempo total � ascrescido em 1 unidade
		timeCounter++;

	}

	return pageFault;
}



int main(int argc, char *argv[]) {
	string fileName;
	vector<int> pagesVector;
	int framesNumber;
	int numberPageFault_FIFO;
	int numberPageFault_LRU;
	int numberPageFault_OPT;

	if (argc > 2) {
		 fileName = argv[2];
		 framesNumber = atoi(argv[1]);
	} else {
		cout << "Missing number of frames or file name." << endl;
		exit(1);
	}

	readFileAndInitVector(pagesVector, fileName);

	numberPageFault_FIFO = FIFO_Algorithm(pagesVector, framesNumber);
	numberPageFault_LRU = LRU_Algorithm(pagesVector, framesNumber);
	numberPageFault_OPT = OPT_Algorithm(pagesVector, framesNumber);

	cout << endl;
	cout << framesNumber << " frames, " << "refs: " << pagesVector.size() << " FIFO: " <<  numberPageFault_FIFO  << " LRU: " << numberPageFault_LRU << " OPT: " << numberPageFault_OPT << endl;
	cout << endl;

	return 0;
}