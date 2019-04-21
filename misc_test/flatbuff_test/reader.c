#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

// Flat Buff Stuff
#include "cfs_evs_reader.h"
#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(cFSEVS_Test, x)
#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))

#define CFE_MISSION_EVS_MAX_MESSAGE_LENGTH 80

int main() {
    printf("Hello!\n");
    int rv;

    char buffer[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH * 2] = {0};

    FILE *fp = fopen("./evs_msg", "rb");
    fread(buffer, 1, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH * 2, fp);

    ns(SendEvent_table_t) sendEvent = ns(SendEvent_as_root(buffer));

    uint16_t EventID = ns(SendEvent_EventID(sendEvent));
    uint16_t EventType = ns(SendEvent_EventType(sendEvent));
    flatbuffers_string_t spec_string = ns(SendEvent_Spec(sendEvent));
    printf("RECEIVED \"%d, %d, %s\"\n", EventID, EventType, spec_string);

    return 0;
}
