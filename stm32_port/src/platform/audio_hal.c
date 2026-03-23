#include "platform/audio_hal.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "stm32h7s78_discovery_audio.h"

typedef struct
{
    bool init_ok;
    volatile uint8_t dma_half_pending;
    volatile uint8_t dma_full_pending;

    uint8_t pending_wall;
    uint8_t pending_paddle;
    uint8_t pending_point;
    uint8_t pending_win_tune;

    bool win_tune_active;
    uint8_t win_tune_step;
    uint32_t win_tune_gap_samples;

    uint32_t phase_q32;
    uint32_t phase_step_q32;
    uint32_t samples_left;
    int16_t amp;

    uint8_t volume_percent;
    uint32_t volume_gain_q12;
} audio_state_t;

static audio_state_t s_audio;

#define AUDIO_SAMPLE_RATE_HZ (16000u)
#define AUDIO_BUFFER_FRAMES  (1024u)
#define AUDIO_HALF_FRAMES    (AUDIO_BUFFER_FRAMES / 2u)

static int16_t s_dma_pcm[AUDIO_BUFFER_FRAMES * 2u];

static uint32_t audio_phase_step_q32(uint32_t freq_hz)
{
    uint64_t num = ((uint64_t)freq_hz << 32);
    return (uint32_t)(num / (uint64_t)AUDIO_SAMPLE_RATE_HZ);
}

static void audio_start_tone(uint32_t freq_hz, uint32_t duration_ms, int16_t amp)
{
    uint32_t n = (AUDIO_SAMPLE_RATE_HZ * duration_ms) / 1000u;
    if (n == 0u) n = 1u;

    s_audio.phase_step_q32 = audio_phase_step_q32(freq_hz);
    s_audio.samples_left = n;

    {
        int64_t v = (int64_t)amp * (int64_t)s_audio.volume_gain_q12;
        v /= 4096ll;
        if (v > 30000) v = 30000;
        if (v < -30000) v = -30000;
        s_audio.amp = (int16_t)v;
    }
}

static bool audio_try_start_win_tune_step(void)
{
    static const uint16_t k_win_freq_hz[] = {523u, 659u, 784u, 1047u};
    static const uint16_t k_win_dur_ms[] = {70u, 70u, 90u, 170u};
    static const uint16_t k_win_gap_ms[] = {20u, 20u, 24u, 0u};

    if (!s_audio.win_tune_active) return false;
    if (s_audio.samples_left > 0u) return false;
    if (s_audio.win_tune_gap_samples > 0u) return false;

    if (s_audio.win_tune_step >= (uint8_t)(sizeof(k_win_freq_hz) / sizeof(k_win_freq_hz[0])))
    {
        s_audio.win_tune_active = false;
        s_audio.win_tune_step = 0u;
        return false;
    }

    audio_start_tone((uint32_t)k_win_freq_hz[s_audio.win_tune_step],
                     (uint32_t)k_win_dur_ms[s_audio.win_tune_step],
                     2000);
    s_audio.win_tune_gap_samples = (AUDIO_SAMPLE_RATE_HZ * (uint32_t)k_win_gap_ms[s_audio.win_tune_step]) / 1000u;
    s_audio.win_tune_step++;
    return true;
}

static int16_t audio_next_sample(void)
{
    if (s_audio.samples_left == 0u)
    {
        if (s_audio.win_tune_gap_samples == 0u)
        {
            if (audio_try_start_win_tune_step())
            {
                /* Started next win note. */
            }
            else if (s_audio.pending_win_tune > 0u)
            {
                s_audio.pending_win_tune = 0u;
                s_audio.win_tune_active = true;
                s_audio.win_tune_step = 0u;
                (void)audio_try_start_win_tune_step();
            }
        }

        if (!s_audio.win_tune_active && (s_audio.pending_point > 0u))
        {
            s_audio.pending_point--;
            audio_start_tone(490u, 140u, 2000);
        }
        else if (!s_audio.win_tune_active && (s_audio.pending_paddle > 0u))
        {
            s_audio.pending_paddle--;
            audio_start_tone(459u, 90u, 1900);
        }
        else if (!s_audio.win_tune_active && (s_audio.pending_wall > 0u))
        {
            s_audio.pending_wall--;
            audio_start_tone(226u, 40u, 1700);
        }
    }

    int16_t s = 0;
    if (s_audio.samples_left > 0u)
    {
        s = (s_audio.phase_q32 & 0x80000000u) ? s_audio.amp : (int16_t)(-s_audio.amp);
        s_audio.phase_q32 += s_audio.phase_step_q32;
        s_audio.samples_left--;
    }

    if (s_audio.win_tune_gap_samples > 0u)
    {
        s_audio.win_tune_gap_samples--;
    }

    return s;
}

