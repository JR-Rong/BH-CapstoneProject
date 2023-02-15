/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-05 19:34:14
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 20:32:28
 * @Description: file content
 */
#include "mapping\shader.hpp"
#include "mapping\mapping.hpp"

namespace mapping {

display_map* p_map;
void mouse_down_callback(GLFWwindow* window, int button, int action, int mods){
    int width, height;
    double xpos, ypos;
    if (action == GLFW_PRESS) switch(button){
			case GLFW_MOUSE_BUTTON_LEFT:
                glfwGetCursorPos(window, &xpos, &ypos);
                p_map -> mouse_pos(xpos, ypos);
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				break;
			default:
				return;
			}
	return;
}

void key_down_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action != GLFW_PRESS) return;
    p_map -> key_action(key);
}

display_map::display_map() {}

void display_map::init() {
    p_map = this;
    m_status = MAP_SATUS::Initializing;
    
    // init window
	std::cout << glfwInit()<< std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width+m_length/2+10, m_height+m_length/2+10, "BUILD GRID MAP", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Something Wrong with window open! Quit!!" << std::endl;
        glfwTerminate();
        return ;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){glViewport(0, 0, width, height);});

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ;
    }

    // init shader
    init_shader();

    init_map();

    m_status = MAP_SATUS::Initialized;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetMouseButtonCallback(m_window, mouse_down_callback);
    glfwSetKeyCallback(m_window, key_down_callback);

}

