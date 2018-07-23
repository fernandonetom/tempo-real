//teste.cpp
#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include "pexec.h"

using namespace std;

string escape(string s) {
	string r="\"";
	string::iterator e=s.end();
	for (string::iterator i=s.begin(); i!=e; ++i) {
		switch (*i) {
		case '\n': r+="\\n"; break;
		case '\r': r+="\\r"; break;
		case '\f': r+="\\f"; break;
		case '\b': r+="\\b"; break;
		case '\a': r+="\\a"; break;
		case '\t': r+="\\t"; break;
		case '\\': r+="\\\\"; break;
		case '\"': r+="\\\""; break;
		case '\'': r+="\\\'"; break;
		default: r+=*i; break;
		}
	}
	return r+"\"";
}

int main() {
	const char *argv[] = {"./printargv","123","456","789",0};
	int o;
	int p = pexec(argv[0],argv,0,&o,0);
	
	if (p>0) {
		cout<<"Child's PID = "<<p<<endl;
		char buff[10*1024];
		int l;
		do {
			l = read(o, buff, 10*1024-1);
			if (l>0) {
				buff[l]=0;
				cout<<"STDOUT from Child: "<<escape(buff)<<endl;
			}
		} while (l>0);
		if (l<0) cout<<"Erro: "<<l<<endl;
	}
	
	return 1;
}
