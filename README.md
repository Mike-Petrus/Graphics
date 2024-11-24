# Graphics

Assignment overview
For implementation details see associated PDFS 

# Assignment 1

Basic linear algebra and transformations

# Assignment 2

1. Construct the view transformation matrix and apply it to the vertices
2. Construct the orthographic projection matrix and apply it to the vertices in camera space
3. Construct the perspective projection matrix and apply it to the to the vertices in camera space to get them in the canonical cube

# Assignment 3

1. Transform the triangles to NDC space and then screen space

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-1-triangle-1.png?raw=true" width="200"/> <img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-1-triangle-2.png?raw=true" width="200"/> <img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-1-triangle-3.png?raw=true" width="200"/>

2. Implement inside_triangle() and draw pixels

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-2-triangle-1.png?raw=true" width="200"/> <img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-2-triangle-2.png?raw=true" width="200"/> <img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-2-triangle-3.png?raw=true" width="200"/>

3. Implement Z-Buffer and draw all triangles with their respective colors

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%203/build/linux/x86_64/release/part-3.png?raw=true" width="200"/>

# Assignment 4

1. Calculate the Blinn-Phong reflection (ambient, diffuse, specular)

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%204/part-1.png?raw=true" width="200"/>

2. Map the texture to the image

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%204/part-2.png?raw=true" width="200"/>

# Assignment 5

1. Implement the recursive deCasteljau algorithm

2. Draw the Bezier Curve in red using the control points

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%205/build/linux/x86_64/release/bezier_curve_0.png?raw=true" width="200"/> <img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%205/build/linux/x86_64/release/bezier_curve_1.png?raw=true" width="200"/>

# Assignment 6

1. Implement ray_sphere_intersect() to return the closest hit of a sphere (if it exists) and render the pixel color of the sphere at that point

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%206/build/linux/x86_64/release/part-1.png?raw=true" width="200"/>

2. Implement ray_triangle_intersect() to return the closest hit of a triangle mesh (if it exists) and render the pixel color of the triangle at that point

<img src="https://github.com/Mike-Petrus/Graphics/blob/main/Assignment%206/build/linux/x86_64/release/part-2.png?raw=true" width="200"/>
