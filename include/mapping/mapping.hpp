/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-05 14:30:17
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 16:46:44
 * @Description: file content
 */

#ifndef MAPPING_HPP_
#define MAPPING_HPP_

#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "mapping/store_map.hpp"

namespace mapping{

enum MAP_SATUS {
    UnInit,
    Initializing,
    Initialized,
    Culculating,
    Finished,
};

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

    std::vector<float> vertex;
    std::vector<float> screen_pos;

    unsigned int vbo_line;
    unsigned int vbo_fill;

    MAP_POINT(int x, int y, POINT_STATUS status, POINT_ORIENTATION orientation, std::vector<float> vertex) :
        x(x),
        y(y),
        status(status),
        orientation(orientation),
        vertex(vertex)
        {}

    friend std::ostream& operator<<(std::ostream& os, const MAP_POINT& obj) {
        os << "{\"type\":" << 1
            << ",\"x\":" << obj.x << ",\"y\":" << obj.y
            << ",\"status\":" << obj.status 
            << ",\"orientation\":" << obj.orientation
            <<  ",\"vertex\":[" << obj.vertex[0] << "," << obj.vertex[1] << ","<< obj.vertex[2] << ","
            << obj.vertex[3] << ","<< obj.vertex[4] << ","<< obj.vertex[5] << ","
            << obj.vertex[6] << ","<< obj.vertex[7] << ","<< obj.vertex[8] << ","
            << obj.vertex[9] << ","<< obj.vertex[10] << ","<< obj.vertex[11] << ","<< "]}" <<std::endl;
        return os;
    }
};

class display_map {
private:
    MAP_SATUS m_status = MAP_SATUS::UnInit;

    float m_grid_bias_x;
    float m_grid_len_x;
    float m_grid_bias_y;
    float m_grid_len_y;

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

    std::vector<std::pair<int*, POINT_STATUS*>> m_action;

    float* m_pos;

private:
    void init_shader();
    void init_map();
    void change_line(std::pair<int, int> start_point, std::pair<int, int> end_point, POINT_STATUS status);
    
    
public:

public:
    display_map();
    void init();
    void display();
    void mouse_pos(double xpos, double ypos);
    void key_action(int key);
    void input_size();

    void gen_map();

    std::vector<std::vector<MAP_POINT>> get_map() const;
    int get_width() const;
    int get_height() const;
    int get_length() const;

    void set_width(int width);
    void set_height(int height);
    void set_length(int length);
    void set_map(std::vector<std::vector<MAP_POINT>> map);
};

}

#endif