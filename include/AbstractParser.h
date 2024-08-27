#ifndef ABSTRACT_PARSER_H
#define ABSTRACT_PARSER_H

class AbstractParser
{
    public:

	AbstractParser();

	virtual ~AbstractParser();

	/** Default parsing function for concrete class implementations. */
	virtual void Parse(const unsigned int& word);

	/** Pure virtual print method that is always called from the default Parse() function. */
	virtual void Print() = 0;
    
    protected:

	/** Setter method for the data/payload portion of an ARINC-429 message. */
	void		setDataBitLen(short value) {DATA_BIT_LEN = value;};

	/** Getter method for the data/payload portion of an ARINC-429 message. */
	const short&	getDataBitLen() {return DATA_BIT_LEN;};

	/** Setter method for bit resolution, as defined in the ARINC-429 standard. */
	void		setBitResolution(float value) {BIT_RESOLUTION = value;};

	/** Getter method for bit resolution, as defined in the ARINC-429 standard. */
	const float&	getBitResolution() {return BIT_RESOLUTION;};

	/** Setter method for defining the number of bits to shift off the front of the message before the beginning of the payload data. These bits represent the label, SDI, and any additional padding that may exist. */
	void		    setBitShiftWidth(unsigned int value) {BIT_SHIFT_WIDTH = value;};

	/** Getter method for the number of bits to shift off the front of the message before the beginning of the payload data. These bits represent label, SDI, and any additional padding that may exist. */
	const unsigned int& getBitShiftWidth() {return BIT_SHIFT_WIDTH;};

	/** Setter for class member variable that contains the payload data value to be returned from Parse(). */
	void		setReturnValue(const float value) {RETURN_VALUE = value;};

	/** Getter for class member variable that contains the payload data value to be returned from Parse(). */
	const float&	getReturnValue() {return RETURN_VALUE;};
    
    private:

	short		DATA_BIT_LEN;
	float		BIT_RESOLUTION;
	unsigned int	BIT_SHIFT_WIDTH;
	float		RETURN_VALUE;

	const float	RESOLUTION_STEP = 2.0f;

};


#endif
