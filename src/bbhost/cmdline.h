#ifndef CMDLINE_H
#define CMDLINE_H

#pragma once

extern int no_antialiasing;
extern int low_bpp;
extern int product_type;
extern int height;
extern int width;
extern char *filename;

void cmdline_usage();
int cmdline_parse(int argc, char *argv[]);

#endif