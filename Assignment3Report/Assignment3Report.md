# Assignment 3:
## 1:
It's a 3 functions process that draws our screen (assembled from two triangles) 
### Init OpenGL Rendering
- We define unique identifiers for vertex array, texture and buffer
- We than generate empty buffer objects that would be associated with those unique identifiers
- Now we create two triangles that would cover the entire screen, as does the corresponding UV texture coordinates for the triangles (within the limits of 0 to 1)
- We bind the buffer! From that point on any buffer calls we make will be directed to the currently bound buffer
  - GlBufferData – we allocate space for the buffer in the size of (triangles vertices + UV texture coordinates)
  - GlBufferSubData – copy coordinates of triangles to the buffer. Those coordinates will be stored from index 0 to size() – 1
  - GlBufferSubData – copy UV texture coordinates to the buffer. Those coordinates will be stored directly after the triangle's vertices – from the end index of triangles to size() – 1 of texture
- The last thing we do in this function relates to the shader. We initialize both the vertex and fragment shaders, and they receive the coordinates of the triangles plus the UV texture coordinates from the appropriate index in the buffer. 
- We than pass the texture to the shader
### Create OpenGL Buffer & Swap Buffer
- Now we truly put the focus on the texture
- Activate texture unit TEXTURE0
- We bind the texture! From that point on any changes we make will apply to the currently bound texture
  - The texture was bound in the form of GL_TEXTURE_2D. We now allocate that texture to the GPU with no data – NULL data
  - And now the similarities between the two functions ends. While **Create OpenGL Buffer** allocated the texture to the GPU with NULL data, the **Swap Buffer** loads the color buffer into the texture on the GPU
  - Lastly, we generate texture mapping and render the triangles
### To Conclude 
- If we combine all of the above, the texture will be displayed onto the screen.
- We can now "delete" all this irrelevant code by simply not calling those functions from the constructor
## 2:
We updated the MeshModel class to load the mesh on the GPU
- We created a struct called **Vertex** and we now have a vector of those structs that will be later sent to the GPU. Each of those Vertex holds a 3D vector of position(x, y, z coordinates), a 3D vector of normal and a 2D vector of texture (UV coordinates)
- The new constructor creates for every face 3 Vertex structs for the 3 vertices of the face – we add them to the Vertex vector
- We also created 2 unique identifiers: VAO (vertex array object), VBO (vertex buffer object). We generated empty buffer objects that would later be associated with those unique identifiers, and we bind them
- The last thing we do is define an array of generic vertex attribute data for the position, normal and texture coordinates of each vertex
## 3:
- As discussed before, the MeshModel constructor defines the "location" for position, normal and texture
- The transformation matrices are being passed down to the shader from the Renderer class (example will be shown later), and we pass the new vertex position to the fragment shader through gl_Position
```glsl
# version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 model;
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * world * model * vec4(Position, 1.0);
} 
```
 ## 4:
The fragment shader only outputs a constant color – red 
```glsl
# version 330

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
} 
```
 ## 5:
- We no longer call those functions in the Renderer constructor
```c++
Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width), viewport_height(viewport_height) {
	/*InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);*/
	CreateRandomColorArray();
}
```
- And so we must make adjustments in main
```c++
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io) {
	…
	//renderer.ClearColorBuffer(clear_color);
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);
	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.Render(scene);
	// renderer.SwapBuffers();
	…
}
```
- The Renderer::Render now looks like this:
```c++
void Renderer::Render(const Scene& scene) {
	if (scene.ModelVectorEmpty() == 1) {
		glm::mat4x4 third = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 fourth = scene.GetActiveCamera().GetProjectionTransformation();

		for (int i = 0; i < scene.GetModelCount(); i++) {
			MeshModel current = scene.GetModel(i);
			glm::mat4 first = scene.GetModel(i).GetModelTransformation();
			glm::mat4 second = scene.GetModel(i).GetWorldTransformation();
			
			colorShader.loadShaders("vshader.glsl", "fshader.glsl");
			colorShader.use();

			colorShader.setUniform("model", first);
			colorShader.setUniform("world", second);
			colorShader.setUniform("view", third);
			colorShader.setUniform("projection", fourth);

			if (scene.fillMode == 0) {
				// Drag our model's faces (triangles) in line mode (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(scene.GetModel(i).vao);
				glDrawArrays(GL_TRIANGLES, 0, scene.GetModel(i).GetModelVertices().size());
				glBindVertexArray(0);
			}
			else if (scene.fillMode == 2) {
				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(scene.GetModel(i).vao);
				glDrawArrays(GL_TRIANGLES, 0, scene.GetModel(i).GetModelVertices().size());
				glBindVertexArray(0);
			}
		}
	}
}
```
- We are now able to clearly see the mesh – lines
![meshLines](meshLines.png)
- We are now able to clearly see the mesh – filled faces
![meshFill](meshFill.png)
## 6:
- Now that we have everything all set, it was so much easier for us to implement Phong shading. We created a new vertex and fragment shaders specifically for this, and we pass all relevant parameters to them.
- Since now all calculations regarding ambient, diffuse and specular colors happen in the fragment shader – everything works so much faster. The GPU is very sensitive to incremental changes in parameters. 
- In order to demonstrate those improved performances, let's start with the regular sanity check – the ball
- First we change the position of the light – the changes are instantaneous  
![ball1](ball1.png)
![ball2](ball2.png)
![ball3](ball3.png)
- Now we change the position of the camera – up until now we stood at (0.0, 0.0, 1.0) looking to (0.0, 0.0, 0.0) (negative z axis). Now we stand at (0.0, 0.0, -10.0) looking to (0.0, 0.0, 0.0) (positive z axis)
![ball4](ball4.png)
- Now we've loaded models from the github repository given to us in the assignment - let's change a couple of things:
- Different shine levels: the first looks more natural, the second one looks more metallic 
![shine1](shine1.png)
![shine2](shine2.png)
- Different camera position: because the light position is origin (0.0, 0.0, 0.0) – now we look towards the positive z axis and see the entire scene "from behind" – the car receives more light now than the Cheburashka
![camera1](camera1.png)
 - Different light position + ambient light color: the difference is immediate and noticeable