void display_map::init_shader() {
    unsigned int VertexShader_Prime = glCreateShader(GL_VERTEX_SHADER);
    unsigned int VertexShader_Middle_Fill = glCreateShader(GL_VERTEX_SHADER);
    unsigned int FragmentShader_Prime = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int FragmentShader_Middle_Fill = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int ShaderPrograme_Prime = glCreateProgram();
    unsigned int ShaderPrograme_Middle_Fill = glCreateProgram();

    int  success;
    char infoLog[512];

    glShaderSource(VertexShader_Prime, 1, &VertexShader_Prime_Source, NULL);
    glCompileShader(VertexShader_Prime);
    glGetShaderiv(VertexShader_Prime, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(VertexShader_Prime, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::Prime::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(VertexShader_Middle_Fill, 1, &VertexShader_Middle_Fill_Source, NULL);
    glCompileShader(VertexShader_Middle_Fill);
    glGetShaderiv(VertexShader_Middle_Fill, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(VertexShader_Middle_Fill, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::Middle::Fill::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(FragmentShader_Prime, 1, &FragmentShader_Prime_Source, NULL);
    glCompileShader(FragmentShader_Prime);
    glGetShaderiv(FragmentShader_Prime, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(FragmentShader_Prime, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::Prime::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(FragmentShader_Middle_Fill, 1, &FragmentShader_Middle_Fill_Source, NULL);
    glCompileShader(FragmentShader_Middle_Fill);
    glGetShaderiv(FragmentShader_Middle_Fill, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(FragmentShader_Middle_Fill, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::Middle::Fill::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(ShaderPrograme_Prime, VertexShader_Prime);
    glAttachShader(ShaderPrograme_Prime, FragmentShader_Prime);
    glLinkProgram(ShaderPrograme_Prime);
    glGetProgramiv(ShaderPrograme_Prime, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ShaderPrograme_Prime, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::Prime::LINK_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(VertexShader_Prime);
    glDeleteShader(FragmentShader_Prime);

    glAttachShader(ShaderPrograme_Middle_Fill, VertexShader_Middle_Fill);
    glAttachShader(ShaderPrograme_Middle_Fill, FragmentShader_Middle_Fill);
    glLinkProgram(ShaderPrograme_Middle_Fill);
    glGetProgramiv(ShaderPrograme_Middle_Fill, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ShaderPrograme_Middle_Fill, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::Middle::Fill::LINK_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(VertexShader_Middle_Fill);
    glDeleteShader(FragmentShader_Middle_Fill);

    this -> m_line_program.program = ShaderPrograme_Prime;
    this -> m_line_program.color = glGetUniformLocation(this -> m_line_program.program, "vColor");
    this -> m_line_program.pos = glGetUniformLocation(this -> m_line_program.program, "vPos");
    this -> m_fill_program.program = ShaderPrograme_Middle_Fill;
    this -> m_fill_program.color = glGetUniformLocation(this -> m_fill_program.program, "vColor");
    this -> m_fill_program.pos = glGetUniformLocation(this -> m_fill_program.program, "vPos");
}

void display_map::init_map() {
    float side = m_length/2 + 10;
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    float bias = -0.9f;
    float len = 1.8*m_length;
    m_grid_bias_x = -(static_cast<float>(width) - 2 * side - static_cast<float>(m_length) / 2) / static_cast<float>(width);
    m_grid_len_x = 2 * static_cast<float>(m_length) * (static_cast<float>(width) - 2 * side) / static_cast<float>(width);
    m_grid_bias_y = -(static_cast<float>(height) - 2 * side - static_cast<float>(m_length) / 2) / static_cast<float>(height);
    m_grid_len_y = 2 * static_cast<float>(m_length) * (static_cast<float>(height) - 2 * side) / static_cast<float>(height);
    std::cout << m_grid_bias_x << " " << m_grid_bias_y << " " << m_grid_len_x << " " << m_grid_len_y << std::endl;
    glGenVertexArrays(1, &m_vao_line);
    glBindVertexArray(m_vao_line);
    glGenBuffers (1, &m_vbo_line);
    float pos[12] = { - m_grid_len_x/2/m_width,  - m_grid_len_y/2/m_height, 0,
                      + m_grid_len_x/2/m_width,  - m_grid_len_y/2/m_height, 0,
                      + m_grid_len_x/2/m_width,  + m_grid_len_y/2/m_height, 0,
                      - m_grid_len_x/2/m_width,  + m_grid_len_y/2/m_height, 0
                      };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_vao_fill);
    glBindVertexArray(m_vao_fill);
    glGenBuffers (1, &m_vbo_fill);
    float pos_fill[12] = { - m_grid_len_x/2/m_width,  - m_grid_len_y/2/m_height, 0,
                      + m_grid_len_x/2/m_width,  - m_grid_len_y/2/m_height, 0,
                      - m_grid_len_x/2/m_width,  + m_grid_len_y/2/m_height, 0,
                      + m_grid_len_x/2/m_width,  + m_grid_len_y/2/m_height, 0
                      };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fill);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_fill), pos_fill, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void display_map::display(){
    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.8274f, 0.8706f, 0.9137f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i=0; i<m_map.size();i++){
            for (size_t j =0;j < m_map[i].size();j++){
                glm::mat4 trans;
                trans = glm::translate(trans, glm::vec3(i*m_grid_len_x/m_width + m_grid_bias_x, j*m_grid_len_y/m_height + m_grid_bias_y, 0.0f));
                glUseProgram(m_fill_program.program);
                glBindVertexArray(m_vao_fill);
                glUniformMatrix4fv(this -> m_fill_program.pos, 1, GL_FALSE, glm::value_ptr(trans));
                if (m_map[i][j].status == UnCleanedRoad) {
                    glUniform4f(m_fill_program.color, 0.9, 0.9, 0.2, 0.9);
                } else if (m_map[i][j].status == Obstacle) {
                    glUniform4f(m_fill_program.color, 0.2, 0.2, 0.2, 0.9);
                } else {
                    glUniform4f(m_fill_program.color, 0.2, 0.9, 0.2, 0.9);
                }

                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                
                glUseProgram(m_line_program.program);
                glBindVertexArray(m_vao_line);
                glUniformMatrix4fv(this -> m_line_program.pos, 1, GL_FALSE, glm::value_ptr(trans));
                glUniform4f(m_line_program.color, 1, 0.0, 0.0, 1);
                glPointSize(5.0f);
                glDrawArrays(GL_LINE_LOOP, 0, 4);
            }
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void display_map::mouse_pos(double xpos, double ypos){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    xpos = 2 * xpos / width - 1;
    ypos = 2 * ypos / height - 1;
    ypos *= -1;
    for (int i=0; i<m_map.size();i++){
        for(int j=0;j<m_map[i].size();j++){
            if(m_map[i][j].vertex[0] < xpos &&  m_map[i][j].vertex[1] < ypos && m_map[i][j].vertex[6] > xpos && m_map[i][j].vertex[7] > ypos) {
                std::cout << "xpos is:" << xpos << " and ypos is:" << ypos 
                << " vertex[0] is" << m_map[i][j].vertex[0] 
                << " vertex[1] is" << m_map[i][j].vertex[1] 
                << " vertex[6] is" << m_map[i][j].vertex[6]
                << " vertex[7] is" << m_map[i][j].vertex[7]  << std::endl;
                int* pos = new int[2]{i, j};
                POINT_STATUS* act = new POINT_STATUS[2]{Obstacle, UnCleanedRoad};
                m_action.push_back(std::make_pair(pos, act));
                m_map[i][j].status = UnCleanedRoad;
                break;
            }
        }
    }
    if(m_action.size() > 1 && m_action.size() % 2 == 0) {
        change_line(std::make_pair(m_action[m_action.size()-2].first[0], m_action[m_action.size()-2].first[1]),
                    std::make_pair(m_action[m_action.size()-1].first[0], m_action[m_action.size()-1].first[1]),
                    UnCleanedRoad);
    }
}

void display_map::key_action(int key){
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        std::cout << "ShutDown Window!" << std::endl;
        glfwSetWindowShouldClose(m_window, GL_TRUE);
        break;
    case GLFW_KEY_BACKSPACE:
        {
            if(m_action.size() == 0 || m_action.size() % 2 != 0) break;
            change_line(std::make_pair(m_action[m_action.size()-2].first[0], m_action[m_action.size()-2].first[1]),
                                        std::make_pair(m_action[m_action.size()-1].first[0], m_action[m_action.size()-1].first[1]),
                                        Obstacle);
            auto action = m_action.back();
            m_map[action.first[0]][action.first[1]].status = action.second[0];
            m_action.pop_back();
            action = m_action.back();
            m_map[action.first[0]][action.first[1]].status = action.second[0];
            m_action.pop_back();
        }
        break;
    default:
        break;
    }
}

void display_map::change_line(std::pair<int, int> start_point, std::pair<int, int> end_point, POINT_STATUS status){

    if(abs(end_point.first - start_point.first) > abs(end_point.second - start_point.second)) {

        if (end_point.second -  start_point.second == 0) {
            int x_forward = (end_point.first -  start_point.first) / abs(end_point.first -  start_point.first);
            for(int i=1;i<=abs(end_point.first -  start_point.first); i++){
               m_map[start_point.first + x_forward * i][start_point.second].status = status;
               x_forward > 0 ? 
                m_map[start_point.first + x_forward * i][start_point.second].orientation = Right : 
                m_map[start_point.first + x_forward * i][start_point.second].orientation = Left;
            }
            return ;
        }

        int times = abs(end_point.first -  start_point.first + 1) / abs(end_point.second -  start_point.second + 1);
        int x_forward = (end_point.first -  start_point.first + 1) / abs(end_point.first -  start_point.first + 1);
        int y_forward = (end_point.second -  start_point.second + 1) / abs(end_point.second -  start_point.second + 1);
        int x_cur, y_cur;

        for(int i=0;i<abs(end_point.second -  start_point.second); i++) {
            for(int j=1;j<=times; j++) {
                m_map[start_point.first + x_forward * i + x_forward * j][start_point.second + y_forward * i].status = status;
                x_forward > 0 ? 
                    m_map[start_point.first + x_forward * i + x_forward * j][start_point.second + y_forward * i].orientation = Right : 
                    m_map[start_point.first + x_forward * i + x_forward * j][start_point.second + y_forward * i].orientation = Left;
            }
            m_map[start_point.first + x_forward * (i+1) * times][start_point.second + y_forward * (i + 1)].status = status;
            x_cur = start_point.first + x_forward * (i+1) * times;
            y_cur = start_point.second + y_forward * (i + 1);
            y_forward > 0 ? m_map[x_cur][y_cur].orientation = Up : m_map[x_cur][y_cur].orientation = Down;
        }

        for(int i=0;i<=abs(end_point.first -  start_point.first + 1) % abs(end_point.second -  start_point.second + 1); i++){
            m_map[x_forward * i + x_cur][y_cur].status = status;
            x_forward > 0 ? 
                m_map[x_forward * i + x_cur][y_cur].orientation = Right : 
                m_map[x_forward * i + x_cur][y_cur].orientation = Left;
        }

    } else {

        if (abs(end_point.first -  start_point.first) == 0) {
            int y_forward = (end_point.second -  start_point.second) / abs(end_point.second -  start_point.second);
            for(int i=1;i<=abs(end_point.second -  start_point.second); i++){
                m_map[start_point.first][start_point.second + y_forward * i].status = status;
                y_forward > 0 ? 
                    m_map[start_point.first][start_point.second + y_forward * i].orientation = Up: 
                    m_map[start_point.first][start_point.second + y_forward * i].orientation = Down;
            }
            return ;
        }

        int times = abs(end_point.second -  start_point.second) / abs(end_point.first -  start_point.first);
        int x_forward = (end_point.first -  start_point.first) / abs(end_point.first -  start_point.first);
        int y_forward = (end_point.second -  start_point.second) / abs(end_point.second -  start_point.second);
        int x_cur, y_cur;

        for(int i=0;i<abs(end_point.first -  start_point.first); i++) {
            for(int j=1;j<=times; j++) {
                m_map[start_point.first + x_forward * i][start_point.second + y_forward * i + y_forward * j].status = status;
                y_forward > 0 ? 
                    m_map[start_point.first + x_forward * i][start_point.second + y_forward * i + y_forward * j].orientation = Up : 
                    m_map[start_point.first + x_forward * i][start_point.second + y_forward * i + y_forward * j].orientation = Down;
            }
            m_map[start_point.first + x_forward * (i + 1)][start_point.second + y_forward * (i+1) * times].status = status;
            x_cur = start_point.first + x_forward * (i + 1);
            y_cur = start_point.second + y_forward * (i+1) * times;
            x_forward > 0 ? m_map[x_cur][y_cur].orientation = Right : m_map[x_cur][y_cur].orientation = Left;
            
        }

        for(int i=1;i<=abs(end_point.second -  start_point.second) % abs(end_point.first -  start_point.first); i++){
            m_map[x_cur][y_forward * i + y_cur].status = status;
        }

    }
}

std::vector<std::vector<MAP_POINT>> display_map::get_map() const {
    return m_map;
}

int display_map::get_width() const {
    return m_width;
}

int display_map::get_height() const {
    return m_height;
}

int display_map::get_length() const {
    return m_length;
}

void display_map::set_width(int width) {
    this -> m_width = width;
}

void display_map::set_height(int height) {
    this -> m_height = height;
} 

void display_map::set_length(int length) {
    this -> m_length = length;
}

void display_map::set_map(std::vector<std::vector<MAP_POINT>> map) {
    this -> m_map = map;
}

void display_map::input_size(){
    unsigned int width; 
    unsigned int height;
    unsigned int len;
    std::cout << "please input map width:";
    std::cin >> width;
    std::cout << "please input map height:";
    std::cin >> height;
    std::cout << "please input map grid length:";
    std::cin >> len;
    
    this -> m_height = height;
    this -> m_width = width;
    this -> m_length = len;
}

void display_map::gen_map(){
    float pos[12] = { - m_grid_len_x/2/m_width,  - m_grid_len_y/2/m_height, 0,
                      + m_grid_len_x/2/m_width,  - m_grid_len_y/2/m_height, 0,
                      + m_grid_len_x/2/m_width,  + m_grid_len_y/2/m_height, 0,
                      - m_grid_len_x/2/m_width,  + m_grid_len_y/2/m_height, 0
                      };
    for(int i = 0; i < m_width/m_length; i++) {
        std::vector<MAP_POINT> col;
        for(int j = 0; j < m_height/m_length; j++) {
            std::vector<float> pos_ = {i*m_grid_len_x/m_width + m_grid_bias_x + pos[0], j*m_grid_len_y/m_height + m_grid_bias_y + pos[1], 0,
                              i*m_grid_len_x/m_width + m_grid_bias_x + pos[3], j*m_grid_len_y/m_height + m_grid_bias_y + pos[4], 0,
                              i*m_grid_len_x/m_width + m_grid_bias_x + pos[6], j*m_grid_len_y/m_height + m_grid_bias_y + pos[7], 0,
                              i*m_grid_len_x/m_width + m_grid_bias_x + pos[9], j*m_grid_len_y/m_height + m_grid_bias_y + pos[10], 0};

            MAP_POINT point{i, j, POINT_STATUS::Obstacle, POINT_ORIENTATION::None, pos_};
            col.push_back(point);

        }
        this -> m_map.push_back(col);
    }
}

} // namepace mapping
