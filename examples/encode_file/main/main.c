#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <esp_vfs_fat.h>
#include <driver/sdmmc_host.h>
#include <sdmmc_cmd.h>
#include <speex/speex.h>

#define FRAME_SIZE   160
#define PACKAGE_SIZE 64

void speex_encode_task()
{
    printf("SPEEX: Start encode\n");

    int16_t in[FRAME_SIZE];
    char buf[PACKAGE_SIZE];

    // Create a new encoder state in narrowband mode
    void *encoder_state = speex_encoder_init(&speex_nb_mode);

    // Set quality to 8 (15 kbps)
    uint32_t tmp = 8;
    speex_encoder_ctl(encoder_state, SPEEX_SET_QUALITY, &tmp);

    FILE *fin = fopen("/sdcard/rawpcm.wav", "r");
    if (!fin)
    {
        printf("Could not open /sdcard/rawpcm.wav\n");
        while (1) {}
    }
    FILE *fout = fopen("/sdcard/speex.spx", "w");

    // Initialization of the structure that holds the bits
    SpeexBits bits;
    speex_bits_init(&bits);

    while (1)
    {
        fread(in, 2, FRAME_SIZE, fin);
        if (feof(fin))
            break;

        // Flush all the bits in the struct so we can encode a new frame
        speex_bits_reset(&bits);
        // Encode frame
        speex_encode_int(encoder_state, in, &bits);
        // Copy the bits to an array of char that can be written
        size_t s = speex_bits_write(&bits, buf, PACKAGE_SIZE);
        // write to file
        fwrite(buf, 1, s, fout);
    }
    // Destroy the decoder state
    speex_encoder_destroy(encoder_state);
    // Destroy the bit-stream struct
    speex_bits_destroy(&bits);

    fclose(fout);
    fclose(fin);

    printf("SPEEX: Done encode\n");

    while (1) {}
}

void app_main()
{
    // init SD-card
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 10
    };
    sdmmc_card_t *card;
    ESP_ERROR_CHECK(esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card));
    sdmmc_card_print_info(stdout, card);

    xTaskCreate(speex_encode_task, "speex_encode_task", 4096 * 2, NULL, 5, NULL);
}

