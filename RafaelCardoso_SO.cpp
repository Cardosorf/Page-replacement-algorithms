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

		//Se a página não se encontra no quadro, a nova página é colocada na posição determinada pela positionframeArray e acrescenta-se uma unidade ao contator de falta de página
		//A variável positionframeArray é acrescida em uma unidade e o módulo é feito (para que ela só varia entre 0 e o tamanho do quadro)
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

		//Verifica se a página se encontra ou não nos quadros
		it = find(frameArray.begin(), frameArray.end(), pagesVector[i]);
		if (it == frameArray.end()) {

			//A página não se encontra nos quadros.
			//Verifica se os quadros estão com espaços vazios 
			it = find(frameArray.begin(), frameArray.end(), -1);
			if (*it == -1) {
				//Coloca a nova página no espaço vazio do quadro
				*it = pagesVector[i];

				//Envelhece as páginas que já estavam no quadro e a nova página fica no tempo zero
				for (int j = 0; j < frameTimeArray.size(); j++) {
					if (frameArray[j] != pagesVector[i]) {
						frameTimeArray[j]++;
					}
					else {
						frameTimeArray[j] = 0;
					}
				}

				//Acrescenta-se uma unidade ao contator de falta de página
				pageFault++;
				continue;
			}

			//Busca a página do quadro que tem o maior tempo para ser substutuída
			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (maxTime < frameTimeArray[j]) {
					maxTime = frameTimeArray[j];
					positionFrameArray = j;
				}
			}

			////Acrescenta-se uma unidade ao contator de falta de página, substitui a página de maior tempo pela nova página e ajust ao seu tempo para zero.
			pageFault++;
			frameArray[positionFrameArray] = pagesVector[i];
			frameTimeArray[positionFrameArray] = 0;

			//Envelhece as páginas que já estavam no quadro
			for (int j = 0; j < frameTimeArray.size(); j++) {
				if (frameArray[j] != pagesVector[i]) {
					frameTimeArray[j]++;
				}
			}

		} 

		//Já existe a página no quadro. Envelhece as demais e ajusta para o tempo zero essa página
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

		//Se a nova página não existir nos quadros, será verificado se o quadro ainda há espaços vazios
		if (it == frameArray.end()) {
			it = find(frameArray.begin(), frameArray.end(), -1);
			
			//Se o quadro tiver espaços vazios, a nova página será inserida nesse espaço e o contator de falta de páginas será ascrescido em 1 unidade, assim como o contador de tempo total
			if (*it == -1) {
				*it = pagesVector[i];
				pageFault++;
				timeCounter++;
				continue;
			}

			for (int j = 0; j < frameArray.size(); j++) {
				it2 = find(pagesVector.begin() + timeCounter, pagesVector.end(), frameArray[j]);

				//Se a página que está no quadro não irá aparecer mais como uma nova página a ser inserida, então essa sai do quadro e a nova página é colocada em seu lugar
				if (it2 == pagesVector.end()) {
					frameArray[j] = pagesVector[i];
					canEnter = false;
					break;
				}

				//Busca-se pela página que irá demorar mais tempo a aparecer para ser inserida no futuro
				else {
					canEnter = true;
					int tempDist = distance(pagesVector.begin() + timeCounter, it2);
					if (tempDist > dist) {
						dist = tempDist;
						positionFrameArray = j;
					}
				}
			}

			//Assim, insere-se a nova página, no quadro, no local da página que demorará mais tempo a ser inserida no futuro
			if (canEnter) {
				frameArray[positionFrameArray] = pagesVector[i];
				dist = -1;
				canEnter = false;
			}

			//Contator de falta de páginas é ascrescido em 1 unidade
			pageFault++;
		}

		//Contator de tempo total é ascrescido em 1 unidade
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