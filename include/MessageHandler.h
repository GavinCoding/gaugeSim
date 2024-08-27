#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

class MessageHandler
{
    public:

	void Init();
	
	/** Method that is called to handle an incoming ARINC-429 message. This function evaluates the label and invokes the appropriate parser class to parse it. */
	float Handle(const unsigned int& word);

    protected:


    private:

	enum SSM_REPRESENTATION {

	    BCD	= 1,
	    BNR,
	    DISCRETE
	};

	enum SSM_MATRIX {

	    NO = 1,	// Normal Operation
	    FT,		// Functional Test
	    FW,		// Failure Warning
	    NCD		// No Computed Data
	};

	typedef struct {

	    unsigned char	label_val_octal;
	    unsigned char	data_len;	    // some messages pad the lower data bits; alternatively, some messages repurpose the SDI bits as the starting point for data
	    unsigned char	bit_shift_width;
	    char		label_val_str[129];
	    SSM_REPRESENTATION	ssm;
	    float		bit_resolution;
	    float		max_value;

	} arinc_word_t;

	arinc_word_t word_defs[255];

	/** Method to reverse the ordering of bits contained in the label portion of an ARINC-429 message. 
	 *  The label is defined as MSB->LSB, that is to say, the MSB is at bit position 1, and the LSB in bit position 8. 
	 *  This method reverses the ordering so LSB is at bit position 1, and MSB is at bit position 8. */
	const unsigned char  reverse_bits(unsigned char byte);

	/** Method to convert an 8-bit integer representation to octal. The label portion of an ARINC-429 message is represented as octal. */
	const unsigned short int_to_oct(unsigned char input);

	float parse(const unsigned int& word, const arinc_word_t& word_def);

};

#endif
