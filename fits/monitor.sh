#!/bin/bash
tail -n 120 -f $(find . -name "*.o$1")
