#pragma once
#ifndef PATTERNDATABASE_H
#define PATTERNDATABASE_H

#include "Utility.h"

static Byte* CORNER_TABLE = new Byte[CORNERTABLESIZE];
static Byte* EDGE1_TABLE = new Byte[EDGETABLESIZE];
static Byte* EDGE2_TABLE = new Byte[EDGETABLESIZE];

static bool CORNER_TABLE_EXIST = false;
static bool EDGE1_TABLE_EXIST = false;
static bool EDGE2_TABLE_EXIST = false;

#endif // PATTERNDATABASE_H
