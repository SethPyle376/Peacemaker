#pragma once

#include <iostream>
#include <math.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include "GL\glew.h"

#include "FastFourierTransform.h"
#include "Complex.h"

struct oceanVertex
{
	GLfloat x, y, z;
	GLfloat nx, ny, nz;
	GLfloat	a, b, c;
	GLfloat	_a, _b, _c;
	GLfloat	ox, oy, oz;
};

struct complex_vector_normal
{
	complex h;
	glm::vec2 D;
	glm::vec3 n;
};

class Ocean
{
private:
	bool geometry;

	float g;

	int N, Nplus1;
	float A;

	glm::vec2 w;
	float length;

	complex *h_tilde, *h_tilde_slopex, *h_tilde_slopez,
		*h_tilde_dx, *h_tilde_dz;

	FastFourierTransform *fft;

	oceanVertex *vertices;
	unsigned int *indices;
	unsigned int indices_count;
	
	GLuint vboVertices;
	GLuint vboIndices;

	GLuint glProgram;
	GLuint glShaderV;
	GLuint glShaderF;

	GLuint vertex;
	GLuint normal;
	GLuint texture;
	GLuint light_position;
	GLuint projection;
	GLuint view;
	GLuint model;

public:
	Ocean(const int N, const float A, const glm::vec2 w, const float length, const bool geometry, GLuint shaderID) :
		g(9.81f), geometry(geometry), N(N), Nplus1(N + 1), A(A), w(w), length(length),
		vertices(0), indices(0), h_tilde(0), h_tilde_slopex(0), h_tilde_slopez(0), h_tilde_dx(0), h_tilde_dz(0), fft(0)
	{
		h_tilde = new complex[N*N];
		h_tilde_slopex = new complex[N*N];
		h_tilde_slopez = new complex[N*N];
		h_tilde_dx = new complex[N*N];
		h_tilde_dz = new complex[N*N];
		fft = new FastFourierTransform(N);
		vertices = new oceanVertex[Nplus1*Nplus1];
		indices = new unsigned int[Nplus1*Nplus1 * 10];

		glProgram = shaderID;

		int index;

		complex htilde0, htilde0mk_conj;
		for (int m_prime = 0; m_prime < Nplus1; m_prime++) {
			for (int n_prime = 0; n_prime < Nplus1; n_prime++) {
				index = m_prime * Nplus1 + n_prime;

				htilde0 = hTilde_0(n_prime, m_prime);
				htilde0mk_conj = hTilde_0(-n_prime, -m_prime).conj();

				vertices[index].a = htilde0.a;
				vertices[index].b = htilde0.b;
				vertices[index]._a = htilde0mk_conj.a;
				vertices[index]._b = htilde0mk_conj.b;

				vertices[index].ox = vertices[index].x = (n_prime - N / 2.0f) * length / N;
				vertices[index].oy = vertices[index].y = 0.0f;
				vertices[index].oz = vertices[index].z = (m_prime - N / 2.0f) * length / N;

				vertices[index].nx = 0.0f;
				vertices[index].ny = 1.0f;
				vertices[index].nz = 0.0f;
			}
		}

		indices_count = 0;
		for (int m_prime = 0; m_prime < N; m_prime++) {
			for (int n_prime = 0; n_prime < N; n_prime++) {
				index = m_prime * Nplus1 + n_prime;

				if (geometry) {
					indices[indices_count++] = index;				// lines
					indices[indices_count++] = index + 1;
					indices[indices_count++] = index;
					indices[indices_count++] = index + Nplus1;
					indices[indices_count++] = index;
					indices[indices_count++] = index + Nplus1 + 1;
					if (n_prime == N - 1) {
						indices[indices_count++] = index + 1;
						indices[indices_count++] = index + Nplus1 + 1;
					}
					if (m_prime == N - 1) {
						indices[indices_count++] = index + Nplus1;
						indices[indices_count++] = index + Nplus1 + 1;
					}
				}
				else {
					indices[indices_count++] = index;				// two triangles
					indices[indices_count++] = index + Nplus1;
					indices[indices_count++] = index + Nplus1 + 1;
					indices[indices_count++] = index;
					indices[indices_count++] = index + Nplus1 + 1;
					indices[indices_count++] = index + 1;
				}
			}
		}

		
		vertex = glGetAttribLocation(glProgram, "vertex");
		normal = glGetAttribLocation(glProgram, "normal");
		texture = glGetAttribLocation(glProgram, "texture");
		light_position = glGetUniformLocation(glProgram, "light_position");
		projection = glGetUniformLocation(glProgram, "Projection");
		view = glGetUniformLocation(glProgram, "View");
		model = glGetUniformLocation(glProgram, "Model");

		glGenBuffers(1, &vboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(oceanVertex)*(Nplus1)*(Nplus1), vertices, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &vboIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}
	~Ocean();
	float dispersion(int n_prime, int m_prime);		// deep water
	float phillips(int n_prime, int m_prime);		// phillips spectrum
	complex hTilde_0(int n_prime, int m_prime);
	complex hTilde(float t, int n_prime, int m_prime);
	complex_vector_normal h_D_and_n(glm::vec2 x, float t);
	void evaluateWaves(float t);
	void evaluateWavesFFT(float t);
	void render(float t, glm::vec3 light_pos, glm::mat4 Projection, glm::mat4 View, glm::mat4 Model, bool use_fft);
};