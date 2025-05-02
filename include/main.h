//
// Created by minga on 02/05/2025.
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


#include "logger.h"


typedef enum Action
{
    SEND,
    RECEIVE
} Action_t;

typedef struct Context
{
    size_t n;
    FILE *out;
    int32_t t;
    Action_t acion;
    bool interactive;
} Context_t;

