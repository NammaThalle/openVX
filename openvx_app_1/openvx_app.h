#ifndef OPENVX_APP_H_
#define OPENVX_APP_H_

#include <TI/tivx.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define APP_ASSERT(x) assert((x))
#define APP_ASSERT_VALID_REF(ref) (APP_ASSERT(vxGetStatus((vx_reference)(ref)) == VX_SUCCESS));

#endif