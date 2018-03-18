#include "Ocean.h"

float uniformRandomVariable() {
	return (float)rand() / RAND_MAX;
}

complex gaussianRandomVariable() {
	float x1, x2, w;
	do {
		x1 = 2.f * uniformRandomVariable() - 1.f;
		x2 = 2.f * uniformRandomVariable() - 1.f;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.f);
	w = sqrt((-2.f * log(w)) / w);
	return complex(x1 * w, x2 * w);
}

Ocean::~Ocean() {
	if (h_tilde)		delete[] h_tilde;
	if (h_tilde_slopex)	delete[] h_tilde_slopex;
	if (h_tilde_slopez)	delete[] h_tilde_slopez;
	if (h_tilde_dx)		delete[] h_tilde_dx;
	if (h_tilde_dz)		delete[] h_tilde_dz;
	if (fft)		delete fft;
	if (vertices)		delete[] vertices;
	if (indices)		delete[] indices;
}

float Ocean::dispersion(int n_prime, int m_prime) {
	float w_0 = 2.0f * M_PI / 200.0f;
	float kx = M_PI * (2 * n_prime - N) / length;
	float kz = M_PI * (2 * m_prime - N) / length;
	return floor(sqrt(g * sqrt(kx * kx + kz * kz)) / w_0) * w_0;
}

float Ocean::phillips(int n_prime, int m_prime) {
	glm::vec2 k(M_PI * (2 * n_prime - N) / length,
		M_PI * (2 * m_prime - N) / length);

	float k_length = glm::length(k);


	if (k_length < 0.000001) return 0.0;

	float k_length2 = k_length * k_length;
	float k_length4 = k_length2 * k_length2;



	glm::vec2 kUnit = glm::normalize(k);
	glm::vec2 wUnit = glm::normalize(w);


	float k_dot_w = glm::dot(kUnit, wUnit);



	float k_dot_w2 = k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w;

	float w_length = glm::length(w);
	float L = w_length * w_length / g;
	float L2 = L * L;

	float damping = 0.001;
	float l2 = L2 * damping * damping;

	return A * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 * exp(-k_length2 * l2);
}

complex Ocean::hTilde_0(int n_prime, int m_prime) {
	complex r = gaussianRandomVariable();
	return r * sqrt(phillips(n_prime, m_prime) / 2.0f);
}

complex Ocean::hTilde(float t, int n_prime, int m_prime) {
	int index = m_prime * Nplus1 + n_prime;

	complex htilde0(vertices[index].a, vertices[index].b);
	complex htilde0mkconj(vertices[index]._a, vertices[index]._b);

	float omegat = dispersion(n_prime, m_prime) * t;

	float cos_ = cos(omegat);
	float sin_ = sin(omegat);

	complex c0(cos_, sin_);
	complex c1(cos_, -sin_);

	complex res = htilde0 * c0 + htilde0mkconj * c1;

	return htilde0 * c0 + htilde0mkconj * c1;
}

complex_vector_normal Ocean::h_D_and_n(glm::vec2 x, float t) {
	complex h(0.0f, 0.0f);
	glm::vec2 D(0.0f, 0.0f);
	glm::vec3 n(0.0f, 0.0f, 0.0f);

	complex c, res, htilde_c;
	glm::vec2 k;
	float kx, kz, k_length, k_dot_x;

	for (int m_prime = 0; m_prime < N; m_prime++) {
		kz = 2.0f * M_PI * (m_prime - N / 2.0f) / length;
		for (int n_prime = 0; n_prime < N; n_prime++) {
			kx = 2.0f * M_PI * (n_prime - N / 2.0f) / length;
			k = glm::vec2(kx, kz);

			k_length = sqrt(k.x * k.x + k.y * k.y);
			k_dot_x = glm::dot(k, x);

			c = complex(cos(k_dot_x), sin(k_dot_x));
			htilde_c = hTilde(t, n_prime, m_prime) * c;

			h = h + htilde_c;

			n = n + glm::vec3(-kx * htilde_c.b, 0.0f, -kz * htilde_c.b);

			if (k_length < 0.000001) continue;
			D = D + glm::vec2(kx / k_length * htilde_c.b, kz / k_length * htilde_c.b);
		}
	}

	//glm::vec3 inverse = glm::vec3(0.0f, 1.0f, 0.0f);

	//inverse = inverse - n;

	//float nLength = inverse.length();

	//glm::vec3 nUnit = glm::vec3(inverse.x / nLength, inverse.y / nLength, inverse.z / nLength);

	//n = (glm::vec3(0.0f, 1.0f, 0.0f) - n).unit();


	//n = nUnit;

	n = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) - n);

	complex_vector_normal cvn;
	cvn.h = h;
	cvn.D = D;
	cvn.n = n;
	return cvn;
}

