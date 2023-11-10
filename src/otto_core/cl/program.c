#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

const char *_OTTO_KERNELS_CORE[] = {
    OTTO_CLKERNEL("vector/elementary.cl"),
    OTTO_CLKERNEL("matrix/elementary.cl"),
};
const char *_OTTO_KERNELS_CORE_NAMES[] = {
    // Vectors
    "otto_vector_add__u8",
    "otto_vector_add__u16",
    "otto_vector_add__u32",
    "otto_vector_add__u64",
    "otto_vector_add__i8",
    "otto_vector_add__i16",
    "otto_vector_add__i32",
    "otto_vector_add__i64",
    "otto_vector_add__f32",
    "otto_vector_add__f64",

    "otto_vector_sub__u8",
    "otto_vector_sub__u16",
    "otto_vector_sub__u32",
    "otto_vector_sub__u64",
    "otto_vector_sub__i8",
    "otto_vector_sub__i16",
    "otto_vector_sub__i32",
    "otto_vector_sub__i64",
    "otto_vector_sub__f32",
    "otto_vector_sub__f64",

    "otto_vector_mul__u8",
    "otto_vector_mul__u16",
    "otto_vector_mul__u32",
    "otto_vector_mul__u64",
    "otto_vector_mul__i8",
    "otto_vector_mul__i16",
    "otto_vector_mul__i32",
    "otto_vector_mul__i64",
    "otto_vector_mul__f32",
    "otto_vector_mul__f64",

    "otto_vector_div__u8",
    "otto_vector_div__u16",
    "otto_vector_div__u32",
    "otto_vector_div__u64",
    "otto_vector_div__i8",
    "otto_vector_div__i16",
    "otto_vector_div__i32",
    "otto_vector_div__i64",
    "otto_vector_div__f32",
    "otto_vector_div__f64",

    // Matrices
};
const size_t _OTTO_KERNELS_CORE_COUNT =
    sizeof(_OTTO_KERNELS_CORE) / sizeof(char *);
const size_t _OTTO_KERNELS_CORE_NAMES_COUNT =
    sizeof(_OTTO_KERNELS_CORE_NAMES) / sizeof(char *);

const char *_OTTO_KERNELS_ALL[] = {
    OTTO_CLKERNEL("vector/elementary.cl"),
    OTTO_CLKERNEL("matrix/elementary.cl"),
};
const char *_OTTO_KERNELS_ALL_NAMES[] = {
    // Vectors
    "otto_vector_add__u8",
    "otto_vector_add__u16",
    "otto_vector_add__u32",
    "otto_vector_add__u64",
    "otto_vector_add__i8",
    "otto_vector_add__i16",
    "otto_vector_add__i32",
    "otto_vector_add__i64",
    "otto_vector_add__f32",
    "otto_vector_add__f64",

    "otto_vector_sub__u8",
    "otto_vector_sub__u16",
    "otto_vector_sub__u32",
    "otto_vector_sub__u64",
    "otto_vector_sub__i8",
    "otto_vector_sub__i16",
    "otto_vector_sub__i32",
    "otto_vector_sub__i64",
    "otto_vector_sub__f32",
    "otto_vector_sub__f64",

    "otto_vector_mul__u8",
    "otto_vector_mul__u16",
    "otto_vector_mul__u32",
    "otto_vector_mul__u64",
    "otto_vector_mul__i8",
    "otto_vector_mul__i16",
    "otto_vector_mul__i32",
    "otto_vector_mul__i64",
    "otto_vector_mul__f32",
    "otto_vector_mul__f64",

    "otto_vector_div__u8",
    "otto_vector_div__u16",
    "otto_vector_div__u32",
    "otto_vector_div__u64",
    "otto_vector_div__i8",
    "otto_vector_div__i16",
    "otto_vector_div__i32",
    "otto_vector_div__i64",
    "otto_vector_div__f32",
    "otto_vector_div__f64",

    // Matrices
};
const size_t _OTTO_KERNELS_ALL_COUNT =
    sizeof(_OTTO_KERNELS_ALL) / sizeof(char *);
const size_t _OTTO_KERNELS_ALL_NAMES_COUNT =
    sizeof(_OTTO_KERNELS_ALL_NAMES) / sizeof(char *);

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
  if (sources == NULL) {
    logi_error("Could not allocate files");
    return OTTO_STATUS_FAILURE;
  }
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
    source = malloc((size + 1) * sizeof(char));
    if (source == NULL) {
      logi_warn("Could not allocate contents");
      continue;
    }
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

otto_status_t otto_program_from_default(otto_runtime_t *ctx,
                                        const otto_program_kernels_t kernels,
                                        const char *build_options,
                                        otto_program_t *out) {
  const char **files;
  const char **ker_names;
  size_t count;
  size_t ker_count;
  switch (kernels) {
  case OTTO_KERNELS_NONE:
    logi_info("Using no kernels");
    return OTTO_STATUS_SUCCESS;

  case OTTO_KERNELS_CORE:
    files = _OTTO_KERNELS_CORE;
    count = _OTTO_KERNELS_CORE_COUNT;
    ker_names = _OTTO_KERNELS_CORE_NAMES;
    ker_count = _OTTO_KERNELS_CORE_NAMES_COUNT;
    logi_info("Using CORE kernels");
    break;

  case OTTO_KERNELS_ALL:
    files = _OTTO_KERNELS_ALL;
    count = _OTTO_KERNELS_ALL_COUNT;
    ker_names = _OTTO_KERNELS_ALL_NAMES;
    ker_count = _OTTO_KERNELS_ALL_NAMES_COUNT;
    logi_info("Using ALL kernels");
    break;

  default:
    logi_error("Could not interpret kernels");
    return OTTO_STATUS_FAILURE;
  }

  logi_debug("Loading %i files", count);
  logi_debug("Loading %i kernels", ker_count);

  OTTO_CALL_I(otto_program_from_files(ctx, files, count, build_options, out),
              "Could not create program from files");

  // Since we know the kernels we can create them right away
  for (size_t i = 0; i < ker_count; i++, ker_names++) {
    OTTO_CALL_I(otto_kernel_new(out, *ker_names, 3, ctx, NULL),
                "Could not load kernel '%s'", *ker_names);
  }
  return OTTO_STATUS_SUCCESS;
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
