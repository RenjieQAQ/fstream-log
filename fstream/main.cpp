
#include <iostream>                                                                                                                             
#include <string>                                                                                                                           
#include <fstream>                                                                                                                     
#include <vector>                                                                                                                    
#include <deque>
using namespace std;

struct s_fsnlog {
	string name;
	size_t start;
	size_t offset;
};

inline ostream & operator <<(ostream & os, s_fsnlog & s_log) {
	os << s_log.name << "," << s_log.start << "," << s_log.offset << endl;
	return os;
}

class fsnLog {
	//const char* name = "/media/mmcblk1p1/fsnlog";
	const char* name = "fsnlog";
	fstream fio;
	deque<string> str_queue;
	//½ûÖ¹ÊµÀý»¯
	fsnLog() :fio(name, fstream::in | fstream::out | ios::ate){

	}

public:
	void closeLog() {
		fio.sync();
		fio.close();
	}

	void newfsn(const char *fsn_name,size_t offset) {
		if (!fio.is_open())
			fio.open(name, fstream::out | ios::ate );
		fio.seekp(0, ios::end);
		fio << fsn_name << "," << offset << "," << offset << endl;
	}

	void add(size_t num) {
		istream& is = fio.seekg(-10, ios::end);
		string str;
		is >> str;
		auto x = str.find_last_of(",");
		fio.seekp(-10 + x + 1, ios::end);
		fio << num << endl;
	}

	

	void load() {
		string str;
		if (!fio.is_open())
			fio.open(name, fstream::in);
		fio.seekg(0);
		while (fio >> str) {
			str_queue.push_back(str);
		}
		fio.close();
	}

	void sync() {
		string str;
		fio.open(name, fstream::out | ios::ate | ios::trunc);
		while (!str_queue.empty()) {
			fio << first_ine() << endl;
			dequeue();
		}
		fio.sync();
		fio.close();
	}

	string &first_ine() {
		return str_queue.front();
	}

	s_fsnlog read() {
		return to_log(str_queue.front());
	}

	void dequeue() {
		str_queue.pop_front();
	}

	void print() {
		for (auto iter = str_queue.begin(); iter != str_queue.end(); iter++) {
			cout << *iter << endl;
		}
	}
	
	void test() {
		string str;
		fio.seekg(0);
		fio >> str;
		s_fsnlog s_log = to_log(str);
		cout << s_log << endl;
	}

	inline static fsnLog &getlog() {
		static fsnLog *log = new fsnLog;
		return *log;
	}

private:
	s_fsnlog to_log(const string& line) {
		string::size_type pos1, pos2;
		pos1 = line.find(',');
		pos2 = line.find(',',pos1+1);
		s_fsnlog s_log;
		size_t len = line.size();
		s_log.name = line.substr(0, pos1);
		s_log.start = stoi(line.substr(pos1+1, pos2 - pos1-1));
		s_log.offset = stoi(line.substr(pos2+1, line.size() - pos2-1));
		return s_log;
	}
	void firstline(string &name, size_t& start, size_t& offset) {
		string str;
		fio.seekg(0);
		getline(fio, name, ',');
		getline(fio, str, ',');
		start = stoi(str);
		fio >> offset;
	}
};

int main() {
	fsnLog &log = fsnLog::getlog();
	//string name;
	//size_t start, offset;
	////log.firstline(name, start, offset);
	//cout << sizeof(s_fsnlog) << endl;

	log.newfsn("15sd.fsn", 45);
	log.add(50);
	log.load();
	log.dequeue();
	log.dequeue();
	log.dequeue();
	log.print();
	log.sync();
	log.newfsn("gfgd.fsn", 0);
	log.add(550);
	system("pause");
}
