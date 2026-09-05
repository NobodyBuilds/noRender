#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "norender.h"
#include "helper.h"
#include "vertex.h"
#include"frag.h"
#include"renderdata.h"
#include "buffers.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"



 GLFWwindow* window;



 GLFWwindow* norender::getWindowHandle() {
	 return window;
 }


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if ((float)width == 0 || (float)height == 0) {
		printf("width or height cant be 0");
		return;
	}
	noRender.setScreenSize((float)width, (float)height);
	glViewport(0, 0, width, height);
	if (noRender.getMode() == 3) {
		view.cx = (float)width * 0.5f;
		view.cz = (float)height * 0.5f;
		view.height = (float)height;
		view.aspect = (float)width / (float)height;
	};

};


int norender::createWindow(int width, int height, const char* Windowname, int vsync) {// vsync is 1 for on, 0 for off

	if (!glfwInit()) {
		printf("glfw init failed");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(width, height, Windowname, nullptr, nullptr);

	if (!window) {
		printf("window creation failed");
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(vsync);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to init GLAD\n";
		return -1;
	}
	norender::screenwidth = width;
	norender::screenheight = height;

	view.cx = width * 0.5f;
	view.cz = height * 0.5f;
	view.height = (float)height;
	view.aspect = (float)width / (float)height;
	view.width();
	return 0;
};

void norender::closeWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();

};

bool norender::isWindowOpen() {
	return !glfwWindowShouldClose(window);
};

void norender::pollEvents() {
	glfwPollEvents();
};
void norender::swapBuffers() {
	glfwSwapBuffers(window);
}

void norender::clearScreen(float r,float g ,float b) {
	glClearColor(r, g, b, 1.0f);
	if (noRender.getMode() == 3)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
}



void norender::init() {


	
}


void norender::setup2D() {
	norender::mode = 2;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void norender::setup3D() {
	norender::mode = 3;
	glEnable(GL_DEPTH_TEST);
	
}
void norender::setupCamera() {
	if (noRender.getMode() == 2) {
		static bool firstcall = true;
		if (firstcall) {
			printf("ERROR: camera is not available for 2d scene\n");
			firstcall = false;
		}
		return;
	}
	if (blockedInput) return;
	updateCameraVectors(camera);
	cameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetScrollCallback(window, scrollCallback);
}


void norender::updateCamera() {
	updatefov(noRender.fov);
	updateCameraMovement(window);
}
float norender::getCameraX() {
	return camera.position.x;
}
float norender::getCameraY() {
	return camera.position.y;
}
float norender::getCameraZ() {
	return camera.position.z;

}
void norender::setScreenSize(float w ,float h) {
	norender::screenwidth = (int)w;
	norender::screenheight = (int)h;
}

void norender::setInputBlocked(bool blocked) {
	norender::blockedInput = blocked;
}


spriteData norender::loadSprite(const char* address) {
	int w, h, channels;
	stbi_set_flip_vertically_on_load(true);


	unsigned char* data = stbi_load(address, &w, &h, &channels, 4);

	if (!data) {
		printf("Failed to load %s: %s\n", address, stbi_failure_reason());
		return {};
	}
	std::vector<quadtexture2d> container;
	container.reserve(w * h);
	for (int i = 0; i < w * h; i++) {
		quadtexture2d t;
		t.r = data[i * 4 + 0] / 255.0f;
		t.g = data[i * 4 + 1] / 255.0f;
		t.b = data[i * 4 + 2] / 255.0f;
		t.opacity = data[i * 4 + 3] / 255.0f;

		container.push_back(t);
	}
	stbi_image_free(data);

	return {container,w,h};
}
//vboids
  
unsigned int vboids::triangle_vbo() const {
	initTriangleBuffer2d();
	return triVBO; }
unsigned int vboids::triangle_instanced_vbo(int count,int id) const {
	initInstancedTriangleBuffer2d( count,id);
	return triInstDataVBO[id]; }
unsigned int vboids::circle_vbo() const { 
	initCircleBuffer2d();
	return cirVBO; }
unsigned int vboids::circle_instanced_vbo(int count,int id) const { 
	initInstancedCircleBuffer2d(count,id);
	return circleInstDataVBO[id]; }
unsigned int vboids::quad_vbo() const { 
	initQuadBuffer2d();
	return quadVBO; }
unsigned int vboids::quad_instanced_vbo(int count,int id) const { 
	initInstancedQuadBuffer2d(count,id);
	return quadInstDataVBO[id]; }
unsigned int vboids::line_vbo() const { 
	initLineBuffer2d();
	return lineVBO; }
unsigned int vboids::line_instanced_vbo(int count,int id) const {
	initlineinstancedBuffer2d(count,id);
	return chainVBO[id]; }

unsigned int vboids::quad_texture_tex(int pixelwidth,int pixelheight, int id)const {
	initquadfsbuffer(pixelwidth,pixelheight, id);
	return quadfsTEX[id];
}


static constexpr int meshStrideFloats = 14;

struct ImagePixels {
	int width = 0;
	int height = 0;
	std::vector<unsigned char> rgba;
};

static void multiplyMat4(const float* a, const float* b, float* out) {
	float r[16];
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			r[col * 4 + row] =
				a[0 * 4 + row] * b[col * 4 + 0] +
				a[1 * 4 + row] * b[col * 4 + 1] +
				a[2 * 4 + row] * b[col * 4 + 2] +
				a[3 * 4 + row] * b[col * 4 + 3];
		}
	}
	memcpy(out, r, sizeof(r));
}