![lightP1](lightP1.png)
## 7:
- Now we begin dealing with texture mapping. 
- The true magic of texture is that it allow us to add various details to the model without the need of added vertices and faces. That allows us to keep the .obj file relatively small and still have a detailed image
- Some examples to that fact – before and after pictures of objects
- **Skull**
![skull1](skull1.png)
![skull2](skull2.png)
- **Duck**
![duck1](duck1.png)
![duck2](duck2.png)
- We can also see how the light affects the texture with the **Fish**: the first image is the original texture. The second one contains both a change in light position and a small change of ambient light color. You can now see that the fish has become much darker than before  
![fish1](fish1.png)
![fish2](fish2.png)
- During our implementation of texture mapping, we allow 3 types of canonical projections:
- Planar Projection: we "held" an image of a chess board above the cow's head and projected the image onto the object. Practically speaking, we removed the z coordinate and so (U = x, V = y). as you can see in the video, the result is… wrong. There are big uneven patches of colors spread across the cow's body, and that makes it very hard to see the small details of the cow such as ears and face features. Clearly this projection is better suited for flat surfaces rather than round ones. 
![planar](planar.gif)
 - Cylindrical Projection: as you can see in the video, we warped the image into a cylindrical shape and basically wrapped the object with it. Practically speaking, (U = z, V = angle between remaining axes = arctan(x, y)). When we turn the cow, you can clearly see the cylindrical shape of the texture, and how good it sits on the cow (unlike planar)
![cylinder](cylinder.gif)
- Spherical Projection: in this projection, the texture is assembled from the polar coordinates of the x and y position of every vertex. The image is warped into a spherical shape by the formula of (U = distance r = sqrt(pow(x, 2) + pow(y, 2)), V = angle between x and y axes = arctan(x, y)). As you can see in the video, that creates long lines (stripes) that are drawn across the cow. When we turn the cow around, you can clearly see the "contours" of the cow. For example, we can differentiate the back and shoulders of the cow because each has a different set of lines. 
![sphere](sphere.gif)
- Another great example to accentuate the difference between the projections: comparing flat surfaces to cylindrical ones
- Planar:
![plane1](plane1.png)
- Cylindrical:
![cylinder1](cylinder1.png)
- Spherical:
![sphere1](sphere1.png)
## 8:
- **Normal Mapping**
- In comparison to regular texture, normal mapping gives the illusion that the image itself is a 3D model rather than a 2D image
- We do that by using the texture to calculate another image – a normal map. Than we combine the two images together and basically accentuate the bumps and dents of the model – which in itself give it a more 3D feeling rather than a flat image.
- The first image of diamonds seem relatively dark and muted. It's very hard to notice the certain shape of each diamond
![normal1](normal1.png)
- The second image, however, looks more real. We can notice more shape and contours of each diamond, and although it seems as if there is more light in the picture than the original one – the lighting is the same.
![normal2](normal2.png)
- **Environment Mapping**
- In comparison to regular texture, environment mapping gives the illusion of reflection – the object reflects the texture of it's surrounding.
- We do that by splitting our calculations: we have the real position of the object (determined by the camera's view and projection matrices, model transformation and world transformation), and than we have the model's **artificial** position and normal. Those parameters would only be affected by model and world transformations – so that they can reflect the background determined by the movement of the camera.
- Here we have a video where the rectangle changes according to the position of the camera (changing our environment) – and the teapot reflects that change.  
![environment](environment.gif)
- **Toon Shading**
- Let's first compare between Phong and Toon shading
- Phong shading
![toon1](toon1.png)
- Toon shading with the same light and model colors – but we only consider 2 colors in our calculations
![toon2](toon2.png)
- Toon shading with 10 colors to consider instead of only 2. It looks more shiny in comparison to 2 colors – that looks much darker and dimmed 
![toon3](toon3.png)
- In addition, in comparison to regular texture, toon shading gives the textured object the illusion of being hand drawn – rather than scanned as a 3D objects
- Toon shading with 12 colors to consider
![toon4](toon4.png)
- Toon shading with texture on the same 12 colors
![toon5](toon5.png)
- Toon shading with texture only now we have 20 colors rather than 12. The difference is most noticeable in the face – the texture is much better when you have more colors
![toon6](toon6.png)


