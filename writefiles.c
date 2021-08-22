#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "utils.h"
#include "defs.h"
#include "label_table.h"

/*return the next binary num (4 cells in the array)*/
int * get_next_num(int * val, int * img_value, int i){
	int j;
	for (i , j =0; i<(i+4); i++, J++){
		val[j]=	item_img.data[i];
	}
	if (val[j]== NULL){return NULL;}
return val;
}


/*convert array of binary digits to hex*/
long int convert_bin_to_hex(int *binaryval){
	long int l = binaryval[0] | (binaryval[1] << 8) | (binaryval[2] << 16) | (binaryval[3] << 24);/*convert array to long int*/
	long int hexadecimalval = 0, i = 1, remainder;

	while (binaryval != 0)
	{
	remainder = binaryval % 10;
	hexadecimalval = hexadecimalval + remainder * i;
	i = i * 2;
        binaryval = binaryval / 10;
	}
    return(hexadecimalval);

}



bool write_ob(image *item_img, image *data_img, long icf, long dcf, char *filename) {
	int i,j;
	FILE *file_pointer;
	/* add extension of file to open */
	char *output_filename = strallocat(filename, ".ob");
	/* Try to open the file for writing */
	file_pointer = fopen(output_filename, "w");
	free(output_filename);
	if (file_pointer == NULL) {
		printf("Can't create or rewrite to file %s.", output_filename);
		return FALSE;
	}

	/* print data/code word count on top */
	fprintf(file_pointer, "%ld %ld \n", icf - 100, dcf);
	file_pointer = fopen(output_filename, "a");
	for (address = 100 ;address<icf; address+=4){
		fprintf(file_pointer, "\n%04d",address); /*add the address in decimal*/
		for (i=0,j=1;i<8;i+=2,j+=2){
			int val1[4];
			int val2[4];
			val1 = get_next_num(val1, item_img, i);
			val2 = get_next_num(val2, item_img, j);
		
			char val1_hex, val2_hex;
			val1_hex = convert_bin_to_hex(val1);
			val2_hex = convert_bin_to_hex(val2);
			fprintf(file_pointer, "%lX%lX ", val2_hex, val1_hex);/*add machine code in hex*/
			}
		item_img = item_img->next;
	}
/*add to file from data img*/
	int length = 0; /*how long the data img of the spesific line is*/
	i = 0;
	j = 1;
	for (address = icf ;address<icf+dcf; address+=4){
		fprintf(file_pointer, "\n%04d",address); /*add the address in decimal*/
		int size = sizeof(data_img.data) / sizeof(int);
		if(length == 0) {length =size/8;}/*if we fill the 4 places is the file we */
		int k=0;/*place in file line*/
		while (length>0 && K<4 && i<8 && j<8){
			int val1[4];
			int val2[4];
			val1 = get_next_num(val1, data_img, i);
			val2 = get_next_num(val2, data_img, j);
		
			char val1_hex, val2_hex;
			val1_hex = convert_bin_to_hex(val1);
			val2_hex = convert_bin_to_hex(val2);
			fprintf(file_pointer, "%lX%lX ", val2_hex, val1_hex);/*add machine code in hex*/
			i=i+2;
			j=j+2;
			length= length-1;
			k=k+1;	
			if(length=0 && k<4){/*if the data is over but we didnt fill the 4 places in the file. we jump to the next line*/
				data_img=data_img->next;
				if (data_img == NULL){break;}/*we got the the end of the data img*/
				size = sizeof(data_img.data) / sizeof(int);
				length = size/8;
				i = 0;
				j = 1;
			}
			
		}
		



	}
	fclose(file_pointer);
	return TRUE;
}


bool write_entries(table *symbol_table, char *filename){
	FILE *file_pointer;
	/* add extension of file to open */
	char *output_filename = strallocat(filename, ".ent");
	/* Try to open the file for writing */
	file_pointer = fopen(output_filename, "w");
	free(output_filename);
	if (file_pointer == NULL) {
		printf("Can't create or rewrite to file %s.", output_filename);
		return FALSE;
	}
	while (symbol_table != NULL){
		if(symbol_table.symbol.second_attribute == A_ENTRY){
			fprintf(file_pointer, "%c %d \n", symbol_table.symbol.name, symbol_table.symbol.value);
			symbol_table = (*symbol_table)->next;
			file_pointer = fopen(output_filename, "a");
		}
	symbol_table = (*symbol_table)->next;	
	}
	
	fclose(file_pointer);
return TRUE;
}

bool write_external(table *symbol_table, char *filename){
	FILE *file_pointer;
	/* add extension of file to open */
	char *output_filename = strallocat(filename, ".ext");
	/* Try to open the file for writing */
	file_pointer = fopen(output_filename, "w");
	free(output_filename);
	if (file_pointer == NULL) {
		printf("Can't create or rewrite to file %s.", output_filename);
		return FALSE;
	}
	while (symbol_table != NULL){
		if(symbol_table.symbol.attribute == A_EXTERNAL){
			fprintf(file_pointer, "%c %d \n", symbol_table.symbol.name, symbol_table.symbol.value);
			symbol_table = (*symbol_table)->next;
			file_pointer = fopen(output_filename, "a");
		}
	symbol_table = (*symbol_table)->next;	
	}
	
	fclose(file_pointer);
return TRUE;
}

bool check_for_entries(table *symbol_table){
	while (symbol_table != NULL){
		if(symbol_table.symbol.second_attribute == A_ENTRY){
			return TRUE;
		}
		symbol_table = (*symbol_table)->next;
	}
return FALSE;
}

bool check_for_external(table *symbol_table){
	while (symbol_table != NULL){
		if(symbol_table.symbol.attribute == A_EXTERNAL){
			return TRUE;
		}
		symbol_table = (*symbol_table)->next;
	}
return FALSE;
}