static void getNodeWorldTransform(const cgltf_node* node, float* out) {
	float local[16];
	cgltf_node_transform_local(node, local);

	if (!node->parent) {
		memcpy(out, local, sizeof(local));
		return;
	}

	float parent[16];
	getNodeWorldTransform(node->parent, parent);
	multiplyMat4(parent, local, out);
}

static void transformPosition(const float* mat, float* pos) {
	float x = pos[0], y = pos[1], z = pos[2];
	pos[0] = mat[0] * x + mat[4] * y + mat[8] * z + mat[12];
	pos[1] = mat[1] * x + mat[5] * y + mat[9] * z + mat[13];
	pos[2] = mat[2] * x + mat[6] * y + mat[10] * z + mat[14];
}

static void normalize3(float* v) {
	float len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (len > 0.00001f) {
		v[0] /= len; v[1] /= len; v[2] /= len;
	}
}

static void transformNormal(const float* mat, float* norm) {
	float x = norm[0], y = norm[1], z = norm[2];
	norm[0] = mat[0] * x + mat[4] * y + mat[8] * z;
	norm[1] = mat[1] * x + mat[5] * y + mat[9] * z;
	norm[2] = mat[2] * x + mat[6] * y + mat[10] * z;
	normalize3(norm);
}

static std::string meshBaseDirectory(const char* address) {
	std::string path = address ? address : "";
	size_t slash = path.find_last_of("/\\");
	return slash == std::string::npos ? std::string() : path.substr(0, slash + 1);
}

static bool isAbsolutePath(const std::string& path) {
	return (path.size() > 1 && path[1] == ':') || (!path.empty() && (path[0] == '/' || path[0] == '\\'));
}

static float srgbToLinear(float value) {
	value = std::clamp(value, 0.0f, 1.0f);
	return value <= 0.04045f ? value / 12.92f : pow((value + 0.055f) / 1.055f, 2.4f);
}

static float wrapTextureCoord(float value, cgltf_wrap_mode mode) {
	if (mode == cgltf_wrap_mode_clamp_to_edge) {
		return std::clamp(value, 0.0f, 1.0f);
	}
	if (mode == cgltf_wrap_mode_mirrored_repeat) {
		float wrapped = fmod(value, 2.0f);
		if (wrapped < 0.0f) wrapped += 2.0f;
		return wrapped <= 1.0f ? wrapped : 2.0f - wrapped;
	}
	value = value - floor(value);
	return value < 0.0f ? value + 1.0f : value;
}

