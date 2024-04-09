#!/bin/bash
PATH_=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
LD_LIBRARY_PATH=${PATH_}/../lib ${PATH_}/protoc $@