#include <stddef.h>

#include <otto/cl/cl.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

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
