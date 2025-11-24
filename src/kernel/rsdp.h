#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "ebda.h"
#include <stdbool.h>

    typedef struct rsdp_t {
        char Signature[8];
        uint8_t Checksum;
        char OEMID[6];
        uint8_t Revision;
        uint32_t RsdtAddress;
    } __attribute__((packed)) rsdp_t;

    typedef struct xsdp_t {
        rsdp_t rsdp;
    } __attribute__((packed)) xsdp_t;

#define RSD_PTR_SIG "RSD PTR "
    bool check_rsdp_signature(rsdp_t *rsdp)
    {
        return strncmp(rsdp->Signature, RSD_PTR_SIG, strlen(RSD_PTR_SIG)) == 0;
    }

    bool rsdp_checksum(rsdp_t *rsdp)
    {
        uint8_t sum = 0;
        for(int i = 0; i < sizeof(rsdp_t); i++) {
            sum += ((uint8_t*)rsdp)[i];
        }
        return sum == 0;
    }

    const char *find_rsdp(rsdp_t **rsdp_ptr_buff)
    {
        // https://wiki.osdev.org/RSDP
        unsigned char *ebda = get_ebda_address();
        // attempt to find in
        for(int i = 0; i < (16 * i < 1000); i++) {
            rsdp_t *rsdp = (rsdp_t*) (ebda + i * 16);
            if(check_rsdp_signature(rsdp) && rsdp_checksum(rsdp)) {
                if(rsdp_ptr_buff) {
                    *rsdp_ptr_buff = rsdp;
                }
                return "RSDP found in EBDA";
            }
        }
        for(int i = 0; i * 16 + 0x000E0000 < 0x000FFFFF; i++) {
            rsdp_t *rsdp = (rsdp_t*)(((unsigned char*)0xE0000) + i * 16);
            if(check_rsdp_signature(rsdp) && rsdp_checksum(rsdp)) {
                if(rsdp_ptr_buff) {
                    *rsdp_ptr_buff = rsdp;
                }
                return "RSDP found in the latter region";
            }
        }

        *rsdp_ptr_buff = NULL;
        return "RSDP not found.";
    }

#ifdef __cplusplus
}
#endif
