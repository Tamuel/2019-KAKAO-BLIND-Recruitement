#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <set>
#include <list>

using namespace std;

// 2019 1st 5hour

// 5. 길 찾기 게임
class node {
private:
public:
	int id;
	int x;
	int y;
	node* parent = NULL;
	node* left = NULL;
	node* right = NULL;
	node() {
		x = 0;
		y = 0;
		id = 0;
	}

	node(int _x, int _y, int _id) {
		x = _x;
		y = _y;
		id = _id;
	}

	void insert(node* _node) {
		if (_node->y < y) { // Child
			if (_node->x < x) {  // Left
				if (left == NULL) {
					_node->parent = this;
					left = _node;
				}
				else {
					if (left->y < _node->y) { // New left
						left->parent = _node;
						if (left->x < _node->x)
							_node->left = left;
						else
							_node->right = left;
						left = _node;
					}
					else { // Incert to child
						left->insert(_node);
					}
				}
			}
			else { // Right
				if (right == NULL) {
					_node->parent = this;
					right = _node;
				}
				else {
					if (right->y < _node->y) { // New right
						right->parent = _node;
						if (right->x < _node->x)
							_node->right = right;
						else
							_node->right = right;
						right = _node;
					}
					else { // Incert to child
						right->insert(_node);
					}
				}
			}
		}
	}
};

void findRoute() {
	vector<vector<int>> nodeinfo = {
		{5, 3},
		{11, 5},
		{13, 3},
		{3, 5},
		{6, 1},
		{1, 3},
		{8, 6},
		{7, 2},
		{2, 2}
	};
	vector<vector<int>> answer;

	multimap<vector<int>, int, greater<vector<int>>> sorted_node;
	int idx = 1;
	for (auto n : nodeinfo)
		sorted_node.insert(pair<vector<int>, int>({n[1], n[0]}, idx++));

	node root((*sorted_node.begin()).first[1], (*sorted_node.begin()).first[0], sorted_node.begin()->second);
	for (auto n : sorted_node) {
		node* no = new node(n.first[1], n.first[0], n.second);
		root.insert(no);
	}

	// Preorder traversal
	vector<int> preorder;
	deque<node*> travel;
	travel.push_back(&root);
	while (travel.size() > 0) {
		node* cur = travel.front();
		preorder.push_back(cur->id);
		travel.pop_front();
		if (cur->right != NULL)
			travel.push_front(cur->right);
		if (cur->left != NULL)
			travel.push_front(cur->left);
	}

	// Postorder traversal
	vector<int> postorder;
	travel.push_front(&root);
	while (travel.size() > 0) {
		node* cur = travel.front();
		if (cur->left != NULL) {
			travel.push_front(cur->left);
			continue;
		}
		else if (cur->right != NULL) {
			travel.push_front(cur->right);
			continue;
		}
		else {
			travel.pop_front();
			postorder.push_back(cur->id);
			if (cur->parent == NULL)
				break;
			else if (cur->parent->left == cur)
				cur->parent->left = NULL;
			else
				cur->parent->right = NULL;
		}
	}
	answer.push_back(preorder);
	answer.push_back(postorder);
}

int main() {
	findRoute();

	return 0;
}