static bool decodeBase64ImageUri(const cgltf_options& options, const char* uri, ImagePixels& image) {
	const char* comma = strchr(uri, ',');
	if (!comma || comma - uri < 7 || strncmp(comma - 7, ";base64", 7) != 0) {
		return false;
	}

	std::string encoded = comma + 1;
	encoded.erase(std::remove_if(encoded.begin(), encoded.end(), [](unsigned char c) {
		return c == '\r' || c == '\n' || c == '\t' || c == ' ';
	}), encoded.end());

	int padding = 0;
	if (!encoded.empty() && encoded[encoded.size() - 1] == '=') padding++;
	if (encoded.size() > 1 && encoded[encoded.size() - 2] == '=') padding++;
	cgltf_size decodedSize = (encoded.size() / 4) * 3 - padding;

	void* decoded = nullptr;
	if (cgltf_load_buffer_base64(&options, decodedSize, encoded.c_str(), &decoded) != cgltf_result_success) {
		return false;
	}

	int width = 0, height = 0, channels = 0;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* pixels = stbi_load_from_memory(
		static_cast<const stbi_uc*>(decoded), (int)decodedSize, &width, &height, &channels, 4);

	if (options.memory.free_func) {
		options.memory.free_func(options.memory.user_data, decoded);
	}
	else {
		free(decoded);
	}

	if (!pixels) {
		return false;
	}

	image.width = width;
	image.height = height;
	image.rgba.assign(pixels, pixels + width * height * 4);
	stbi_image_free(pixels);
	return true;
}

static bool loadImagePixels(const cgltf_options& options, const char* meshPath, cgltf_image* source, ImagePixels& image) {
	if (!source) {
		return false;
	}

	int width = 0, height = 0, channels = 0;
	unsigned char* pixels = nullptr;
	stbi_set_flip_vertically_on_load(false);

	if (source->buffer_view) {
		const uint8_t* data = cgltf_buffer_view_data(source->buffer_view);
		if (data) {
			pixels = stbi_load_from_memory(data, (int)source->buffer_view->size, &width, &height, &channels, 4);
		}
	}
	else if (source->uri) {
		if (strncmp(source->uri, "data:", 5) == 0) {
			return decodeBase64ImageUri(options, source->uri, image);
		}

		std::string decodedUri = source->uri;
		cgltf_size decodedLength = cgltf_decode_uri(decodedUri.data());
		decodedUri.resize(decodedLength);
		std::string resolved = isAbsolutePath(decodedUri) ? decodedUri : meshBaseDirectory(meshPath) + decodedUri;
		pixels = stbi_load(resolved.c_str(), &width, &height, &channels, 4);
	}

	if (!pixels) {
		return false;
	}

	image.width = width;
	image.height = height;
	image.rgba.assign(pixels, pixels + width * height * 4);
	stbi_image_free(pixels);
	return true;
}

static const ImagePixels* getTexturePixels(
	const cgltf_options& options,
	const char* meshPath,
	cgltf_texture_view& textureView,
	std::unordered_map<cgltf_image*, ImagePixels>& cache)
{
	if (!textureView.texture || !textureView.texture->image) {
		return nullptr;
	}

	cgltf_image* image = textureView.texture->image;
	auto found = cache.find(image);
	if (found != cache.end()) {
		return found->second.rgba.empty() ? nullptr : &found->second;
	}

	ImagePixels loaded;
	loadImagePixels(options, meshPath, image, loaded);
	auto inserted = cache.emplace(image, std::move(loaded));
	return inserted.first->second.rgba.empty() ? nullptr : &inserted.first->second;
}

