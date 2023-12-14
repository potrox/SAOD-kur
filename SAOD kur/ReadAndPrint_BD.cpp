#include <string>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>

#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>

using namespace std;

struct record
{
	char author[12];
	char title[32];
	char publisher[16];
	short int year;
	short int num_of_page;

	record() = default;
};

struct node {
	record* data;
	node* next;
};

struct MyQueue {
	node* head;
	node* tail;

	MyQueue() : head(nullptr), tail(nullptr) {}

	void push(record* data) {
		node* newNode = new node;
		newNode->data = data;
		newNode->next = nullptr;

		if (!head) {
			head = newNode;
			tail = newNode;
		}
		else {
			tail->next = newNode;
			tail = newNode;
		}
	}

	bool empty() const {
		return !head;
	}

	record* front() const {
		if (head) {
			return head->data;
		}
		throw runtime_error("Queue is empty");
	}

	void pop() {
		if (head) {
			node* temp = head;
			head = head->next;
			delete temp;
		}
	}

	int size() const {
		int count = 0;
		node* current = head;
		while (current) {
			count++;
			current = current->next;
		}
		return count;
	}
};

struct BTree 
{
	std::vector<MyQueue> data;
	int balance = 0;
	BTree* left = nullptr;
	BTree* right = nullptr;

	BTree();
};

void InsertInBTree(MyQueue& data, BTree*& root, bool& VR, bool& HR)
{
	if (root == nullptr) {
		root = new BTree;
		root->data.push_back(data);
		root->balance = 0;
		root->left = nullptr;
		root->right = nullptr;
		VR = false;
		HR = false;
	}
	else {
		if (data.front()->year < root->data.front().head->data->year) {
			InsertInBTree(data, root->left, VR, HR);
			if (VR) {
				if (root->balance == 0) {
					BTree* q = root->left;
					root->left = q->right;
					q->right = root;
					root = q;
					q->balance = 1;
					VR = false;
					HR = true;
				}
				else {
					root->balance = 0;
					HR = true;
				}
			}
			else {
				HR = false;
			}
		}
		else if (data.front()->year > root->data.front().head->data->year) {
			InsertInBTree(data, root->right, VR, HR);
			if (VR) {
				root->balance = 1;
				VR = false;
				HR = true;
			}
			else if (HR) {
				if (root->balance > 0) {
					BTree* q = root->right;
					root->right = q->left;
					q->left = root;
					root->balance = 0;
					q->balance = 0;
					root->left = root;
					root = q;
					VR = true;
					HR = false;
				}
				else {
					HR = false;
				}
			}
			else {
				HR = false;
			}
		}
		else {
			root->data.push_back(data);
		}
	}
}

void InOrderTraversal(BTree* root, int& counter) {
	if (root == nullptr) {
		return;
	}

	InOrderTraversal(root->left, counter);
	for (size_t i = 0; i < root->data.size(); i++) {
		counter++;
		std::cout << counter << "\t|| " << root->data[i].head->data->author << "\t|| "
			<< root->data[i].head->data->num_of_page << "\t|| "
			<< root->data[i].head->data->publisher << "\t|| "
			<< root->data[i].head->data->title << "\t|| "
			<< root->data[i].head->data->year << "||" << std::endl;
	}
	InOrderTraversal(root->right, counter);
}

BTree::BTree()
{
	balance = 0;
	left = nullptr;
	right = nullptr;
}

char* fill(record** index, int i)
{
	char* currentWord = new char[32];

	for (int j = 0, wordCount = 0; j < 32; j++) {
		if ((int)index[i]->title[j] == 32) {
			++wordCount;
		}
		if (wordCount == 2) {
			for (int k = j + 1, count = 0; k < 32; k++, count++) {
				currentWord[count] = index[i]->title[k];
			}
			break;
		}
	}

	return currentWord;
}

void heapify(record** index, int size, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < size && (strcmp(fill(index, l), fill(index, largest))>0))
		largest = l;

	if (r < size && (strcmp(fill(index, r), fill(index, largest))>0))
		largest = r;

	if (largest != i)
	{
		std::swap(index[i], index[largest]);

		heapify(index, size, largest);
	}
}

