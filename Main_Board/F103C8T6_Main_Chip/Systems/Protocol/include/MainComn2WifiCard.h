/*
 * MainComn2WifiCard.h
 *
 *  Created on: May 17, 2026
 *      Author: Admin
 */

#ifndef PROTOCOL_INCLUDE_MAINCOMN2WIFICARD_H_
#define PROTOCOL_INCLUDE_MAINCOMN2WIFICARD_H_

#include <stddef.h>
#include <stdint.h>
#include "PlcConfig.h"

#define MAX_FRAME_LEN   256
#define MAX_PAYLOAD_LEN (MAX_FRAME_LEN - 6)

typedef enum {
    CMD_DATA_PUSH = 0x01,
    CMD_HEARTBEAT_REQ,
    CMD_HEARTBEAT_RSP,
    CMD_ACK,
    CMD_NACK,
} CommandType_t;

typedef enum {
    BUILT_FRAME_OK = 0x01,
    BUILT_FRAME_ERR_PAYLOAD_TOO_LARGE,
    BUILT_FRAME_ERR_NULL_PTR,
} FrameBuildStatus_t;

typedef enum {
    PARSED_FRAME_OK = 0x01,
    PARSED_FRAME_ERR_SOF_D,
    PARSED_FRAME_ERR_EOF_D,
    PARSED_FRAME_ERR_CRC,
    PARSED_FRAME_INCOMPLETE,
    PARSED_FRAME_NULL_PTR,
} FrameParseStatus_t;

typedef enum {
    SOF_D = 0xAA,
    EOF_D = 0x55,
} FrameDelimiter_t;

/*
 * NACK payload format: [failed_cmd][error_code].
 * These values are part of the STM32 <-> ESP32 wire protocol.
 */
typedef enum {
    NACK_ERR_CRC_FAIL = 0x01,
    NACK_ERR_UNKNOWN_CMD,
    NACK_ERR_INVALID_PAYLOAD_LEN,
    NACK_ERR_PAYLOAD_TOO_LARGE,
    NACK_ERR_MALFORMED_FRAME,
    NACK_ERR_FRAME_INCOMPLETE,
} NackErrorCode_t;

typedef struct {
    uint8_t mDataPayload[MAX_PAYLOAD_LEN];
    CommandType_t mCommand;
    uint16_t mLength;
} FrameStructure_t;

/*======= API Functions =======*/
void InitializeInternalFrameStructure(void);
FrameBuildStatus_t BuildFrame(CommandType_t cmd, const uint8_t* payload, uint8_t len, uint8_t* out_buffer, uint16_t* out_len);
FrameParseStatus_t ParseFrame(const uint8_t* buffer, uint16_t len);
void GetFrame(FrameStructure_t* get_frame);

#endif /* PROTOCOL_INCLUDE_MAINCOMN2WIFICARD_H_ */
