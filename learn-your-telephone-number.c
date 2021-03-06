/* See LICENSE file for copyright and license details. */
#include <alloca.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define t(...) do { if (__VA_ARGS__) goto fail; } while (0)


static const char *argv0;


static char *
compare(const char *restrict e, const char *restrict c)
{
	size_t en = strlen(e);
	size_t cn = strlen(c);
	size_t **matrix = alloca((en + 1) * sizeof(size_t *));
	char **path_matrix = alloca((en + 1) * sizeof(char *));
	size_t ei, ci, ri = 0;
	char* rc = NULL;
	size_t d, l, u, lu;
	int ch;

	for (ei = 0; ei <= en; ei++) {
		matrix[ei] = alloca((cn + 1) * sizeof(size_t));
		matrix[ei][0] = ei;
		path_matrix[ei] = alloca((cn + 1) * sizeof(char));
		path_matrix[ei][0] = '|';
	}
	for (ci = 0; ci <= cn; ci++) {
		matrix[0][ci] = ci;
		path_matrix[0][ci] = '-';
	}
	path_matrix[0][0] = '\0';

	e--, c--;

	for (ei = 1; ei <= en; ei++) {
		for (ci = 1; ci <= cn; ci++) {
			d = matrix[ei - 1][ci - 1];
			l = matrix[ei + 0][ci - 1];
			u = matrix[ei - 1][ci + 0];
			lu = 1 + (l < u ? l : u);
			ch = (e[ei] != c[ci]);
			d += (size_t)ch;
			matrix[ei][ci] = d < lu ? d : lu;
			path_matrix[ei][ci] = d < lu ? (ch ? '\\' : '=') : (l < u ? '-' : '|');
		}
	}

	t (!(rc = malloc((en + cn + 2) * sizeof(char))));

#if 0
	for (ei = 0; ei <= en; ei++) {
		for (ci = 0; ci <= cn; ci++)
			printf("%zu%s", matrix[ei][ci], ci == cn ? "" :
			       path_matrix[ei][ci + 1] == '-' ? "\033[1;32m-\033[m" : " ");
		printf("\n");
		if (ei < en)
			for (ci = 0; ci < cn; ci++)
				printf("%s%s", path_matrix[ei + 1][ci + 0] == '|' ? "\033[1;31m|\033[m" : " ",
				       path_matrix[ei + 1][ci + 1] == '\\' ? "\033[1;33m\\\033[m" :
				       path_matrix[ei + 1][ci + 1] == '=' ? "\033[1;39m\\\033[m" : " ");
		printf("\n");
	}
#endif

	rc[ri++] = '\0';
	for (ei = en, ci = cn; ei + ci;) {
		switch (path_matrix[ei][ci]) {
		case '=':   ei--, ci--;  rc[ri++] = '=';  break;
		case '\\':  ei--, ci--;  rc[ri++] = '!';  break;
		case '|':   ei--;        rc[ri++] = '-';  break;
		case '-':         ci--;  rc[ri++] = '+';  break;
		default:
			abort();
		}
	}
	rc[ri] = '\0';

fail:
	return rc;
}


int
main(int argc, char *argv[])
{
	char *correct;
	char *entered = NULL;
	size_t n = 0;
	void *new;
	char *w;
	char *r;
	char *path = NULL;
	char *path_;
	char *p;
	int rc = 0;

	argv0 = argc ? argv[0] : "learn-your-telephone-number";
	if (argc != 2)
		goto usage;

	correct = alloca((2 * strlen(argv[1]) + 1) * sizeof(char));
	for (w = correct, r = argv[1]; *r; r++) {
		if      (isdigit(*r)) *w++ = *r;
		else if (isalpha(*r)) goto usage;
		else if (*r == '+')   *w++ = '0', *w++ = '0';
	}
	*w = '\0';

	t (fprintf(stdout, "Enter your telephone number: ") < 0);
	t (fflush(stdout));
	t (errno = 0, getline(&entered, &n, stdin) < 0);
	n = strlen(entered) - 1;
	entered[n] = '\0';

	t (!(new = realloc(entered, (2 * n + 1) * sizeof(char))));
	entered = new;
	memmove(r = (entered + n), w = (entered), n + 1);
	for (; *r; r++) {
		if      (isdigit(*r)) *w++ = *r;
		else if (*r == '+')   *w++ = '0', *w++ = '0';
	}
	*w = '\0';

	if (!strcmp(entered, correct))
		goto done;
	t (!(path = compare(entered, correct)));

	path_ = strchr(path + 1, '\0');
	t (fprintf(stdout, "\033[A\033[KEntered: ") < 0);
	for (p = entered; *--path_;) {
		switch (*path_) {
		case '=': t (fprintf(stdout, "\033[39m%c", *p++) < 0); break;
		case '!': t (fprintf(stdout, "\033[31m%c", *p++) < 0); break;
		case '+': t (fprintf(stdout, " ")                < 0); break;
		case '-': t (fprintf(stdout, "\033[31m%c", *p++) < 0); break;
		}
	}
	t (fprintf(stdout, "\033[m\n") < 0);

	path_ = strchr(path + 1, '\0');
	t (fprintf(stdout, "Correct: ") < 0);
	for (p = correct; *--path_;) {
		switch (*path_){
		case '=': t (fprintf(stdout, "\033[39m%c", *p++) < 0); break;
		case '!': t (fprintf(stdout, "\033[32m%c", *p++) < 0); break;
		case '+': t (fprintf(stdout, "\033[32m%c", *p++) < 0); break;
		case '-': t (fprintf(stdout, " ")                < 0); break;
		}
	}
	t (fprintf(stdout, "\033[m\n") < 0);

	rc = 1;
done:
	free(entered);
	free(path);
	return rc;
fail:
	rc = 1;
	if (!errno)
		goto done;
	perror(argv0);
	rc = 2;
	goto done;
usage:
	fprintf(stderr, "usage: %s your-telephone-number\n", argv0);
	return 2;
}

