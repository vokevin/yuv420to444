# yuv420to444

This is a simple code to show how to convert YUV 420 planar format to YUV planar/semi-planar. For up-converting the UV channels, it supports the basic methods which are nearest neighbor and bi-linear filtering. Default is nearest neighbor.

The conversion can be done directly from 420 -> 444, or 420 -> 422 -> 444.

Usage: yuv420to444 <input yuv 420p file> <width> <height> <output file>
       options:
         planar = to enable planar output format
         bilinear = to enable bi-linear filtering

To generate the YUV file from JPEG, use the ffmpeg.

Use the YUView (https://github.com/IENT/YUView) to check the output YUV444 file.
