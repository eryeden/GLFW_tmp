#include <iostream>
#include <vector>
#include <map>
#include <string>

// Include GLEW
#ifdef __APPLE__ //For OSX
#define _CRT_SECURE_NO_WARNINGS
#define GL_GLEXT_PROTOTYPES
#include <gl/glew.h>
#endif

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Free type
#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hpp"
#include "World.hpp"


/*
 * 用語集
 *
 * vertex attribute
 * : OpenGLにおいて一つの頂点情報は、３Dであるから、３つの要素で成分表示される。この３つの要素の集まり（便宜上ベクトルと呼ぶ）を vertex attribute と呼ぶ
 * : vertex shader はこれ（vertex attribute）を受け取り、これに対して座標系変換、その他変換を行う
 *
 * vertex buffer object
 * : vertex attribute は一つの頂点情報を示すが、この実体は多くの場合、３つのfloatからなる。これはビデオカード上のメモリに保存されている必要があり、頂点情報は複数個からなるため、このまとまりを管理する必要がある。
 * : これら頂点情報群の実体を示し、転送、記録等の管理を行いやすくしまとめているものが、vertex buffer objectである
 *
 * vertex array object
 * : VBO(vertex buffer object)はvertex attributes の実体を保持するが、その実体がどのようにパッキングされVBO中に保存されているかの情報をOpenGLに与える必要がある
 * : その再、glVertexAttribPointer関数を用いて、その設定を行う。これにより、OpenGLは頂点情報を取得できるようになる。しかし、このパッキング情報を描画時に毎回設定するのは効率的でないため、
 * : これらパッキング設定情報を保存するようにする。これがvertex array objectである。
 *
 */

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;


void RenderText(GLuint shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    glUseProgram(shader);
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(){
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

#ifdef __APPLE__
    //最大バージョンは4.1だった @ Yosemite
    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "Hello Text Rendering", nullptr, nullptr);
    if( window == nullptr ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    //このウィンドウを対象に描画を行う
    glfwMakeContextCurrent(window);

#ifdef __APPLE__
    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
#endif

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //Enable Depth test
    glEnable(GL_DEPTH_TEST);
    //Depth test configuration
    glDepthFunc(GL_LESS);

    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load shader with Geometry shader
    const std::string shader_prefix = SHADER_PREFIX; //SHADER_PREFIX is defined by cmake
    const std::string vertex_shader = "Text_vs.glsl";
    const std::string fragment_shader = "Text_fs.glsl";
    GLuint text_program_id = LoadShaders(
            (shader_prefix + vertex_shader).c_str()
            , (shader_prefix + fragment_shader).c_str()
    );
    //Use shader program
    glUseProgram(text_program_id);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
    glUniformMatrix4fv(glGetUniformLocation(text_program_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    //Initialize Free Type
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    std::string font_dir_prefix = FONTS_DIR_PREFIX; //Defined in CMakeLists.txt
    std::string font_path = "arial.ttf";
    FT_Face face;
    if (FT_New_Face(ft, (font_dir_prefix + font_path).c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int frame_width, frame_height;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check and call events
        glfwPollEvents();


        glfwGetFramebufferSize(window, &frame_width, &frame_height);
        projection = glm::ortho(0.0f, static_cast<GLfloat>(frame_width), 0.0f, static_cast<GLfloat>(frame_height));
        glUniformMatrix4fv(glGetUniformLocation(text_program_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderText(text_program_id, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(text_program_id, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;

}


/*
 * 急急如律令
 */