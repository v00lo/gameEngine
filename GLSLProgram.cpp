#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>

//Initialize privata variables
GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{

}

GLSLProgram::~GLSLProgram()
{

}
//Method for creating a program and shader objects for later compiling
void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
//Create an empty program object to which we can attach shader objects, with a handle for referencing
    _programID = glCreateProgram();
//Create an empty vertex shader object with a handle for referencing    
    _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

//Check if glCreateShader successfuly created shader object and a handle
    if(_vertexShaderID == 0){
        fatalError("vertexShader failed to be created");
    }

//Create an empty fragment shader object with a handle for referencing    
    _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
//Check if glCreateShader successfuly created shader object and a handle
    if(_fragmentShaderID == 0){
        fatalError("fragmentShader failed to be created");
    }
//Compile shaders method
    compileShader(vertexShaderFilePath, _vertexShaderID);
    compileShader(fragmentShaderFilePath, _fragmentShaderID);

}
//Attach the compiled shaders to the program object
void GLSLProgram::linkShaders()
{
//Attach compiled shader to a program object    
    glAttachShader(_programID, _vertexShaderID);
    glAttachShader(_programID, _fragmentShaderID);
//Link the program object and use attached shaders to create executable that will run on shader-oriented processor
    glLinkProgram(_programID);

//Check if shaders were properly attached and linked to a program object    
    GLint isLinked = 0;
    glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE){
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);
//Necessary cleanup of program and shader objects after failed linking        
        glDeleteProgram(_programID);
        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);

        std::printf("%s\n", &(infoLog[0]));
        fatalError("Shaders failed to link");
    }
//Detach shaders objects from a program object        
    glDetachShader(_programID, _vertexShaderID);
    glDetachShader(_programID, _fragmentShaderID);
//Delete shaders and the free memory    
    glDeleteShader(_vertexShaderID);
    glDeleteShader(_fragmentShaderID);
}
//Associate a user-defined attritute variable in the program object with vertex attribute index
void GLSLProgram::addAttribute(const std::string& attribName)
{
    glBindAttribLocation(_programID, _numAttributes++, attribName.c_str());
}

GLuint GLSLProgram::getUniformLocation(const std::string& uniformName)
{
    GLint location = glGetUniformLocation(_programID, uniformName.c_str());
    if(location == GL_INVALID_INDEX){
        fatalError("uniform " + uniformName + " not found in shader");
    }
    return location;
}
    
//Enable the generic vertex attribute array specified by index i
void GLSLProgram::use()
{
    glUseProgram(_programID);
    for(int i = 0; i < _numAttributes; i++){
        glEnableVertexAttribArray(i);
    }
}

void GLSLProgram::unuse()
{
    glUseProgram(0);
    for(int i = 0; i < _numAttributes; i++){
        glDisableVertexAttribArray(i);
    }
}

//Open the file with shader source code, copy the contents to shader object, and compile the source
void GLSLProgram::compileShader(const std::string& filePath, GLuint shaderID){

//Open shader file and copy the contents to fileContents    
    std::fstream shaderFile(filePath);
    if(shaderFile.fail()){
        perror(filePath.c_str());
        fatalError("failed to open " + filePath);
    }

    std::string fileContents = "";
    std::string line;

    while(std::getline(shaderFile, line)){
        fileContents += line + "\n";
    }

    shaderFile.close();
//char* is a necessary parameter for glShaderSource to interpret shader file contents
    const char* contentsPtr = fileContents.c_str();
//Copy the fileContents (shader source) to the shader object specified by shaderID
    glShaderSource(shaderID, 1, &contentsPtr, nullptr);
//Compile the shader source code stored in the shader object
    glCompileShader(shaderID);

    GLint success = 0;
//Check for the compile status and store it in the 'success' variable
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
//If compilation failed, get the compilation log and print it to the screen
    if(success == GL_FALSE){
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        glDeleteShader(shaderID);
        std::printf("%s\n", &(errorLog[0]));
        fatalError("Shader " + filePath + " failed to compile");
    }
}
