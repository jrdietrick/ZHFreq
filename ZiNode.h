#ifndef _ZINODE_H_
#define _ZINODE_H_

class ZiNode
{
    public:
        int rank;
        char data[3];
        double raw_freq;
        double centile;
        char fayin[10];
        int tone;

        ZiNode(int, char*, double, double, char*, int);
};

#endif
