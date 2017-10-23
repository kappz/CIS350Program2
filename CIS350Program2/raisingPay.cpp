/*
Author: Peter O'Donohue
Creation Date: 10/05/17
Modification Date: 10/23/17
Description: This program computes and prints the minimum number of employee requests required 
for all employees of a particular company to receive a raise. This program operates under the
assumption that every employee has a single boss, and a boss only sends a raise request up the 
line if the percentage of employees underneath them making requests is greater than X%. On the 
first line after executing the program, input the number of employees(minus the owner), as well 
as the threshold percentage required for a boss to push a raise request up the line. On the next 
line input all the bosses, where every boss Y names the boss of employee Y. The program will then
print a tree representing the hierarchical structure of the company, with the last output line 
representing the minimum number of workers required for the owner to give everyone a raise. When 
finished, input 0 0 to exit the program.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
	int id;
	int minRequests4Raise;
	vector<Node*> underlings;
	Node(int n = 0)
	{
		id = n;
		minRequests4Raise = -1;
	}
};

class Tree
{
private:
	Node* root;
	Node* find(Node* root, int target);
	void print(Node* ptr, int level);
public:
	Tree();
	Tree(int id);
	void printTree();
	Node* find(int target);
	Node* getRoot();
};

Tree::Tree()
{
	root = nullptr;
}
Tree::Tree(int id)
{
	root = new Node(id);
}
void Tree::printTree()
{
	int level = 0;
	Node* walker = root;
	print(walker, level);
}
void Tree::print(Node* ptr, int level)
{
	if (ptr != nullptr)
	{
		for (int i = 0; i < level; ++i)
			cout << "   ";
		cout << ptr->id << endl;
		for (Node* foo : ptr->underlings)
			print(foo, level + 1);
	}
	return;
}
Node* Tree::find(int target)
{
	Node* treePtr = root;
	Node* searchPtr = nullptr;
	searchPtr = find(treePtr, target);
	return searchPtr;
}
Node* Tree::find(Node* root, int target)
{
	Node* workerPtr = nullptr;

	if (root == nullptr || root->id == target)
		return root;
	else
	{
		for (Node* employee : root->underlings)
		{
			workerPtr = find(employee, target);
			if (workerPtr != nullptr)
				return workerPtr;
		}
	}
	return workerPtr;
}
Node* Tree::getRoot()
{
	Node* rootPtr = root;
	return rootPtr;
}

class Forest
{
private:
	vector<Tree> trees;
public:
	Forest();
	void clear();
	void print();
	void insert(int boss, int underling);
	Node* find(int target);
};

Forest::Forest()
{
	Tree firstTree(0);
	trees.push_back(firstTree);
}
void Forest::clear()
{
	// FIX, MEM LEAK
	trees.erase(trees.begin(), trees.end());
}
Node* Forest::find(int target)
{
	Node* nodeFound = nullptr;
	for (Tree tree2Search : trees)
	{
		nodeFound = tree2Search.find(target);
		if (nodeFound != nullptr)
			if (nodeFound->id == target)
				return nodeFound;
	}
	return nodeFound;
}
void Forest::print()
{
	trees[0].printTree();
}
void Forest::insert(int boss, int underling)
{
	Node *tempRoot = nullptr;
	Node* bossPtr = nullptr;
	Node* workerPtr = nullptr;
	bossPtr = find(boss);
	workerPtr = find(underling);

	if (bossPtr == nullptr && workerPtr == nullptr)
	{
		Tree newTree(boss);
		workerPtr = new Node(underling);
		Node* treeRoot = newTree.getRoot();
		treeRoot->underlings.push_back(workerPtr);
		trees.push_back(newTree);
	}
	else if (bossPtr != nullptr && workerPtr == nullptr)
	{
		bossPtr->underlings.push_back(new Node(underling));
	}
	else if (bossPtr == nullptr && workerPtr != nullptr)
	{
		Tree newTree(boss);
		tempRoot = newTree.getRoot();
		tempRoot->underlings.push_back(workerPtr);
		tempRoot = nullptr;
	}
	else if (bossPtr != nullptr && workerPtr != nullptr)
	{
		int treeCounter = 0;
		int tempRoot = -1;
		Tree forestWalker = trees[0];
		Node* foo;
		foo = forestWalker.getRoot();
		tempRoot = foo->id;

		bossPtr->underlings.push_back(workerPtr);
		while (tempRoot != underling)
		{
			++treeCounter;
			forestWalker = trees[treeCounter];
			foo = forestWalker.getRoot();
			tempRoot = foo->id;
		}
		trees.erase(trees.begin() + treeCounter);
	}
}

int main()
{
	Forest company;
	int storeBoss = 0;
	int threshhold = 0;
	int numEmployees = 0;
	string companyInfo;
	vector<int> bosses;
	istringstream inputBuffer;
	do
	{
		getline(cin, companyInfo);
		inputBuffer.clear();
		inputBuffer.str(companyInfo);
		inputBuffer >> numEmployees;
		inputBuffer >> threshhold;
		companyInfo.erase();
		getline(cin, companyInfo);
		inputBuffer.clear();
		inputBuffer.str(companyInfo);
		while (inputBuffer >> storeBoss)
			bosses.push_back(storeBoss);
		for (int i = 0; i < numEmployees; ++i)
		{
			company.insert(bosses[i], i + 1);
		}
		company.print();
		company.clear();
		bosses.clear();
		cin >> numEmployees >> threshhold;
	} while (numEmployees != 0 && threshhold != 0);
	return 0;
}