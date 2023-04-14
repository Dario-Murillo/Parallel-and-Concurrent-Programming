// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

__useconds_t random_between(__useconds_t min, __useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0);
}
