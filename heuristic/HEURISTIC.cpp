#include <bits/stdc++.h>
#include "gurobi_c++.h"
#include "../commons/LKHParser.h"
#include "../metaheuristics/common/MersenneTwister.h"
#include "../commons/Utils.h"

using namespace std;

#define FORR(i, a, b) for (int i = a; i < b; ++i)
#define FOR(i, a) FORR(i, 0, a)
#define pb push_back

typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector< vi > vvi;
typedef vector<double> vd;
typedef vector< vd > vvd;
typedef vector<ii> vii;

// double d[1000][1000]; // Cost Matrix
vvd d; // Cost matrix
double T[1000]; // passenger's minimum fare
int E[1000]; // Embarking vertice (postion) and disembarkation (list content)
int min_bonus; // minimum bonus quota
int bonus[1000]; // bonus list
int start = 0; // start vertex
int instanceType = 1;

// storage
vi costs;
vi times;
vi lowerbds;

const string LKH_PATH = "../../commons/LKH-2.0.7/";
const string LK_FILES_PATH = "../../commons/LK_FILES/";

GRBEnv env = GRBEnv();
GRBModel model = GRBModel(env);

void heuristic(string filename, int n, int p, int r);

int main(int argc, char const *argv[]) {
	
	FILE * unused __attribute__((unused));
	unused = freopen (argv[1], "r", stdin);

	FileHelper fileHelper;
	// reading file
	string filename = fileHelper.getFileNameFromPath( string(argv[1]) );

	stringstream sst;
    sst << argv[2];
    sst >> instanceType; sst.clear();

	int n, p, r;

	cin >> n >> p >> r;

	d = vvd(n, vd(n, 0.0));
	FOR(i, n) {
		// d.pb(vd());
		FOR(j, n) {
			cin >> d[i][j];
		}
	}

	/*FOR(i, n) {
		// d.pb(vd());
		FOR(j, n) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}*/

	FOR(i, p) {
		double a; 
		int b, c;
		cin >> a >> c >> b;
		T[i] = a;
		E[i] = b;
	}	

	FOR(i, n) {
		bonus[i] = 0;
	}

	cin >> min_bonus;

	FOR(i, n) {
		int vertice, bonus_value;
		cin >> vertice >> bonus_value;
		bonus[vertice] = bonus_value;
	}

	// start city
	bonus[start] = 0;
	
	heuristic(filename, n, p, r);
	
	return 0;
}

int compareCity(ii a, ii b) {
    return a.first - b.first;
}

int _compare(const void* a, const void* b) {
	return *(int*) a - *(int*) b;
}

bool bonusCmp(ii b1, ii b2) {
	// return b1.second >= b2.second;
	return b1.second <= b2.second;

}

int findCity(ii cityBonus, vii citiesBonus) {
    int found = -1;
    for (int i = 0; i < citiesBonus.size() && found == -1; i++) {
        if ( compareCity(cityBonus, citiesBonus[i]) == 0 ) {
            found = i;
        }
    }
    return found;
}

// for debug
void print_vii(vii v) {
	cout << "Vector (city, bonus): \n";
	for (int i = 0; i < v.size(); ++i) {
		cout << i << " - (" << v[i].first << ", " << v[i].second << ")" << endl;
	}
	cout << endl;
}

void printVi(vi v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}

// return the positions of the chosen city
int roullete(vii B) {
	
	double totalSum = 0.0;
	
	for (int i = 0; i < B.size(); i++) {
		totalSum+= B[i].second;
	}

	vd sum;
	sum.push_back( (double) B[0].second / totalSum );

	for (int i = 1; i < B.size(); i++) {
		sum.push_back( (double) (sum[i - 1] + B[i].second) / totalSum );
	}

	double guess = (double) MersenneTwister::getInstance()->getRand(1, MAX_VALUE_PROB)/MAX_VALUE_PROB;

    return utils::binarySearch(guess, sum, _compare);

}

bool removeCity(vii &B, ii cityBonus) {
	int idx = findCity(cityBonus, B);
    if (idx >= 0 && idx < B.size()) {
        B.erase(B.begin() + idx);
        return true;
    }
    return false;
}

vi semiGreedyRoute(vii B, int bonusTarget) {
	vi route;
	route.pb(0);
	int currBonus = 0;
	
	while (currBonus < bonusTarget) {
		int chosenOne = roullete(B);
		route.pb(B[chosenOne].first);
		//print_vii(B);
		currBonus+= B[chosenOne].second;
		removeCity(B, B[chosenOne]);
	}

	return route;
}

