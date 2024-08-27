#ifndef INDICATED_AIRSPEED_PARSER_H
#define INDICATED_AIRSPEED_PARSER_H

#include <AbstractParser.h>

class IndicatedAirspeedParser : public AbstractParser
{
    public:

	IndicatedAirspeedParser();
	~IndicatedAirspeedParser();

	/** Concrete class implementation of the public virtual Print() method. */
	void Print();

    protected:


    private:


};


#endif