void HeapSort(record** index, int size)
{
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		heapify(index, size, i);
	}

	for (int i = size - 1; i >= 0; i--)
	{
		std::swap(index[0], index[i]);
		heapify(index, i, 0);
	}
}

void GetIndexArr(record** index, record* list, int size)
{
	for (int i = 0; i < size; i++)
	{
		index[i] = &list[i];
	}
}

void Sort(record** index, int size)
{
	HeapSort(index, size);
}


MyQueue BinarySearch(record** index, const string& targetKey) {
	MyQueue result;

	int arrSize = 4000;
	int left = 0;
	int right = arrSize - 1;

	while (left <= right) {
		int mid = (left + right) / 2;

			string key(fill(index, mid), 3);

			if (key == targetKey) {
				result.push(index[mid]);

				int leftIndex = mid - 1;
				while (leftIndex >= 0 && strncmp(fill(index, leftIndex), targetKey.c_str(), 3) == 0) {
					result.push(index[leftIndex]);
					leftIndex--;
				}

				int rightIndex = mid + 1;
				while (rightIndex < arrSize && strncmp(fill(index, rightIndex), targetKey.c_str(), 3) == 0) {
					result.push(index[rightIndex]);
					rightIndex++;
				}
				return result;
			}
			else if (key < targetKey) {
				left = mid + 1;
			}
			else {
				right = mid - 1;
			}
	}

	return result;
}

void displayDelimiter()
{
	std::cout << "--------"
		<< "++" << "--------------"
		<< "++" << "------"
		<< "++" << "----------------------"
		<< "++" << "--------------------------------------"
		<< "++-----++\n";
}

void displayHead()
{
	displayDelimiter();
	std::cout << "Number" << "\t"
		<< "||" << "\tAuthor  " 
		<< "|| Page "
		<< "||" << "\tPublisher\t"
		<< "||" << "\t\ttitle\t\t\t"
		<< "||" << " Year" 
		<< "||\n";
	displayDelimiter();
}

void displayControl(int state)
{
	displayDelimiter();
	std::cout << "\t\t\t<- Previous \t\t Next ->\t\tESC: Exit"
		<< "\n\t\t   S: Sort database\tC: Get standart database\tF: find Surname";

	if (state == 1)
	{
		std::cout << std::endl << "\tB: back to index";
	}

	std::cout << std::endl << "--------------------"
		<< "-----------------------------------------"
		<< "--------------------------------------------";
}

int displayTable(record** index, int& currentPage)
{
	system("cls");
	displayHead();
	for (int i = currentPage * 20; i < currentPage * 20 + 20; i++)
	{
		std::cout << i + 1 << "\t|| " << index[i]->author << "\t|| "
			<< index[i]->num_of_page << "\t|| "
			<< index[i]->publisher << "\t|| "
			<< index[i]->title << "\t|| "
			<< index[i]->year << "||" << std::endl;
	}
	displayControl(0);

	return 0;
}

void printQueue(const MyQueue& result, int currentPage)
{
	node* currentNode = result.head;
	int counter = 0;
	int startIndex = currentPage * 20;

	while (counter < startIndex && currentNode) {
		currentNode = currentNode->next;
		counter++;
	}
	displayHead();
	while (currentNode && counter < startIndex + 20) {
		record* recordPtr = currentNode->data;

		std::cout << counter + 1 << "\t|| " << recordPtr->author << "\t|| "
			<< recordPtr->num_of_page << "\t|| "
			<< recordPtr->publisher << "\t|| "
			<< recordPtr->title << "\t|| "
			<< recordPtr->year << "||" << std::endl;

		currentNode = currentNode->next;
		counter++;
	}
	displayControl(1);
}