void Ocean::evaluateWaves(float t)
{
	float lambda = -1.0;
	int index;
	glm::vec2 x;
	glm::vec2 d;
	complex_vector_normal h_d_and_n;
	for (int m_prime = 0; m_prime < N; m_prime++) {
		for (int n_prime = 0; n_prime < N; n_prime++) {
			index = m_prime * Nplus1 + n_prime;

			x = glm::vec2(vertices[index].x, vertices[index].z);

			h_d_and_n = h_D_and_n(x, t);

			vertices[index].y = h_d_and_n.h.a;

			vertices[index].x = vertices[index].ox + lambda * h_d_and_n.D.x;
			vertices[index].z = vertices[index].oz + lambda * h_d_and_n.D.y;

			vertices[index].nx = h_d_and_n.n.x;
			vertices[index].ny = h_d_and_n.n.y;
			vertices[index].nz = h_d_and_n.n.z;

			if (n_prime == 0 && m_prime == 0) {
				vertices[index + N + Nplus1 * N].y = h_d_and_n.h.a;

				vertices[index + N + Nplus1 * N].x = vertices[index + N + Nplus1 * N].ox + lambda * h_d_and_n.D.x;
				vertices[index + N + Nplus1 * N].z = vertices[index + N + Nplus1 * N].oz + lambda * h_d_and_n.D.y;

				vertices[index + N + Nplus1 * N].nx = h_d_and_n.n.x;
				vertices[index + N + Nplus1 * N].ny = h_d_and_n.n.y;
				vertices[index + N + Nplus1 * N].nz = h_d_and_n.n.z;
			}
			if (n_prime == 0) {
				vertices[index + N].y = h_d_and_n.h.a;

				vertices[index + N].x = vertices[index + N].ox + lambda * h_d_and_n.D.x;
				vertices[index + N].z = vertices[index + N].oz + lambda * h_d_and_n.D.y;

				vertices[index + N].nx = h_d_and_n.n.x;
				vertices[index + N].ny = h_d_and_n.n.y;
				vertices[index + N].nz = h_d_and_n.n.z;
			}
			if (m_prime == 0) {
				vertices[index + Nplus1 * N].y = h_d_and_n.h.a;

				vertices[index + Nplus1 * N].x = vertices[index + Nplus1 * N].ox + lambda * h_d_and_n.D.x;
				vertices[index + Nplus1 * N].z = vertices[index + Nplus1 * N].oz + lambda * h_d_and_n.D.y;

				vertices[index + Nplus1 * N].nx = h_d_and_n.n.x;
				vertices[index + Nplus1 * N].ny = h_d_and_n.n.y;
				vertices[index + Nplus1 * N].nz = h_d_and_n.n.z;
			}
		}
	}
}

