#ifndef HEADING_PARSER_H
#define HEADING_PARSER_H

#include <AbstractParser.h>

class HeadingParser : public AbstractParser
{
    public:

	HeadingParser();
	~HeadingParser();

	/** Concrete class implementation of the public virtual Print() method. */
	void Print();

    protected:


    private:


};


#endif
