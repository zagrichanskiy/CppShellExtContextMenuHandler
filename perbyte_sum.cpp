#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <new>
using namespace std;

class readfile {
private:
	fstream fin;
	enum { length = 4 };

	unsigned long long process() {
		unsigned long long sum = 0;
		unsigned int temp;
		char * buff = new (&temp) char[length];
		while (fin.read(buff, length))
			sum += temp;
		return sum;
	}
public:
	readfile(const char * filename = "input.txt") : fin(fstream(filename)) {}
	readfile(const string & filename) : fin(fstream(filename.c_str())) {}
	readfile(const readfile &) = delete;
	readfile & operator=(const readfile &) = delete;
	operator unsigned long long() {	return process(); }
	unsigned long long operator()() { return process(); }
	void operator()(unsigned long long & result) { result = process(); }
};

int main() {
	//fstream file("test.txt", ios_base::out | ios_base::binary);
	//unsigned int value = UINT_MAX;
	//file.write((const char *) &value, sizeof(value));
	//file.close();
	//unsigned long long new_value = readfile("test.txt");
	//cout << value << endl
	//	 << new_value << endl;

	cout << "Enter name of file: ";
	string filename;
	getline(cin, filename);
	unsigned long long sum = readfile(filename);
	cout << "byte sum: " << sum << endl;

	return 0;
}