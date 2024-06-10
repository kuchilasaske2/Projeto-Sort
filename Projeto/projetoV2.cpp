#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>

using namespace std;
using namespace chrono;

mutex mtx;

// Função de ordenação Shell Sort
void shellSort(vector<int> &arr) {
	int n = arr.size();
	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; i++) {
			int temp = arr[i];
			int j;
			for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
				arr[j] = arr[j - gap];
			}
			arr[j] = temp;
		}
	}
}

// Função de ordenação Merge Sort
void merge(vector<int> &arr, int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	vector<int> L(n1), R(n2);

	for (int i = 0; i < n1; i++) {
		L[i] = arr[left + i];
	}
	for (int i = 0; i < n2; i++) {
		R[i] = arr[mid + 1 + i];
	}

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		} else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(vector<int> &arr, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);

		merge(arr, left, mid, right);
	}
}

// Função de ordenação Heap Sort
void heapify(vector<int> &arr, int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[largest]) {
		largest = left;
	}

	if (right < n && arr[right] > arr[largest]) {
		largest = right;
	}

	if (largest != i) {
		swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}

void heapSort(vector<int> &arr) {
	int n = arr.size();
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(arr, n, i);
	}

	for (int i = n - 1; i >= 0; i--) {
		swap(arr[0], arr[i]);
		heapify(arr, i, 0);
	}
}

// Função para ler um arquivo e armazenar os números em um vetor
void readFile(const string &filename, vector<int> &data) {
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Erro ao abrir o arquivo: " << filename << endl;
		return;
	}

	int number;
	vector<int> localData;
	while (file >> number) {
		localData.push_back(number);
	}
	file.close();

	lock_guard<mutex> lock(mtx);
	data.insert(data.end(), localData.begin(), localData.end());
}

int main() {
	// Declaração dos caminhos dos arquivos .txt
	vector<string> filePaths = {
		"dtaleat500dup0.txt",
		"dtaleat5kdup0.txt"
	};

	vector<int> data;

	// Leitura dos arquivos
	vector<thread> threads;
	for (const auto &filePath : filePaths) {
		threads.emplace_back(readFile, filePath, ref(data));
	}

	for (auto &th : threads) {
		th.join();
	}

	size_t dataSize = data.size();
	cout << "Tamanho total dos arquivos lidos: " << dataSize << " elementos" << endl;

	vector<int> dataForShellSort = data;
	vector<int> dataForMergeSort = data;
	vector<int> dataForHeapSort = data;

	// Medição do tempo de ordenação para Shell Sort
	auto startShellSort = high_resolution_clock::now();
	shellSort(dataForShellSort);
	auto stopShellSort = high_resolution_clock::now();
	auto durationShellSort = duration_cast<milliseconds>(stopShellSort - startShellSort);
	cout << "Tempo de ordenação (Shell Sort): " << durationShellSort.count() << " milissegundos" << endl;

	// Medição do tempo de ordenação para Merge Sort
	auto startMergeSort = high_resolution_clock::now();
	mergeSort(dataForMergeSort, 0, dataForMergeSort.size() - 1);
	auto stopMergeSort = high_resolution_clock::now();
	auto durationMergeSort = duration_cast<milliseconds>(stopMergeSort - startMergeSort);
	cout << "Tempo de ordenação (Merge Sort): " << durationMergeSort.count() << " milissegundos" << endl;

	// Medição do tempo de ordenação para Heap Sort
	auto startHeapSort = high_resolution_clock::now();
	heapSort(dataForHeapSort);
	auto stopHeapSort = high_resolution_clock::now();
	auto durationHeapSort = duration_cast<milliseconds>(stopHeapSort - startHeapSort);
	cout << "Tempo de ordenação (Heap Sort): " << durationHeapSort.count() << " milissegundos" << endl;

	return 0;
}