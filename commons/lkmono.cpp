/* 
 * Author: Emerson Carvalho
 */

#include <bits/stdc++.h>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;

class edge {
public:
	int first;
	int second;

	edge(int f, int s) {
		this->first = f;
		this->second = s;
	}

	int getCost(vvi cost) {
		return cost[first][second];
	}

	bool operator==(edge e) {
		if (first == e.first && second == e.second
				|| first == e.second && second == e.first)
			return true;

		return false;
	}
};

class solution {
private:
	vi sol;
	int cost;

public:

	solution(vi s, vvi matCost) {
		sol = s;
		generateCosts(matCost);
	}

	solution() {

	}

	solution(const solution &s) {
		sol = s.getSol();
		cost = s.getCost();
	}

	vi getSol() const {
		return sol;
	}

	void swapRange(int i, int j, vvi matCost) {
		if (j < i) {
			int aux = i;
			i = j;
			j = aux;
		}

		int i_ = (i - 1 < 0) ? sol.size() - 1 : i - 1;
		int j_ = (j + 1) % sol.size();

		int c1aux = cost - matCost[sol[i]][sol[i_]] - matCost[sol[j]][sol[j_]];
		//int c2aux = cost2 - costB[ sol[i] ][ sol[i_] ] - costB[ sol[j] ][ sol[j_] ];

		int a = i;
		int b = j;

		while (a < b) {
			int aux = sol[a];
			sol[a] = sol[b];
			sol[b] = aux;
			a++;
			b--;
		}

		cost = c1aux + matCost[sol[i]][sol[i_]] + matCost[sol[j]][sol[j_]];
		//cost2 = c2aux + costB[ sol[i] ][ sol[i_] ] + costB[ sol[j] ][ sol[j_] ];
	}

	int operator[](int i) {
		return sol[i];
	}

	int getCost() const {
		return cost;
	}

	bool operator==(solution s) {
		if (cost == s.getCost())
			return true;
		return false;
	}

	int size() const {
		return sol.size();
	}

	void shiftLeft() {
		int x = sol[0];
		for (int i = 1; i < sol.size(); i++) {
			sol[i - 1] = sol[i];
		}
		sol[sol.size() - 1] = x;
	}

	void generateCosts(vvi matCost) {

		cost = 0;
		//cost2 = 0;

		for (int i = 0; i < sol.size(); i++) {
			int a = sol[i];
			int b = sol[(i + 1) % sol.size()];

			cost += matCost[a][b];
			//cost2 += costB[a][b];
		}
	}

	void print() {
		for (int i = 0; i < sol.size(); i++) {
			printf("%i ", sol[i]);
		}
		printf("\n");
	}

	void printCost() {
		printf("%i\n", getCost());
	}
};

class LeanKernigan {
public:
	solution best;

	vvi marked;
	int G = 0;
	vvi costA;
	int alpha = 2;
	int n;

	vvi objectiveA;
	char *file1;

	void LK_mono_search(solution &s, int depth) {
		edge q(s[0], s[s.size() - 1]);

		marked[q.first][q.second] = marked[q.second][q.first] = 1;

		if (depth < alpha) {
			for (int i = 2; i < s.size() - 1; i++) {
				edge ne(s[0], s[i]);
				if (marked[ne.first][ne.second] == 0) {
					int g = q.getCost(costA) - ne.getCost(costA);
					if (G + g > 0) {
						G += g;
						marked[ne.first][ne.second] =
								marked[ne.second][ne.first] = 1;
						s.swapRange(0, i - 1, costA);

						if (s.getCost() < best.getCost()) {
							best = s;
							LK_mono_search(s, depth + 1);
						}

						s.swapRange(0, i - 1, costA);
						marked[ne.first][ne.second] =
								marked[ne.second][ne.first] = 0;
						G -= g;
					}
				}
			}

		} else {

			for (int i = 2; i < s.size() - 1; i++) {
				edge ne(s[0], s[i]);
				if (marked[ne.first][ne.second] == 0) {
					int g = q.getCost(costA) - ne.getCost(costA);

					if (G + g > 0) {
						s.swapRange(0, i - 1, costA);
						if (s.getCost() < best.getCost()) {
							best = s;
						}
						s.swapRange(0, i - 1, costA);
					}
				}
			}
		}

		marked[q.first][q.second] = marked[q.second][q.first] = 0;
	}

