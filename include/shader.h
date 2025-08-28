#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

class Shader {
public:
    unsigned int m_id;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    
    void set_bool(const std::string& name, bool val) const;
    void set_int(const std::string& name, int val) const;
    void set_float(const std::string& name, float val) const;
};