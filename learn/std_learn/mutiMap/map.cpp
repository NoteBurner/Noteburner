#include <map>
#include <iostream>

using namespace std;





int main(){
	multimap<string,int> m;
	m.insert(make_pair("�ŷ�", 80));
	m.insert(make_pair("����", 70));
	m.insert(make_pair("�ŷ�", 40));
	m.insert(make_pair("����", 30));

	for (multimap<string, int>::iterator it = m.begin(); it != m.end(); it++) {
		cout << (*it).first << " " << (*it).second;
	}
	
	return 0;
}