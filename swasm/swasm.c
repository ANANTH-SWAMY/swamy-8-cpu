#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD  0b00100000
#define SUB  0b01000000
#define JMP  0b01100000
#define ISZ  0b10000000
#define STP  0b11100000

char* slice(char* str, int start, int end) {
	// str: string to be sliced
	// start: start index
	// end: end index (-1 if the string has to be sliced until the end)

	if (start > strlen(str)) {
		return NULL;
	}

	int len;
	if (end == -1) {
		len = strlen(str) - start;
	} else {
		len = end - start;
	}

	char* temp = malloc(len);
	strncpy(temp, str + start, len);
	temp[len] = 0;
	return temp;
}

char* strip(char* str) {
	char* l = str;
	char* r = str + strlen(str) - 1;

	while (isspace(*l) || isspace(*r)) {
		if (isspace(*l)) {
			l++;
		}

		if (isspace(*r)) {
			r--;
		}
	}

	*(r+1) = 0;

	return l;
}

char* to_upper(char* str) {
	for (char *c = str; *c; c++) {
		*c = toupper(*c);
	}

	return str;
}

int is_digits(char* str) {
	while (*str) {
		if (!isdigit(*str)) {
			return 0;
		}
		str++;
	}

	return 1;
}

void write_binary(char* filename, FILE* file) {
	// make it proper
	int filename_len = strlen(filename);
	filename[filename_len - 1] = 'n';
	filename[filename_len - 2] = 'i';
	filename[filename_len - 3] = 'b';

	FILE* bin_file = fopen(filename, "wb");
	if (!bin_file) {
		printf("Error: Unable to open file for writing\n");
		exit(1);
	}

	char line[16];
	while(fgets(line, sizeof(line), file)){
		int instr_len = strcspn(line, " ");
		char* instr_str = slice(line, 0, instr_len);
		instr_str = to_upper(instr_str);
		instr_str = strip(instr_str);

		if (!strcmp("STP", instr_str)) {
			 // TODO
		} else {
			char* operand_str = slice(line, instr_len + 1, -1);
			if (!operand_str) {
				printf("Error: No operand for \"%s\"\n", instr_str);
				exit(1);
			}

			operand_str = strip(operand_str);

			if (!is_digits(operand_str)) {
				printf("Error: Invalid operand \"%s\" for \"%s\"\n", operand_str, instr_str);
				exit(1);
			}

			int operand = atoi(operand_str);

			if (!strcmp("ADD", instr_str)) {

			} else if (!strcmp("SUB", instr_str)) {

			} else if (!strcmp("JMP", instr_str)) {

			} else if (!strcmp("ISZ", instr_str)) {

			} else {
				printf("Error: Invalid instruction \"%s\"\n", instr_str);
				exit(1);
			}

			free(operand_str);
		}

		free(instr_str);
	}
}

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	FILE* file = fopen(argv[1], "r");
	if(!file){
		printf("Error: Unable to open file\n");
		return 1;
	}

	write_binary(argv[1], file);

	return 0;
}
