#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <otto/cl/cl.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

int64_t get_file_size(FILE *file) {
  fseek(file, 0, SEEK_END);
  int64_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

otto_status_t otto_program_from_sources(const otto_runtime_t *ctx,
                                        const char **sources,
                                        const size_t count,
                                        const char *build_options,
                                        otto_program_t *out) {
  cl_int status = CL_SUCCESS;

  logi_info("Creating program");
  cl_program p =
      clCreateProgramWithSource(ctx->ctx, count, sources, NULL, &status);
  if (status != CL_SUCCESS) {
    logi_error("Could not create program from sources (%d)", status);
    return OTTO_STATUS_FAILURE;
  }

  otto_program_t prog = {
      .p = p,
  };

  if (build_options != NULL) {
    otto_status_t s = otto_program_build(&prog, ctx, build_options);
    if (s != OTTO_STATUS_SUCCESS) {
      logi_error("Failed to build, make sure to run `otto_program_build` "
                 "on the program to try again");
    }
  } else {
    logi_warn("Build options was null, make sure to run `otto_program_build` "
              "on the program");
  }
  *out = prog;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_program_from_files(otto_runtime_t *ctx, const char **files,
                                      const size_t count,
                                      const char *build_options,
                                      otto_program_t *out) {
  char **sources = malloc(count * sizeof(char *));
  FILE *f;
  char *source;
  int64_t size = 0;

  for (size_t i = 0; i < count; i++, files++) {
    logi_info("Reading file '%s'", *files);
    f = fopen(*files, "r");
    if (!f) {
      logi_warn("Could not open file '%s', skipping it", *files);
      continue;
    }

    size = get_file_size(f);
    logi_debug("File size: %i", size);
    source = malloc(size * sizeof(char));
    fread(source, 1, size, f);
    source[size] = 0;
    fclose(f);
    logi_debug("Closed file '%s'", *files);

    sources[i] = source;
  }

  ctx->_sources = (const char **)sources;
  ctx->_sources_count = count;
  return otto_program_from_sources(ctx, (const char **)sources, count,
                                   build_options, out);
}

otto_status_t otto_program_cleanup(const otto_program_t *prog) {
  OTTO_CL_CALL_I(clReleaseProgram(prog->p), "Failed releasing program (%d)",
                 err_);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_program_build(const otto_program_t *prog,
                                 const otto_runtime_t *ctx,
                                 const char *options) {
  logi_info("Building program");
  OTTO_CL_CALL_I(clBuildProgram(prog->p, ctx->device_num, &ctx->devices,
                                options, NULL, NULL),
                 "Failed building program");
  return OTTO_STATUS_SUCCESS;
}
