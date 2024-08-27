#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <curl/curl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <bitset>
#include <cstring>
#include <cstdlib>

#include "include/labels.h"
#include "include/AbstractParser.h"
#include "include/AltitudeParser.h"
#include "include/IndicatedAirspeedParser.h"
#include "include/TrueAirspeedParser.h"
#include "include/LatitudeParser.h"
#include "include/LongitudeParser.h"
#include "include/HeadingParser.h"
#include "include/PitchParser.h"
#include "include/RollParser.h"
#include "include/MessageHandler.h"

#define BUFFER_SIZE 1024
#define PORT 48003 // Updated port

void process_and_send_data(uint32_t,MessageHandler);

void print_binary(uint32_t data) {

    std::bitset<32> bits(data);
    
    for(int i = 31; i >= 0; i--){
        std::cout << bits[i];
        if(i % 4 == 0)
        {
            std::cout << ' ';
        }   
    }
    printf("\n");
}

u_int8_t  reverse_bits(uint8_t n){
    uint8_t res = 0;
    for(int i = 0; i< 8; i++){
        res <<=1;
        res |= (n & 1);
        n >>= 1;
    }
    return res;
}

void MessageHandler::Init()
{
    FILE* fp = fopen("include/word.bin", "rb");
    if (fp)
    {
	// get file size, determine number of arinc_word entries
	struct stat statbuf = {0};
	fstat(fileno(fp), &statbuf);
	long long iter = (statbuf.st_size / sizeof(arinc_word_t));

	memset(word_defs, 0, sizeof(word_defs));

	for (long long i = 0; i < iter; ++i)
	{
	    arinc_word_t word_def_in = {0};

	    fread(&word_def_in, 1, sizeof(arinc_word_t), fp); 

	    word_defs[(int)word_def_in.label_val_octal] = word_def_in;

	    // printf("\nword_def_in.label_val_str   = %s\n",  word_def_in.label_val_str);
	    // printf("word_def_in.label_val_octal = %d (int: %d)\n",    int_to_oct(word_def_in.label_val_octal), word_def_in.label_val_octal);
	    // printf("word_def_in.data_len        = %d\n",    word_def_in.data_len);
	    // printf("word_def_in.bit_shift_width = %d\n",    word_def_in.bit_shift_width);
	    // printf("word_def_in.ssm             = %d\n",    word_def_in.ssm);
	    // printf("word_def_in.bit_resolution  = %.4lf\n", word_def_in.bit_resolution);
	    // printf("word_def_in.max_value	= %.4lf\n", word_def_in.max_value);
	}
	fclose(fp);
	fp = NULL;
    }
    else
    {
	//TODO: warn of missing file, bail out, no point in continuing
	printf("FAILURE: definition file (word.bin) missing!\n");
	fp = NULL;
    }
}


float MessageHandler::Handle(const unsigned int& word)
{
    float parse_val = 0.0f;

    // isolate low 8 bits
    unsigned char tmp_label = reverse_bits((word & 0xFF));

    //arinc_word_t word_defs[377] = {0};
    arinc_word_t word_def	= {0};

    word_def = word_defs[tmp_label]; //int_to_oct(tmp_label)];

    if (word_def.label_val_octal != 0)
    {
	parse_val = parse(word, word_def);

    //printf("%s: %.4lf\n", word_def.label_val_str, parse_val);
    return parse_val;
	//printf("%s: %.4lf\n", word_def.label_val_str, parse_val);
    }
    return -1;
}

const unsigned char MessageHandler::reverse_bits(unsigned char byte)
{
   byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
   byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
   byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;

   return byte;
}

const unsigned short MessageHandler::int_to_oct(unsigned char input)
{
    unsigned short  octal = 0;
    unsigned int    count = 1;
  
    while (input != 0)
    {
        int remainder = input % 8;
  
        octal += (remainder * count);
  
        count *= 10;
        input /= 8;
    }

    return (short)octal;
}

