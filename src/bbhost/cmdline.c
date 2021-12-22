#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"

int no_antialiasing = 0;
int low_bpp = 0;
int product_type = 0;
int height = 480;
int width = 640;
char *filename = "winntbbu.dll";

void cmdline_usage()
{
    printf("\
usage: bbhost [options] [path to winntbbu.dll]\n\
\n\
 --low-bpp\tmisleads winntbb into thinking the color depth is 4bpp\n\
 --no-aa\tdisables anti-aliasing\n\
 --size WxH\tsets the host window to the specified size\n\
 --sku N\tinitializes winntbb with the specified product type (build 2296+)"
    );
}

int cmdline_parse(int argc, char *argv[])
{
    int got_filename = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!_stricmp(argv[i], "-?")) {
            return 0;
        }

        if (!_stricmp(argv[i], "--no-aa")) {
            no_antialiasing = 1;
            continue;
        }

        if (!_stricmp(argv[i], "--low-bpp")) {
            low_bpp = 1;
            continue;
        }

        if (!_stricmp(argv[i], "--sku")) {
            if(++i == argc) {
                fprintf(stderr, "error: expected SKU number, hit end of command line\n");
                return 0;
            }

            if(sscanf(argv[i], "%u", &product_type) != 1) {
                fprintf(stderr, "error: expected SKU number, got \"%s\"\n", argv[i]);
                return 0;
            }

            continue;
        }

        if (!_stricmp(argv[i], "-size")) {
            if(++i == argc) {
                fprintf(stderr, "error: expected window size, hit end of command line\n");
                return 0;
            }

            if(sscanf(argv[i], "%ux%u", &width, &height) != 2) {
                fprintf(stderr, "error: expected window size, got \"%s\"\n", argv[i]);
                return 0;
            }

            continue;
        }

        if (!got_filename++) {
            filename = argv[i];
        } else {
            fprintf(stderr, "error: hit unexpected \"%s\"\n", argv[i]);
            return 0;
        }
    }

    return 1;
}