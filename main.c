#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 100

size_t rcount = 0, pcount = 0;

struct Point2D {
	float x;
	float y;
};

struct Rect{
	struct Point2D bottom_left;
	struct Point2D up_right;
};

struct Point2D* parray[MAX_SIZE];
struct Rect* rarray[MAX_SIZE];

bool deserialization(char* path){
	FILE* fp;
	char line[100];
	size_t len = 0;
	int read;
	fp = fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open input file!\n");
		exit(1);
	}
	char* delim = " ";
	char *pch, *tline, *end_found;
	while (fgets(line, sizeof(line), fp)) {
		tline = (char*)calloc(strlen(line) + 1, sizeof(char));
		strcpy(tline, line);
		pch = strtok(tline, delim);
		if (pch[0] == 'r'){
			// read reacangle data
			struct Rect* rect = (struct Rect*)malloc(sizeof *rect);
			size_t seg_count = 0;
			while (pch != NULL)
			{
				pch = strtok(NULL, delim);
				switch (seg_count){
				case 0:
					rect->bottom_left.x = atof(pch);
					break;
				case 1:
					rect->up_right.y = atof(pch);
					break;
				case 2:
					rect->up_right.x = atof(pch);
					break;
				case 3:
					rect->bottom_left.y = atof(pch);
					break;
				}
				seg_count += 1;
			}
			rarray[rcount] = rect;
			rcount += 1;
		}
		else if (pch[0] != '*'){
			end_found = strstr(tline, "9999.9");
			if (end_found != NULL){
				fclose(fp);
				return true;
			}
			// read point data
			struct Point2D* point = (struct Point2D*)malloc(sizeof *point);
			point->x = atof(pch);
			point->y = atof(strtok(NULL, delim));
			parray[pcount] = point;
			pcount += 1;
		}
		else
			continue;
	}
	return true;
}

float dot_product_2d(float v[], float u[])
{
	float result = 0.0;
	for (unsigned int i = 0; i < 2; i++)
		result += v[i] * u[i];
	return result;
}

bool is_point_in(struct Rect* rect, struct Point2D* point){
	struct Point2D pb; pb.x = rect->up_right.x; pb.y = rect->bottom_left.y;
	float AM[] = { point->x - rect->bottom_left.x, point->y - rect->bottom_left.y };
	float BM[] = { point->x - pb.x, point->y - pb.y };
	float AB[] = { pb.x - rect->bottom_left.x, pb.y - rect->bottom_left.y };
	float BC[] = { rect->up_right.x - pb.x, rect->up_right.y - pb.y };

	return (dot_product_2d(AB, AB) > dot_product_2d(AB, AM) && dot_product_2d(AB, AM) > 0) && (dot_product_2d(BC, BC) > dot_product_2d(BC, BM) && dot_product_2d(BC, AM) > 0);
}

int main()
{
	if (!deserialization("input"))
		return EXIT_FAILURE;

	FILE* fp = fopen("output", "w+");
	// calculate & output
	for (unsigned int i = 0; i < pcount; i++){
		bool found = false;
		for (unsigned int j = 0; j < rcount; j++){
			if (is_point_in(rarray[j], parray[i]) == true){
				found = true;
				fprintf(fp, "Point %i is contained in figure %i\n", i + 1, j + 1);
			}
		}
		if (!found)
			fprintf(fp, "Point %i is not contained in any figure\n", i + 1);
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