float MessageHandler::parse(const unsigned int& word, const arinc_word_t& word_def)
{

    float ret_val	    = 0.0f;
    float bit_resolution    = word_def.bit_resolution;
    float max_value	    = word_def.max_value;

    std::vector<float> table;

    // generate lookup table
    for (unsigned int i = 0; i < (unsigned int)word_def.data_len; ++i)
    {
	max_value /= 2;
	table.push_back(max_value);
    }

    std::vector<float>::iterator iter = table.end()-1;

    //shift off label, SDI, and, if applicable, low padding
    //unsigned int data = (word >> BIT_SHIFT_WIDTH);
    unsigned int data = (word >> word_def.bit_shift_width);

    for (;;)
    {
	// isolate lsb
	int bit = (data & 1);

	// if bit is set, add resolution
	if (bit == 1)
	    //ret_val += bit_resolution;
	    ret_val += *iter;

	// shift off lsb
	data = (data >> 1);

	// increment resolution
	bit_resolution *= 2.0f;

	if (iter == table.begin())
	    break;

	--iter;
    }

    // check sign bit
    if (word_def.ssm == BNR)
	if (word & 0x10000000)
	    ret_val *= -1.0f;

    return ret_val;
}

int main() {
    int sockfd;
    struct sockaddr_in6 servaddr, cliaddr;
    socklen_t len;
    int n;

    // Create socket
    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fill server information
    servaddr.sin6_family = AF_INET6; // IPv6
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(PORT);

    // Bind the socket with the server address
    printf("Binding to port %d...\n", PORT);
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    MessageHandler msg_handler;
    msg_handler.Init();

    printf("Successfully bound to port %d...\n", PORT);
    printf("Server listening on port %d...\n", PORT);



    while (true) {
        uint32_t arncMsg;
        ssize_t bytes_received = recv(sockfd, &arncMsg, sizeof(arncMsg), 0);

        if(bytes_received = sizeof(arncMsg)){
            process_and_send_data(arncMsg,msg_handler);
        }else if (bytes_received == -1){
            std::cerr<< "data read issue" << std::endl;
        }else if(bytes_received == 0){
            std::cerr << "Connection closed" <<std::endl;
            break;
        }
    }

    close(sockfd);
    return 0;
}

// Function to process and send extracted data
void process_and_send_data(uint32_t data, MessageHandler msg_handler) {
    float result = msg_handler.Handle(data);
    //std::cout << "processing data: \n";

    //print_binary(data);
    
    uint8_t label = static_cast<u_int8_t>(data & 0xFF);
    label = reverse_bits(label);

    std::cout << "Label: " << static_cast<int>(label) << " Data: " << result << std::endl;
    
    char msg[50];

    switch(static_cast<int>(label)){
        case 160: //INDICATED_AIRSPEED -> not 100% accurate
            // snprintf(msg,50,"A: %.4lf",result);
            // printf("%s\n",msg);
            break; 
        case 136: //TRUE_AIRSPEED      -> is accurate 
            snprintf(msg,50,"A: %.4lf",result);
            printf("%s\n",msg);
            break;
        case 212: //PITCH_ANGLE
            snprintf(msg,50,"P: %.4lf",result);
            printf("%s\n",msg);
            break;
        case 213: //ROLL_ANGLE
            snprintf(msg,50,"R: %.4lf",result);
            printf("%s\n",msg);
            break;
        case 204: //HEADING
            snprintf(msg,50,"H: %.4lf",result);
            printf("%s\n",msg);
            break;
        case 72: //LAT
            // snprintf(msg,50,"P: %f",result);
            // printf("\n%s\n",msg);
            break;
        case 73: //LONG  
            // snprintf(msg,50,"P: %f",result);
            // printf("\n%s\n",msg);
            break;
        case 62:  //ALTITUDE
            snprintf(msg,50,"L: %.4lf",result);
            printf("%s\n",msg);
            break;
        default: 
            printf("label error?\n\n");
    }


    // Initialize cURL
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8081/udpdata");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(msg));

        // Set Content-Type header
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: text/plain");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        res = curl_easy_perform(curl);

        //Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}