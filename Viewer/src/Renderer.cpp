#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include "Menus.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width), viewport_height(viewport_height) {
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
	CreateRandomColorArray();
}

Renderer::~Renderer() {
	delete[] color_buffer;
	delete[] z_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color) {
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;
	
	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color) {
	// Implement bresenham algorithm

	// if it's the same point, you draw the point and that's it
	if (p1 == p2) {
		PutPixel(p1.x, p1.y, color);
		return;
	}
	
	// calculating x difference
	int deltaP = p2.x - p1.x;
	// if deltaP is 0, it's a straight line up or down 
	if (deltaP == 0) {
		drawX(p1, p2, color);
		return;
	}
	
	// calculating y difference
	int deltaQ = p2.y - p1.y;
	// if deltaQ is 0, it's a straight line left or right 
	if (deltaQ == 0) {
		drawY(p1, p2, color);
		return;
	}

	/* since deltaQ / deltaP are absolute values, slopeA can be
	* 0 < a < 1
	* a > 1 -> in that case, x and y switch roles */
	float slopeA = ((float)abs(deltaQ)) / ((float)abs(deltaP));	
	if (slopeA < 1) {
		slopeFloat(p1, p2, color, deltaQ, deltaP);
	}
	else {
		slopeInt(p1, p2, color, deltaQ, deltaP);
	}
}

void Renderer::drawX(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color) {
	int start, finish, xValue = p1.x;
	// choose starting point as the lowest of the two
	if (p1.y > p2.y) {
		start = p2.y;
		finish = p1.y;
	}
	else {
		start = p1.y;
		finish = p2.y;
	}
	while (start <= finish) {
		PutPixel(xValue, start, color);
		start++;
	}
}

void Renderer::drawY(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color) {
	int start, finish, yValue = p1.y;
	// choose starting point as the lowest of the two
	if (p1.x > p2.x) {
		start = p2.x;
		finish = p1.x;
	}
	else {
		start = p1.x;
		finish = p2.x;
	}
	while (start <= finish) {
		PutPixel(start, yValue, color);
		start++;
	}
}

void Renderer::slopeFloat(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color, int DQ, int DP) {
	int mX0, mX1, mY0, mY1, mDx, mDy;
	int index, D, start;

	if (p1.x > p2.x) {
		mX0 = p2.x;
		mX1 = p1.x;
		mY0 = p2.y;
		mY1 = p1.y;
		mDx = -DP;
		mDy = -DQ;
		start = p2.y;
	}
	else {
		mX0 = p1.x;
		mX1 = p2.x;
		mY0 = p1.y;
		mY1 = p2.y;
		mDx = DP;
		mDy = DQ;
		start = p1.y;
	}

	if (mDy < 0) {
		index = -1;
		mDy *= (-1);
	}
	else {
		index = 1;
	}
	D = (2 * mDy) - mDx;

	for (int i = mX0; i < mX1; i++) {
		PutPixel(i, start, color);
		if (D > 0) {
			start += index;
			D += (2 * mDy) - (2 * mDx);
		}
		else {
			D += (2 * mDy);
		}
	}
}

void Renderer::slopeInt(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color, int DQ, int DP) {
	int mX0, mX1, mY0, mY1, mDx, mDy;
	int index, D, start;

	if (p1.y > p2.y) {
		mX0 = p2.x;
		mX1 = p1.x;
		mY0 = p2.y;
		mY1 = p1.y;
		mDx = -DP;
		mDy = -DQ;
		start = p2.x;
	}
	else {
		mX0 = p1.x;
		mX1 = p2.x;
		mY0 = p1.y;
		mY1 = p2.y;
		mDx = DP;
		mDy = DQ;
		start = p1.x;
	}

	if (mDx < 0) {
		index = -1;
		mDx *= (-1);
	}
	else {
		index = 1;
	}
	D = (2 * mDx) - mDy;

	for (int i = mY0; i < mY1; i++) {
		PutPixel(start, i, color);
		if (D > 0) {
			start += index;
			D += (2 * mDx) - (2 * mDy);
		}
		else {
			D += (2 * mDx);
		}
	}
}

