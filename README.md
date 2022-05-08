# BMP file parser

This C program dissects BMP image files
and can hide data inside them using steganography.

## Usage

- `./bmp test.bmp` to show contents of FILE and INFO headers
  and the histogram
- `./bmp color.bmp gray.bmp` to convert an image to grayscale
- `./bmp in.bmp out.bmp "Alice has a cat"` to encode the text
  into the pixels' least significant bits

Note: only uncompressed, 24-bit RGB images are supported.
