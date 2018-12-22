# Image2ASCII

## Creating ASCII video
Looking at [this project](https://github.com/joelibaceta/video-to-ascii) specifically, it seems that the go-to for creating images from ASCII is to simply color each character based on the average color of each sub-area.  This seems like it could be greatly improved.  This image seems completely illegible without colors, which I think should not be required.

Here's an example of good ASCII art (IMHO) that lacks colors.  
![Example of good ASCII art](https://static1.squarespace.com/static/5a0b990680bd5e8101834740/t/5ac01c4b70a6ad85f4b336b7/1522539613237/Screen+Shot+2018-03-31+at+7.39.37+PM.png?format=500w)

Why not do the following: 
1. Segment the screen into pixel regions (depending on how accurate we want this to be, tradeoff between less characters and more accuracy)
2. For each pixel region, augment the differences apparent between two sides of the image and find the best match ASCII character.  

| Character  | Image |
| ------------- | ------------- |
| \ |   |

| Builder  | Image |
| ------------- | ------------- |
| [video-to-ascii](https://github.com/joelibaceta/video-to-ascii) | ![this image](https://github.com/joelibaceta/video-to-ascii/blob/master/images/imgPixelImage.png)  |
| Hand-Drawn  | TBD  |
| Auto-Generated  | TBD  |

