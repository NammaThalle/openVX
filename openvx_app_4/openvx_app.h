#ifndef OPENVX_APP_H_
#define OPENVX_APP_H_

#include <TI/tivx.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define LOG

#ifdef LOG
#define APP_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define APP_LOG(f_, ...)
#endif

#define UYVY_BYTES_PER_PIXEL    2
#define RGB_BYTES_PER_PIXEL     3
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
    
    FILE * uyvy_fp;
    FILE * rgb_fp;

} app_obj_t;


#define APP_ASSERT(x) assert((x))
#define APP_ASSERT_VALID_REF(ref) (APP_ASSERT(vxGetStatus((vx_reference)(ref)) == VX_SUCCESS));

#endif