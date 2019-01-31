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

// 3. ÈÄº¸Å°
void candidateKey() {
	vector<vector<string>> relation = {
		{ "100", "ryan", "music", "2" },
		{ "200", "apeach", "math", "2" },
		{ "300", "tube", "computer", "3" },
		{ "400", "con", "computer", "4" },
		{ "500", "muzi", "music", "3" },
		{ "600", "apeach", "music", "2" },
	};

	int n_att = relation[0].size();
	int n_relation = relation.size();
	vector<int> result;
	for (int i = 1; i < (1 << n_att); i++) {
		set<string> checker;
		for (vector<string> re : relation) {
			string temp = "";
			for (int j = 0; j < n_att; j++)
				if (i & (1 << j))
					temp += re[j];
			if (!checker.insert(temp).second)
				break;
		}

		if (checker.size() == n_relation) {
			bool check = true;
			for (int j : result)
				if ((j & i) == j) {
					check = false;
					break;
				}
			if (check) result.push_back(i);
		}
	}

	cout << result.size() << endl;
}

int main() {
	candidateKey();

	return 0;
}