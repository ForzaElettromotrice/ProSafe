//
// Created by minga on 02/05/2025.
//

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


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
    Action_t action;
    bool interactive;
} Context_t;

