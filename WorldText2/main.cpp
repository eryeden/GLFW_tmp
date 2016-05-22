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


int main(){

    world::Window wd(640, 480, "Hello Text Rendering");

    world::Text txt;



    while(wd.IsClose()){
        wd.HandleEvent();

        wd.ClearColor(0.2f, 0.3f, 0.3f);
        wd.Draw(txt, glm::vec2(25.0f, 25.0f), glm::vec3(0.5, 0.8f, 0.2f), "HelloText", 1.0);


        wd.SwapBuffers();

    }

}


/*
 * 急急如律令
 */