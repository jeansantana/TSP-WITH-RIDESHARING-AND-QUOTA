#include "LKHParser.h"

LKHParser::LKHParser(string LKH_PATH, string LK_FILES_PATH, vector<int> set, vector < vector<double> > costs, string nameInstance, string tspType) {
    this->set = set;
    this->nameInstance = nameInstance;
    this->costs = costs;
    this->tspType = tspType;
    this->LKH_PATH = LKH_PATH;
    this->LK_FILES_PATH = LK_FILES_PATH;
}

void LKHParser::createTSPInstance() {

    // set = {0 3 1}

    /*cout << "cost matrix:\n";
    for (int i = 0; i < costs.size(); ++i) {
        for (int j = 0; j < costs.size(); ++j) {
            cout << costs[i][j] << " ";
        }
        cout << endl;
    }*/

    vector< vector<int> > m;

    for (int i = 0; i < this->set.size(); ++i) {
        m.push_back( vector<int>( this->set.size(), 0.0 ) );
    }

    string matrix = "";
    // create the matrix doing the mapping
    for (int i = 0; i < this->set.size(); ++i) {
        for (int j = 0; j < this->set.size(); ++j) {
            /*cout << "position:\n";
            cout << i << "," << j << endl;
            cout << "Set content: \n";
            cout << set[i] << " and " << set[j] << endl;*/
            m[i][j] = (int) (this->costs[ this->set[i] ][ this->set[j] ] + 0.5);
            matrix+= to_string(m[i][j]) + " ";
        }
        matrix+= "\n";
    }

    string head =   "NAME: " + this->nameInstance + "\n" +
                    "TYPE: "+ this->tspType + "\n" +
                    "COMMENT: TSP\n" +
                    "DIMENSION: " + to_string(this->set.size()) + "\n" +
                    "EDGE_WEIGHT_TYPE: EXPLICIT\n" +
                    "EDGE_WEIGHT_FORMAT: FULL_MATRIX\n" +
                    "EDGE_WEIGHT_SECTION\n";

    string fileTsp = head + matrix + "EOF\n";

    string filePar =    "PROBLEM_FILE = " + LK_FILES_PATH + this->nameInstance + ".tsp\n" + 
                        "RUNS = 10\n" + 
                        "TOUR_FILE = " + LK_FILES_PATH + this->nameInstance + ".tour\n";

    FileHelper fhelper;
    // create .tsp file
    fhelper.createFile(this->nameInstance + ".tsp", fileTsp, LK_FILES_PATH);
    // create .par file
    fhelper.createFile(this->nameInstance + ".par", filePar, LK_FILES_PATH);

}

vector<int> LKHParser::LKHSolution() {
    this->createTSPInstance();
    // exec the LKH and creates the .tour solution file
    string cmd = "./" + LKH_PATH + "LKH " + LK_FILES_PATH + this->nameInstance + ".par";
    int res = system(cmd.c_str());
    // cout << res << endl;
    // now we able to read that file
    FileHelper fhelper;
    string tour = fhelper.readFile(LK_FILES_PATH + this->nameInstance + ".tour");

    vector<int> tr;

    int begin = tour.find("TOUR_SECTION") + string("TOUR_SECTION").size() + 1;
    int end = tour.find("EOF");
    string number = "";
    for (int i = begin; i < end-1; i++) {
        if (tour[i] != '\n') {
            number+= tour[i];
        } else {
            // the LKH solution starts in 1, so here, we start from 0
            int val = stoi(number.c_str()) - 1;
            int putVal = set[ val ];
            tr.push_back(putVal);
            number = "";
        }
    }

    return tr;

}