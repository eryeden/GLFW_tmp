#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "World.hpp"


using namespace glm;

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


//微分値の出力
double f(double x, double w, double a){
    return w - a * cos(x);
}

//RK4
double rk4(double x, double t, double dt, double param[]){
    double k1, k2, k3, k4;
    double w = param[0];
    double a = param[1];
    k1 = dt * f(x, w, a);
    k2 = dt * f(x + k1 * 0.5, w, a);
    k3 = dt * f(x + k2 * 0.5, w, a);
    k4 = dt * f(x + k3,       w, a);
    return (k1 + 2.0*k2 + 2.0*k3 + k4)/6.0 + x;
}

class COsci{
public:


    double Update(const COsci & osci_){
        double k1, k2, k3, k4;
        k1 = dt * f(osci_.phi)

    };


private:

    double f(double phi_);
    double q(double df){return ep * sin(df);};
    double ep;
    double w;
    double dt;
    double phi;
};

/*
 * 悩み：相互作用項をどうするか
 * システム：phi_dot = omega + eps * q( phi_i - phi_(i+1) )
 */
double x_dot(double w_, double eps_,double x_, double xx_){
    return (w_ + eps_ * sin(xx_ - x_));
}

void RK4(double x[], double x_out[], double param[], double dt){
    double w1, w2, eps;
    w1 = param[0];
    w2 = param[1];
    eps = param[2];

    double k1[2], k2[2], k3[2], k4[2];
    k1[0] = dt * x_dot(w1, eps, x[0], x[1]);
    k1[1] = dt * x_dot(w2, eps, x[1], x[0]);

    k2[0] = dt * x_dot(w1, eps, x[0] + k1[0] * 0.5, x[1] + k1[1] * 0.5);
    k2[1] = dt * x_dot(w1, eps, x[1] + k1[1] * 0.5, x[0] + k1[0] * 0.5);

    k3[0] = dt * x_dot(w1, eps, x[0] + k2[0] * 0.5, x[1] + k2[1] * 0.5);
    k3[1] = dt * x_dot(w1, eps, x[1] + k2[1] * 0.5, x[0] + k2[0] * 0.5);

    k4[0] = dt * x_dot(w1, eps, x[0] + k3[0], x[1] + k1[1]);
    k4[1] = dt * x_dot(w1, eps, x[1] + k3[1], x[0] + k1[0]);

    x_out[0] = (k1[0] + k2[0] + k3[0] + k4[0])/6.0 + x[0];

}

int main()
{
    world::Window wd(800, 600, "Test");
    wd.EnableFrameCapturing();
    wd.SetVisibleArea(5);
    wd.SetCenterPoint(vec2(0, 0));

    //単位円
    int no_p_uc = 100;
    std::vector<vec2> unit_circle;
    unit_circle.resize(no_p_uc + 2);
    for(int i = 0; i < (no_p_uc + 2); ++i){
        unit_circle[i].x = cos(2.0 * M_PI / (double)no_p_uc * (double)i);
        unit_circle[i].y = sin(2.0 * M_PI / (double)no_p_uc * (double)i);
    }
    world::Line line_uc(unit_circle);

    //w_aの線
    std::vector<vec2> w_a;
    w_a.push_back(vec2(0, 10));
    w_a.push_back(vec2(0, -10));
    world::Line line_w_a(w_a);


    world::Circle ccl(0.2, 50);

//    world::Text tu(wd);
//    world::Text tu(wd, world::Constants::DEFAULT_FONT_PATH_PREFIX + "ProggyClean.ttf");
    world::Text tu(wd, world::Constants::DEFAULT_FONT_PATH_PREFIX + "Inconsolata-Regular.ttf");


    double fps = 0;
    std::string buff_fps = "0";
    char buff[128];


    double phi[2];

    double dt = 1.0/60.0;
    double t = 0;

    while (wd.IsClose()) {
        wd.HandleEvent();

        wd.ClearColor(0.7f, 0.7f, 0.7f);

        //単位円の描画
        wd.Draw(line_uc, vec2(0, 0), vec3(0, 0, 0), 0.003);

        //円の描画
        wd.Draw(ccl, vec2(cos(phi), sin(phi)), vec3(0.8f, 0.2f, 0.2f));


        sprintf(buff, "FPS:%3.2f", fps);
        buff_fps = buff;
        tu.RenderText(buff_fps, glm::vec2(10, 10), glm::vec3(0.4f, 0.2f, 0.1f), 0.5);

        sprintf(buff, "w = %2.2f, a = %2.2f, phi:%3.3f [rad]", w, a, phi);
        tu.RenderText(std::string(buff), glm::vec2(10, 50), glm::vec3(0.4f, 0.2f, 0.1f), 0.5);

        sprintf(buff, "time:%4.3f [s]", t);
        tu.RenderText(std::string(buff), glm::vec2(10, 90), glm::vec3(0.4f, 0.2f, 0.1f), 0.5);


        t += dt;
        phi = rk4(phi, t, dt, param);

        fps = update_fps_counter(const_cast<GLFWwindow *>(wd.GetWindowContext()));
        wd.SwapBuffers();
    }

}

