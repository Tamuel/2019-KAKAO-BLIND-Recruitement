#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

enum OP {ENTER, LEAVE, CHANGE};

int main() {
	map<string, string> idNick;
	vector<pair<string, OP>> idOp;

	vector<string> records;
	while(true) {
		string temp;
		getline(cin, temp);
		if (temp == "") break;
		records.push_back(temp);
	}

	for (string r : records) {
		if (r[0] == 'E') {
			string idAndNick = r.substr(6);
			string id = idAndNick.substr(0, idAndNick.find(' '));
			string nick = idAndNick.substr(idAndNick.find(' ') + 1);
			idNick[id] = nick;
			idOp.push_back({ id, ENTER });
		}
		else if (r[0] == 'L') {
			string id = r.substr(6).substr(0);
			idOp.push_back({ id, LEAVE });
		}
		else if (r[0] == 'C') {
			string idAndNick = r.substr(7);
			string id = idAndNick.substr(0, idAndNick.find(' '));
			string nick = idAndNick.substr(idAndNick.find(' ') + 1);
			idNick[id] = nick;
		}
	}

	for (pair<string, OP> io : idOp) {
		string op = "";
		if (io.second == ENTER) op = "들어왔습니다.";
		else if (io.second == ENTER) op = "나갔습니다.";
		cout << idNick[io.first] << "님이 " << op << endl;
	}

	return 0;
}
