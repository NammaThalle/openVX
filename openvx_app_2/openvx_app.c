#include "openvx_app.h"

static const int IMAGE_WIDTH = 1920;
static const int IMAGE_HEIGHT = 1080;

void openvx_app(int argc, char *argv[])
{
  vx_context context;
  vx_image image;
  vx_imagepatch_addressing_t image_addr;
  vx_rectangle_t rect;
  vx_map_id map_id;
  void *data_ptr;
  vx_uint32 bytes_per_pixel = 2;
  vx_uint32 x, y;
  vx_uint8 *pixel;
  FILE *fp;

  context = vxCreateContext();
  APP_ASSERT_VALID_REF(context);

  image = vxCreateImage(context, IMAGE_WIDTH, IMAGE_HEIGHT, VX_DF_IMAGE_UYVY);

  rect.start_x = 0;
  rect.start_y = 0;
  rect.end_x = IMAGE_WIDTH;
  rect.end_y = IMAGE_HEIGHT;

  vxMapImagePatch(image, &rect, 0, &map_id, &image_addr, &data_ptr,
                  VX_READ_ONLY, VX_MEMORY_TYPE_HOST, VX_NOGAP_X);

  if (!data_ptr)
  {
    printf("Error mapping image to memory\n");
    vxReleaseImage(&image);
    return;
  }

  pixel = (vx_uint8 *)data_ptr;

  for (y = 0; y < IMAGE_HEIGHT; y++)
  {
    for (x = 0; x < IMAGE_WIDTH; x += 2)
    {
      if (y < (IMAGE_HEIGHT / 3))
      {
        *pixel = 84;
        *(pixel + 1) = 76;
        *(pixel + 2) = 255;
        *(pixel + 3) = 76;
      }
      else if (y < (2 * IMAGE_HEIGHT / 3))
      {
        *pixel = 43;
        *(pixel + 1) = 149;
        *(pixel + 2) = 21;
        *(pixel + 3) = 149;
      }
      else
      {
        *pixel = 255;
        *(pixel + 1) = 29;
        *(pixel + 2) = 107;
        *(pixel + 3) = 29;
      }
      pixel += (bytes_per_pixel * 2);
    }
  }

  fp = fopen("image.uyvy", "wb");
  if (fp)
  {
    fwrite(data_ptr, 1, IMAGE_WIDTH * IMAGE_HEIGHT * bytes_per_pixel, fp);
    fclose(fp);
    printf("Created image.uyvy\n");
  }
  else
  {
    printf("Error creating image.uyvy\n");
  }

  vxUnmapImagePatch(image, map_id);
  vxReleaseImage(&image);

  vxReleaseContext(&context);
}