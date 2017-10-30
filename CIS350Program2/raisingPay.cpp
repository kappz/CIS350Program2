/*
Author: Peter O'Donohue
Creation Date: 10/05/17
Modification Date: 10/30/17
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
		// PRE: Object of type Node declared, n >= 0.
		// POST: Node stored in memory. id = n, minRequests4Raise = -1
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
	void compMinNumRequests(Node* workerPtr, float treshold);
public:
	Tree();
	Tree(int id);
	void printTree();
	void compMinNumRequests(float threshold);
	Node* find(int target);
	Node* getRoot();
};

Tree::Tree()
{
	// PRE: Object delared without a default argument
	// POST: root = nullptr
	root = nullptr;
}
Tree::Tree(int id)
{
	// PRE: Object delared, id >= 0
	// POST: root stored in memory. root->id = id
	root = new Node(id);
}
void Tree::printTree()
{
	// PRE: print() has been called on Forest object
	// POST: private function print() called. 
	int level = 0;
	Node* walker = root;
	print(walker, level);
}
void Tree::print(Node* ptr, int level)
{
	// PRE: print() has been called on Forest object
	// POST: employee printed to screen. print() called on employee's underlings.
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
void Tree::compMinNumRequests(float threshold)
{
	// PRE: compMinNumRequests() called on Forest object
	// POST: compMinNumRequests() private function called
	compMinNumRequests(getRoot(), threshold);
}
void Tree::compMinNumRequests(Node* workerPtr, float threshold)
{
	// PRE: compMinNumRequests called on Forest object
	// POST: employee minRequest4Raise computed. 
	int totalMinNumReq = 0;
	int minNumWorkers = 0;
	float numWorkers = 0;
	Node* workerWalker = workerPtr;
	vector<int> employeeMins;

	if (workerWalker->underlings.size() == 0)
	{
		workerWalker->minRequests4Raise = 1;
	}
	else
	{
		for (Node* employee : workerWalker->underlings)
			compMinNumRequests(employee, threshold);
		numWorkers = workerWalker->underlings.size();
		numWorkers *= threshold;
		minNumWorkers = ceil(numWorkers); // round up to surpass the threshold
		for (Node* employee : workerWalker->underlings)
		{
			employeeMins.push_back(employee->minRequests4Raise);
		}
		sort(employeeMins.begin(), employeeMins.end());
		for (int i = 0; i < minNumWorkers; ++i)
			totalMinNumReq += employeeMins[i];
		workerWalker->minRequests4Raise = totalMinNumReq;

	}
}
Node* Tree::find(int target)
{
	// PRE: find() called on Forest object. target = any number.
	// PRE: tree has been searched. returns ptr to Node containing target, returns nulltpr otherwise.
	Node* treePtr = root;
	Node* searchPtr = nullptr;
	searchPtr = find(treePtr, target);
	return searchPtr;
}
Node* Tree::find(Node* root, int target)
{
	// PRE: find() called on Forest object. root = ptr to tree Node, target = any number.
	// PRE: tree has been searched. Returns ptr to Node containing target, returns nulltpr otherwise.
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
	// PRE: getRoot() called on tree object.
	// POST: returns ptr to tree root.
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
	void compMinNumRequests(float threshhold);
	int getMinNumReq();
	Node* find(int target);
};

Forest::Forest()
{
	// PRE: Forest object declared w/no arguments.
	// POST: trees contains one element.
	Tree firstTree(0);
	trees.push_back(firstTree);
}
void Forest::clear()
{
	
	// PRE: clear() called on Forest object
	// POST: object's member 'trees' has been erased from memory
	trees.erase(trees.begin(), trees.end());
}
Node* Forest::find(int target)
{
	// PRE: find() called on forest object. target = any number
	// POST: Forest searched through. Returns ptr to Node if target found, returns nullptr otherwise
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
	// PRE: print() has been called on Forest object
	// POST: object's trees member has been printed to screen 
	trees[0].printTree();
}
void Forest::insert(int boss, int underling)
{
	// PRE: insert() called on Forest object. boss & underling = any integer
	// POST: boss & underling stored in trees. 
	int treesCount = 0;
	int tempRootId = -1;
	Tree newTree(0);
	Node *tempRoot = nullptr;
	Node* bossPtr = find(boss);
	Node* workerPtr = find(underling);

	if (bossPtr == nullptr && workerPtr == nullptr)
	{
		bossPtr = newTree.getRoot();
		bossPtr->id = boss;
		workerPtr = new Node(underling);
		bossPtr->underlings.push_back(workerPtr);
		trees.push_back(newTree);
	}
	else if (workerPtr == nullptr)
	{
		bossPtr->underlings.push_back(new Node(underling));
	}
	else if (workerPtr != nullptr)
	{
		if (bossPtr == nullptr)
		{
			bossPtr = newTree.getRoot();
			bossPtr->id = boss;
			bossPtr->underlings.push_back(workerPtr);
			trees.push_back(newTree);
		}
		else
			bossPtr->underlings.push_back(workerPtr);
		newTree = trees[0];
		tempRoot = newTree.getRoot();
		tempRootId = tempRoot->id;
		while (tempRootId != underling)
		{
			++treesCount;
			newTree = trees[treesCount];
			tempRoot = newTree.getRoot();
			tempRootId = tempRoot->id;
		}
		trees.erase(trees.begin() + treesCount);
	}
}
int Forest::getMinNumReq()
{
	// PRE: getMinNumReq() called on Forest object
	// POST: minRequets4Raise returned to caller.
	Node* tempRoot = trees[0].getRoot();
	return tempRoot->minRequests4Raise;
}
void Forest::compMinNumRequests(float threshold)
{
	// PRE: compMinNumRequests() called on Forest object
	// POST: minNumRequests4Raise computed 
	trees[0].compMinNumRequests(threshold);
}

int main()
{
	int storeBoss = 0;
	int numEmployees = 0;
	float threshold = 0.0;
	vector<int> bosses;
	Forest company;

	cin >> numEmployees >> threshold;
	do
	{
		for (int i = 0; i < numEmployees; ++i)
		{
			cin >> storeBoss;
			bosses.push_back(storeBoss);
		}
		for (int i = 0; i < numEmployees; ++i)
		{
			company.insert(bosses[i], i + 1);
		}

		threshold *= .01;
//		company.print();
		company.compMinNumRequests(threshold);
		cout << company.getMinNumReq() << endl;
		company.clear();
		bosses.clear();

		cin >> numEmployees >> threshold;
	} while (!(numEmployees == 0 && threshold == 0));
	return 0;
}