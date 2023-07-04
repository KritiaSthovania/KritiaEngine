#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../CoreModule/MathStructs.h"
#include "../CoreModule/Utilities.h"
#include "../CoreModule/Object.h"
#include "../CoreModule/Interface/ISerializable.h"

namespace KritiaEngine {
    class Shader : Object, IJsonSerializable, PathDeserializable, IJsonDeserializable{
    public:
        Shader() = default;
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
        void LoadShaderFile(const char* vertexPath, const char* fragmentPath);
        void LoadShaderFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
        // activate the shader
        // ------------------------------------------------------------------------
        void Use();
        // utility uniform functions
        // ------------------------------------------------------------------------
        void SetBool(const std::string& name, bool value) const;
        // ------------------------------------------------------------------------
        void SetInt(const std::string& name, int value) const;
        // ------------------------------------------------------------------------
        void SetFloat(const std::string& name, float value) const;
        void SetMat4(const std::string& name, const Matrix4x4& mat4) const;
        void SetVec3(const std::string& name, const Vector3& vec3) const;
        void SetMat3(const std::string& name, const Matrix3x3& mat3) const;
        bool HasUniform(const std::string& uniformName) const;
        unsigned int GetUniformBlockIndex(const std::string& name) const;
        void UniformBlockBinding(unsigned int index, unsigned int bindingPoint);
        // 通过 JsonSerializable 继承
        virtual std::string SerializeToJson() override;

        // 通过 PathDeserializable 继承
        virtual void DeserializeFromPath(const std::string& path) override;

        virtual void DeserializeFromJson(const json& json) override;

        std::vector<char> VkVertexCode;
        std::vector<char> VkFragmentCode;
    private:
        bool loaded = false;
        GLuint id;
        std::string vertexPath;
        std::string fragmentPath;
        std::string geometryPath;
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void CheckCompileErrors(unsigned int shader, std::string type);
        std::vector<char> ReadFile(const std::string& filename);
    };
}


#endif