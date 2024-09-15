// Nagendra Upadhyay. 02/19/23. CS360 Lab3. The code below is a decoding machine, it takes in a definition file and an input file. The definition file is used to decode the input file.
// The code uses the connections between the strings/character and bit sequence to decode effectively.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define the maximum length of the string
#define MAX_STRLEN 10001

// Define the struct for the Huffman tree node
struct huff_node {
    struct huff_node *zero;
    struct huff_node *one;
    char *s_zero;
    char *s_one;
};
  
// Recursive function to free the memory used by the Huffman tree
void free_tree(struct huff_node *root) {
    if (root == NULL){ 
        return;
    }
    free_tree(root->zero);
    free_tree(root->one);
    if (root->s_zero != NULL){
        free(root->s_zero);
    }
    if (root->s_one != NULL){
        free(root->s_one);
    }
    free(root);
}

int main(int argc, char **argv) {
    // check if program is called with correct number of arguments
    if(argc != 3) {
        fprintf(stderr, "Incorrect file call\n");
        exit(0);
    }
    // open huffman code definition file
    FILE *code_file = fopen(argv[1], "r");
    if(!code_file) {
        fprintf(stderr, "Error: cannot open code definition file '%s'\n", argv[1]);
        exit(0);
    }
    // open input file
    FILE *input_file = fopen(argv[2], "rb");
    if(!input_file) {
        fprintf(stderr, "Error: cannot open input file '%s'\n", argv[2]);
        exit(0);
    }
 
    // allocate memory for root node of huffman tree and initialize root node
    struct huff_node *root = malloc(sizeof(struct huff_node));
    root->zero = NULL;
    root->one = NULL;
    root->s_zero = NULL;
    root->s_one = NULL;
 
    // initialize variables for reading huffman code from file
    char code_string[MAX_STRLEN];
    char code_sequence[MAX_STRLEN];
    char Cdata;
    int boolean = 0;
    int PosCounter = 0;
    int SCounter = 0;
 
    struct huff_node *current;
    //0 is letter and 1 is number
    // This loop reads the binary data from the compressed file byte-by-byte
    while(fread(&Cdata, 1, 1, code_file)) {
        // Check if the current byte is a null character
        if (Cdata == '\0'){
            // If boolean is 0, then it means that we have finished reading the Huffman code for a string
            if (boolean == 0){
                boolean = 1;
            // If boolean is 1, then it means that we have finished reading the character data and we can start inserting things into the Huffman tree
            } else if (boolean == 1){
                boolean = 0;
                current = root;
                // Iterate over the bits in the Huffman code for the current character
                for (unsigned int i = 0; i < strlen(code_sequence); i++) {
                    // If we have reached the end of the code sequence, insert the character data into the tree
                    if(code_sequence[i+1] == '\0'){
                        // If the current bit is 0, then insert the character data into the left child of the current node
                        if (code_sequence[i] == '0'){
                            current->s_zero = strdup(code_string);
                            current = root;
                            code_string[0] = '\0';
                            code_sequence[0] = '\0';
                        // If the current bit is 1, then insert the character data into the right child of the current node
                        } else if (code_sequence[i] == '1'){
                            current->s_one = strdup(code_string);
                            current = root;
                            code_string[0] = '\0';
                            code_sequence[0] = '\0';
                        }
                    // If we have not reached the end of the Huffman code, continue traversing the tree
                    }else if (code_sequence[i] == '0') {
                        // If the left child of the current node does not exist, create a new node and assign it to the left child
                        if (current->zero == NULL) {
                            current->zero = malloc(sizeof(struct huff_node));
                        }
			            current = current->zero;
                    } else if (code_sequence[i] == '1') {
                        if (current->one == NULL) {
                            current->one = malloc(sizeof(struct huff_node));
                        }
                        current = current->one;
                    }
                }
            }
            // Reset the counters for the code string and sequence
            PosCounter = 0;
            SCounter = 0;
        // If boolean is 0, then we are still reading the character data for a leaf node
        }else if(boolean == 0){
            // Append the current byte to the code sequence and increment the sequence counter
            code_string[PosCounter] = Cdata;
            code_string[PosCounter+1] = '\0';
            PosCounter++;
        }else if(boolean == 1){
            code_sequence[SCounter] = Cdata;
            code_sequence[SCounter+1] = '\0';
            SCounter++;
        }
    }
    fclose(code_file);
 
    // Read the number of bits from the end of the file
    fseek(input_file, -4, SEEK_END);
    int num_bits = 0;
    // Read 4 bytes from the file and store it as an integer in num_bits
    fread(&num_bits, 1, 4, input_file);
    // Allocate memory for the binary data
    char* BitData = malloc((num_bits)* sizeof(char));
    
    // Read the binary data from the file
    fseek(input_file, 0, SEEK_SET);
    unsigned int BitCounter = 0;
 
    // Loop through the file and read each bit of data
    while (BitCounter < (unsigned int) num_bits) {
        // Read a byte of data from the file
        unsigned char data = fgetc(input_file);
        int i = 0;
        // Loop through each bit in the byte
        while(i < 8){
            // If we have read all the bits, break out of the loop
            if(BitCounter == (unsigned int) num_bits){
                break;
            }
            // If the last bit of the byte is 1 or 0, then set the corresponding bit in BitData to 1 or 0
            if(data & 1){
                BitData[BitCounter] = '1';
            }else{
                BitData[BitCounter] = '0';
            }
            // Shift the bits in the byte to the right by 1 to read the next bit
            data = data >> 1;
            BitCounter++; 
            i++;
        }
    }
    // Add null terminator to binary data string
    BitData[BitCounter] = '\0';
    
    fseek(input_file, 0, SEEK_END);
    // Get the size of the input file
    int FileSize = ftell(input_file);
    // Set the file pointer back to the beginning of the file
    fseek(input_file, 0, SEEK_SET);
    // Calculate the expected size of the file
    int expected_size = ((num_bits) / 8) + 4;

    // If the file is less than 4 bytes, there is an error
    if(FileSize < 4){
        fprintf(stderr,"Error: file is not the correct size.\n");
        exit(1);
    }
    // If the file is smaller than expected, there is an error
    if (FileSize < expected_size) {
        fprintf(stderr, "Error: Total bits = %d, but file's size is %d\n", num_bits, FileSize);
        exit(1);
    }
    
    // Close input file
    fclose(input_file);

    // Traverse the Huffman tree to decode the binary data
    struct huff_node *p = root;

    // Loop through the binary data string
    for(int j = 0; BitData[j] != '\0'; j++){   
        if(BitData[j] == '0'){
            // If the left child is null
            if(p->zero == NULL){
                // If the left child is a leaf node with no charaters, then the bit is incorrect
                if(p->s_zero == NULL){
                    fprintf(stderr, "Unrecognized bits\n");
                    exit(1);
                }
                // Print the character for the left child
                printf("%s", p->s_zero);
                // Reset the current node to the root
                p = root;
            }else {
                p = p->zero;
            }
        } else if (BitData[j] == '1'){
            if(p->one == NULL){
                if(p->s_one == NULL){
                    fprintf(stderr, "Unrecognized bits\n");
                    exit(1);
                }
                // Print the letter for the right child
                printf("%s", p->s_one);
                p = root;
            }else{
                // Move to the right child
                p = p->one;
            }
        }
    }

    // Free memory and exit
    free_tree(root);
    free(BitData);
    return 0;
}
