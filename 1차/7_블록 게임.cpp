#include <iostream>
#include <vector>
#include <set>
#define _x first
#define _y second
#define BLOCK -1

using namespace std;

int main() {
	int N;
	cin >> N;
	vector<vector<pair<int, int>>> offsets = {
		{ { 0, 0 },{ 1, 0 },{ 0, 1 },{ 1, 1 },{ 0, 2 },{ 1, 2 } },
		{ { 0, 0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },{ 2, 1 } },
		{ { 0, 0 },{ -1, 0 },{ 0, 1 },{ -1, 1 },{ 0, 2 },{ -1, 2 } },
		{ { 0, 0 },{ -1, 0 },{ -2, 0 },{ 0, 1 },{ -1, 1 },{ -2, 1 } }
	};
	int board[50][50];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	int nBreak = 0;
	int nBreakPrev = -1;
	while (nBreak != nBreakPrev) {
		nBreakPrev = nBreak;
		vector<pair<int, int>> pos;
		for (int k = 0; k < 2; k++)
			for (int j = 0; j < N; j++)
				for (int i = 1; i < N; i++)
					if (board[i][j] != 0) {
						board[i - 1][j] = BLOCK;
						pos.push_back({ j, i - 1 });
						break;
					}

		for (pair<int, int>& p : pos) {
			for (vector<pair<int, int>>& off : offsets) {
				set<int> checker;
				int count = 0;
				int blockCount = 0;
				for (pair<int, int>& o : off) {
					int x = p._x + o._x, y = p._y + o._y;
					if (x < 0 || x >= N || y < 0 || y >= N) break;
					checker.insert(board[y][x]);
					if (checker.size() >= 3) break;
					if (board[y][x] != 0) count++;
					if (board[y][x] == BLOCK) blockCount++;
				}
				if (count == 6 && blockCount == 2) {
					for (pair<int, int>& o : off) {
						int x = p._x + o._x, y = p._y + o._y;
						board[y][x] = 0;
					}
					nBreak++;
					break;
				}
			}
		}
		for (pair<int, int>& p : pos)
			if (board[p._y][p._x] == BLOCK)
				board[p._y][p._x] = 0;
	}

	cout << nBreak << endl;
	return 0;
}
