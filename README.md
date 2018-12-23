# Image2ASCII

## Motivation
Looking at common approaches to imagery->ascii conversion, it seems that the go-to for creating images is to simply color each character based on the average color and density of each defined region.  What about black and white images, then?  The approach seems like it could be improved.  

## Approach
1. Convert image to grayscale.  
2. Segment the screen into pixel regions (depending on how accurate we want this to be, tradeoff between less characters and more accuracy)
2. For each pixel region, observe the mean pixel value differences by segmenting the image diagonally, vertically, and horizontally, then choosing one of `/\|-` to replace it with.  

## Examples
(You'll probably want to change letter spacing if you use the output elsewhere)
<table>
  <tr>
    <td>
      <img src="exampleimages/butterfly.png"></img> 
    </td>
    <td>
<pre>/\\-                     /-//\
\|-\/--                /--/  /
\||/| -\              /  |\-||
| \- /\/--          /-\/- -/ |
 \ /--|-- -        / --/--\-- 
 | /---\ \-\      //- /--/  | 
   /   \   \\|\/|//       \   
  /|\   --- \\/-// /---- ||\  
  |        --\/\//--       |  
   |\-------  /\| --------|   
    --\/ ----||||---- \//-    
    / -  /-- |  | \- - \|\    
   / |--/| //||| \\  \\-\/|   
   |// |/ - /||||\  -|  -||   
   // /\  |/||||  \  -/\ \|   
   ||    | // || \\ |   \||   
   |/\/|| /   ||   \ | \/ |   
    \  -\ |//    \||\/-  /    
     \    \/      \/    /     
       ----        ---- </pre>
    </td>
    <td>
      <img src="exampleimages/pencil.jpeg"></img> 
    </td>
    <td>
<pre>                     -----    
                   //    \\   
                 /         \- 
                /- -        \\
              /     --        
             //      \\      /
           /                / 
          //\\         /   /  
        /     \\      -  /    
       //           /   -     
     /           \-   /       
    //           /\ /-        
   /|          /   /          
   | -\          /            
  /|   |    /   /             
        ---   /               
 /|       |  -                
 /\    /----                  
/  \----                      
|--- </pre>
    </td>
  </tr>
  </table>
  
## Conversion Process
Clone this project in XCode and pass the image in question.  

