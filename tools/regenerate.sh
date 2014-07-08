#!/bin/sh

tools/gen_wire_io.py h > include/wire_io_gen.h
tools/gen_wire_io.py c > src/wire_io_gen.c.inc
