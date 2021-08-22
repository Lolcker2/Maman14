#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "utils.h"
#include "defs.h"
#include "label_table.h"

bool process_line_spass(char* line, image *item_img, table *symbol_table, long *ic, operation *operation_table) {
	char *indexInLine;
	char *token;
	long i = 0;
	label output_label;
	
	i = move_not_empty_char(line, i);
	/* Empty/Comment line - no processing needed! */
	if (line[i] == ';' || line[i] == '\n') return TRUE;
	indexInLine = strchr(line, ':');
	/*check for label */
	if (indexInLine != NULL) {
		i = indexInLine - line;
		i++;
	}
	i = move_not_empty_char(line, i);

	/* .instruction */
	if (line[i] == '.') {
		/*if it's entry we add it to the symbol table*/
		if (strncmp(".entry", line, 6) == 0) {
			i += 6;
			move_not_empty_char(line, i);
			token = strtok(line + i, " \n\t");
			/* if label is already marked as entry, ignore. */
			if (token == NULL) {
				fprint_error("Invalid line, You have to specify a label name for .entry instruction.", line);
				return FALSE;
			}
			if (find_label(token, *symbol_table, output_label) == 0 {
				fprint_error("Invalid line, symbol not exist in symbol table.", line);
				return FALSE;
			}
			
			token = strtok(line + i, "\n"); /*get name of label*/
			if (token[0] == '&') {token++;}
			if (find_label(token, *symbol_table, output_label) == 1 {
				item = get_label(token, *symbol_table);
				add_attribute_to_item(item, A_ENTRY);
			}
		}
		return TRUE;
	
	}
	if (complete_img(item_img)== 0){ return TRUE} /*not need to complete the image code*/
	
	return fix_code_image(ic, line,i, item_img, symbol_table, operation_table);
}


}
