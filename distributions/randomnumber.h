#ifndef RANDOMNUMBER_H
#define RANDOMNUMBER_H


class RandomNumber {

public:
	
	RandomNumber();
	~RandomNumber();
    virtual double Generate() =0;
	void SetSeed(long);
	long GetSeed();
    
protected:

    long present_seed;
    long seed;
};

#endif // RANDOMNUMBER_H
