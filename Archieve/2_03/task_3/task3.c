#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void task3()
{
	cJSON * item;

	item = cJSON_Parse("[1,2,3]");
/*	item = cJSON_Parse("{"\
		"\"name\" : \"Ivanov\","\
		"\"age\" : 25"\
		"}"
		);*/

	char *text = cJSON_PrintUnformatted(item);
	puts(text);
	free(text);

	cJSON_ReplaceItemInArray(item, 1, cJSON_CreateFalse());
}

int main(int argc, char ** argv)
{
	task3();

	return 0;
}

