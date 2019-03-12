#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

string toLower(string& str) {
	string temp = "";
	for (char c : str) temp += tolower(c);
	return temp;
}
string toLower(char* str) {
	string temp = "";
	for (int i = 0; str[i] != '\0'; i++) temp += tolower(str[i]);
	return temp;
}

int main() {
	string word = toLower("blind");
	vector<string> pages = {
		"<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://a.com\"/>\n</head>  \n<body>\nBlind Lorem Blind ipsum dolor Blind test sit amet, consectetur adipiscing elit. \n<a href=\"https://b.com\"> Link to b </a>\n</body>\n</html>", "<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://b.com\"/>\n</head>  \n<body>\nSuspendisse potenti. Vivamus venenatis tellus non turpis bibendum, \n<a href=\"https://a.com\"> Link to a </a>\nblind sed congue urna varius. Suspendisse feugiat nisl ligula, quis malesuada felis hendrerit ut.\n<a href=\"https://c.com\"> Link to c </a>\n</body>\n</html>", "<html lang=\"ko\" xml:lang=\"ko\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n  <meta charset=\"utf-8\">\n  <meta property=\"og:url\" content=\"https://c.com\"/>\n</head>  \n<body>\nUt condimentum urna at felis sodales rutrum. Sed dapibus cursus diam, non interdum nulla tempor nec. Phasellus rutrum enim at orci consectetu blind\n<a href=\"https://a.com\"> Link to a </a>\n</body>\n</html>"
	};

	vector<string> ids;
	map<string, double> linkscore;
	map<string, int> basescore;
	map<string, vector<string>> outlink;

	for (string p : pages) {
		bool head = false;
		bool meta = false;
		bool body = false;
		bool href = false;
		bool content = false;
		bool colStart = false;
		string currentUrl = "";
		for (int i = 0; i < p.length(); i++) {
			switch (p[i]) {
			case '<':
				if (p.substr(i + 1, 4) == "head")
					head = true;
				else if (head && p.substr(i + 1, 4) == "meta")
					meta = true;
				else if (p.substr(i + 1, 4) == "body")
					body = true;
				else if (body && p.substr(i + 1, 2) == "a ")
					href = true;
				else if (p.substr(i + 1, 5) == "/head")
					head = false;
				else if (p.substr(i + 1, 5) == "/body")
					body = false;
				else if (body && p.substr(i + 1, 2) == "/a")
					href = false;
				break;
			case '>':
				if (meta) {
					meta = false;
					if (content) content = false;
				}
				if (href) href = false;
				break;
			case 'c':
				if (meta && p.substr(i, 7) == "content")
					content = true;
				break;
			case '"':
				if (content) {
					string url = "";
					int u;
					for (u = i + 1; p[u] != '"'; u++)
						url += p[u];
					i = u;
					currentUrl = url;
					ids.push_back(currentUrl);
					basescore[currentUrl] = 0;
				}
				else if (href) {
					string url = "";
					int u;
					for (u = i + 1; p[u] != '"'; u++)
						url += p[u];
					i = u;
					outlink[currentUrl].push_back(url);
				}
				break;
			}
			if (body) {
				if (tolower(p[i]) >= 'a' && tolower(p[i]) <= 'z') {
					int idx;
					for (idx = i; tolower(p[idx]) >= 'a' && tolower(p[idx]) <= 'z'; idx++);
					string sub = toLower(p.substr(i, idx - i));
					if (sub == word) basescore[currentUrl]++;
					i = idx;
				}
			}
		}
	}

	for (auto& m : outlink)
		for (auto& ol : m.second)
			linkscore[ol] += (double)basescore[m.first] / (double)m.second.size();

	int max = 0;
	for (int i = 1; i < ids.size(); i++) {
		if (basescore[ids[i]] + linkscore[ids[i]] > basescore[ids[max]] + linkscore[ids[max]])
			max = i;
	}
	cout << max << endl;

	return 0;
}
