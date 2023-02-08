/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-05 14:30:17
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-06 12:51:06
 * @Description: file content
 */

#include <vector>
#include <glad\glad.h>
#include <glfw\glfw3.h>

#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

namespace mapping{

enum POINT_STATUS {
    Obstacle,
    UnCleanedRoad,
    CleanedRoad,
};

enum POINT_ORIENTATION {
    None,
    Up,
    UpRight,
    Right,
    RightDown,
    Down,
    DownLeft,
    Left,
};

struct PROGRAM_PARAM {
    unsigned int program;
    unsigned int color;
    unsigned int pos;
};

struct MAP_POINT {
    int x;
    int y;
    POINT_STATUS status;
    POINT_ORIENTATION orientation;
    std::vector<float> car_pass_time;

    float* vertex;

    unsigned int vbo_line;
    unsigned int vbo_fill;

    MAP_POINT(int x, int y, POINT_STATUS status, POINT_ORIENTATION orientation, float* vertex) :
        x(x),
        y(y),
        status(status),
        orientation(orientation),
        vertex(vertex)
        {}
};

class display_map {
private:
    float m_grid_bias;
    float m_grid_len;

    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_length;
    GLFWwindow* m_window;
    PROGRAM_PARAM m_line_program;
    PROGRAM_PARAM m_fill_program;
    std::vector<std::vector<MAP_POINT>> m_map;
    unsigned int m_vbo_line;
    unsigned int m_vao_line;
    unsigned int m_vbo_fill;
    unsigned int m_vao_fill;
private:
    void init_shader();
    void processInput(GLFWwindow *window);
    void init_map();
    void display();

public:
    display_map();
    void init();

};

}
