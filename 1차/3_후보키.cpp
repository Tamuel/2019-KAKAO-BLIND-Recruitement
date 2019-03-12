#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

int main() {
	vector<vector<string>> relation = {
		{ "100", "ryan", "music", "2" },
		{ "200", "apeach", "math", "2" },
		{ "300", "tube", "computer", "3" },
		{ "400", "con", "computer", "4" },
		{ "500", "muzi", "music", "3" },
		{ "600", "apeach", "music", "2" },
	};
	set<int> candidateKey;
	int check = 0;

	for (int i = 1; i < (1 << relation[0].size()); i++) {
		int t;
		set<vector<string>> checker;
		for (t = 0; t < relation.size(); t++) {
			vector<string> temp;
			for (int c = 0; c < relation[0].size(); c++)
				if (i & (1 << c)) temp.push_back(relation[t][c]);
			if (!checker.insert(temp).second)
				break;
		}
		if (t == relation.size())
			if ((i & check) == 0) {
				candidateKey.insert(i);
				check |= i;
			}
	}

	cout << candidateKey.size() << endl;
	return 0;
}
