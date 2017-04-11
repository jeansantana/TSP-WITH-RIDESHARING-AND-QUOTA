#include <bits/stdc++.h>

using namespace std;

string formatNumeber(int num) {
	if (num < 10) 
		return "0" + to_string(num);
	else
		return to_string(num);
}

void createFile(string name, string content, string path) {
	ofstream outfile (path + name);
	outfile << content << endl;
	outfile.close();
}

int main(int argc, char const *argv[]) {
	
	int n, p, k;
	int E[1000];//Vértice de embarque (posição) e desembarque (conteúdo)
	double M[1000][1000];//Matriz de custos
	double T[1000];//Tarifa máxima por passageiro
	string content = "";
	int count = 1;
	
	while(cin >> n >> p >> k) {
		content = "";
		string str;
		cin.ignore();
		content+= to_string(n) + " " + to_string(p) + " " + to_string(k) + "\n\n";
		
		getline(cin, str);
		
		for (int i = 0; i < n; ++i) {
			getline(cin, str);
			content+= str + "\n";
		}
		content+= "\n";
		getline(cin, str);

		for (int i = 0; i < p; ++i) {
			getline(cin, str);
			content+= str + "\n";
		}

		getline(cin, str);

		int min_bonus;
		cin >> min_bonus;
		cin.ignore();
		content+= "\n" + to_string(min_bonus) + "\n";

		for (int i = 0; i < n - 1; ++i) {
			getline(cin, str);
			content+= str + "\n";
		}

		getline(cin, str);
		content+= str;
		createFile("TIC" + formatNumeber(count) + "-" + to_string(n) + "-" + to_string(p) + "-" + to_string(k) + ".in", content, "paper/");
		count++;
	}

	return 0;
}