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

// 2. ½ÇÆÐÀ²
void falseRate() {
	vector<int> answer;
	int N = 4;
	vector<int> stages = {4, 4, 4, 4, 4};
	vector<int> stagePass(N + 1);

	sort(stages.begin(), stages.end(), greater<int>());

	int pass = 0;
	for (int i = N, j = 0; i >= 0; i--) {
		for (; j < stages.size(); j++) {
			if (stages[j] > i)
				pass++;
			else
				break;
		}
		stagePass[i] = pass;
	}

	multimap<float, int, greater<float>> sortedStages;
	for (int i = 1; i <= N; i++) {
		if (stagePass[i - 1] == 0)
			sortedStages.insert(pair<float, int>(0, i));
		else
			sortedStages.insert(pair<float, int>(float(stagePass[i - 1] - stagePass[i])/float(stagePass[i - 1]), i));
	}
	
	for (multimap<float, int>::iterator s = sortedStages.begin(); s != sortedStages.end(); s++) {
		answer.push_back(s->second);
		cout << s->first << ", " << s->second << endl;
	}
}

int main() {
	falseRate();

	return 0;
}