static void audio_fill_frames(uint32_t frame_offset, uint32_t frame_count)
{
    uint32_t base = frame_offset * 2u;
    for (uint32_t i = 0; i < frame_count; i++)
    {
        int16_t s = audio_next_sample();
        s_dma_pcm[base + (2u * i) + 0u] = s;
        s_dma_pcm[base + (2u * i) + 1u] = s;
    }
}

static uint8_t audio_sat_add_u8(uint8_t a, uint8_t b)
{
    uint16_t sum = (uint16_t)a + (uint16_t)b;
    return (sum > 255u) ? 255u : (uint8_t)sum;
}

bool audio_hal_init(void)
{
    if (s_audio.init_ok) return true;

    BSP_AUDIO_Init_t init = {
        .Device = AUDIO_OUT_HEADPHONE,
        .SampleRate = AUDIO_FREQUENCY_16K,
        .BitsPerSample = AUDIO_RESOLUTION_16B,
        .ChannelsNbr = 2u,
        .Volume = 55u,
    };

    if (BSP_AUDIO_OUT_Init(0u, &init) != BSP_ERROR_NONE)
    {
        s_audio.init_ok = false;
        return false;
    }

    s_audio.volume_percent = 60u;
    s_audio.volume_gain_q12 = 4096u;
    audio_hal_set_volume(s_audio.volume_percent);

    audio_fill_frames(0u, AUDIO_BUFFER_FRAMES);

    if (BSP_AUDIO_OUT_Play(0u, (uint8_t *)s_dma_pcm, sizeof(s_dma_pcm)) != BSP_ERROR_NONE)
    {
        (void)BSP_AUDIO_OUT_DeInit(0u);
        s_audio.init_ok = false;
        return false;
    }

    s_audio.init_ok = true;
    return true;
}

void audio_hal_set_volume(uint8_t percent)
{
    if (percent > 100u) percent = 100u;
    s_audio.volume_percent = percent;

    if (percent == 0u)
    {
        s_audio.volume_gain_q12 = 0u;
    }
    else if (percent <= 50u)
    {
        s_audio.volume_gain_q12 = ((uint32_t)percent * 4096u) / 50u;
    }
    else
    {
        const uint32_t max_gain_q12 = 32535u;
        s_audio.volume_gain_q12 = 4096u + (((uint32_t)(percent - 50u) * (max_gain_q12 - 4096u)) / 50u);
    }
}

void audio_hal_queue_wall_bounce(uint8_t count)
{
    s_audio.pending_wall = audio_sat_add_u8(s_audio.pending_wall, count);
}

void audio_hal_queue_paddle_hit(uint8_t count)
{
    s_audio.pending_paddle = audio_sat_add_u8(s_audio.pending_paddle, count);
}

void audio_hal_queue_point_scored(uint8_t count)
{
    s_audio.pending_point = audio_sat_add_u8(s_audio.pending_point, count);
}

void audio_hal_queue_win_tune(void)
{
    s_audio.pending_win_tune = 1u;
}

void audio_hal_update(void)
{
    if (!s_audio.init_ok) return;

    while (s_audio.dma_half_pending > 0u)
    {
        s_audio.dma_half_pending--;
        audio_fill_frames(0u, AUDIO_HALF_FRAMES);
    }

    while (s_audio.dma_full_pending > 0u)
    {
        s_audio.dma_full_pending--;
        audio_fill_frames(AUDIO_HALF_FRAMES, AUDIO_HALF_FRAMES);
    }
}

void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
    if (Instance == 0u)
    {
        if (s_audio.dma_half_pending < 255u) s_audio.dma_half_pending++;
    }
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
    if (Instance == 0u)
    {
        if (s_audio.dma_full_pending < 255u) s_audio.dma_full_pending++;
    }
}

void GPDMA1_Channel2_IRQHandler(void)
{
    BSP_AUDIO_OUT_IRQHandler(0u, AUDIO_OUT_HEADPHONE);
}
