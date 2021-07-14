import numpy as np
import cv2
import argparse
import os

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('uyvy_file', type=str, help='uyvy file')
  parser.add_argument('frame_size', type=str, help='WxH')
  args = parser.parse_args()
  w, h = [int(i) for i in args.frame_size.split('x')]

  uyvy = np.fromfile(args.uyvy_file, dtype=np.uint8).reshape((h, w, 2))
  bgr = cv2.cvtColor(uyvy, cv2.COLOR_YUV2BGR_UYVY)
  cv2.imwrite('{}.png'.format(os.path.basename(args.uyvy_file)), bgr)
