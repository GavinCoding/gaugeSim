#ifndef ALTITUDE_PARSER_H
#define ALTITUDE_PARSER_H

#include <AbstractParser.h>

class AltitudeParser : public AbstractParser
{
    public:

	AltitudeParser();
	~AltitudeParser();

	/** Concrete class implementation of the public virtual Print() method. */
	void Print();

    protected:


    private:


};


#endif
