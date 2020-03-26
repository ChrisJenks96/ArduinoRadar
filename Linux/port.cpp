#include "port.h"

int Port::Poll()
{
    return -1;
}

void Port::Get()
{

}

void Port::Send()
{

}

int Port::Scan()
{
    COMID = -1;
    for (COMID = 0; COMID < COMMax; COMID++){
        //poll the connected port, if we get some data back, success
        if (this->Poll() != -1)
            return COMID;
    }

    //reset COM ID as there we no successful results
    COMID = -1;
    return COMID;
}