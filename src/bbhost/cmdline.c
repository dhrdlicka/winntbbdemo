#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"

int no_antialiasing = 0;
int low_bpp = 0;
int ansi = 0;
int product_type = 0;
int height = 480;
int width = 640;
char *filename = NULL;

void cmdline_usage()
{
    printf("\
usage: bbhost [options] [path to winntbb]\n\
\n\
 --low-bpp\tmisleads winntbb into thinking the color depth is 4bpp\n\
 --no-aa\tdisables anti-aliasing\n\
 --size WxH\tsets the host window to the specified size\n\
 --sku N\tinitializes winntbb with the specified product type (build 2296+)\n\
 --ansi\t\twinntbba.dll mode - uses ansi strings instead of unicode\n"
    );
}

int cmdline_parse(int argc, char *argv[])
{
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

        if (!_stricmp(argv[i], "--ansi")) {
            ansi = 1;
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

        if (!_stricmp(argv[i], "--size")) {
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

        if (!filename) {
            filename = argv[i];
        } else {
            fprintf(stderr, "error: hit unexpected \"%s\"\n", argv[i]);
            return 0;
        }
    }

    if (!filename) {
        filename = ansi ? "winntbba.dll" : "winntbbu.dll";
    }

    return 1;
}