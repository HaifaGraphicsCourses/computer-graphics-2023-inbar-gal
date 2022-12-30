#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer {
	public:
		Renderer(int viewportWidth, int viewportHeight);
		virtual ~Renderer();
		void Render(const Scene& scene);
		void SwapBuffers();
		void ClearColorBuffer(const glm::vec3& color);
		int GetViewportWidth() const;
		int GetViewportHeight() const;
	private:
		void PutPixel(const int i, const int j, const glm::vec3& color);
		void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
		void drawX(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
		void drawY(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
		void slopeFloat(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color, int DQ, int DP);
		void slopeInt(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color, int DQ, int DP);
		
		void DrawAxes(const Scene& scene);
		void DrawBoundingBox(const Scene& scene);
		void DrawFaceNormals(const Scene& scene);
		void DrawVertexNormals(const Scene& scene);

		void DrawBoundingRectangle(const Scene& scene);
		void CreateRandomColorArray();
		std::vector<glm::vec3> faceColors;

		void DrawZBufferGrey(const Scene& scene);
		void DrawZBufferColor(const Scene& scene);
		float AreaOfTriangle(float x0, float y0, float x1, float y1, float x2, float y2);
		void PutPixelpolygon(const int i, const int j, const glm::vec3& color, float z, int mode, int gs);

		void CreateBuffers(int w, int h);
		void CreateOpenglBuffer();
		void InitOpenglRendering();

		float* color_buffer;
		int viewport_width;
		int viewport_height;
		GLuint gl_screen_tex;
		GLuint gl_screen_vtc;
		float* z_buffer;
};