void Ocean::evaluateWavesFFT(float t) {
	float kx, kz, len, lambda = -1.0f;
	int index, index1;

	for (int m_prime = 0; m_prime < N; m_prime++) {
		kz = M_PI * (2.0f * m_prime - N) / length;
		for (int n_prime = 0; n_prime < N; n_prime++) {
			kx = M_PI * (2 * n_prime - N) / length;
			len = sqrt(kx * kx + kz * kz);
			index = m_prime * N + n_prime;

			h_tilde[index] = hTilde(t, n_prime, m_prime);
			h_tilde_slopex[index] = h_tilde[index] * complex(0, kx);
			h_tilde_slopez[index] = h_tilde[index] * complex(0, kz);
			if (len < 0.000001f) {
				h_tilde_dx[index] = complex(0.0f, 0.0f);
				h_tilde_dz[index] = complex(0.0f, 0.0f);
			}
			else {
				h_tilde_dx[index] = h_tilde[index] * complex(0, -kx / len);
				h_tilde_dz[index] = h_tilde[index] * complex(0, -kz / len);
			}
		}
	}

	for (int m_prime = 0; m_prime < N; m_prime++) {
		fft->fft(h_tilde, h_tilde, 1, m_prime * N);
		fft->fft(h_tilde_slopex, h_tilde_slopex, 1, m_prime * N);
		fft->fft(h_tilde_slopez, h_tilde_slopez, 1, m_prime * N);
		fft->fft(h_tilde_dx, h_tilde_dx, 1, m_prime * N);
		fft->fft(h_tilde_dz, h_tilde_dz, 1, m_prime * N);
	}
	for (int n_prime = 0; n_prime < N; n_prime++) {
		fft->fft(h_tilde, h_tilde, N, n_prime);
		fft->fft(h_tilde_slopex, h_tilde_slopex, N, n_prime);
		fft->fft(h_tilde_slopez, h_tilde_slopez, N, n_prime);
		fft->fft(h_tilde_dx, h_tilde_dx, N, n_prime);
		fft->fft(h_tilde_dz, h_tilde_dz, N, n_prime);
	}

	int sign;
	float signs[] = { 1.0f, -1.0f };
	glm::vec3 n;
	for (int m_prime = 0; m_prime < N; m_prime++) {
		for (int n_prime = 0; n_prime < N; n_prime++) {
			index = m_prime * N + n_prime;		// index into h_tilde..
			index1 = m_prime * Nplus1 + n_prime;	// index into vertices

			sign = signs[(n_prime + m_prime) & 1];

			h_tilde[index] = h_tilde[index] * sign;

			// height
			vertices[index1].y = h_tilde[index].a;

			// displacement
			h_tilde_dx[index] = h_tilde_dx[index] * sign;
			h_tilde_dz[index] = h_tilde_dz[index] * sign;
			vertices[index1].x = vertices[index1].ox + h_tilde_dx[index].a * lambda;
			vertices[index1].z = vertices[index1].oz + h_tilde_dz[index].a * lambda;

			// normal
			h_tilde_slopex[index] = h_tilde_slopex[index] * sign;
			h_tilde_slopez[index] = h_tilde_slopez[index] * sign;
			n = glm::normalize(glm::vec3(0.0f - h_tilde_slopex[index].a, 1.0f, 0.0f - h_tilde_slopez[index].a));
			vertices[index1].nx = n.x;
			vertices[index1].ny = n.y;
			vertices[index1].nz = n.z;

			// for tiling
			if (n_prime == 0 && m_prime == 0) {
				vertices[index1 + N + Nplus1 * N].y = h_tilde[index].a;

				vertices[index1 + N + Nplus1 * N].x = vertices[index1 + N + Nplus1 * N].ox + h_tilde_dx[index].a * lambda;
				vertices[index1 + N + Nplus1 * N].z = vertices[index1 + N + Nplus1 * N].oz + h_tilde_dz[index].a * lambda;

				vertices[index1 + N + Nplus1 * N].nx = n.x;
				vertices[index1 + N + Nplus1 * N].ny = n.y;
				vertices[index1 + N + Nplus1 * N].nz = n.z;
			}
			if (n_prime == 0) {
				vertices[index1 + N].y = h_tilde[index].a;

				vertices[index1 + N].x = vertices[index1 + N].ox + h_tilde_dx[index].a * lambda;
				vertices[index1 + N].z = vertices[index1 + N].oz + h_tilde_dz[index].a * lambda;

				vertices[index1 + N].nx = n.x;
				vertices[index1 + N].ny = n.y;
				vertices[index1 + N].nz = n.z;
			}
			if (m_prime == 0) {
				vertices[index1 + Nplus1 * N].y = h_tilde[index].a;

				vertices[index1 + Nplus1 * N].x = vertices[index1 + Nplus1 * N].ox + h_tilde_dx[index].a * lambda;
				vertices[index1 + Nplus1 * N].z = vertices[index1 + Nplus1 * N].oz + h_tilde_dz[index].a * lambda;

				vertices[index1 + Nplus1 * N].nx = n.x;
				vertices[index1 + Nplus1 * N].ny = n.y;
				vertices[index1 + Nplus1 * N].nz = n.z;
			}
		}
	}
}

void Ocean::render(float t, glm::vec3 light_pos, glm::mat4 Projection, glm::mat4 View, glm::mat4 Model, bool use_fft) {
	static bool eval = false;

	evaluateWavesFFT(t);
	//evaluateWaves(t);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(glProgram);
	glUniform3f(light_position, light_pos.x, light_pos.y, light_pos.z);
	glUniformMatrix4fv(projection, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(view, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(model, 1, GL_FALSE, &Model[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(oceanVertex) * Nplus1 * Nplus1, vertices);
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(oceanVertex), 0);
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(oceanVertex), (char *)NULL + 12);
	glEnableVertexAttribArray(texture);
	glVertexAttribPointer(texture, 3, GL_FLOAT, GL_FALSE, sizeof(oceanVertex), (char *)NULL + 24);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 10; i++) {
			Model = glm::scale(glm::mat4(1.0f), glm::vec3(5.f, 5.f, 5.f));
			Model = glm::translate(Model, glm::vec3(length * i, 0, length * -j));
			glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
			glDrawElements(geometry ? GL_LINES : GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
		}
	}

	glDisable(GL_BLEND);

	//std::cout << vertices[0].x << " " << vertices[0].y << " " << vertices[0].z << std::endl;
}