#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
	int was_new_line = 0, is_empty = 1, is_word = 0, new_par = 1, was_space = 0, carry = 0, w = 0, s = 0, p = 0;
	char c;
	freopen("input.txt", "r", stdin);
	while (!feof(stdin)) {
		c = getchar();
		if (isalpha(c)) {
			is_empty = 0;
			if (!is_word && !carry)
				w++;
			was_new_line = 0;
			is_word = 1;
			carry = 0;
			was_space = 0;
		}
		if (c == '-') {
			was_new_line = 0;
			carry = 1;
			was_space = 0;
			is_empty = 0;
		}
		if (isspace(c)) {
			if (!carry)
				is_word = 0;
			was_space = 1;
			was_new_line = 0;
		}
		if (c == '.') {
			was_new_line = 0;
			is_word = 0;
			is_empty = 0;
			was_space = 0;
			s++;
		}
		if (c == '\n') {
			if (!is_empty && new_par) {
				p++;
				new_par = 0;
			}
			if (is_empty)
				new_par = 1;
			is_empty = 1;
			was_space = 0;
			was_new_line = 1;
		}
	}
	if (!is_empty && new_par)
		p++;
	fclose(stdin);
	freopen("output.txt", "w", stdout);
	printf("%d %d %d\n", w, s, p);
	fclose(stdout);
	return 0;
}