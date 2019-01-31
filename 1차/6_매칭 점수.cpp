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

// 6. ¸ÅÄª Á¡¼ö
void matchingScore() {
	vector<string> pages = {
		"<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://a.com\"/>\n</head>  \n<body>\nBlind Lorem Blind ipsum dolor Blind test sit amet, consectetur adipiscing elit. \n<a href=\"https://b.com\"> Link to b </a>\n</body>\n</html>",
		"<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://b.com\"/>\n</head>  \n<body>\nSuspendisse potenti. Vivamus venenatis tellus non turpis bibendum, \n<a href=\"https://a.com\"> Link to a </a>\nblind sed congue urna varius. Suspendisse feugiat nisl ligula, quis malesuada felis hendrerit ut.\n<a href=\"https://c.com\"> Link to c </a>\n</body>\n</html>",
		"<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://c.com\"/>\n</head>  \n<body>\nUt condimentum urna at felis sodales rutrum. Sed dapibus cursus diam, non interdum nulla tempor nec. Phasellus rutrum enim at orci consectetu blind\n<a href=\"https://a.com\"> Link to a </a>\n</body>\n</html>",
		"<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://d.com\"/>\n</head>  \n<body>\n blind Blind Lorem Blind ipsum dolor Blind test sit amet, consectetur adipiscing elit. \n</body>\n</html>"
	};
	string word = "blind";

	// To lower case
	for (int i = 0; i < word.size(); i++)
		if ('A' <= word[i] && word[i] <= 'Z')
			word[i] -= 'A' - 'a';

	map<string, float> baseScores;
	map<string, float> linkScores;
	map<int, string> indices;
	map<string, vector<string>> linksMap;
	enum key { DEFAULT, CONTENT, HREF };
	int index = 0;
	for (string html : pages) {
		vector<string> links;
		float _baseScore = 0;
		string cite = "";
		string tempWord = "";
		bool start = false;
		bool keyStart = false;
		bool meta = false;
		bool attribute = false;
		key keyCheck = DEFAULT;
		for (int i = 0; i < html.size(); i++) {
			char cur = ('A' <= html[i] && html[i] <= 'Z') ? (html[i] - ('A' - 'a')) : (html[i]);
			switch (keyCheck) {
			case DEFAULT:
				if ('a' <= cur && cur <= 'z') {
					tempWord += cur;
				}
				else {
					if (cur == '<')
						keyStart = true;
					else if (cur == '>') {
						keyStart = false;
						meta = false;
						attribute = false;
					}
					else if (keyStart && tempWord.compare("a") == 0)
						attribute = true;
					else if (keyStart && tempWord.compare("meta") == 0)
						meta = true;
					else if (meta && keyStart && tempWord.compare("content") == 0)
						keyCheck = CONTENT;
					else if (attribute && keyStart && tempWord.compare("href") == 0)
						keyCheck = HREF;
					if (tempWord.compare(word) == 0)
						_baseScore++;
					tempWord = "";
				}
				break;
			case CONTENT:
				if (!start && cur == '\"') {
					start = true;
				}
				else if (start && cur != '\"') {
					tempWord += cur;
				}
				else if (start && cur == '\"') {
					cite = tempWord;
					tempWord = "";
					start = false;
					keyCheck = DEFAULT;
				}
				break;
			case HREF:
				if (!start && cur == '\"') {
					start = true;
				}
				else if (start && cur != '\"') {
					tempWord += cur;
				}
				else if (start && cur == '\"') {
					links.push_back(tempWord);
					tempWord = "";
					start = false;
					keyCheck = DEFAULT;
				}
				break;
			}
		}
		if (tempWord.compare(word) == 0)
			_baseScore++;
		baseScores[cite] = _baseScore;
		linksMap[cite] = links;
		linkScores[cite] = 0;
		indices[index] = cite;
		index++;
	}

	for (auto i : linksMap)
		for (string j : i.second) {
			if (linkScores.find(j) != linkScores.end() && i.second.size() != 0)
				linkScores[j] += float(baseScores[i.first]) / float(i.second.size());
		}

	multimap<float, int, greater<float>> matchingScore;
	for (auto i : indices)
		matchingScore.insert(pair<float, int>(baseScores[i.second] + linkScores[i.second], i.first));

	cout << matchingScore.begin()->second << endl;

}

int main() {
	matchingScore();

	return 0;
}