void PrintFinderTree(BTree* root, int num) {
	if (root == nullptr) {
		return;
	}

	PrintFinderTree(root->left, num);
	for (size_t i = 0; i < root->data.size(); i++) {
		if (root->data[i].head->data->year == num) {
			std::cout << root->data[i].head->data->author << "\t|| "
				<< root->data[i].head->data->num_of_page << "\t|| "
				<< root->data[i].head->data->publisher << "\t|| "
				<< root->data[i].head->data->title << "\t|| "
				<< root->data[i].head->data->year << "||" << std::endl;
		}
	}
	PrintFinderTree(root->right, num);
}

MyQueue ShowBinarySearch(record** index, int programStatus, int& currentPage)
{
	std::string findYear{};
	MyQueue result{};

	BTree* root{};
	bool VR = false;
	bool HR = false;

	std::cout << std::endl << "Enter Surname: ";
	std::cin >> findYear;

	result = BinarySearch(index, findYear);
	if (result.empty())
	{
		system("cls");
		std::cout << std::endl << "\t\t\t\t\t\tNo one record!";
		Sleep(2000);
		return MyQueue();
	}
	else
	{
		while (programStatus == 1)
		{
			system("cls");
			printQueue(result, currentPage);
			int tempkey = _getch();
			if (tempkey == 116)
			{
				programStatus = 2;
			}
			if (tempkey == 75 and currentPage > 0)
			{
				currentPage -= 1;
				displayTable(index, currentPage);
			}
			if (tempkey == 77 and currentPage < 15)
			{
				currentPage += 1;
				displayTable(index, currentPage);
			}
			if (tempkey == 98)
			{
				currentPage = 0;
				break;
			}
		}
	}

	if (programStatus == 2)
	{
		int counter = 0;
		short int num = 0;
		root = nullptr;
		while (!(result.head == nullptr)) {
			InsertInBTree(result, root, VR, HR);
			result.head = result.head->next;
		}
		system("cls");
		displayHead();
		InOrderTraversal(root, counter);
		displayDelimiter();
		std::cout << "\n\t\tF: Find or press any key to exit...\n\n";
		displayDelimiter();
		int kkey = _getch();
		if (kkey == 102) {
			system("cls");
			std::cout << "\t\t\tEnter year:\n";
			std::cin >> num;
			PrintFinderTree(root, num);
			system("pause");
		}
		programStatus = 1;
	}

	return result;
}

int up(int n, double q, double* array, double chance[]) {
	int i = 0, j = 0;
	for (i = n - 2; i >= 1; i--) {
		if (array[i - 1] < q) array[i] = array[i - 1];
		else {
			j = i;
			break;
		}
		if ((i - 1) == 0 && chance[i - 1] < q) {
			j = 0;
			break;
		}
	}
	array[j] = q;
	return j;
}

void down(int n, int j, int* Length, char** c) {
	char pref[20]{};
	for (int i = 0; i < 20; i++) pref[i] = c[j][i];
	int l = Length[j];
	for (int i = j; i < n - 2; i++) {
		for (int k = 0; k < 20; k++)
			c[i][k] = c[i + 1][k];
		Length[i] = Length[i + 1];
	}
	for (int i = 0; i < 20; i++) {
		c[n - 2][i] = pref[i];
		c[n - 1][i] = pref[i];
	}
	c[n - 1][l] = '1';
	c[n - 2][l] = '0';
	Length[n - 1] = l + 1;
	Length[n - 2] = l + 1;
}

void huffman(int n, double* array, int* Length, char** c, double chance[]) {
	if (n == 2) {
		c[0][0] = '0';
		Length[0] = 1;
		c[1][0] = '1';
		Length[1] = 1;
	}
	else {
		double q = array[n - 2] + array[n - 1];
		int j = up(n, q, array, chance);
		huffman(n - 1, array, Length, c, chance);
		down(n, j, Length, c);
	}
}

std::unordered_map<char, int> get_char_counts_from_file(std::string file_name, int& file_size) {
	const int n = 4000;
	std::ifstream file(file_name, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file");
	}

	char* ch_arr = new char [sizeof(record) * n] {};
	file.read((char*)ch_arr, (std::streamsize)sizeof(record) * n);
	file.close();

	std::unordered_map<char, int> counter_map;
	file_size = 0;

	for (int i = 0; i < sizeof(record) * n; i++)
	{
		counter_map[ch_arr[i]]++;
		file_size++;
	}

	delete[] ch_arr;
	return counter_map;
}

