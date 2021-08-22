#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sec_pass.h"
#include "writefiles.h"
#include "first_pass.h"


/**
 * Processes a single assembly source file, and returns the result status.
 * @param filename The filename
 * @return Whether succeeded
 */
static bool process_file(char *filename);

/**
 * Entry point - 24bit assembler. Assembly language specified in booklet.
 */
int main(int argc, char *argv[]) {
	int i;
	/* To break line if needed */
	bool succeeded = TRUE;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* if last process failed and there's another file, break line: */
		if (!succeeded) puts("");
		/* foreach argument (file name), send it for full processing. */
		succeeded = process_file(argv[i]);
		/* Line break if failed */
	}
	return 0;
}


static bool process_file(char *filename) {
	/* Memory address counters */
	int line_number;
	long ic = IC_INIT_VALUE, dc = 0, icf, dcf;
	bool no_error = TRUE; /* is succeeded so far */
	char *input_filename;
	char temp_line[255]; /* temporary string for storing line, read from file */
	FILE *fpointer; /* Current assembly file descriptor to process */



/*please check how you want to define them*/

	image *data_img; /* Contains an image of the machine code */
	image *item_img;
	/* Our symbol table */
	table symbol_table;

	bool no_error=TRUE;
	

	input_filename = filename;

	/* Open file, skip on failure */
	fpointer = fopen(input_filename, "r");
	if (fpointer == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: file \"%s.as\" is inaccessible for reading. skipping it.\n", filename);
		free(input_filename); /* The only allocated space is for the full file name */
		return FALSE;
	}

	line_origin error_data =  {line_number, input_filename};

	/* start first pass: */
	for (line_number = 1; !feof(fpointer);line_number++) {
	    int i = 0;
	    fgets(temp_line, 255, fpointer); /* Get line */
	    move_not_empty_char(temp_line, i);
	    if(first_pass_line(temp_line, symbol_table, icf, dcf, item_img,
                           data_img, error_data) == 0)
	    {
	        no_error = FALSE;
            return no_error;
	    }
	}


	/* start second pass: */
	rewind(fpointer); /* Start from beginning of file again */

	for (line_number = 1; !feof(fpointer);line_number++) {
			int i = 0;
			fgets(temp_line, 255, fpointer); /* Get line */
			move_not_empty_char(temp_line, i);
			process_line_spass(fpointer, item_img, symbol_table, ic);
			if (process_line_spass(fpointer, item_img, symbol_table, ic) == FALSE){
				no_error = FALSE;	
			}
		}

	if(no_error=TRUE){
		write_ob(item_img, data_img, icf, dcf, filename);
		if(check_for_entries == TRUE){
			write_entries(symbol_table, filename);
		}
		if(check_for_external == TRUE){
		write_external(symbol_table, filename);
		}
	}
/* Now let's free some pointer: */
	/* current file name */
	free(input_filename);
	/* Free symbol table */
	free(symbol_table);
	/* Free code & data buffer contents */
	free(item_img);
	free(data_img);

	/* return whether every assembling succeeded */
	return no_error;
