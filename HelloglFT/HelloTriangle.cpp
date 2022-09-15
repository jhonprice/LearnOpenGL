#include<iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glft/tiny_gltf.h>
#include "../HelloTriangle/LoadShader.h"

static int width = 600, height = 600;
const int NULL_ID = 0;



//<<设置视口>>
static void reshape(GLFWwindow* window, int w, int h)
{
	width = w > 1 ? w : 1;
	height = h > 1 ? h : 1;
	glViewport(0, 0, width, height);
}
//<<处理输入>>
static void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void bindMesh(std::map<int, GLuint>& vbos,
	tinygltf::Model& model, tinygltf::Mesh& mesh) {
	for (size_t i = 0; i < model.bufferViews.size(); ++i) {
		const tinygltf::BufferView& bufferView = model.bufferViews[i];
		if (bufferView.target == 0) {  // TODO impl drawarrays
			std::cout << "WARN: bufferView.target is zero" << std::endl;
			continue;  // Unsupported bufferView.
					   /*
						 From spec2.0 readme:
						 https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
								  ... drawArrays function should be used with a count equal to
						 the count            property of any of the accessors referenced by the
						 attributes            property            (they are all equal for a given
						 primitive).
					   */
		}

		const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
		std::cout << "bufferview.target " << bufferView.target << std::endl;

		GLuint vbo;
		glGenBuffers(1, &vbo);
		vbos[i] = vbo;
		glBindBuffer(bufferView.target, vbo);

		std::cout << "buffer.data.size = " << buffer.data.size()
			<< ", bufferview.byteOffset = " << bufferView.byteOffset
			<< std::endl;

		glBufferData(bufferView.target, bufferView.byteLength,
			&buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
	}

	for (size_t i = 0; i < mesh.primitives.size(); ++i) {
		tinygltf::Primitive primitive = mesh.primitives[i];
		tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

		for (auto& attrib : primitive.attributes) {
			tinygltf::Accessor accessor = model.accessors[attrib.second];
			int byteStride =
				accessor.ByteStride(model.bufferViews[accessor.bufferView]);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

			int size = 1;
			if (accessor.type != TINYGLTF_TYPE_SCALAR) {
				size = accessor.type;
			}

			int vaa = -1;
			if (attrib.first.compare("POSITION") == 0) vaa = 0;
			if (attrib.first.compare("NORMAL") == 0) vaa = 1;
			if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
			if (vaa > -1) {
				glEnableVertexAttribArray(vaa);
				glVertexAttribPointer(vaa, size, accessor.componentType,
					accessor.normalized ? GL_TRUE : GL_FALSE,
					byteStride, BUFFER_OFFSET(accessor.byteOffset));
			}
			else
				std::cout << "vaa missing: " << attrib.first << std::endl;
		}
		std::cout << "model.textures.size(): " << model.textures.size() << std::endl;
		if (model.textures.size() > 0) {
			// fixme: Use material's baseColor
			tinygltf::Texture& tex = model.textures[0];

			if (tex.source > -1) {

				GLuint texid;
				glGenTextures(1, &texid);

				tinygltf::Image& image = model.images[tex.source];

				glBindTexture(GL_TEXTURE_2D, texid);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				GLenum format = GL_RGBA;

				if (image.component == 1) {
					format = GL_RED;
				}
				else if (image.component == 2) {
					format = GL_RG;
				}
				else if (image.component == 3) {
					format = GL_RGB;
				}
				else {
					// ???
				}

				GLenum type = GL_UNSIGNED_BYTE;
				if (image.bits == 8) {
					// ok
				}
				else if (image.bits == 16) {
					type = GL_UNSIGNED_SHORT;
				}
				else {
					// ???
				}

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
					format, type, &image.image.at(0));
			}
		}
	}
}

// bind models
void bindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model,
	tinygltf::Node& node) {
	if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
		bindMesh(vbos, model, model.meshes[node.mesh]);
	}

	for (size_t i = 0; i < node.children.size(); i++) {
		assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
		bindModelNodes(vbos, model, model.nodes[node.children[i]]);
	}
}


void drawMesh(const std::map<int, GLuint>& vbos,
	tinygltf::Model& model, tinygltf::Mesh& mesh) {
	for (size_t i = 0; i < mesh.primitives.size(); ++i) {
		tinygltf::Primitive primitive = mesh.primitives[i];
		tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

		glDrawElements(primitive.mode, indexAccessor.count,
			indexAccessor.componentType,
			BUFFER_OFFSET(indexAccessor.byteOffset));
	}
}

