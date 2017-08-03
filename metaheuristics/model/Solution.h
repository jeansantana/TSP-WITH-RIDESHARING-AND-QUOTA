//
// Created by jeansilva on 03/08/16.
//

#ifndef TSPRQ_SOLUTION_H
#define TSPRQ_SOLUTION_H

#include "../common/macros.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

class Solution {

private:
    double cost;
    int quota;
    int problem;
    bool cmpTo(Solution other);

public:

    Solution(double cost, int quota);

    Solution(double cost, int quota, int problem);

    Solution(vi route, vi boarding);

    Solution();

    double getCost() const;

    void setCost(double cost);

    int getQuota() const;

    void setQuota(int quota);

    bool operator<(const Solution other);

    bool operator>(const Solution other);

    Solution solve(vi route, vi boarding);

    string toString();

    int getProblem() const;

    void setProblem(int problem);

    bool isValid();

};


#endif