static void applyTextureTransform(const cgltf_texture_view& textureView, float& u, float& v) {
	if (!textureView.has_transform) {
		return;
	}

	float scaledU = u * textureView.transform.scale[0];
	float scaledV = v * textureView.transform.scale[1];
	float c = cos(textureView.transform.rotation);
	float s = sin(textureView.transform.rotation);

	u = c * scaledU - s * scaledV + textureView.transform.offset[0];
	v = s * scaledU + c * scaledV + textureView.transform.offset[1];
}

static void sampleTexture(const ImagePixels* image, const cgltf_texture_view& textureView, float u, float v, bool srgb, float* out) {
	out[0] = 1.0f;
	out[1] = 1.0f;
	out[2] = 1.0f;
	out[3] = 1.0f;

	if (!image || image->width <= 0 || image->height <= 0 || image->rgba.empty()) {
		return;
	}

	applyTextureTransform(textureView, u, v);

	cgltf_sampler* sampler = textureView.texture ? textureView.texture->sampler : nullptr;
	cgltf_wrap_mode wrapS = sampler && sampler->wrap_s != 0 ? sampler->wrap_s : cgltf_wrap_mode_repeat;
	cgltf_wrap_mode wrapT = sampler && sampler->wrap_t != 0 ? sampler->wrap_t : cgltf_wrap_mode_repeat;

	u = wrapTextureCoord(u, wrapS);
	v = wrapTextureCoord(v, wrapT);

	int px = std::clamp((int)(u * (image->width - 1) + 0.5f), 0, image->width - 1);
	int py = std::clamp((int)(v * (image->height - 1) + 0.5f), 0, image->height - 1);
	size_t offset = ((size_t)py * image->width + px) * 4;

	out[0] = image->rgba[offset + 0] / 255.0f;
	out[1] = image->rgba[offset + 1] / 255.0f;
	out[2] = image->rgba[offset + 2] / 255.0f;
	out[3] = image->rgba[offset + 3] / 255.0f;

	if (srgb) {
		out[0] = srgbToLinear(out[0]);
		out[1] = srgbToLinear(out[1]);
		out[2] = srgbToLinear(out[2]);
	}
}

static std::vector<unsigned int> readPrimitiveIndices(cgltf_primitive* prim, int vertexCount) {
	std::vector<unsigned int> rawIndices;

	if (prim->indices) {
		cgltf_accessor* indexAccessor = prim->indices;
		rawIndices.reserve(indexAccessor->count);
		for (cgltf_size i = 0; i < indexAccessor->count; i++) {
			rawIndices.push_back((unsigned int)cgltf_accessor_read_index(indexAccessor, i));
		}
	}
	else {
		rawIndices.reserve(vertexCount);
		for (int i = 0; i < vertexCount; i++) {
			rawIndices.push_back((unsigned int)i);
		}
	}

	std::vector<unsigned int> triangles;
	if (prim->type == cgltf_primitive_type_triangles) {
		size_t triangleCount = rawIndices.size() / 3;
		triangles.assign(rawIndices.begin(), rawIndices.begin() + triangleCount * 3);
	}
	else if (prim->type == cgltf_primitive_type_triangle_strip) {
		if (rawIndices.size() >= 3) {
			triangles.reserve((rawIndices.size() - 2) * 3);
			for (size_t i = 0; i + 2 < rawIndices.size(); i++) {
				if (i % 2 == 0) {
					triangles.push_back(rawIndices[i]);
					triangles.push_back(rawIndices[i + 1]);
					triangles.push_back(rawIndices[i + 2]);
				}
				else {
					triangles.push_back(rawIndices[i + 1]);
					triangles.push_back(rawIndices[i]);
					triangles.push_back(rawIndices[i + 2]);
				}
			}
		}
	}
	else if (prim->type == cgltf_primitive_type_triangle_fan) {
		if (rawIndices.size() >= 3) {
			triangles.reserve((rawIndices.size() - 2) * 3);
			for (size_t i = 1; i + 1 < rawIndices.size(); i++) {
				triangles.push_back(rawIndices[0]);
				triangles.push_back(rawIndices[i]);
				triangles.push_back(rawIndices[i + 1]);
			}
		}
	}

	return triangles;
}

