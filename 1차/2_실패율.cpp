#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

int main() {
	int N = 5;
	vector<int> stages = { 2, 1, 2, 6, 2, 4, 3, 3 };
	vector<int> merge(N + 2);
	int total = stages.size();
	for (int s : stages)
		merge[s]++;

	vector<double> failure(1);

	for (int i = 1; i < N + 1; i++) {
		failure.push_back((double)merge[i] / (double)total);
		total -= merge[i];
	}
	
	multimap<double, int, greater<double>> sorted;
	for (int i = 1; i < failure.size(); i++)
		sorted.insert({ failure[i], i });

	for (auto s : sorted)
		cout << s.second << endl;

	return 0;
}
