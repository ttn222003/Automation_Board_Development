/*
 * MainComn2WifiCard.c
 *
 *  Created on: May 17, 2026
 *      Author: TTN
 */

#include "MainComn2WifiCard.h"

/*======= Internal Variable =======*/
typedef struct {
	uint8_t mDataPayload[MAX_PAYLOAD_LEN];
	CommandType_t mCommand;
	uint16_t mLength;
}InternalFrameStructure_t;

static InternalFrameStructure_t mOutFrame;

/*======= Static Function Prototypes =======*/
static uint16_t CalculateCrc(const uint8_t data[], uint16_t len)
{
    uint16_t crc_result = 0xFFFFu;

    for (uint16_t i = 0; i < len; i++) {
        crc_result ^= (uint16_t)data[i];

        for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
            if (crc_result & 0x0001u) {
                crc_result = (crc_result >> 1) ^ CRC_POLYNOMIAL;
            } else {
                crc_result >>= 1;
            }
        }
    }

    return crc_result;
}

static FrameParseStatus_t CheckParsedFrameIncomplete(const uint8_t buffer[], uint16_t len)
{
    if ((len <= 6) && \
        (buffer[2] != CMD_HEARTBEAT_REQ)){
        return PARSED_FRAME_INCOMPLETE;
    }

    if ((len - 6) != buffer[1]) {
        return PARSED_FRAME_INCOMPLETE;
    }

    return PARSED_FRAME_OK;
}

/*==========================================*/


/*======= API =======*/
void InitializeInternalFrameStructure(void)
{

}

FrameBuildStatus_t BuildFrame(CommandType_t cmd, const uint8_t* payload, uint8_t len, uint8_t* out_buffer, uint16_t* out_len)
{
    if ((payload == NULL) && (len > 0)) {
        *out_len = 0;
        return BUILT_FRAME_ERR_NULL_PTR;
    }
    
    if (out_buffer == NULL) {
        *out_len = 0;
        return BUILT_FRAME_ERR_NULL_PTR;
    }

    if (out_len == NULL) {
        return BUILT_FRAME_ERR_NULL_PTR;
    }

    if (len > MAX_PAYLOAD_LEN) {
        *out_len = 0;
        return BUILT_FRAME_ERR_PAYLOAD_TOO_LARGE;
    }

    out_buffer[0] = SOF_D;
    out_buffer[1] = len;
    out_buffer[2] = cmd;

    for (uint8_t i = 0; i < len; i++) {
        out_buffer[3 + i] = payload[i];
    }

    uint8_t crc_data_input[len + 2];

    for (uint8_t i = 0; i < len + 2; i++) {
        crc_data_input[i] = out_buffer[i + 1];
    }

    uint16_t crc_result = CalculateCrc(crc_data_input, len + 2);

    out_buffer[3 + len] = (uint8_t)((crc_result >> 8) & 0xFFu);
    out_buffer[4 + len] = (uint8_t)(crc_result & 0xFFu);
    out_buffer[5 + len] = EOF_D;

    *out_len = 6 + len;

    return BUILT_FRAME_OK;
}

FrameParseStatus_t ParseFrame(const uint8_t* buffer, uint16_t length_of_frame)
{
    if (buffer == NULL) {
        return PARSED_FRAME_NULL_PTR;
    }

    uint8_t real_buffer[MAX_FRAME_LEN];
    uint8_t flag_sof_found = 0;
    uint16_t index_real_buffer = 0;

    for (uint16_t i = 0; i < length_of_frame; i++) {

        if (buffer[i] == SOF_D) {
            flag_sof_found = 1;
        }

        if (flag_sof_found) {
            real_buffer[index_real_buffer++] = buffer[i];
        }
    }

    if (real_buffer[0] != SOF_D) {
        return PARSED_FRAME_ERR_SOF_D;
    }

    if (CheckParsedFrameIncomplete(real_buffer, index_real_buffer) == PARSED_FRAME_INCOMPLETE) {
        return PARSED_FRAME_INCOMPLETE;
    }

    if (real_buffer[index_real_buffer - 1] != EOF_D) {
        return PARSED_FRAME_ERR_EOF_D;
    }
    
    uint16_t crc_result = CalculateCrc(&real_buffer[1], index_real_buffer - 4);

    if (crc_result != ((uint16_t)real_buffer[index_real_buffer - 3] << 8 | real_buffer[index_real_buffer - 2])) {
        return PARSED_FRAME_ERR_CRC;
    }

    mOutFrame.mLength = real_buffer[1];
    mOutFrame.mCommand = real_buffer[2];

    for (uint16_t i = 0; i < mOutFrame.mLength; i++) {
    	mOutFrame.mDataPayload[i] = real_buffer[3 + i];
    }

    return PARSED_FRAME_OK;
}

void GetFrame(FrameStructure_t* get_frame)
{
    get_frame->mCommand = mOutFrame.mCommand;
    get_frame->mLength = mOutFrame.mLength;
    for (uint16_t i = 0; i < get_frame->mLength; i++) {
        get_frame->mDataPayload[i] = mOutFrame.mDataPayload[i];
    }
}
