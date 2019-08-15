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

void speex_decode_task()
{
    printf("SPEEX: Start decode\n");

    FILE *fin;
    FILE *fout;

    // Holds the audio that will be written to file (16 bits per sample)
    int16_t out[FRAME_SIZE];
    char buf[PACKAGE_SIZE];

    // Create a new decoder state in narrowband mode
    void *decoder_state = speex_decoder_init(&speex_nb_mode);

    // Set the perceptual enhancement on
    uint32_t tmp = 1;
    speex_decoder_ctl(decoder_state, SPEEX_SET_ENH, &tmp);

    fin = fopen("/sdcard/speex.spx", "r");
    if (!fin)
    {
        printf("Could not open /sdcard/speex.spx\n");
        while (1) {}
    }
    fout = fopen("/sdcard/rawpcm.wav", "w");

    // Initialization of the structure that holds the bits
    SpeexBits bits;
    speex_bits_init(&bits);

    while (1)
    {
        // Read data from file
        size_t pkg_size = fread(buf, 1, PACKAGE_SIZE, fin);
        // Copy the data into the bit-stream struct
        speex_bits_read_from(&bits, buf, pkg_size);
        // Decode the data
        speex_decode_int(decoder_state, &bits, out);
        // write to file
        fwrite(out, 2, FRAME_SIZE, fout);

        if (feof(fin))
            break;
    }
    // Destroy the decoder state
    speex_decoder_destroy(decoder_state);
    // Destroy the bit-stream struct
    speex_bits_destroy(&bits);

    fclose(fout);
    fclose(fin);

    printf("SPEEX: Done decode\n");

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

    xTaskCreate(speex_decode_task, "speex_decode_task", 4096 * 2, NULL, 5, NULL);
}

