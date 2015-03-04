#include "mainScene.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glh;
using std::cout;
using std::endl;

static GLfloat animate = 0.0f;
ShaderUniform*uniformWorld = NULL;

static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
};
static const GLuint g_index_buffer_data[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2,
};

void MainScene::Init()
{
    std::string vendor = Misc::GetVendor();
    std::string renderer = Misc::GetRenderer();
    std::string version = Misc::GetVersion();
    std::string shaderversion = Misc::GetShadingLanguageVersion();
    cout << vendor << endl << renderer << endl << version << endl << shaderversion << endl;

    mVertexBuffer = new Buffer();
    mVertexBuffer->Data(Buffer::ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, glh::STATIC_DRAW);

    mIndexBuffer = new Buffer();
    mIndexBuffer->Data(Buffer::ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, glh::STATIC_DRAW);

    Shader* vert = new Shader(Shader::VERTEX_SHADER);
    vert->SourceFromFile("W:\\GLHeart\\resources\\simple.vert");
    vert->Compile();
    if (!vert->IsShaderCompiledSuccessful())
    {
        cout << vert->GetInfoLog() << endl;
    }

    Shader* frag = new Shader(Shader::FRAGMENT_SHADER);
    frag->SourceFromFile("W:\\GLHeart\\resources\\simple.frag");
    frag->Compile();
    if (!frag->IsShaderCompiledSuccessful())
    {
        cout << frag->GetInfoLog() << endl;
    }

    mProgram = new ShaderProgram();
    mProgram->AttachShader(vert);
    mProgram->AttachShader(frag);
    mProgram->Link();

    delete vert;
    delete frag;

    uniformWorld = mProgram->GetUniform("gWorld");
}

void MainScene::Update()
{
    animate += 0.001f;
}

void MainScene::Draw() {
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    mProgram->Use();

    glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(sinf(animate) * 0.2f, 0, 0));
    glm::mat4 rotate = glm::toMat4(    glm::quat(glm::vec3(animate, animate * 1.5f, animate * 2)));
    GLfloat s = sinf(animate * 2) * 0.4f + 0.6f;
    glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(s, s, s));
    uniformWorld->SetValue(translate * rotate * scale);

    glEnableVertexAttribArray(0);

    mVertexBuffer->Bind(Buffer::ARRAY_BUFFER);
    mIndexBuffer->Bind(Buffer::ELEMENT_ARRAY_BUFFER);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
}