#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__((packed)) ACPISDTHEADER;

typedef struct SDT {
  ACPISDTHeader h;
  // length: [(h.Length - sizeof(h)) / 4];
  //uint32_t **next;
  uint32_t next[];
} __attribute__((packed)) SDT;

#ifdef __cplusplus
}
#endif
