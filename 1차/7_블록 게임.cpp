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

// 7. 블록 게임
void blockGame() {
	vector<vector<int>> board = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 6, 0, 0, 0 },
		{ 0, 0, 3, 0, 0, 0, 6, 6, 0, 0 },
		{ 0, 3, 3, 3, 2, 0, 6, 0, 0, 0 },
		{ 0, 0, 4, 1, 2, 2, 2, 0, 0, 0 },
		{ 0, 0, 4, 1, 1, 1, 5, 0, 0, 0 },
		{ 0, 0, 4, 4, 5, 5, 5, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	int N = board.size();
	vector<vector<int>> impossibleChecker(N, vector<int>(N));
	vector<vector<int>> needtoChecker(N, vector<int>(N));
	
	vector<vector<vector<int>>> impossibleOffs = {
		{ { -1, 0 }, { -1, -1 } },
		{ { 1, 0 }, { 1, -1 } }
	};

	vector<vector<vector<int>>> needtoOff = {
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 }, { 1, 2 } }
	};

	vector<vector<int>> findOff = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}
	};

	set<int> count;
	set<int> impossibleCount;

	// 1 안되는 영역 찾기
	// 2 안되는 영역 및 아랫 부분 표시
	// 3 필요한 부분 찾기
	// 4 안되는 영역에 포함된 필요한 부분 삭제
	// 5 필요한 부분의 블럭도 안되는 것으로 표시
	// 6 변화 없을 때까지 4 - 5 반복

	// Check impossible
	deque<pair<int, int>> impossible;
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			if (y > 0 && board[y - 1][x] != 0) {
				for (auto imp : impossibleOffs) {
					int nCheck = 1;
					int currentBlock = board[y - 1][x];
					for (vector<int> pos : imp) {
						int _x = pos[0] + x;
						int _y = pos[1] + y;
						if (_x < 0 || _x >= N || _y < 0 || _y >= N)
							break;
						if (board[_y][_x] == currentBlock)
							nCheck++;
						else
							break;
					}
					if (nCheck == 3) {
						impossibleCount.insert(board[y - 1][x]);
						impossible.push_back(pair<int, int>(x, y - 1));
						break;
					}
				}
			}

	if (impossibleCount.size() == 0) {
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (board[i][j] != 0)
					count.insert(board[i][j]);
		cout << count.size() << endl;
		return;
	}

	int prevImpCount = -1;
	while (true) {
		if (prevImpCount == impossibleCount.size())
			break;

		prevImpCount = impossibleCount.size();

		while (impossible.size() > 0) {
			pair<int, int> pos = impossible.front();
			impossible.pop_front();

			deque<pair<int, int>> travel;
			travel.push_back(pos);
			for (int i = 0; i < 4; i++) {
				pair<int, int> cur = travel.front();
				travel.pop_front();
				for (auto f : findOff) {
					int _x = f[0] + cur.first;
					int _y = f[1] + cur.second;
					if (_x < 0 || _x >= N || _y < 0 || _y >= N)
						continue;
					if (board[_y][_x] == board[cur.second][cur.first])
						travel.push_back(pair<int, int>(_x, _y));
					if (impossibleChecker[_y][_x] == 0 && board[_y][_x] == board[cur.second][cur.first]) {
						impossibleChecker[_y][_x] = 1;
						for (int __y = _y; __y < N; __y++)
							impossibleChecker[__y][_x] = 1;
					}
				}
			}
		}

		// Check needto
		for (int y = 0; y < N; y++)
			for (int x = 0; x < N; x++)
				for (auto ne : needtoOff) {
					map<int, int> checker;
					int cur = -1;
					int tempX, tempY;
					for (auto n : ne) {
						int _x = n[0] + x;
						int _y = n[1] + y;
						if (_x < 0 || _x >= N || _y < 0 || _y >= N)
							break;
						if (board[_y][_x] != 0)
							checker[board[_y][_x]]++;
						if (checker[board[_y][_x]] == 4) {
							cur = board[_y][_x];
							tempX = _x;
							tempY = _y;
							break;
						}
					}
					if (cur != -1) {
						for (auto n : ne) {
							int _x = n[0] + x;
							int _y = n[1] + y;
							if (_x < 0 || _x >= N || _y < 0 || _y >= N)
								break;
							if (board[_y][_x] != cur) {
								if (impossibleChecker[_y][_x] == 1) {
									impossible.push_back(pair<int, int>(tempX, tempY));
									impossibleCount.insert(board[tempY][tempX]);
								}
								else
									needtoChecker[_y][_x] = 1;
							}
						}
						count.insert(cur);
						break;
					}
				}
	}
	cout << count.size() - impossibleCount.size() << endl;
}

int main() {
	blockGame();

	return 0;
}