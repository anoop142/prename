/* 
Pretty Rename
Strip non alphabets from starting of file name.

Copyright Anoop S
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <ctype.h>

#define log_err(MESSAGE, ...) fprintf(stderr,\
        MESSAGE "\n", ##__VA_ARGS__)
	
#define check_fail(FAIL_CONDITION, MESSAGE, ...) if((FAIL_CONDITION)) {\
    log_err(MESSAGE, ##__VA_ARGS__); exit(EXIT_FAILURE); }


int list_files(glob_t *pglob, char *pattern){

	int rc = -1;

	int glob_flags = GLOB_TILDE;

	rc = glob(pattern, glob_flags, NULL, pglob);

	check_fail(rc != 0 && rc != GLOB_NOMATCH, "failed to glob" );

	return 0;

}

int is_file_exist(const char *file_name){
	FILE *file;

	if((file=fopen(file_name,"r")) == NULL)
	{
		return 0;
	}
	fclose(file);
	return 1;
}

int pretty_rename(char *filename){
	if(isalpha(*filename) == 0){
		int i;
		for(i=1; isalpha(*(filename + i)) == 0 &&  i < strlen(filename); i++ );
		/* skip if the file name is all non aplha chars*/
		if((strchr(filename+i, '.')) || (strlen(filename + i) == 0) ){

			char *new_filename = strndup(filename + i, strlen(filename) - i);
			if(is_file_exist(new_filename)){
				fprintf(stderr, "%s -> %s, %s exists, so skipped!\n",filename, new_filename,new_filename);
			}
			else{
				rename(filename, new_filename);
			}
				free(new_filename);
		}
	}
	return 0;

}

int main(int argc, char *argv[]){

	const char *usage = "usage: %s  pattern\n";
	const char *example = "example: %s *.mp3\n";
	const char *about = "prettyrename : A tool to strip non alpha characters from starting of files by Anoop\n";

	if(argc >1){

		if(strcmp(argv[1], "-h") ==0 || strcmp(argv[1], "--help") == 0 ){
			printf("%s",about);
			printf(usage,argv[0]);
			printf(example, argv[0]);
			exit(EXIT_SUCCESS);
		}
	}

	if(argc <2){
		fprintf(stderr, usage, argv[0]);
		fprintf(stderr, example, argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		glob_t files_found;

		for(int i=1; i< argc; i++){

			list_files(&files_found, argv[i]);

			for(int i=0; i < files_found.gl_pathc; i++){
				pretty_rename(files_found.gl_pathv[i]);
			}

			globfree(&files_found);
		}
	}
}
