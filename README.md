# MathCalculatorCPP
A program to recognize numbers from images and calculate the processed image.

All cpp file is included in "src" folder   
All header file is included in "header" folder   
   
This program has the following flow:    
1. Capture screen to get the image of the image containing some math expression    
   There is some sample screen-captured images in the folder Image/Rename    
   
2. Using opencv:   
   -Extract each number and math sign from the whole screen-captured image   
   -Compare with each template in the folder Image/Select and return the most matching result   
   -Convert the whole math expression in image type to string 
   -Use function: takeSceneMatAndReturnString() to do all above tasks    
   -Functions related to image processing is in the file ImageProcessor.cpp   
   
3. Calculate:   
   -Calculate the math string and check if this math-expression is TRUE of FALSE   
   -Using functions in MathExpression.cpp file to perform this
   