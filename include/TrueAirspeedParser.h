#ifndef TRUE_AIRSPEED_PARSER_H
#define TRUE_AIRSPEED_PARSER_H

#include <AbstractParser.h>

class TrueAirspeedParser : public AbstractParser
{
    public:

	TrueAirspeedParser();
	~TrueAirspeedParser();

	/** Concrete class implementation of the public virtual Print() method. */
	void Print();

    protected:


    private:


};


#endif
