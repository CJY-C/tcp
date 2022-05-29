#pragma once 

#include "TcpNode.h"

namespace Network {

class ServerNode
: public TcpNode
{
public:
    ServerNode() = default;
    ServerNode(std::string name) : TcpNode(name){}
    ~ServerNode() = default;
private:
    
};

}