static void buildMissingNormals(const std::vector<float>& positions, const std::vector<unsigned int>& indices, std::vector<float>& normals) {
	normals.assign(positions.size(), 0.0f);

	for (size_t i = 0; i + 2 < indices.size(); i += 3) {
		unsigned int ia = indices[i + 0];
		unsigned int ib = indices[i + 1];
		unsigned int ic = indices[i + 2];
		if ((ia * 3 + 2) >= positions.size() || (ib * 3 + 2) >= positions.size() || (ic * 3 + 2) >= positions.size()) {
			continue;
		}

		glm::vec3 a(positions[ia * 3 + 0], positions[ia * 3 + 1], positions[ia * 3 + 2]);
		glm::vec3 b(positions[ib * 3 + 0], positions[ib * 3 + 1], positions[ib * 3 + 2]);
		glm::vec3 c(positions[ic * 3 + 0], positions[ic * 3 + 1], positions[ic * 3 + 2]);
		glm::vec3 n = glm::cross(b - a, c - a);
		if (glm::length(n) <= 0.00001f) {
			continue;
		}
		n = glm::normalize(n);

		for (unsigned int index : { ia, ib, ic }) {
			normals[index * 3 + 0] += n.x;
			normals[index * 3 + 1] += n.y;
			normals[index * 3 + 2] += n.z;
		}
	}

	for (size_t i = 0; i + 2 < normals.size(); i += 3) {
		float n[3] = { normals[i + 0], normals[i + 1], normals[i + 2] };
		if (n[0] == 0.0f && n[1] == 0.0f && n[2] == 0.0f) {
			n[1] = 1.0f;
		}
		normalize3(n);
		normals[i + 0] = n[0];
		normals[i + 1] = n[1];
		normals[i + 2] = n[2];
	}
}

