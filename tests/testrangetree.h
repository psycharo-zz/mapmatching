#ifndef TESTRANGETREE_H
#define TESTRANGETREE_H

#include "autotest.h"

#include "graph.h"


class TestRangeTree : public QObject
{
Q_OBJECT
private slots:
    // constructor test
    void constructor();

    // search test
    void search();

};


//DECLARE_TEST(TestRangeTree)

#endif // TESTRANGETREE_H
