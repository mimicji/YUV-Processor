# YUV-Processor #

YUV-Processor is a homework (Lab4.1) of Computer Architecture. It is developed by [Kaihang Ji](https://github.com/mimicji). All rights reserved.

Mainly it has two functions:

- Fade in and fade out one image.
- Create a gradient which overlays two images.

To choose an Instruction Set, choose a namespace from Non_Simd / MMX / AVX / SSE.

Happy feast of winter veil!

## Platform ##

WIN32 and Linux are both available. An Intel CPU which supports AVX2 is requested for running it.

However, on Linux AVX may cause a segmentation fault at yuv2rgb:Line 198 because of reading `*src_y`. The reason remains unknowned, even I modified it into 
`*src_y = OFFSET_16;`. However, we can modify the content at `src_y` by pointer `src_yuv->y16`.

## Input/Output file format ##

YUV420

default: 1920*1080

## Demo ##

Click [here](http://pan.baidu.com/s/1slMtMP3) to see demos. You may need a YUV player to open it. [YUV Player Deluxer](http://www.yuvplayer.com/)
