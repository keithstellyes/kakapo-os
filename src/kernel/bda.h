#pragma once

typedef struct bda_t {
    uint16_t serial_port[4];
    uint16_t parallel_port[4];
    uint16_t equipment;
    uint8_t manufacturing_test_data;
    uint16_t memorysize_kb;
} bda_t __attribute__((packed));

#define BDA_PTR ((bda_t*)0x0400)
