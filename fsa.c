#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int o = -1;
	bool from_file = true;
	bool standard_input = true;
	bool interactive = false;
	bool error = false;
	char *machine_string = NULL;
	char *input_string = NULL;
	
	while ((o = getopt(argc, argv, ":m:e:i")) != -1)
	{
		switch (o)
		{
			case 'm':
				from_file = false;
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

	if (error)
	{
		fprintf(stderr, "Usage: fsa [-i] [-e input_string] <-m machine_string | machine_filename>\n");
	}

	return 0;
}