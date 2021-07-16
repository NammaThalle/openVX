#include "openvx_app.h"

//create variables to store the image resolution
static const int IMAGE_WIDTH = 1920;
static const int IMAGE_HEIGHT = 1080;

app_obj_t openvx_app_obj;

vx_status openvx_app_init(app_obj_t *obj)
{
    vx_status status = VX_FAILURE;

    //Create all the containers
    obj->context = vxCreateContext();
    APP_ASSERT_VALID_REF(obj->context);
    APP_LOG("LOG : CONTEXT CREATED\n");

    //Initialize the rectangle
    obj->rectangle.start_x = 0;
    obj->rectangle.start_y = 0;
    obj->rectangle.end_x = IMAGE_WIDTH;
    obj->rectangle.end_y = IMAGE_HEIGHT;

    obj->uyvy_fp = fopen("/mnt/work/ti/install/psdkra_old/vision_apps/apps/exercise/openvx_app_4/image.uyvy", "wb+");
    obj->rgb_fp = fopen("/mnt/work/ti/install/psdkra_old/vision_apps/apps/exercise/openvx_app_4/image.rgb", "wb");

    return VX_SUCCESS;
}

vx_status openvx_app_create_graph(app_obj_t *obj)
{
    vx_status status = VX_FAILURE;

    obj->graph = vxCreateGraph(obj->context);
    APP_LOG("LOG : GRAPH CREATED\n");

    obj->uyvy = vxCreateImage(obj->context, IMAGE_WIDTH, IMAGE_HEIGHT, VX_DF_IMAGE_UYVY);
    APP_LOG("LOG : UYVY IMAGE CREATED\n");

    obj->rgb = vxCreateImage(obj->context, IMAGE_WIDTH, IMAGE_HEIGHT, VX_DF_IMAGE_RGB);
    APP_LOG("LOG : RGB IMAGE CREATED\n");

    vxMapImagePatch(obj->uyvy,
                    &obj->rectangle,
                    0,
                    &obj->uyvy_map_id,
                    &obj->uyvy_image_addr,
                    &obj->uyvy_data_ptr,
                    VX_READ_AND_WRITE,
                    VX_MEMORY_TYPE_HOST,
                    VX_NOGAP_X);

    vxMapImagePatch(obj->rgb,
                    &obj->rectangle,
                    0,
                    &obj->rgb_map_id,
                    &obj->rgb_image_addr,
                    &obj->rgb_data_ptr,
                    VX_READ_AND_WRITE,
                    VX_MEMORY_TYPE_HOST,
                    VX_NOGAP_X);

    if (!obj->uyvy_data_ptr || !obj->rgb_data_ptr)
    {
        APP_LOG("ERROR MAPPING IMAGE TO MEMORY");
        return status;
    }

    return VX_SUCCESS;
}

vx_status openvx_app_run_graph(app_obj_t *obj)
{
    vx_status status = VX_FAILURE;

    vx_uint8 *pixel;

    pixel = (vx_uint8 *)obj->uyvy_data_ptr;

    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < IMAGE_WIDTH; x++)
        {
            if (y < (IMAGE_HEIGHT / 3))
            {
                *pixel = 43;
                *(pixel + 1) = 149;
                *(pixel + 2) = 21;
                *(pixel + 3) = 149;
                // *pixel = 255;
                // *(pixel + 1) = 87;
                // *(pixel + 2) = 51;
            }
            else if (y < (2 * IMAGE_HEIGHT / 3) && y > (IMAGE_HEIGHT / 3))
            {
                *pixel = 74;
                *(pixel + 1) = 93;
                *(pixel + 2) = 61;
                *(pixel + 3) = 93;
                // *pixel = 255;
                // *(pixel + 1) = 255;
                // *(pixel + 2) = 255;
            }
            else
            {
                *pixel = 101;
                *(pixel + 1) = 46;
                *(pixel + 2) = 94;
                *(pixel + 3) = 46;
                // *pixel = 0;
                // *(pixel + 1) = 255;
                // *(pixel + 2) = 0;
            }
            pixel += UYVY_BYTES_PER_PIXEL;
        }
    }
    
    fwrite(obj->uyvy_data_ptr, 1, IMAGE_HEIGHT * IMAGE_WIDTH * UYVY_BYTES_PER_PIXEL, obj->uyvy_fp);
    
    fread(obj->uyvy_data_ptr, 1, IMAGE_WIDTH * IMAGE_HEIGHT * UYVY_BYTES_PER_PIXEL, obj->uyvy_fp);
    vx_node node = vxColorConvertNode(obj->graph, obj->uyvy, obj->rgb);
    APP_LOG("LOG : CONVERTING COLOR\n");

    status = vxVerifyGraph(obj->graph);

    if (status == VX_SUCCESS)
    {
        status = vxProcessGraph(obj->graph);
        if (status == VX_SUCCESS)
        {
            APP_LOG("LOG : PROCESS GRAPH DONE\n");
        }
        else
        {
            APP_LOG("LOG : PROCESS GRAPH FAILED\n");
        }
    }

    int bytes = fwrite(obj->rgb_data_ptr, 1, IMAGE_WIDTH * IMAGE_HEIGHT * RGB_BYTES_PER_PIXEL, obj->rgb_fp);
    APP_LOG("LOG : CREATED image.rgb\n");
    APP_LOG("LOG : %d BYTES WRITTEN\n", bytes);

    return VX_SUCCESS;
}

vx_status openvx_app_deinit(app_obj_t *obj)
{
    vx_status status = VX_FAILURE;

    vxUnmapImagePatch(obj->rgb, obj->rgb_map_id);
    vxUnmapImagePatch(obj->uyvy, obj->uyvy_map_id);

    vxReleaseImage(&obj->rgb);
    vxReleaseImage(&obj->uyvy);

    vxReleaseGraph(&obj->graph);

    fclose(obj->rgb_fp);
    fclose(obj->uyvy_fp);

    vxReleaseContext(&obj->context);

    return status;
}

vx_status openvx_app(int argc, char *argv[])
{
    app_obj_t *obj = &openvx_app_obj;
    vx_status status = VX_FAILURE;

    status = openvx_app_init(obj);
    APP_LOG("LOG : APP INIT DONE\n");

    if (status == VX_SUCCESS)
    {
        status = openvx_app_create_graph(obj);
        APP_LOG("LOG : APP CREATE GRAPH DONE\n");
    }

    if (status == VX_SUCCESS)
    {
        status = openvx_app_run_graph(obj);
        APP_LOG("LOG : APP RUN GRAPH DONE\n");
    }

    if (status == VX_SUCCESS)
    {
        status = openvx_app_deinit(obj);
        APP_LOG("LOG : APP DEINIT DONE\n");
    }

    return status;
}