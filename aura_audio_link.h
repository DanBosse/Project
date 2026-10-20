#ifndef AURA_AUDIO_LINK_H
#define AURA_AUDIO_LINK_H

#include <stdint.h>

void aura_audio_link_init(void);
int aura_audio_stream_bind(uint32_t channel_count, uint32_t sample_rate);

#endif
