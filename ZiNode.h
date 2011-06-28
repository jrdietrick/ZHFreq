#pragma once

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
	~ZiNode(void);
};
