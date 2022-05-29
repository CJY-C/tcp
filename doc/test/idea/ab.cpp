#include <iostream>

class Ab
{
public:
    Ab() = default;
    virtual ~Ab() = default;

    virtual void Hello()
    {
    }

public:
    int id;
};

class Cab :public Ab
{
public:
    virtual void Hello() override
    {
        std::cout << "cid: " << cid << std::endl;
    }

public:
    int cid;
};

void foo(Ab* ab)
{
    ab->Hello();
}

int main()
{
    Cab cab;
    cab.cid = 123;
    foo(&cab);

    return 0;
}
