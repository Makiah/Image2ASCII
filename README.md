# Image2ASCII

## Motivation
After looking at repositories for image->ascii conversion, it seems that the main approach is to simply color each character based on the average color and density of each pixel region the character is intended to represent.  These images are then largely illegible when font coloring is removed, so this approach seems like it could be improved.  

## Current Approach
1. Convert image to grayscale.  
2. Segment the screen into pixel regions (depending on how accurate we want this to be, tradeoff between fewer characters and greater accuracy)
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
Clone this project in XCode and pass the project the image in question.  

