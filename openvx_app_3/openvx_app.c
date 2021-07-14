#include "openvx_app.h"

static const int IMAGE_WIDTH = 1920;
static const int IMAGE_HEIGHT = 1080;

// wa#define DEBUG

#ifdef DEBUG
#define APP_DEBUG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define APP_DEBUG(f_, ...)
#endif


vx_status openvx_app(int argc, char *argv[])
{
    vx_context context;
    vx_graph graph;
    vx_rectangle_t rect;
    vx_image uyvy, rgb;
    vx_map_id uyvy_map_id, rgb_map_id;
    vx_imagepatch_addressing_t uyvy_image_addr, rgb_image_addr;
    vx_node node;
    vx_status status;

    void *uyvy_data_ptr, *rgb_data_ptr;
    FILE *fprgb, *fpuyvy;
    uint32_t x, y;

    context = vxCreateContext();
    graph = vxCreateGraph(context);

    APP_DEBUG("DEBUG : Graph created\n");

    uyvy = vxCreateImage(context, IMAGE_WIDTH, IMAGE_HEIGHT, VX_DF_IMAGE_UYVY);
    rgb = vxCreateImage(context, IMAGE_WIDTH, IMAGE_HEIGHT, VX_DF_IMAGE_RGB);

    APP_DEBUG("DEBUG : Images created\n");

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = IMAGE_WIDTH;
    rect.end_y = IMAGE_HEIGHT;

    vxMapImagePatch(uyvy,
                    &rect,
                    0,
                    &uyvy_map_id,
                    &uyvy_image_addr,
                    &uyvy_data_ptr,
                    VX_READ_AND_WRITE,
                    VX_MEMORY_TYPE_HOST,
                    VX_NOGAP_X);

    vxMapImagePatch(rgb,
                    &rect,
                    0,
                    &rgb_map_id,
                    &rgb_image_addr,
                    &rgb_data_ptr,
                    VX_READ_AND_WRITE,
                    VX_MEMORY_TYPE_HOST,
                    VX_NOGAP_X);

    APP_DEBUG("DEBUG : Images Mapped\n");
    
    fpuyvy = fopen("/mnt/work/ti/install/psdkra_old/vision_apps/apps/Excercise/openvx_app_3/image.uyvy", "rb");
    if (!fpuyvy)
    {
        printf("Error opening image.uyvy");
        return VX_FAILURE;
    }

    APP_DEBUG("DEBUG : image.uyvy opened\n");

    fread(uyvy_data_ptr, 1, IMAGE_WIDTH * IMAGE_HEIGHT * 2, fpuyvy);

    APP_DEBUG("DEBUG : file read\n");

    node = vxColorConvertNode(graph, uyvy, rgb);

    APP_DEBUG("DEBUG : Performing Color Convert\n");

    status = vxVerifyGraph(graph);

    if (status == VX_SUCCESS)
    {
        APP_DEBUG("DEBUG : Graph Verified\n");
        status = vxProcessGraph(graph);
        if (status == VX_SUCCESS)
        {
            APP_DEBUG("DEBUG : Graph Processed successfully\n");
        }
        else
        {
            APP_DEBUG("DEBUG : Graph Processing failed\n");
        }
    }
    else
    {
        APP_DEBUG("DEBUG : Verify graph failed\n");
    }

    fprgb = fopen("image.rgb", "wb");
    if (!fprgb)
    {
        printf("Error opening image.rgb");
        return VX_FAILURE;
    }

    APP_DEBUG("DEBUG : image.rgb opened\n");

    int bytes = fwrite(rgb_data_ptr, 1, IMAGE_HEIGHT * IMAGE_WIDTH * 3, fprgb);

    APP_DEBUG("DEBUG : %d bytes written\n", bytes);

    fclose(fprgb);
    fclose(fpuyvy);

    vxUnmapImagePatch(uyvy, uyvy_map_id);
    vxUnmapImagePatch(rgb, rgb_map_id);

    vxReleaseImage(&uyvy);
    vxReleaseImage(&rgb);

    vxReleaseGraph(&graph);

    vxReleaseContext(&context);

    printf("Image converted from uyvy to rgb\n");
    return VX_SUCCESS;
}