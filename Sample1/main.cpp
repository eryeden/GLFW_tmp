#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "World.hpp"


double update_fps_counter(GLFWwindow * _window) {
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    static double fps = 0;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "@ fps: %.2f", fps);
        glfwSetWindowTitle(_window, tmp);
        frame_count = 0;
    }
    frame_count++;
    return fps;
}

int main()
{
    world::Window wd(800, 600, "Test");
    wd.EnableFrameCapturing();
    wd.DisableFrameCapturing();

    world::Circle ccl(0.1, 50);

//    world::Text tu(wd);
//    world::Text tu(wd, world::Constants::DEFAULT_FONT_PATH_PREFIX + "ProggyClean.ttf");
    world::Text tu(wd, world::Constants::DEFAULT_FONT_PATH_PREFIX + "Inconsolata-Regular.ttf");


    double fps = 0;
    std::string buff_fps = "0";
    char buff[128];

    long i = 0;

    while (wd.IsClose()) {
        wd.HandleEvent();

        wd.ClearColor(0.2f, 0.3f, 0.3f);


        wd.Draw(ccl, glm::vec2(0, 0), glm::vec3(0.5, 0.8f, 0.2f));
        sprintf(buff, "%3.2f &%%", fps);
        buff_fps = buff;
        tu.RenderText(buff_fps, glm::vec2(10, 10), glm::vec3(0.5, 0.8f, 0.2f), 0.5);


        fps = update_fps_counter(const_cast<GLFWwindow *>(wd.GetWindowContext()));
        wd.SwapBuffers();
    }

}





/*
 * 急急如律令
 */