vi twoOpt(vi route, int i, int k) {
	/*cout << "i = " << i << " k =" << k << endl;
    printVi(route);*/
    reverse(route.begin() + i, route.begin() + k + 1);
    /*printVi(route);
    cout << "-------\n";*/
    return route;
}

double getTSPCost(vi route) {
	double cost = 0.0;
	for (int i = 0; i < route.size() - 1; ++i) {
		cost+= d[route[i]][route[i + 1]];
	}
	return cost + d[route.back()][route[0]];
}

vi localSearch2Opt(vi route) {
	vi best = route;
    for (int i = 1; i < route.size(); ++i) {
        for (int k = i + 1; k < route.size(); ++k) {
            vi candidate = twoOpt(best, i, k);
            /*cout << (bool) (candidate == best) << endl;
            printVi(candidate);
            cout << "candidate: " << getTSPCost(candidate) << endl;
            cout << "best: " << getTSPCost(best) << endl;
            printVi(best);
            cout << "-------------\n\n\n";*/
            if (getTSPCost(candidate) < getTSPCost(best)) {
                best = candidate;
            }
        }
    }
    return best;
}

void heuristic(string filename, int n, int p, int r) {

	try {
    	// definindo as vars

		int x[n][n]; //x_{ij}
		GRBVar v[p][n][n]; //v^l_{ij}

		GRBVar alfa[n][n];

		FOR(i, n) {
			FOR(j, n) {
				if (i != j) {
					
					string si = to_string(i);
					string sj = to_string(j);

					//x[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_" + si + sj);

					FOR (k, p) {
						v[k][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "v_" + to_string(k) + "_" + si + sj);
					}

					alfa[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "alfa_" + si + sj);

				}
			}
		}

		model.update();

		// initializing the variable set xij

		// Lin–Kernighan heuristic for get a good route to optimize the boarding

		// Vertices greedy selection to build path
		// Get the first route
		vi gvertices;
		//gvertices.pb(0);

		vii B; // (city c id, bonus to be collected in city c)
		FORR (i, 1, n) {
			B.pb( ii(i, bonus[i]) );
		}

		/*cout << "\nVERTICES SORTED BY BONUS:\n";
		FOR (i, n) {
			cout << B[i].first << ", " << B[i].second << endl;
		}*/
		//print_vii(B);
		sort(B.begin(), B.end(), bonusCmp);
		//print_vii(B);
		//cout << "Start here: \n";
		gvertices = semiGreedyRoute(B, min_bonus);


		/*int totalBonus = 0;
		FOR(i, B.size()) {
			if (totalBonus < min_bonus) {
				if (B[i].first != 0) {
					gvertices.pb(B[i].first);
					totalBonus+= B[i].second;
				}
			} else {
				break;
			}
		}*/

		/*cout << "VERTICES SET:\n";
		FOR(i, gvertices.size()) {
			cout << gvertices[i] << " ";
		}
		cout << "\n";*/

		// improve the route by using a local search
		vi path;
		if (instanceType != 2) {
			LKHParser parser(LKH_PATH, LK_FILES_PATH, gvertices, d, filename, string("TSP"));
    		path = parser.LKHSolution();
    	} else {
    		path = localSearch2Opt(gvertices);
    	}

		/*cout << "LK ROUTE: " << endl;
		FOR (i, path.size()) {
			cout << path[i] << " ";
		}
		cout << endl;*/

		// init the x matrix		
		FOR(i, n) {
			FOR (j, n) {
				x[i][j] = 0;
			}
		}

		// vi path = {0, 19, 7, 9, 1, 13, 12, 5, 8, 4, 6, 3, 14, 15, 10, 16, 18, 11, 2, 17};
		// vi path = {0, 1, 4, 3, 2};

		FOR(i, path.size() - 1) {
			x[ path[i] ][ path[i + 1] ] = 1;
		}
		x[ path.back() ][0] = 1;

		/*FOR (i, n) {
			FOR (j, n) {
				cout << x[i][j] << " ";
			}
			cout << endl;
		}*/
	 	
	 	// Capacidade do carro
	 	GRBLinExpr expr = 0;
	 	FOR(i, n) {
	 		FOR(j, n) {
	 			FOR(l, p) {
	 				if (i != j) {
	 					expr+= v[l][i][j];
	 				}
	 			}
	 			if (i != j) model.addConstr(expr <= r * x[i][j], "c8");
	 			expr = 0;
	 		}
	 	}

	 	// pagamento de tarifa

	 	GRBLinExpr expq = 0;
	 	FOR(i, n) {
	 		FOR(j, n) {
	 			if (i != j) {
	 				FOR(l, p) {
	 					expq+= v[l][i][j];	 				
	 				}
	 				GRBQuadExpr e = (expq + 1) * alfa[i][j];
	 				model.addQConstr( e == 1, "c81_" + to_string(i) + to_string(j) );
	 				expq = 0;
	 			}
	 		}
	 	}

	 	GRBQuadExpr quad = 0;
	 	FOR(l, p) {
	 		FOR(i, n) {
	 			FOR(j, n) {
	 				if (i != j)
	 					quad+= v[l][i][j] * d[i][j] * alfa[i][j];
	 			}
	 		}
	 		model.addQConstr(quad - T[l] <= 0, "c82_" + to_string(l));
	 		quad = 0;
	 	}

	 
	 	// Embarque e desembarque

	 	GRBLinExpr chegadai = 0, saidai = 0;

	 	FOR(l, p) {
	 		FOR(i, n) {
	 			FOR(j, n) {
	 				if (i != j && i != l && i != E[l]) {
	 					chegadai+= v[l][j][i];
	 					saidai+= v[l][i][j];
	 				}
	 			}
	 			model.addConstr(chegadai - saidai == 0, "c9_" + to_string(l) + to_string(i));
	 			chegadai = saidai = 0;
	 		}
	 	}

	 	GRBLinExpr e1 = 0;
	 	GRBLinExpr e2 = 0;
	 	FOR(l, p) {
	 		FOR(i, n) {
	 			if (i != l ) {
	 				e1+= v[l][i][l];
	 			}
	 			if (i != E[l]) {
	 				e2+= v[l][E[l]][i];
	 			}
	 		}
	 		model.addConstr(e1 + e2 == 0, "c10_" + to_string(l));
	 		e1 = e2 = 0;
	 	}

	 	// Origem: l; destino: E[l]
	 	GRBLinExpr sum = 0;
	 	FORR(l, 1, p) {
 			FORR(i, 1, n) {
 				sum+= v[l][0][i];
 			}
	 		model.addConstr(sum == 0, "c12_" + to_string(l));
	 		sum = 0;
	 	}	 	

	 	// ===== OBJECTIVE FUNCTION ====
	 	GRBQuadExpr foExpr = 0;
 		FOR(i, n) {
			FOR(j, n) {
				if (i != j) {
					foExpr+= x[i][j] * d[i][j] * alfa[i][j];
				}
			}
		}

	 	model.setObjective(foExpr, GRB_MINIMIZE);

	 	model.getEnv().set(GRB_IntParam_OutputFlag, 0);

	 	model.optimize();
		
		cout << "Solution:\n";

	 	FOR(i, path.size()) {
	 		cout << path[i] << " ";
	 	}
	 	cout << endl;

	 	vi emb;
	 	emb.assign(n, 0);

	 	// preprocessing on route

	 	map<int, int> mapRoute;
	 	FOR(i, n) {
	 		mapRoute.insert( ii(path[i], i) );
	 	}

	 	//int c = 0;
	    FOR(l, p) {
	        FOR(i, n) {
	            FOR(j, n) {
	                if (i != j && v[l][i][j].get(GRB_DoubleAttr_X) != 0) {
	                	// cout << l << "_" << i << "_" << j << endl;
	                	emb[ mapRoute[l] ] = 1;
	                }
	            }
	        }
	    }
    	
    	/*FOR(i, path.size()) {
    		cout << emb[i] << " ";
    	}
    	cout << endl;*/

    	cout << "Cost: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    	// cout << "Upper Bound: " << model.get(GRB_DoubleAttr_UB) << endl;
		cout << "Lower Bound: " << model.get(GRB_DoubleAttr_ObjBound) << endl;

		// cout << "Time: " << model.get(GRB_DoubleAttr_Runtime) << endl;
		// printf("Time: %.2f\n", model.get(GRB_DoubleAttr_Runtime));

	} catch (GRBException e) {
    	cout << "Error code = " << e.getErrorCode() << endl;
    	cout << e.getMessage() << endl;
  	} catch(...) {
    	cout << "Exception during optimization" << endl;
  	}
}