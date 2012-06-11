#ifndef TESTMATCHING_H
#define TESTMATCHING_H

#include "autotest.h"

class TestMatching : public QObject
{
Q_OBJECT
private slots:
    void matchFrechet();
};



DECLARE_TEST(TestMatching);


#endif // TESTMATCHING_H
