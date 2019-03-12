#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef struct coor{
	int x;
	int y;
	bool operator< (const struct coor& c) const {
		if (y != c.y)
			return y > c.y;
		return x < c.x;
	}
} coor;

typedef struct node {
	int id;
	coor co;
	struct node* left;
	struct node* right;
} node;

void makeTree(node* root, node* temp) {
	if (root->co.x > temp->co.x) {
		if (root->left == NULL) {
			root->left = temp;
			return;
		}
		else makeTree(root->left, temp);
	}
	if (root->co.x < temp->co.x) {
		if (root->right == NULL) {
			root->right = temp;
			return;
		}
		else makeTree(root->right, temp);
	}
}

void preorder(node* temp, vector<int>& results) {
	if (temp) {
		results.push_back(temp->id);
		preorder(temp->left, results);
		preorder(temp->right, results);
	}
}

void postorder(node* temp, vector<int>& results) {
	if (temp) {
		postorder(temp->left, results);
		postorder(temp->right, results);
		results.push_back(temp->id);
	}
}

int main() {
	vector<pair<int, int>> nodeinfo = {
		{5, 3}, {11, 5}, {13, 3}, {3, 5}, {6, 1}, {1, 3}, {8, 6}, {7, 2}, {2, 2}
	};

	map<coor, int> nodeId;

	for (int i = 0; i < nodeinfo.size(); i++)
		nodeId.insert({ { nodeinfo[i].first, nodeinfo[i].second }, i + 1 });

	node* root = new node{ nodeId.begin()->second, nodeId.begin()->first, NULL, NULL };
	for (map<coor, int>::iterator iter = nodeId.begin(); iter != nodeId.end(); iter++) {
		node* temp = new node{ iter->second, iter->first, NULL, NULL };
		makeTree(root, temp);
	}

	vector<int> pre, post;
	preorder(root, pre);
	postorder(root, post);

	return 0;
}