MeshData norender::loadMesh(const char* address) {

	cgltf_options option{};
	cgltf_data* data = NULL;

	cgltf_result result = cgltf_parse_file(&option, address, &data);
	if (result != cgltf_result_success) {
		printf("Failed to parse %s\n", address);
		return {};
	}
	result = cgltf_load_buffers(&option, data, address);
	if (result != cgltf_result_success) {
		printf("Failed to load buffers for %s\n", address);
		cgltf_free(data);
		return {};
	}

	std::vector<unsigned int> allIndices;
	std::vector<float> allInterleaved;
	std::unordered_map<cgltf_image*, ImagePixels> imageCache;
	imageCache.reserve(data->images_count > 0 ? data->images_count : 1);
	bool hasTransparency = false;

	for (size_t n = 0; n < data->nodes_count; n++) {
		cgltf_node* node = &data->nodes[n];
		if (!node->mesh) continue;

		float matrix[16];
		getNodeWorldTransform(node, matrix);

		cgltf_mesh* mesh = node->mesh;
		for (size_t p = 0; p < mesh->primitives_count; p++) {
			cgltf_primitive* prim = &mesh->primitives[p];
			if (prim->type != cgltf_primitive_type_triangles &&
				prim->type != cgltf_primitive_type_triangle_strip &&
				prim->type != cgltf_primitive_type_triangle_fan) {
				continue;
			}

			// Track vertex offset for indices
			unsigned int vertexOffset = (unsigned int)(allInterleaved.size() / meshStrideFloats);

			float baseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			float metallic = 0.0f;
			float roughness = 0.85f;
			float alphaCutoff = 0.5f;
			float alphaMode = 0.0f;
			cgltf_texture_view* baseColorTexture = nullptr;
			cgltf_texture_view* metallicRoughnessTexture = nullptr;
			int baseColorTexCoordIndex = 0;
			int metallicRoughnessTexCoordIndex = 0;

			if (prim->material) {
				cgltf_material* material = prim->material;
				alphaCutoff = material->alpha_cutoff;
				if (material->alpha_mode == cgltf_alpha_mode_mask) {
					alphaMode = 1.0f;
				}
				else if (material->alpha_mode == cgltf_alpha_mode_blend) {
					alphaMode = 2.0f;
					hasTransparency = true;
				}

				if (material->has_pbr_metallic_roughness) {
					baseColor[0] = material->pbr_metallic_roughness.base_color_factor[0];
					baseColor[1] = material->pbr_metallic_roughness.base_color_factor[1];
					baseColor[2] = material->pbr_metallic_roughness.base_color_factor[2];
					baseColor[3] = material->pbr_metallic_roughness.base_color_factor[3];
					metallic = material->pbr_metallic_roughness.metallic_factor;
					roughness = material->pbr_metallic_roughness.roughness_factor;

					if (material->pbr_metallic_roughness.base_color_texture.texture) {
						baseColorTexture = &material->pbr_metallic_roughness.base_color_texture;
						baseColorTexCoordIndex = baseColorTexture->texcoord;
					}
					if (material->pbr_metallic_roughness.metallic_roughness_texture.texture) {
						metallicRoughnessTexture = &material->pbr_metallic_roughness.metallic_roughness_texture;
						metallicRoughnessTexCoordIndex = metallicRoughnessTexture->texcoord;
					}
				}

				if (material->has_transmission && material->transmission.transmission_factor > 0.0f) {
					float transmission = std::clamp(material->transmission.transmission_factor, 0.0f, 1.0f);
					baseColor[3] = baseColor[3] * (1.0f - 0.65f * transmission);
					alphaMode = 2.0f;
					hasTransparency = true;
				}
			}

			// extract positions and normals
			cgltf_accessor* posAccessor = NULL;
			cgltf_accessor* normAccessor = NULL;
			cgltf_accessor* colorAccessor = NULL;
			cgltf_accessor* baseTexcoordAccessor = NULL;
			cgltf_accessor* metallicRoughnessTexcoordAccessor = NULL;
			for (int i = 0; i < (int)prim->attributes_count; i++) {
				if (prim->attributes[i].type == cgltf_attribute_type_position) {
					posAccessor = prim->attributes[i].data;
				} else if (prim->attributes[i].type == cgltf_attribute_type_normal) {
					normAccessor = prim->attributes[i].data;
				} else if (prim->attributes[i].type == cgltf_attribute_type_color && prim->attributes[i].index == 0) {
					colorAccessor = prim->attributes[i].data;
				} else if (prim->attributes[i].type == cgltf_attribute_type_texcoord) {
					if (prim->attributes[i].index == baseColorTexCoordIndex) {
						baseTexcoordAccessor = prim->attributes[i].data;
					}
					if (prim->attributes[i].index == metallicRoughnessTexCoordIndex) {
						metallicRoughnessTexcoordAccessor = prim->attributes[i].data;
					}
				}
			}

			if (!posAccessor) continue;

			int vertexCount = (int)posAccessor->count;
			std::vector<float> positions(vertexCount * 3);
			for (int i = 0; i < vertexCount; i++) {
				cgltf_accessor_read_float(posAccessor, i, &positions[i * 3], 3);
			}

			bool hasNormals = (normAccessor != NULL);
			std::vector<float> normals;
			if (hasNormals) {
				normals.resize(vertexCount * 3);
				for (int i = 0; i < vertexCount; i++) {
					cgltf_accessor_read_float(normAccessor, i, &normals[i * 3], 3);
				}
			}

			std::vector<unsigned int> primitiveIndices = readPrimitiveIndices(prim, vertexCount);
			if (primitiveIndices.empty()) {
				continue;
			}

			if (!hasNormals) {
				buildMissingNormals(positions, primitiveIndices, normals);
			}

			const ImagePixels* baseColorPixels = (baseColorTexture && baseTexcoordAccessor) ? getTexturePixels(option, address, *baseColorTexture, imageCache) : nullptr;
			const ImagePixels* metallicRoughnessPixels = (metallicRoughnessTexture && metallicRoughnessTexcoordAccessor) ? getTexturePixels(option, address, *metallicRoughnessTexture, imageCache) : nullptr;

			// transform, interleave, and append
			for (int i = 0; i < vertexCount; i++) {
				float pos[3] = { positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2] };
				transformPosition(matrix, pos);

				float norm[3] = { normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2] };
				transformNormal(matrix, norm);

				float vertexColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				if (colorAccessor) {
					cgltf_accessor_read_float(colorAccessor, i, vertexColor, 4);
				}

				float uv[2] = { 0.0f, 0.0f };
				if (baseTexcoordAccessor) {
					cgltf_accessor_read_float(baseTexcoordAccessor, i, uv, 2);
				}

				float baseTextureColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				if (baseColorTexture && baseTexcoordAccessor) {
					sampleTexture(baseColorPixels, *baseColorTexture, uv[0], uv[1], true, baseTextureColor);
				}

				float materialColor[4] = {
					baseColor[0] * vertexColor[0] * baseTextureColor[0],
					baseColor[1] * vertexColor[1] * baseTextureColor[1],
					baseColor[2] * vertexColor[2] * baseTextureColor[2],
					baseColor[3] * vertexColor[3] * baseTextureColor[3],
				};

				float mrUv[2] = { uv[0], uv[1] };
				if (metallicRoughnessTexcoordAccessor) {
					cgltf_accessor_read_float(metallicRoughnessTexcoordAccessor, i, mrUv, 2);
				}

				float metallicRoughnessSample[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				if (metallicRoughnessTexture && metallicRoughnessTexcoordAccessor) {
					sampleTexture(metallicRoughnessPixels, *metallicRoughnessTexture, mrUv[0], mrUv[1], false, metallicRoughnessSample);
				}

				float finalMetallic = std::clamp(metallic * metallicRoughnessSample[2], 0.0f, 1.0f);
				float finalRoughness = std::clamp(roughness * metallicRoughnessSample[1], 0.04f, 1.0f);

				if (alphaMode > 1.5f && materialColor[3] < 0.999f) {
					hasTransparency = true;
				}

				allInterleaved.push_back(pos[0]);
				allInterleaved.push_back(pos[1]);
				allInterleaved.push_back(pos[2]);
				allInterleaved.push_back(norm[0]);
				allInterleaved.push_back(norm[1]);
				allInterleaved.push_back(norm[2]);
				allInterleaved.push_back(materialColor[0]);
				allInterleaved.push_back(materialColor[1]);
				allInterleaved.push_back(materialColor[2]);
				allInterleaved.push_back(materialColor[3]);
				allInterleaved.push_back(finalMetallic);
				allInterleaved.push_back(finalRoughness);
				allInterleaved.push_back(alphaCutoff);
				allInterleaved.push_back(alphaMode);
			}

			for (unsigned int idx : primitiveIndices) {
				allIndices.push_back(idx + vertexOffset);
			}
		}
	}

	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, allInterleaved.size() * sizeof(float),
		allInterleaved.data(), GL_STATIC_DRAW);

	// layout 0 = position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, meshStrideFloats * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// layout 1 = normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, meshStrideFloats * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// layout 2 = base color + alpha
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, meshStrideFloats * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// layout 3 = metallic, roughness, alpha cutoff, alpha mode
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, meshStrideFloats * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// index data (EBO) — must be bound while VAO is still bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int),
		allIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	if (meshProgram == 0) {
		meshProgram = createProgram(meshVert, meshFrag);
	}

	cgltf_free(data);

	printf("Loaded mesh: %s (%zu vertices, %zu indices)\n", address, allInterleaved.size()/meshStrideFloats, allIndices.size());

	return { vao, vbo, ebo, (int)allIndices.size(), hasTransparency };
}
