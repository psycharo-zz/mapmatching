#include "testrangetree.h"



#include "rangetree.h"



#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;



void TestRangeTree::constructor()
{
    vector<UTMNode> points;

    for (int i = 0; i < 10; ++i)
    {
        points.push_back(UTMNode(i, i));
        for (int j = i+1; j < 10; ++j)
        {
            points.push_back(UTMNode(i, j));
            points.push_back(UTMNode(j, i));
        }
    }

    RangeTree tree(points);


}


void TestRangeTree::search()
{

    vector<UTMNode> points;

    for (int i = 0; i < 10; ++i)
    {
        points.push_back(UTMNode(i, i));
        for (int j = i+1; j < 10; ++j)
        {
            points.push_back(UTMNode(i, j));
            points.push_back(UTMNode(j, i));
        }
    }

    RangeTree tree(points);

    auto comp = [](const UTMNode &a, const UTMNode &b) { return (a.x == b.x) && (a.y == b.y); };
    auto orderComp = [](const UTMNode &a, const UTMNode &b)
    {
                     if (a.x < b.x)
                        return true;
                     else if (a.x == b.x)
                        return a.y < b.y;
                     return false;
    };

    list<UTMNode> result;
    set<UTMNode, decltype(orderComp)> resultSet(orderComp);
    set<UTMNode, decltype(orderComp)> expectedResultSet(orderComp);


    // test case 1
    result = tree.search(UTMNode(0, 0), 4, 4);
    for (int i = 0; i <= 4; ++i)
        for (int j = i; j <= 4; ++j)
        {
            expectedResultSet.insert(UTMNode(i, j));
            expectedResultSet.insert(UTMNode(j, i));
        }
    resultSet.clear();
    resultSet.insert(result.begin(), result.end());
    QVERIFY(equal(resultSet.begin(), resultSet.end(), expectedResultSet.begin(), comp));
    QVERIFY(equal(expectedResultSet.begin(), expectedResultSet.end(), resultSet.begin(), comp));

    // test case 2
    expectedResultSet.clear();
    expectedResultSet.insert(UTMNode(2, 2));
    expectedResultSet.insert(UTMNode(3, 2));
    resultSet.clear();
    result = tree.search(UTMNode(1.5, 1.5), 2, 1);
    resultSet.insert(result.begin(), result.end());
    QVERIFY(equal(resultSet.begin(), resultSet.end(), expectedResultSet.begin(), comp));
    QVERIFY(equal(expectedResultSet.begin(), expectedResultSet.end(), resultSet.begin(), comp));

    // test case 3
    expectedResultSet.clear();
    expectedResultSet.insert(UTMNode(2, 2));
    expectedResultSet.insert(UTMNode(2, 3));
    resultSet.clear();
    result = tree.search(UTMNode(1.5, 1.5), 1, 2);
    resultSet.insert(result.begin(), result.end());
    QVERIFY(equal(resultSet.begin(), resultSet.end(), expectedResultSet.begin(), comp));
    QVERIFY(equal(expectedResultSet.begin(), expectedResultSet.end(), resultSet.begin(), comp));


    // test case 4
    expectedResultSet.clear();
    expectedResultSet.insert(UTMNode(2, 2));
    resultSet.clear();
    result = tree.search(UTMNode(1.5, 1.5), 1, 1);
    resultSet.insert(result.begin(), result.end());
    QVERIFY(equal(resultSet.begin(), resultSet.end(), expectedResultSet.begin(), comp));
    QVERIFY(equal(expectedResultSet.begin(), expectedResultSet.end(), resultSet.begin(), comp));

}