// recursively draw node and children nodes of model
void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
	tinygltf::Model& model, tinygltf::Node& node) {
	if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
		drawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
	}
	for (size_t i = 0; i < node.children.size(); i++) {
		drawModelNodes(vaoAndEbos, model, model.nodes[node.children[i]]);
	}
}
void drawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
	tinygltf::Model& model) {
	glBindVertexArray(vaoAndEbos.first);

	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
	for (size_t i = 0; i < scene.nodes.size(); ++i) {
		drawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
	}

	glBindVertexArray(0);
}
std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model& model) {
	std::map<int, GLuint> vbos;
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
	for (size_t i = 0; i < scene.nodes.size(); ++i) {
		assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
		bindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
	}

	glBindVertexArray(0);
	// cleanup vbos but do not delete index buffers yet
	for (auto it = vbos.cbegin(); it != vbos.cend();) {
		tinygltf::BufferView bufferView = model.bufferViews[it->first];
		if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
			glDeleteBuffers(1, &vbos[it->first]);
			vbos.erase(it++);
		}
		else {
			++it;
		}
	}

	return { vao, vbos };
}


int main(int argc, char** argv)
{

//<<初始化GLFW>>
#pragma region InitGLFW
	auto initValue = glfwInit();
	if (!initValue) std::cerr << "GLFW FAILED" << std::endl;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#pragma endregion


//<<创建窗口>>
#pragma region InitWindows
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
#pragma endregion


//<<初始化GL3W>>
#pragma region InitGl3W
	if (gl3wInit()) {
		std::cerr << "failed to initialize OpenGL" << std::endl;
		return -1;
	}
	if (!gl3wIsSupported(3, 3)) {
		std::cerr << "OpenGL 3.3 not supported" << std::endl;
		return -1;
	}
	std::cerr << "OpenGL: " << glGetString(GL_VERSION)
		<< " GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#pragma endregion


//<<视口设置>>
#pragma region ViewPortSet
	glfwSetFramebufferSizeCallback(window, reshape);
#pragma endregion

//<<OpenGL对象的各种ID>>
#pragma region IDManaged

	//VertexShader
	unsigned int vertexShader_ID;
	int vertexShader_ID_NUM = 1;

	//FragmentShader
	unsigned int fragShader_ID;
	int fragShader_ID_NUM = 1;

	//Program
	unsigned int shaderProgram_ID;


	
#pragma endregion IDManaged


//<<编译着色器，输出调试信息>>
#pragma region SetVertexShader
	string vertexShaderSourceCodeString = readShader("../shader/hellovertex.vert");
	const char* vertexShaderSourceCode = vertexShaderSourceCodeString.c_str();
	vertexShader_ID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader_ID, vertexShader_ID_NUM, &vertexShaderSourceCode, nullptr);
	glCompileShader(vertexShader_ID);


	debugInfo(vertexShader_ID);
#pragma endregion SetVertexShader

#pragma region SetFragShader
	string fragShaderSourceCodeString = readShader("../shader/hellofrag.frag");
	const char* fragShaderSourceCode = fragShaderSourceCodeString.c_str();
	fragShader_ID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShader_ID, fragShader_ID_NUM, &fragShaderSourceCode, nullptr);
	glCompileShader(fragShader_ID);


	debugInfo(fragShader_ID);
#pragma endregion SetFragShader


//<<构建link着色器程序，输出调试信息>>
#pragma region SetProgram
	shaderProgram_ID = glCreateProgram();
	glAttachShader(shaderProgram_ID, vertexShader_ID);
	glAttachShader(shaderProgram_ID, fragShader_ID);
	glLinkProgram(shaderProgram_ID);

	programInfo(shaderProgram_ID);
#pragma endregion SetProgram

	glUseProgram(shaderProgram_ID);

	//glft
	tinygltf::Model model;
	tinygltf::TinyGLTF gltf_ctx;
	gltf_ctx.SetStoreOriginalJSONForExtrasAndExtensions(false);
	bool ret = false;
	std::string err;
	std::string warn;
	ret = gltf_ctx.LoadASCIIFromFile(&model, &err, &warn,
		"D:\\model\\glTF-Sample-Models\\2.0\\Triangle\\glTF\\Triangle.gltf");
	if (!ret) {
		std::cerr << "Failed to parse glTF" << std::endl;
		return -1;
	}
	else {
		std::cerr << "Ok to parse glTF" << std::endl;
	}
	auto tmp = bindModel(model);






//<<渲染循环>>
#pragma region RenderPart
	while (!glfwWindowShouldClose(window)) {

		//<<处理输入>>
		processInput(window);
		//<<渲染函数>>
		//display(window,shaderProgram_ID,1,1);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram_ID);
		drawModel(tmp,model);
		//<<事件触发器>>
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
#pragma endregion



	glfwTerminate();
	return 0;
}