	solution LK_mono(solution s) {
		solution lastBest = s;
		best = s;

		for (int i = 0; i < s.size() - 1; i++) {
			marked.assign(s.size(), vi(s.size(), 0));
			G = 0;
			LK_mono_search(s, 0);

			if (best.getCost() < lastBest.getCost()) {
				do {
					lastBest = best;
					marked.assign(s.size(), vi(s.size(), 0));
					G = 0;
					LK_mono_search(lastBest, 0);
				} while (best.getCost() < lastBest.getCost());
			}
			s.shiftLeft();
		}
		return lastBest;
	}

	void readFiles() {				// Read files and define the global value n
		ifstream fileA;								//, fileB;
		vi aux(3);

		fileA.open(file1);
		//fileB.open(file2);

		while (fileA >> aux[0] >> aux[1] >> aux[2]) {
			objectiveA.push_back(aux);
		}

		/*while(fileB >> aux[0] >>aux[1] >> aux[2]){
		 objectiveB.push_back(aux);
		 }*/

		n = objectiveA.size();

		fileA.close();
		//fileB.close();
	}

	inline int distance(int x1, int y1, int x2, int y2) { //distance between two points
		return int(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) + 0.5);
	}

	void generateDistances() { 	// calculates the distance between all points
		int sz = objectiveA.size();

		costA.assign(sz, vi(sz));
		//costB.assign(sz, vi(sz));

		for (int i = 0; i < sz - 1; i++) {
			for (int j = i; j < sz; j++) { //  x1                y1                 x2              y2
				costA[i][j] = costA[j][i] = distance(objectiveA[i][1],
						objectiveA[i][2], objectiveA[j][1], objectiveA[j][2]);
				//costB[i][j] = costB[j][i] = distance(objectiveB[i][1], objectiveB[i][2], objectiveB[j][1], objectiveB[j][2]);
			}
		}

	}

	solution randomSolution() {
		vi aux(n, 0);
		vi sol(n, 0);
		int i = 0;
		while (i < n) {
			int x = rand() % n;
			if (aux[x] == 0) {
				sol[i] = x;
				i++;
			}
		}
		solution resp(sol, costA);
		return resp;
	}

	solution greedySolution() {

		vi marked(n, 0);
		vi s(n);

		int x = rand() % n;

		s[0] = x;
		marked[x] = 1;

		for (int i = 1; i < n; i++) {
			int costMin = INT_MAX;
			int position = -1;

			for (int j = 0; j < n; j++) {
				int cost = costA[s[i - 1]][j];      //*w1 + costB[s[i-1]][j]*w2;
				if (cost < costMin && marked[j] == 0) {
					costMin = cost;
					position = j;
				}
			}

			s[i] = position;
			marked[position] = 1;
		}

		solution resp(s, costA);

		return resp;
	}

	pair<int, vi> execute(int num_vertices, vvi m) {

		//file1 = argv[1];
		srand(time(NULL)); 			// Auxilia na geração de números randomicos
		//readFiles(); 								// read files with objectives and define the value n
		//generateDistances(); 						// genearates the matrix of distances between all vertices
		n = num_vertices;

		costA = m;

		solution inicial = greedySolution();

		solution final = LK_mono(inicial);

		return pair<int, vi>(final.getCost(), final.getSol());
		//cout << inicial.getCost() << " " << final.getCost() << endl;

		//return final.getCost();
	}
};
