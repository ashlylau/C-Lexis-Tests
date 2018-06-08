#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "dragon.h"

#define LEVEL 6;

/* x, y: coordinates of turtle */
static long x, y;

/* When drawing a pixel to the image, x and y must be divided by this value.
 * This enables both the dragon curve and twin dragon to rendered without
 * clipping.
 */
static long scale;

/* drawn_pixels: the length of the path travelled. */
static long drawn_pixels;

/* direction: the current direction of the turtle. */
static vector_t direction;

/* Returns a vector that describes the initial direction of the turtle. Each
 * iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations)
{
  switch (total_iterations) {
  	case 0:
			direction.dx = 1;
			direction.dy = 0;
		case 1:
			direction.dx = 1;
			direction.dy = 1;
		case 2:
			direction.dx = 0;
			direction.dy = 1;
		case 3:
			direction.dx = -1;
			direction.dy = 1;
		case 4:
			direction.dx = -1;
			direction.dy = 0;
		case 5:
			direction.dx = -1;
			direction.dy = -1;
		case 6:
			direction.dx = 0;
			direction.dy = -1;
		case 7:
			direction.dx = 1;
			direction.dy = -1;
		default:
			perror("starting_direction");
			exit(EXIT_FAILURE);
  }
	return direction;
}

/* Draws a pixel to dst at location (x, y). The pixel intensity is chosen as a
 * function of image size and the number of pixels drawn.
 *
 * The gray level changes over entire size of path; the pixel value oscillates
 * along the path to give some contrast between segments close to each other
 * spatially.
 */
void draw_greyscale(image_t *dst, long x, long y)
{
	int height = dst->height;
	int level = LEVEL * drawn_pixels / (height * height);

	uint8_t grey_value;

	switch (level) {
		case 0:
			grey_value = 100;
		case 1:
			grey_value = 120;
		case 2:
			grey_value = 150;
		case 3:
			grey_value = 180;
		case 4:
			grey_value = 200;
		default:
			grey_value = 255;
	}

	set_pixel(dst, (int)x, (int)y, grey_value);

}


/* Iterates though the characters of str, recusively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations)
{
  //TODO
}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations)
{
	//TODO

}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv)
{
//TODO
	return EXIT_SUCCESS;
}