std::vector<std::pair<double, char>> calc_probabilities(const std::unordered_map<char, int>& counter_map, int count) {
	std::vector<std::pair<double, char>> probabilities;
	probabilities.reserve(counter_map.size());
	for (auto& i : counter_map) {
		probabilities.emplace_back(std::make_pair((double)i.second / count, i.first));
	}
	return probabilities;
}

int coding()
{
	system("CLS");

	int file_size;
	std::unordered_map<char, int> counter_map;
	counter_map = get_char_counts_from_file("testBase1.dat", file_size);

	auto probabilities = calc_probabilities(counter_map, file_size);
	counter_map.clear();

	std::sort(probabilities.begin(), probabilities.end(), std::greater<std::pair<double, char>>());
	std::cout << "Probabil.  char\n";
	for (auto& i : probabilities) {
		std::cout << std::fixed << i.first << " | " << i.second << '\n';
	}

	const int n = (int)probabilities.size();

	char** c = new char* [n];
	for (int i = 0; i < n; i++)
	{
		c[i] = new char[20];
	}

	int* Length = new int[n];
	for (int i = 0; i < n; i++)
	{
		Length[i] = 0;
	}

	auto p = new double[n];
	double* chance_l = new double[n];

	for (int i = 0; i < n; ++i) {
		p[i] = probabilities[i].first;
		chance_l[i] = p[i];

	}

	huffman(n, chance_l, Length, c, p);
	std::cout << "\nHaffmanCode:\n";
	std::cout << "\nCh  Prob      Code\n";
	double avg_len = 0;
	double entropy = 0;
	for (int i = 0; i < n; i++) {
		avg_len += Length[i] * p[i];
		entropy -= p[i] * log2(p[i]);
		printf("%c | %.5lf | ", probabilities[i].second, p[i]);
		for (int j = 0; j < Length[i]; ++j) {
			printf("%c", c[i][j]);
		}
		std::cout << '\n';
	}
	std::cout << "Average length = " << avg_len << std::endl
		<< "Entropy = " << entropy << std::endl
		<< "Average length <" << entropy + 1 << std::endl
		<< "N = " << n << std::endl << std::endl;

	system("pause");

	delete[] p;
	delete[] chance_l;
	return 0;
}

int main()
{
	FILE *fp;
	fp = fopen("testBase1.dat", "rb");
	int ProgramStatus = 1;
	int currentPage = 0;
	int i = 0, sum = 20, size = 4000;
	record* list = new record[size];
	record** index = new record* [size];
	string targetKey{};
	MyQueue result{};
	i = fread((record*)list, sizeof(record), size, fp);
	GetIndexArr(index, list, size);
	displayTable(index, currentPage);
	fclose(fp);
	while (ProgramStatus)
	{
		switch (_getch())
		{
		case 27:
			ProgramStatus = 0;
		case 75:
			system("cls");
			if (currentPage > 0)
			{
				currentPage -= 1;
				sum -= 20;
				displayTable(index, currentPage);
			}
			else
			{
				displayTable(index, currentPage);
			}
			break;
		case 77:
			system("cls");
			if (currentPage < 200)
			{
				currentPage += 1;
				sum += 20;
				displayTable(index, currentPage);
			}
			else
			{
				displayTable(index, currentPage);
			}
			break;
		case 99:
			system("cls");
			GetIndexArr(index, list, size);
			displayTable(index, currentPage);
			break;
		case 115:
			system("cls");
			Sort(index, size);
			displayTable(index, currentPage);
			break;
		case 112:
			currentPage = 0;
			ShowBinarySearch(index, ProgramStatus, currentPage);
			displayTable(index, currentPage);
			break;
		case 49:
			coding();
			displayTable(index, currentPage);
		default: 
			break;
		}
	}
}