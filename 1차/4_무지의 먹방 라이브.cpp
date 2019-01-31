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

// 4. 무지의 먹방 라이브
typedef long long ll;

int muzziEatting() {
	vector<int> food_times = { 3, 1, 2 };
	int k = 5;

	map<int, int> food_list;
	ll sum = 0;
	for (int i = 0; i < food_times.size(); i++) {
		sum += food_times[i];
		food_list[food_times[i]]++;
	}
	if (sum <= k)
		return -1;
	
	int length = food_times.size();
	int prev = 0;
	for (auto f : food_list) {
		if (ll(f.first - prev) * length <= k) {
			k -= ll(f.first - prev) * length;
			length -= f.second;
		}
		else {
			int index = k % length;
			for (int i = 0; i < food_times.size(); i++) {
				if (food_times[i] >= f.first) {
					if (index == 0)
						return i + 1;
					index--;
				}
			}
		}
		prev = f.first;
	}
	return -1;
}

int main() {
	muzziEatting();

	return 0;
}