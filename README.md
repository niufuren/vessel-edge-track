##Vessel Edge Track

This project aims to track edge points starting from an inner circle of a ring mask in a retinal image. It builds on the result of Canny edge detection algorithm. The traked edge points are those detected by Canny edge detection. This is a preliminary step to diagnose eye disease by processing the retinal image. The tracked edges will be used further to find out vessels and diagnose whether an eye vessel is abnormal or not. 

An example of a retinal image is shown below:

![alt tag](https://raw2.github.com/niufuren/vessel-edge-track/master/images/image1.png)

The processing result is the image below, where Canny edge detection is marked in blue, the ring mask is marked in green, and the tracked edge points are marked in red. The detailed coordinates of the tracked edge points can be found in [here](https://raw2.github.com/niufuren/vessel-edge-track/master/results/edgePointCoordinate.txt)

![alt tag](https://raw2.github.com/niufuren/vessel-edge-track/master/results/result.jpg)