void Renderer::CreateBuffers(int w, int h) {
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	z_buffer = new float[3 * w * h];
	for (int i = 0; i < 3 * w * h; i++) {
		z_buffer[i] = -1000000.f;
	}
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering() {
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenglBuffer() {
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers() {
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color) {
	for (int i = 0; i < viewport_width; i++) {
		for (int j = 0; j < viewport_height; j++) {
			PutPixel(i, j, color);
		}
	}
	for (int i = 0; i < 3 * viewport_width * viewport_height; i++) {
		z_buffer[i] = -1000000.f;
	}
}

void Renderer::Render(const Scene& scene) {
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	if (scene.ModelVectorEmpty() == 1) {
		if (scene.fillMode == 0) {
			for (int i = 0; i < scene.GetModelCount(); i++) {
				glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
				glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();
				MeshModel current = scene.GetModel(i).GetNewModel(first, second);

				for (int i = 0; i < current.GetFacesCount(); i++) {
					// get face, than it's vertices, than draw line between the vertices
					Face currentF = current.GetFace(i);
					int one = currentF.GetVertexIndex(0) - 1, two = currentF.GetVertexIndex(1) - 1, three = currentF.GetVertexIndex(2) - 1;
					glm::vec3 v1 = current.GetVertex(one), v2 = current.GetVertex(two), v3 = current.GetVertex(three);
					// glm::ivec2 trueV1(v1.x + half_width, v1.y + half_height), trueV2(v2.x + half_width, v2.y + half_height), trueV3(v3.x + half_width, v3.y + half_height);
					glm::ivec2 trueV1(v1.x, v1.y), trueV2(v2.x, v2.y), trueV3(v3.x, v3.y);

					DrawLine(trueV1, trueV2, glm::vec3(1, 1, 1));
					DrawLine(trueV2, trueV3, glm::vec3(1, 1, 1));
					DrawLine(trueV3, trueV1, glm::vec3(1, 1, 1));
				}
			}
		}
		else if (scene.fillMode == 1) {
			DrawZBufferGrey(scene);
		}
		else if (scene.fillMode == 2 || scene.fillMode == 3) {
			DrawZBufferColor(scene);
		}		

		DrawAxes(scene);
		DrawBoundingBox(scene);
		DrawFaceNormals(scene);
		DrawVertexNormals(scene);

		DrawBoundingRectangle(scene);
	}
}

int Renderer::GetViewportWidth() const {
	return viewport_width;
}

int Renderer::GetViewportHeight() const {
	return viewport_height;
}

void Renderer::DrawAxes(const Scene& scene) {
	if (scene.showAxes) {
		glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();

		for (int i = 0; i < scene.GetModelCount(); i++) {
			MeshModel current = scene.GetModel(i);

			glm::vec3 beginX(current.minX, 0.0, 0.0);
			glm::vec3 endX(current.maxX, 0.0, 0.0);
			glm::vec3 beginY(0.0, current.minY, 0.0);
			glm::vec3 endY(0.0, current.maxY, 0.0);
			glm::vec3 beginZ(0.0, 0.0, current.minZ);
			glm::vec3 endZ(0.0, 0.0, current.maxZ);

			current.ChangeVectors(beginX, 1, first, second);
			current.ChangeVectors(endX, 1, first, second);
			current.ChangeVectors(beginY, 1, first, second);
			current.ChangeVectors(endY, 1, first, second);
			current.ChangeVectors(beginZ, 1, first, second);
			current.ChangeVectors(endZ, 1, first, second);

			// red
			DrawLine(beginX, endX, glm::vec3(1, 0, 0));
			// green
			DrawLine(beginY, endY, glm::vec3(0, 1, 0));
			// blue
			DrawLine(beginZ, endZ, glm::vec3(0, 0, 1));
		}

		MeshModel current = scene.GetActiveModel();
		glm::vec3 beginX(-1920.0, 0.0, 0.0);
		glm::vec3 endX(1920.0, 0.0, 0.0);
		glm::vec3 beginY(0.0, -1920.0, 0.0);
		glm::vec3 endY(0.0, 1920.0, 0.0);
		glm::vec3 beginZ(0.0, 0.0, -1920.0);
		glm::vec3 endZ(0.0, 0.0, 1920.0);

		current.ChangeVectors(beginX, 2, first, second);
		current.ChangeVectors(endX, 2, first, second);
		current.ChangeVectors(beginY, 2, first, second);
		current.ChangeVectors(endY, 2, first, second);
		current.ChangeVectors(beginZ, 2, first, second);
		current.ChangeVectors(endZ, 2, first, second);

		// purple
		DrawLine(beginX, endX, glm::vec3(1, 0, 1));
		// cyan
		DrawLine(beginY, endY, glm::vec3(0, 1, 1));
		// yellow
		DrawLine(beginZ, endZ, glm::vec3(1, 1, 0));
	}
}

void Renderer::DrawBoundingBox(const Scene& scene) {
	if (scene.showBoundingBox) {
		glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();

		for (int i = 0; i < scene.GetModelCount(); i++) {
			MeshModel current = scene.GetModel(i);

			glm::vec3 point1(current.minX, current.minY, current.minZ);
			glm::vec3 point2(current.minX, current.minY, current.maxZ);
			glm::vec3 point3(current.minX, current.maxY, current.minZ);
			glm::vec3 point4(current.minX, current.maxY, current.maxZ);
			glm::vec3 point5(current.maxX, current.minY, current.minZ);
			glm::vec3 point6(current.maxX, current.minY, current.maxZ);
			glm::vec3 point7(current.maxX, current.maxY, current.minZ);
			glm::vec3 point8(current.maxX, current.maxY, current.maxZ);

			current.ChangeVectors(point1, 1, first, second);
			current.ChangeVectors(point2, 1, first, second);
			current.ChangeVectors(point3, 1, first, second);
			current.ChangeVectors(point4, 1, first, second);
			current.ChangeVectors(point5, 1, first, second);
			current.ChangeVectors(point6, 1, first, second);
			current.ChangeVectors(point7, 1, first, second);
			current.ChangeVectors(point8, 1, first, second);

			DrawLine(point1, point2, glm::vec3(1, 0.647, 0));
			DrawLine(point1, point3, glm::vec3(1, 0.647, 0));
			DrawLine(point1, point5, glm::vec3(1, 0.647, 0));
			DrawLine(point2, point4, glm::vec3(1, 0.647, 0));
			DrawLine(point2, point6, glm::vec3(1, 0.647, 0));
			DrawLine(point3, point4, glm::vec3(1, 0.647, 0));
			DrawLine(point3, point7, glm::vec3(1, 0.647, 0));
			DrawLine(point5, point6, glm::vec3(1, 0.647, 0));
			DrawLine(point5, point7, glm::vec3(1, 0.647, 0));
			DrawLine(point6, point8, glm::vec3(1, 0.647, 0));
			DrawLine(point8, point4, glm::vec3(1, 0.647, 0));
			DrawLine(point8, point7, glm::vec3(1, 0.647, 0));
		}
	}
}

void Renderer::DrawFaceNormals(const Scene& scene) {
	if (scene.showFaceNormals) {
		for (int i = 0; i < scene.GetModelCount(); i++) {
			glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
			glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();
			MeshModel current = scene.GetModel(i);
			
			for (int i = 0; i < current.GetFacesCount(); i++) {
				Face currentF = current.GetFace(i);
				int one = currentF.GetVertexIndex(0) - 1, two = currentF.GetVertexIndex(1) - 1, three = currentF.GetVertexIndex(2) - 1;
				glm::vec3 v1 = current.GetVertex(one), v2 = current.GetVertex(two), v3 = current.GetVertex(three);
				
				glm::vec3 u = v2 - v1;
				glm::vec3 v = v3 - v1;

				glm::vec3 faceNormal = glm::vec3(0, 0, 0);
				faceNormal.x = (u.y * v.z) - (u.z * v.y);
				faceNormal.y = (u.z * v.x) - (u.x * v.z);
				faceNormal.z = (u.x * v.y) - (u.y * v.x);

				glm::vec3 faceCenter = glm::vec3(0, 0, 0);
				faceCenter.x = (v1.x + v2.x + v3.x) / 3;
				faceCenter.y = (v1.y + v2.y + v3.y) / 3;
				faceCenter.z = (v1.z + v2.z + v3.z) / 3;
				
				faceNormal.x += faceCenter.x;
				faceNormal.y += faceCenter.y;
				
				current.ChangeVectors(faceNormal, 1, first, second);
				current.ChangeVectors(faceCenter, 1, first, second);

				DrawLine(faceNormal, faceCenter, glm::vec3(0, 1, 0));
			}
		}
	}
}

void Renderer::DrawVertexNormals(const Scene& scene) {
	if (scene.showVertexNormals) {
		for (int i = 0; i < scene.GetModelCount(); i++) {
			glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
			glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();
			MeshModel current = scene.GetModel(i);

			for (int i = 0; i < current.GetFacesCount(); i++) {
				Face currentF = current.GetFace(i);
				int one = currentF.GetVertexIndex(0) - 1, two = currentF.GetVertexIndex(1) - 1, three = currentF.GetVertexIndex(2) - 1;
				glm::vec3 v1 = current.GetVertex(one), v2 = current.GetVertex(two), v3 = current.GetVertex(three);

				one = currentF.GetNormalIndex(0) - 1, two = currentF.GetNormalIndex(1) - 1, three = currentF.GetNormalIndex(2) - 1;
				glm::vec3 n1 = current.GetNormal(one), n2 = current.GetNormal(two), n3 = current.GetNormal(three);

				n1.x += v1.x;
				n1.y += v1.y;
				n2.x += v2.x;
				n2.y += v2.y;
				n3.x += v3.x;
				n3.y += v3.y;

				current.ChangeVectors(n1, 1, first, second);
				current.ChangeVectors(n2, 1, first, second);
				current.ChangeVectors(n3, 1, first, second);
				current.ChangeVectors(v1, 1, first, second);
				current.ChangeVectors(v2, 1, first, second);
				current.ChangeVectors(v3, 1, first, second);

				DrawLine(n1, v1, glm::vec3(1, 0, 0));
				DrawLine(n2, v2, glm::vec3(1, 0, 0));
				DrawLine(n3, v3, glm::vec3(1, 0, 0));
			}
		}
	}
}

void Renderer::DrawBoundingRectangle(const Scene& scene) {
	if (scene.showBoundingRectangle) {
		float tMinX, tMaxX, tMinY, tMaxY, tMinZ, tMaxZ;

		for (int i = 0; i < scene.GetModelCount(); i++) {
			glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
			glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();
			MeshModel current = scene.GetModel(i).GetNewModel(first, second);

			for (int i = 0, j = 0; i < current.GetFacesCount(); i++, j++) {
				Face currentF = current.GetFace(i);
				int one = currentF.GetVertexIndex(0) - 1, two = currentF.GetVertexIndex(1) - 1, three = currentF.GetVertexIndex(2) - 1;
				glm::vec3 v1 = current.GetVertex(one), v2 = current.GetVertex(two), v3 = current.GetVertex(three);
				
				tMinX = fmin(v1.x, v2.x);
				tMinX = fmin(tMinX, v3.x);
				tMinY = fmin(v1.y, v2.y);
				tMinY = fmin(tMinY, v3.y);
				tMinZ = fmin(v1.z, v2.z);
				tMinZ = fmin(tMinZ, v3.z);

				tMaxX = fmax(v1.x, v2.x);
				tMaxX = fmax(tMaxX, v3.x);
				tMaxY = fmax(v1.y, v2.y);
				tMaxY = fmax(tMaxY, v3.y);
				tMaxZ = fmax(v1.z, v2.z);
				tMaxZ = fmax(tMaxY, v3.z);

				glm::vec3 c0(tMinX, tMinY, tMinZ);
				glm::vec3 c1(tMinX, tMinY, tMaxZ);
				glm::vec3 c2(tMinX, tMaxY, tMinZ);
				glm::vec3 c3(tMinX, tMaxY, tMaxZ);
				glm::vec3 c4(tMaxX, tMinY, tMinZ);
				glm::vec3 c5(tMaxX, tMinY, tMaxZ);
				glm::vec3 c6(tMaxX, tMaxY, tMinZ);
				glm::vec3 c7(tMaxX, tMaxY, tMaxZ);

				if (j >= 256) {
					j = 0;
				}

				glm::vec3 color = faceColors[j];

				DrawLine(c0, c1, color);
				DrawLine(c0, c2, color);
				DrawLine(c0, c4, color);
				DrawLine(c1, c3, color);
				DrawLine(c1, c5, color);
				DrawLine(c2, c3, color);
				DrawLine(c2, c6, color);
				DrawLine(c4, c5, color);
				DrawLine(c4, c6, color);
				DrawLine(c5, c7, color);
				DrawLine(c7, c3, color);
				DrawLine(c7, c6, color);
			}
		}
	}
}

void Renderer::CreateRandomColorArray() {
	float r, g, b;
	srand(time(NULL));

	for (int i = 0; i < 256; i++) {
		r = (rand() % 256) / 256.0;
		g = (rand() % 256) / 256.0;
		b = (rand() % 256) / 256.0;
		faceColors.push_back(glm::vec3(r, g, b));
	}
	faceColors.shrink_to_fit();
}

void Renderer::DrawZBufferGrey(const Scene& scene) {
	for (int i = 0; i < scene.GetModelCount(); i++) {
		glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();
		MeshModel current = scene.GetModel(i).GetNewModel(first, second);

		for (int i = 0, j = 0; i < current.GetFacesCount(); i++, j++) {
			Face currentF = current.GetFace(i);
			int one = currentF.GetVertexIndex(0) - 1, two = currentF.GetVertexIndex(1) - 1, three = currentF.GetVertexIndex(2) - 1;
			glm::vec3 v1 = current.GetVertex(one), v2 = current.GetVertex(two), v3 = current.GetVertex(three);

			int minXColor = min(min(v1.x, v2.x), v3.x);
			int minYColor = min(min(v1.y, v2.y), v3.y);
			int maxXColor = max(max(v1.x, v2.x), v3.x);
			int maxYColor = max(max(v1.y, v2.y), v3.y);

			for (int c1 = minXColor; c1 <= maxXColor; c1++) {
				for (int c2 = minYColor; c2 <= maxYColor; c2++) {
					if (((c1 - v1.x) * (v2.y - v1.y) - (c2 - v1.y) * (v2.x - v1.x) >= 0) &&
						(c1 - v2.x) * (v3.y - v2.y) - (c2 - v2.y) * (v3.x - v2.x) >= 0 &&
						(c1 - v3.x) * (v1.y - v3.y) - (c2 - v3.y) * (v1.x - v3.x) >= 0) {
						float currentZ1 = v1.z, currentZ2 = v2.z, currentZ3 = v3.z;
						float faceArea, area12, area23, area31, depth;

						area12 = AreaOfTriangle(v2.x, v2.y, v1.x, v1.y, c1, c2);
						area23 = AreaOfTriangle(v2.x, v2.y, v3.x, v3.y, c1, c2);
						area31 = AreaOfTriangle(v1.x, v1.y, v3.x, v3.y, c1, c2);
						faceArea = AreaOfTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);

						depth = (area12 / faceArea) * currentZ3 + (area23 / faceArea) * currentZ1 + (area31 / faceArea) * currentZ2;
						PutPixelpolygon(c1, c2, glm::vec3(0.5, 0.5, 0.5), depth, 1, scene.greyScaleLevel);
					}
				}
			}
		}
	}
}

void Renderer::DrawZBufferColor(const Scene& scene) {
	for (int i = 0; i < scene.GetModelCount(); i++) {
		glm::mat4x4 first = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 second = scene.GetActiveCamera().GetProjectionTransformation();
		MeshModel current = scene.GetModel(i).GetNewModel(first, second);

		for (int i = 0, j = 0; i < current.GetFacesCount(); i++, j++) {
			Face currentF = current.GetFace(i);
			int one = currentF.GetVertexIndex(0) - 1, two = currentF.GetVertexIndex(1) - 1, three = currentF.GetVertexIndex(2) - 1;
			glm::vec3 v1 = current.GetVertex(one), v2 = current.GetVertex(two), v3 = current.GetVertex(three);
			
			int minXColor = min(min(v1.x, v2.x), v3.x);
			int minYColor = min(min(v1.y, v2.y), v3.y);
			int maxXColor = max(max(v1.x, v2.x), v3.x);
			int maxYColor = max(max(v1.y, v2.y), v3.y);

			if (j >= 256) {
				j = 0;
			}

			glm::vec3 color = faceColors[j];

			for (int c1 = minXColor; c1 <= maxXColor; c1++) {
				for (int c2 = minYColor; c2 <= maxYColor; c2++) {
					if (((c1 - v1.x) * (v2.y - v1.y) - (c2 - v1.y) * (v2.x - v1.x) >= 0) &&
						(c1 - v2.x) * (v3.y - v2.y) - (c2 - v2.y) * (v3.x - v2.x) >= 0 &&
						(c1 - v3.x) * (v1.y - v3.y) - (c2 - v3.y) * (v1.x - v3.x) >= 0) {
						float currentZ1 = v1.z, currentZ2 = v2.z, currentZ3 = v3.z;
						float faceArea, area12, area23, area31, depth;

						area12 = AreaOfTriangle(v2.x, v2.y, v1.x, v1.y, c1, c2);
						area23 = AreaOfTriangle(v2.x, v2.y, v3.x, v3.y, c1, c2);
						area31 = AreaOfTriangle(v1.x, v1.y, v3.x, v3.y, c1, c2);
						faceArea = AreaOfTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);

						depth = (area12 / faceArea) * currentZ3 + (area23 / faceArea) * currentZ1 + (area31 / faceArea) * currentZ2;
						if (scene.fillMode == 2) {
							PutPixelpolygon(c1, c2, color, depth, 2, 0);
						}
						else if (scene.fillMode == 3) {
							DrawLine(glm::ivec2(c1, c2), glm::ivec2(c1, c2), scene.clear_color);
						}						
					}
				}
			}
		}
	}
}

float Renderer::AreaOfTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
	float dArea = (x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)) / 2.0;

	if (dArea > 0.0) {
		return dArea;
	}
	else {
		return -dArea;
	}
}

