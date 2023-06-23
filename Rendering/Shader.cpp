#include "Shader.h"
#include "../CoreModule/Settings.h"
#include "RenderingProvider.h"
#include <fstream>
KritiaEngine::Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
	LoadShaderFile(vertexPath, fragmentPath);
}

KritiaEngine::Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
    this->geometryPath = geometryPath;
    LoadShaderFile(vertexPath, fragmentPath, geometryPath);
}

void KritiaEngine::Shader::LoadShaderFile(const char* vertexPath, const char* fragmentPath)
{
    if (Settings::renderingBackend == Rendering::RenderingProvider::RenderingBackend::OpenGL) {

        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            std::cout << "Path: " << vertexPath << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");
        // shader Program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        CheckCompileErrors(id, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        loaded = true;
    }
}

void KritiaEngine::Shader::LoadShaderFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    if (Settings::renderingBackend == Rendering::RenderingProvider::RenderingBackend::OpenGL) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            gShaderFile.open(geometryPath);
            std::stringstream vShaderStream, fShaderStream, gShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            gShaderStream << gShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            gShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            geometryCode = gShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            std::cout << "Path: " << vertexPath << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        const char* gShaderCode = geometryCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment, geometry;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");
        // geometry shader
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        CheckCompileErrors(geometry, "GEOMETRY");
        // shader Program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glAttachShader(id, geometry);
        glLinkProgram(id);
        CheckCompileErrors(id, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteShader(geometry);
        loaded = true;
    }
    
}

void KritiaEngine::Shader::Use()
{
    if (loaded) {
        glUseProgram(id);
    }
}

void KritiaEngine::Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void KritiaEngine::Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void KritiaEngine::Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void KritiaEngine::Shader::SetMat4(const std::string& name, const Matrix4x4& mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr((glm::mat4)mat4));
}

void KritiaEngine::Shader::SetVec3(const std::string& name, const Vector3& vec3) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(glm::vec3(vec3.x, vec3.y, vec3.z)));
}

void KritiaEngine::Shader::SetMat3(const std::string& name, const Matrix3x3& mat3) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr((glm::mat3)mat3));
}

bool KritiaEngine::Shader::HasUniform(const std::string& uniformName) const {
    if (glGetUniformLocation(id, uniformName.c_str()) != -1) {
        return true;
    } else {
        return false;
    }
}

unsigned int KritiaEngine::Shader::GetUniformBlockIndex(const std::string& name) const {
    return glGetUniformBlockIndex(id, name.c_str());
}

void KritiaEngine::Shader::UniformBlockBinding(unsigned int index, unsigned int bindingPoint) {
    glUniformBlockBinding(id, index, bindingPoint);
}

void KritiaEngine::Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

std::string KritiaEngine::Shader::SerializeToJson() {
    json json;
    json["Type"] = "Shader";
    json["VertexPath"] = vertexPath;
    json["FragmentPath"] = fragmentPath;
    json["GeometryPath"] = geometryPath == "" ? "Null" : geometryPath;
    return json.dump();
}

void KritiaEngine::Shader::DeserializeFromPath(const std::string& path) {
    std::ifstream input(path);
    json json = json::parse(input);
    DeserializeFromJson(json);
    input.close();
}

void KritiaEngine::Shader::DeserializeFromJson(const json& json) {
    assert(json["Type"] == "Shader");
    vertexPath = json["VertexPath"];
    fragmentPath = json["FragmentPath"];
    if (json["GeometryPath"] != "Null") {
        geometryPath = json["GeometryPath"];
    }
}
