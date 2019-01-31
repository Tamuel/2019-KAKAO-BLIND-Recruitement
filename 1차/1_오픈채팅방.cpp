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
// 1. ¿ÀÇÂÃ¤ÆÃ¹æ
enum op { ENTER, LEAVE, CHANGE };

class user {
private:
public:
	string mUserID;
	string mNickName;

	user() {

	}

	user(string _userID, string _nick) {
		mUserID = _userID;
		mNickName = _nick;
	}
};

class operation {
private:
public:
	user* mUser;
	op mOp;
	string opArg;
	operation(user* _user, op _op, string _arg) {
		mUser = _user;
		mOp = _op;
		opArg = _arg;
	}
	operation() {

	}
};

void openChatting() {
	vector<string> answer;
	vector<string> record = {
		"Enter uid1234 Muzi", "Enter uid4567 Prodo","Leave uid1234","Enter uid1234 Prodo","Change uid4567 Ryan"
	};
	vector<operation> operations;
	map<string, user> users;

	for (int i = 0; i < record.size(); i++) {
		string op_str = record[i].substr(0, record[i].find(' '));
		string temp = record[i].substr(record[i].find(' ') + 1);
		string id = temp.substr(0, temp.find(' '));
		string arg = temp.substr(temp.find(' ') + 1);

		operation _op;
		switch (op_str[0]) {
		case 'E':
			if (users.find(id) == users.end())
				users.insert(pair<string, user>(id, user(id, arg)));
			else {
				users[id].mNickName = arg;
			}
			_op.mUser = &(users[id]);
			_op.mOp = ENTER;
			_op.opArg = arg;
			break;
		case 'L':
			_op.mUser = &(users[id]);
			_op.mOp = LEAVE;
			_op.opArg = arg;
			break;
		case 'C':
			_op.mUser = &(users[id]);
			users[id].mNickName = arg;
			_op.mOp = CHANGE;
			_op.opArg = arg;
			break;
		}
		operations.push_back(_op);
	}

	for (vector<operation>::iterator o = operations.begin(); o != operations.end(); o++) {
		switch (o->mOp) {
		case ENTER:
			answer.push_back(o->mUser->mNickName + "´ÔÀÌ µé¾î¿Ô½À´Ï´Ù.");
			break;
		case LEAVE:
			answer.push_back(o->mUser->mNickName + "´ÔÀÌ ³ª°¬½À´Ï´Ù.");
			break;
		case CHANGE:
			break;
		}
	}

	for (int i = 0; i < answer.size(); i++)
		cout << answer[i] << endl;
}

int main() {
	openChatting();

	return 0;
}