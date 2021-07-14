#ifndef OPENVX_APP_H_
#define OPENVX_APP_H_

#include <TI/tivx.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct 
{
    vx_context context;
    vx_graph graph;

    vx_image uyvy;
    vx_image rgb;

    vx_rectangle_t rectangle;

    vx_map_id uyvy_map_id;
    vx_map_id rgb_map_id;

    vx_imagepatch_addressing_t uyvy_image_addr;
    vx_imagepatch_addressing_t rgb_image_addr;

    void * uyvy_data_ptr;
    void * rgb_data_ptr;

    vx_uint32 rgb_bytes_per_pixel;
    vx_uint32 uyvy_bytes_per_pixel;

} app_obj_t;


#define APP_ASSERT(x) assert((x))
#define APP_ASSERT_VALID_REF(ref) (APP_ASSERT(vxGetStatus((vx_reference)(ref)) == VX_SUCCESS));

#endif