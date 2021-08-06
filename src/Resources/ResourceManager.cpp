#include <sstream>
#include <fstream>
#include <iostream>

#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"


ResourceManager::ResourceManager(const std::string& executablePath)
{
    size_t last_slash_id = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, last_slash_id);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
    std::ifstream f;
    f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        std::cerr << "Failed to open resource: " << relativeFilePath << std::endl;
        return std::string{};
    }
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath,const std::string& fragmentPath)
{
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty())
    {
        std::cerr << "Vertex shader is empty!" << std::endl;
        return nullptr;
    }

    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty())
    {
      std::cerr << "Fragment shader is empty!" << std::endl;
      return nullptr;
    }

    std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(
      shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)
    ).first->second;
    if (newShader->isCompiled())
    {
      return newShader;
    }
    std::cerr << "Can't load shader program:\n"
      << "Vertex: " << vertexPath << "\n"
      << "Fragment: " << fragmentPath << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string& shaderName)
{
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end())
    {
        return it->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}
