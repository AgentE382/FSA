#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void syserr(const char *m)
{
	perror(m);
	exit(EXIT_FAILURE);
}

void usererr(const char *m)
{
	fprintf(stderr, "%s\n", m);
	exit(EXIT_FAILURE);
}

char *read_file(const char *filename)
{
	char *result;
	size_t pos, size;
	int c;
	FILE *f;

	if (!(result = (char *)malloc(sizeof(*result) + 1)))
		syserr(filename);
	pos = 0;
	size = 1;

	f = fopen(filename, "r");

	if (!f)
		syserr(filename);

	while ((c = fgetc(f)) != EOF)
	{
		result[pos] = c;
		if (++pos == size)
		{
			size <<= 1;
			if (!(result = (char *)realloc(result, sizeof(*result) * size + 1)))
				syserr(filename);
		}
	}

	result[pos] = 0;

	return result;
}

int main(int argc, char * const argv[])
{
	int o = -1;
	bool from_file = true;
	bool standard_input = true;
	bool interactive = false;
	bool error = false;
	char *machine_string = NULL;
	char *input_string = NULL;
	
	// process arguments
	while ((o = getopt(argc, argv, ":m:f:e:i")) != -1)
	{
		switch (o)
		{
			case 'm':
				from_file = false;
				machine_string = optarg;
				break;
			case 'f':
				machine_string = optarg;
				break;
			case 'e':
				standard_input = false;
				input_string = optarg;
				break;
			case 'i':
				interactive = true;
				break;
			case ':':
				fprintf(stderr, "Option '-%c' requires an operand\n", optopt);
				error = true;
				break;
			case '?':
				fprintf(stderr, "Unrecognized option: '-%c'\n", optopt);
				error = true;
				break;
		}
	}

	// handle parsing errors
	if (error || !machine_string)
		usererr("Usage: fsa [-i] [-e input_string] <-m machine_string | -f machine_filename>");

	if (from_file)
		machine_string = read_file(machine_string);

	return 0;
}
