#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "compiler.h"

int main(int argc, char** argv)
{
	compiler_args_t args = { NULL, NULL, 0 };

	int c;
	while ((c = getopt(argc, argv, "i:o:O:h")) != -1)
	{
		switch (c)
		{
			case 'o':
				args.out_file = optarg;
				break;
			case 'O':
				args.opt_lvl = atoi(optarg);
				break;
			case 'h':
				fprintf(stderr, "Usage:\n%s [input_file] [options]\nOptions:\n\t[-o output.file]\t\t\t[input.file]\t\t\tReads the input not from stdin but the given file\n\tRedirects output from stdout into a the given file\n", argv[0]);
				return 0;
			case '?':
				if (optopt == 'c')
				  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
				  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
				  fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			  default:
				abort();
		}
	}

	for (size_t i = optind; i < argc; ++i)
	{
		if (args.in_file)
			return fprintf(stderr, "%s\n", "More than one input file is not supported yet");

		args.in_file = argv[i];
	}

	return compile_file(&args);
}
