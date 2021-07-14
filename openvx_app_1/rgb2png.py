import numpy as np
import cv2
import argparse
import os

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('rgb_file', type=str, help='rgb file')
  parser.add_argument('frame_size', type=str, help='WxH')
  args = parser.parse_args()
  w, h = [int(i) for i in args.frame_size.split('x')]

  rgb = np.fromfile(args.rgb_file, dtype=np.uint8).reshape((h, w, 3))
  bgr = cv2.cvtColor(rgb, cv2.COLOR_RGB2BGR)
  cv2.imwrite('{}.png'.format(os.path.basename(args.rgb_file)), bgr)
