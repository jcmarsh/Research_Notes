# FlatBuffers Test Code

You are going to need (big surprise) flat buffers to run this.
Specifically, get the c version: https://github.com/dvidelabs/flatcc

To build and run:

    make
    ./writer
    ./reader

The `writer` program will write a simple message to the file `evs_msg` which `reader` then reads.
