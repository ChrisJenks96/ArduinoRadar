#ifndef PORT_H
#define PORT_H

class Port
{
    public:
        Port(){COMID = 0; COMMax = 20;}
        Port(unsigned int COMMax){this->COMMax = COMMax; COMID = 0;}
        void Get();
        void Send();
        int GetCOMID(){return COMID;}
        int Scan();
        ~Port(){}
    private:
        //the maximum amount of com ports available
        unsigned int COMMax;
        int COMID;
        //private function that Scan uses, nobody else.
        int Poll();
};

#endif