/****************************************************************
 * fliframe.c
 ****************************************************************/

/******
  Copyright (C) 1993 by Klaus Ehrenfried. 
  
  Permission to use, copy, modify, and distribute this software
  is hereby granted, provided that the above copyright notice appears 
  in all copies and that the software is available to all free of charge. 
  The author disclaims all warranties with regard to this software, 
  including all implied warranties of merchant-ability and fitness. 
  The code is simply distributed as it is.
*******/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fpfli.h"

static unsigned char frame_header[FLI_FRAME_HEADER_SIZE];

#define ERRMSG "Image has wrong format !\n"

/****************************************************************
 * fli_write_frame
 ****************************************************************/

int
 fli_write_frame
 (
   UBYTE * prepre_pixel,
   UBYTE * pre_pixel,
   UBYTE * curr_pixel,
   LONG curr_color[],
   int first_flag
) {
    int chunks, size_color, size_pixel, help;
    int frame_size;

    chunks = 0;
    /* fprintf(stdout," Making color chunk .....\n"); */
    size_color = make_256_color_chunk(curr_color, first_flag);

    if (first_flag == 1) {	/* if first frame */
	fprintf(stdout, " First frame !\n");
	if (size_color == 0) {	/* error occured */
	    fprintf(stderr, "Error: no colors in first frame\n");
	    return (-1);
	}
	/* fprintf(stdout," Making brun chunk .....\n"); */
	size_pixel = make_brun_chunk(curr_pixel);

	if (size_pixel == 0) {	/* error occured */
	    fprintf(stderr, "Error: no pixel in first frame\n");
	    return (-1);
	}
	chunks = 2;
    } else {
	if (size_color > 0)
	    chunks++;

	/* fprintf(stdout," Making delta chunk .....\n"); */
	size_pixel = make_delta_chunk(prepre_pixel, pre_pixel, curr_pixel);

	if (size_pixel > 0)
	    chunks++;
	else
	    fprintf(stdout, " No new pixels\n");
    }

    frame_size = FLI_FRAME_HEADER_SIZE + size_color + size_pixel;

    help = 0;
    add_bytes(frame_header, &help, frame_size, IOM_LONG);
    add_bytes(frame_header, &help, FLI_FRAME_MAGIC, IOM_UWORD);
    add_bytes(frame_header, &help, chunks, IOM_UWORD);
    add_bytes(frame_header, &help, 0x0000, IOM_LONG);
    add_bytes(frame_header, &help, 0x0000, IOM_LONG);

    if (fwrite(frame_header, FLI_FRAME_HEADER_SIZE, 1, output) != 1)
	return (-1);

    if (size_color > 0) {
	if (fwrite(color_chunk_buffer, size_color, 1, output) != 1)
	    return (-1);
    }
    if (size_pixel > 0) {
	if (fwrite(pixel_chunk_buffer, size_pixel, 1, output) != 1)
	    return (-1);
    }
    return (frame_size);
}

/****************************************************************
 * add_bytes
 ****************************************************************/

void add_bytes(unsigned char record[], int *ipos, int value, int mode)
{
    int help;

    switch (mode) {
    case IOM_UBYTE:
	record[(*ipos)++] = value;
	break;

    case IOM_SBYTE:
	value = (value >= 0) ? value : (256 + value);
	value = (value >= 0) ? value : 0;
	record[(*ipos)++] = value;
	break;

    case IOM_UWORD:
	help = value % 256;
	record[(*ipos)++] = help;
	help = (value - help) / 256;
	record[(*ipos)++] = help;
	break;

    case IOM_SWORD:
	value = (value >= 0) ? value : (65536 + value);
	value = (value >= 0) ? value : 0;
	help = value % 256;
	record[(*ipos)++] = help;
	help = (value - help) / 256;
	record[(*ipos)++] = help;
	break;

    case IOM_LONG:
	value = (value >= 0) ? value : 0;
	help = value % 256;
	record[(*ipos)++] = help;
	value = (value - help) / 256;

	help = value % 256;
	record[(*ipos)++] = help;
	value = (value - help) / 256;

	help = value % 256;
	record[(*ipos)++] = help;
	value = (value - help) / 256;

	help = value % 256;
	record[(*ipos)++] = help;
	break;
    }
}