void Renderer::PutPixelpolygon(const int i, const int j, const glm::vec3& color, float z, int mode, float gs) {
	if (i < 0) return;
	if (i >= viewport_width) return;
	if (j < 0) return;
	if (j >= viewport_height) return;

	if (z >= z_buffer[INDEX(viewport_width, i, j, 0)] && mode == 2) {
		color_buffer[INDEX(viewport_width, i, j, 0)] = color.r;
		color_buffer[INDEX(viewport_width, i, j, 1)] = color.g;
		color_buffer[INDEX(viewport_width, i, j, 2)] = color.b;
		
		z_buffer[INDEX(viewport_width, i, j, 0)] = z;
		z_buffer[INDEX(viewport_width, i, j, 1)] = z;
		z_buffer[INDEX(viewport_width, i, j, 2)] = z;
	}
	if (z >= z_buffer[INDEX(viewport_width, i, j, 0)] && mode == 1) {
		color_buffer[INDEX(viewport_width, i, j, 0)] = color.r + z / gs;
		color_buffer[INDEX(viewport_width, i, j, 1)] = color.g + z / gs;
		color_buffer[INDEX(viewport_width, i, j, 2)] = color.b + z / gs;
		
		z_buffer[INDEX(viewport_width, i, j, 0)] = z;
		z_buffer[INDEX(viewport_width, i, j, 1)] = z;
		z_buffer[INDEX(viewport_width, i, j, 2)] = z;
	}	
}