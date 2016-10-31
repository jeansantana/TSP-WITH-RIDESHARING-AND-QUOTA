#include <bits/stdc++.h>
#include "gurobi_c++.h"
#include <unistd.h>
#include <csignal>

using namespace std;

#define FORR(i, a, b) for (int i = a; i < b; ++i)
#define FOR(i, a) FORR(i, 0, a)
#define pb push_back

typedef pair<int, int> ii;
typedef vector<int> vi;

double d[1000][1000]; // Cost matrix
double T[1000]; // passenger's minimum fare
int E[1000]; // Embarking vertice (postion) and disembarkation (list content)
int min_bonus; // minimum bonus quota
int bonus[1000]; // bonus list
int start = 0; // start vertex
const int TIME_LIMIT = 80000;

GRBEnv env = GRBEnv();
GRBModel model = GRBModel(env);

void terminate(int) { 
	model.terminate();
}

class mycallback: public GRBCallback {

	protected:
	
		void callback () {
			//cout << "WHERE HERE: " << where << endl;    	
	    }
};

int main(int argc, char const *argv[]) {

	std::signal(SIGALRM, terminate);
	// 80000 sec = 22,22 hours
    alarm(TIME_LIMIT);

	FILE * unused __attribute__((unused));
	unused = freopen (argv[1], "r", stdin);

	int n, p, r;

	cin >> n >> p >> r;

	double d[n][n];

	FOR(i, n) {
		FOR(j, n) {
			int aux;
			cin >> aux;
			d[i][j] = aux;
		}
	}

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
	
	try {
    	// definindo as vars

		GRBVar x[n][n]; //x_{ij}
		GRBVar v[p][n][n]; //v^l_{ij}
		GRBVar u[n];
		// GRBVar e[p][n]; // var to control the embark of the passenger l in vertice i
		// lienarização da restrição da tarifa
		// GRBVar w[n][n]; // continua [0, 1]
		// GRBVar g[n][n]; // continua [0, 1] 
		// GRBVar beta[n][n]; // continua [0, 1]

		// Linearização da função objetivo
		// GRBVar y[n][n];
		// GRBVar f[n][n];
		// GRBVar alfa[n][n];

		GRBVar alfa[n][n];
		
		FORR(i, 1, n) {
			u[i] = model.addVar(0.0, n-2, 0.0, GRB_INTEGER, "u_" + to_string(i));
		}

		/*FOR(l, p) {
			FOR(i, n) {
				e[l][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "e_" + to_string(l) + to_string(i));
			}
		}*/

		FOR(i, n) {
			FOR(j, n) {
				if (i != j) {
					
					string si = to_string(i);
					string sj = to_string(j);

					x[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x_" + si + sj);
					FOR (k, p) {
						v[k][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "v_" + to_string(k) + "_" + si + sj);
					}

					alfa[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "alfa_" + si + sj);
					/*beta[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "beta_" + si + sj);
					g[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "g_" + si + sj);
					w[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "w_" + si + sj);
					
					alfa[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "alfa_" + si + sj);
					y[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "y_" + si + sj);
					f[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "f_" + si + sj);*/

				}
			}
		}

		model.update();
		// restrição de chegada de vertices
	 	GRBLinExpr expr = 0;

	 	FORR(j, 1, n) {
	 		FOR(i, n) {
	 			if (i != j) expr+= x[i][j];
	 		}
	 		model.addConstr(expr <= 1, "c1_" + to_string(j));
	 		expr = 0;
	 	}

	 	expr = 0;
	 	// restrição de saída de vertices
	 	FORR(i, 1, n) {
	 		FOR(j, n) {
	 			if (i != j) expr+= x[i][j];
	 		}
	 		model.addConstr(expr <= 1, "c2_" + to_string(i));
	 		expr = 0;
	 	}
	 	// restrição de saída em 1
	 	expr = 0;

	 	FORR(i, 1, n) {
	 		expr+= x[0][i];
	 	}
	 	model.addConstr(expr == 1, "c3");

	 	// restrição de chegada em 1
	 	expr = 0;

	 	FORR(i, 1, n) {
	 		expr+= x[i][0];
	 	}
	 	model.addConstr(expr == 1, "c4");

	 	// # de vérices de entrada == #saída

	 	GRBLinExpr exp1 = 0, exp2 = 0;

	 	FORR(j, 1, n) {
	 		FORR(i, 0, n) {
	 			if (i != j) {
					exp1+= x[i][j];
	 				exp2+= x[j][i];
	 			}
	 		}
	 		model.addConstr(exp1 - exp2 == 0, "c5_" + to_string(j));
	 		exp2 = exp1 = 0;
	 	}

	 	// Restrição de subciclo

	 	expr = 0;

	 	FORR(j, 1, n) {
	 		FORR(i, 1, n) {
	 			if (i != j) {
	 				expr = u[i] - u[j] + (n - 1) * x[i][j];
					model.addConstr(expr <= n - 2, "c6_" + to_string(i) + to_string(j)); 
	 			}
	 		}
	 	}

	 	// Coleta mínima

	 	expr = 0;
	 	FOR(i, n) {
	 		FOR(j, n) {
	 			if (i != j) {
	 				expr+= x[i][j] * bonus[i];
	 			}
	 		}
	 	}
	 	model.addConstr(expr >= min_bonus, "c7");

	 	
	 	// Capacidade do carro
	 	expr = 0;
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
	 		//model.addConstr(e2 == 0, "c11_" + to_string(l));
	 		e1 = e2 = 0;
	 	}

	 	/*// embark constraint
	 	FOR (l, p) {
	 		GRBLinExpr exp = 0;
	 		FOR (i, n) {
	 			FOR (j, n) {
	 				if (i != j)
	 					exp+= v[l][i][j];
	 			}
	 			model.addConstr(e[l][i] - exp == 0, "c13_" + to_string(l) + to_string(i));
	 			exp = 0;
	 		}
	 	}*/

	 	// e1 = e2 = 0;

	 	// FOR(l, p) {
	 	// 	FOR(i, n) {
	 			
	 	// 		if (i != l)
	 	// 			e1+= v[l][l][i];
	 	// 		if (i != E[l])
	 	// 			e2+= v[l][i][E[l]]; 
	 			
	 	// 	}
	 	// 	model.addConstr(e1 - e2 == 0, "c11_" + to_string(l));
	 	// 	e1 = e2 = 0;
	 	// }

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
	 	// FOR(l, p) {
 		FOR(i, n) {
			FOR(j, n) {
				if (i != j) {
					foExpr+= x[i][j] * d[i][j] * alfa[i][j];
					// caixeiro
					//expr+= x[i][j] * d[i][j];
				}
			}
		}
	 	// }

	 	model.setObjective(foExpr, GRB_MINIMIZE);

	 	// mycallback cb = mycallback();

	    //model.setCallback(&cb);

	 	model.optimize();
	 	
	 	/*FOR(i, n) {
	 		FOR(j, n) {
	 			if ( i != j ) {
	 				cout << x[i][j].get(GRB_StringAttr_VarName) << " = " 
	 					 << x[i][j].get(GRB_DoubleAttr_X) << " " ;
	 			}
	 		}
	 		cout << endl;
	 	}*/

	 	vi path(n, 0);
	 	//path[0] = 0;
	 	// ii start(0, 0);
	 	FORR(i, 1, n)
	 		path[ ((int) u[i].get(GRB_DoubleAttr_X)) + 1] = i;

	 	cout << "\nSolution:\n";

	 	cout << "0 ";
	 	int pathSize = 1;
	 	FORR (i, 1, path.size()) {
 			if ( path[i] != 0 ) {
 				cout << path[i] << " ";
 				pathSize++;
 			}
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
	                if (i != j && v[l][i][j].get(GRB_DoubleAttr_X) == 1) {
	                	// cout << l << "_" << i << "_" << j << endl;
	                	emb[ mapRoute[l] ] = 1;
	                }
	            }
	        }
	    }
    	
    	FOR(i, pathSize) {
    		cout << emb[i] << " ";
    	}
    	cout << endl;

    	cout << "\nCost: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    	// cout << "Upper Bound: " << model.get(GRB_DoubleAttr_UB) << endl;
		cout << "Lower Bound: " << model.get(GRB_DoubleAttr_ObjBound) << endl;

    	// int c = 0;
	    // FOR(i, n) {
	    //     FOR(j, n) {
	    //         FOR(l, p) {
	    //             if (i != j && v[l][i][j].get(GRB_DoubleAttr_X) == 1) {
	    //                 cout << l << " " << "(" << i << ", " << j << ")\n";
	    //                 c++;
	    //             }
	    //         }
	    //     }
	    // }

	} catch (GRBException e) {
    	cout << "Error code = " << e.getErrorCode() << endl;
    	cout << e.getMessage() << endl;
  	} catch(...) {
    	cout << "Exception during optimization" << endl;
  	}

	return 0;
}