#include <iostream>
#include <iomanip>

using namespace std;

class str_escape {
	const char *str;
	public:
	str_escape(const char *p) : str(p) {};
	friend ostream &operator<< (ostream &out, const str_escape &se);
};

ostream &operator<< (ostream &out, const str_escape &se) {
	int i=0;
	const char *str = se.str;
	while (str[i]) {
		switch(str[i]) {
		case '\r': out<<"\\r"; break;
		case '\n': out<<"\\n"; break;
		case '\t': out<<"\\t"; break;
		case '\f': out<<"\\f"; break;
		case '\\': out<<"\\\\"; break;
		case '\'': out<<"\\\'"; break;
		case '\"': out<<"\\\""; break;
		case '\b': out<<"\\b"; break;
		default: out.put(str[i]); break;
		}
		++i;
	}
	return out;
}

int main (int argc, char *argv[]) {
	cout<<"Number of arguments: "<<argc<<endl;
	for (int i=0; i<argc; ++i) cout<<"argv["<<i<<"] = \""<<str_escape(argv[i])<<'\"'<<endl;
}
