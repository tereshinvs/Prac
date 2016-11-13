#include <stdio.h>
#include <unistd.h>

enum {
	START_ALLOCATED_SIZE = 100,
	NEW_ALLOC_SIZE = 100
};

int read_mem(mem_record **mem, FILE *file) {
	int alr_allocated = START_ALLOCATED_SIZE;
	*mem = (mem_record *)calloc(alr_allocated, sizeof(mem_record));

	int k = 0;
	unsigned start = 0, end = 0, offset = 0, major = 0, minor = 0;
	int desc_num = 0;
	char type = 0;
	while (fscanf(file, "%x-%x r-x%c %x %d-%d %d",
		&start, &end, &type, &offset, &major, &minor, &desc_num) == 7) {

		if (k == alr_allocated) {
			alr_allocated += NEW_ALLOC_SIZE;
			*mem = (mem_record *)realloc(*mem, alr_allocated * sizeof(mem_record));
		}

		mem[k].start = start;
		mem[k].end = end;
		mem[k].type = type;
		mem[k].offset = offset;
		mem[k].major = major;
		mem[k].minor = minor;
		mem[k].desc_num = desc_num;
		k++;
	}

	return k;
}

int main(int argc, char *argv[]) {
	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2], "r");

	mem_record *mem1, *mem2;
	int len1 = read_mem(&mem1, f1), len2 = read_mem(&mem2, f2);
	fclose(f1);
	fclose(f2);

	for (int i = 0; i < len1; i++)
		for (int j = 0; j < len2l j++)
			if (mem1[i].type == 'p' && mem2[j].type == 'p'
				&& mem1[i].offset == mem2[j].offset
				&& mem1[i].desc_num == mem2[j].desc_num
				&& )
	return 0;
}