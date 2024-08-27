#ifndef LATITUDE_PARSER_H
#define LATITUDE_PARSER_H

#include <AbstractParser.h>

class LatitudeParser : public AbstractParser
{
    public:

	LatitudeParser();
	~LatitudeParser();

	/** Concrete class implementation of the public virtual Parse() method. */
	void Parse(const unsigned int& word);

	/** Concrete class implementation of the public virtual Print() method. */
	void Print();

    protected:


    private:

	const float LAT_BIT_RES[20] =
	{
	    0.000172f,
	    0.000344f,
	    0.000686f,
	    0.001373f,
	    0.002746f,
	    0.005493f,
	    0.010986f,
	    0.021972f,
	    0.043945f,
	    0.087890f,
	    0.175781f,
	    0.351562f,
	    0.703125f,
	    1.40625f,
	    2.8125f,
	    5.6255f,
	    11.25f,
	    22.5f,
	    45.0f,
	    90.0f
	};
};


#endif
