#include <iostream>
#include "FileHelper.h"
#include "LKHParser.h"
#include <vector>
#include <string>


using namespace std;

int main() {
    /*FileHelper fh;
    fh.createFile("teste.txt", "Isso é um teste!\nOlá!", "/home/jeansilva/");

    string conteudo = fh.readFile("/home/jeansilva/teste.txt");

    cout << conteudo << endl;*/

	vector<double> a = {0, 10, 20, 50, 20};
	vector<double> b = {50, 0, 40, 60, 10};
	vector<double> c = {40, 10, 0, 70, 10};
	vector<double> d = {10, 20, 10, 0, 20};
	vector<double> e = {80, 30, 50, 40, 0};

	vector<int> set = {0, 4, 1};
	vector< vector<double> > m;

	m.push_back(a);
	m.push_back(b);
	m.push_back(c);
	m.push_back(d);
	m.push_back(e);

	// how to use the parser
    LKHParser parser(set, m, string("teste"), string("ATSP"));
    vector<int> tour = parser.LKHSolution();

	for (int i = 0; i < tour.size(); i++) {
		cout << tour[i] << " ";
	}
	cout << endl;

    return 0;
}