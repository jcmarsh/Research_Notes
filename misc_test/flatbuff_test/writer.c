#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

// Flat Buff Stuff
#include "cfs_evs_builder.h"
#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(cFSEVS_Test, x)
#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))
flatcc_builder_t builder;

#define CFE_MISSION_EVS_MAX_MESSAGE_LENGTH 80

// Future client functions for EVS
int CFE_EVS_SendEvent(uint16_t EventID, uint16_t EventType, const char *Spec, ... ) {
  flatcc_builder_t *B = &builder;

  int rv;
  va_list Ptr;
  char spec_buffer[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH] = {0};
  int ExpandedLength;

  va_start(Ptr, Spec);
  ExpandedLength = vsnprintf(spec_buffer, sizeof(spec_buffer), Spec, Ptr);
  printf("What's the new string? %s\n", spec_buffer);
  va_end(Ptr);

  flatbuffers_string_ref_t s;
  s = flatbuffers_string_create_str(B, spec_buffer);

  void *buffer;
  size_t size;

  /* Construct a buffer specific to schema. */
  ns(SendEvent_create_as_root(B, EventID, EventType, s));

  /* Retrieve buffer - see also `flatcc_builder_get_direct_buffer`. */
  /* buffer = flatcc_builder_finalize_buffer(B, &size); */
  buffer = flatcc_builder_finalize_aligned_buffer(B, &size);

  printf("Writing EVS MSG\n");
  // Open file, write, close file
  FILE *fp;
  fp = fopen("./evs_msg", "wb");
  fwrite(buffer, size, 1, fp);

  /* free(buffer); */
  flatcc_builder_aligned_free(buffer);

  /*
  * Reset, but keep allocated stack etc.,
  * or optionally reduce memory using `flatcc_builder_custom_reset`.
  */
  flatcc_builder_reset(B);

  return 0;
}

int main() {
  printf("Hello!\n");
  int rv;

  // Flat Buff Stuff
  flatcc_builder_t *B = &builder;
  flatcc_builder_init(B);

  CFE_EVS_SendEvent (8, 7, "Writer send event. Version %d.%d.%d.%d", 0, 1, 2, 3);

  return 0;
}
