/*
Author: Peter O'Donohue
Creation Date: 10/05/17
Modification Date: 10/05/17
Description: FILL IN
*/

#include <iostream>
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
		minRequests4Raise = n;
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
	void insertWorker(int id);
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

void Tree::insertWorker(int id)
{
	Node* workerNode = new Node(id);
	root->underlings.push_back(workerNode);
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
			cout << "     ";
		cout << ptr->id << endl;
		for (Node* foo: ptr->underlings)
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
		for (Node* employee: root->underlings)
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
	Node* bossPtr = nullptr;
	Node* workerPtr = nullptr;
	bossPtr = find(boss);
	workerPtr = find(underling);

	if (bossPtr == nullptr && workerPtr == nullptr)
	{
		Tree newTree(boss);
		newTree.insertWorker(underling);
		trees.push_back(newTree);
	}

	else if (bossPtr != nullptr && workerPtr == nullptr)
	{
		bossPtr->underlings.push_back(new Node(underling));
	}
}

int main()
{
	Forest company;
	int boss = 0;
	int employee = 0;
	for (int i = 0; i < 9; ++i)
	{
		cin >> boss >> employee;
		company.insert(boss, employee);
	}
	company.print();
	system("pause");
	return 0;
}
