#ifndef ISYNCHRONIZE_H
#define ISYNCHRONIZE_H

#include <iostream>

using namespace std;

class ISynchronize
{
public:
    ISynchronize() =  default;
    virtual ~ISynchronize() =  default;
    virtual void synchronize() = 0;
};

#endif // !ISYNCHRONIZE_H