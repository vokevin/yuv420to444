# yuv420to444

This is a simple code to show how to convert YUV420 planar format to YUV444 planar/semi-planar. For up-converting the UV channels, it supports the basic methods which are nearest neighbor and bi-linear filtering. Default is nearest neighbor.

The conversion can be done directly from 420 -> 444, or 420 -> 422 -> 444.

Usage: yuv420to444 input_yuv420p_file width height output_file

       options:
         planar = to enable the planar output format
         bilinear = to enable the bi-linear filtering

To generate the YUV file from JPEG, use the ffmpeg.

Use the YUView (https://github.com/IENT/YUView) to check the output YUV444 file.
