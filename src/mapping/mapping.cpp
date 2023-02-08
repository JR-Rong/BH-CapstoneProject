/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-05 19:34:14
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-06 13:01:45
 * @Description: file content
 */
#include "mapping\shader.hpp"
#include "mapping\mapping.hpp"

namespace mapping {

display_map::display_map() {}

void display_map::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void display_map::init() {
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
    // init window
	std::cout << glfwInit()<< std::endl;
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(width+400, height+200, "BUILD GRID MAP", NULL, NULL);
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

    // init map
    init_map();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    display();

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
    float bias = -0.9f;
    float len = 1.8*m_length;
    m_grid_bias = bias;
    m_grid_len = len;
    glGenVertexArrays(1, &m_vao_line);
    glBindVertexArray(m_vao_line);
    glGenBuffers (1, &m_vbo_line);
    float pos[12] = { - m_grid_len/2/m_width,  - m_grid_len/2/m_height, 0,
                      + m_grid_len/2/m_width,  - m_grid_len/2/m_height, 0,
                      + m_grid_len/2/m_width,  + m_grid_len/2/m_height, 0,
                      - m_grid_len/2/m_width,  + m_grid_len/2/m_height, 0
                      };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_vao_fill);
    glBindVertexArray(m_vao_fill);
    glGenBuffers (1, &m_vbo_fill);
    float pos_fill[12] = { - m_grid_len/2/m_width,  - m_grid_len/2/m_height, 0,
                      + m_grid_len/2/m_width,  - m_grid_len/2/m_height, 0,
                      - m_grid_len/2/m_width,  + m_grid_len/2/m_height, 0,
                      + m_grid_len/2/m_width,  + m_grid_len/2/m_height, 0
                      };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fill);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_fill), pos_fill, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for(int i = 0; i < m_width/m_length; i++) {
        std::vector<MAP_POINT> col;
        for(int j = 0; j < m_height/m_length; j++) {
            float pos[12] = {i*len/m_width + bias - len/2/m_width, j*len + bias - len/2/m_height, 0,
                            i*len/m_width + bias + len/2/m_width, j*len + bias - len/2/m_height, 0,
                            i*len/m_width + bias + len/2/m_width, j*len + bias + len/2/m_height, 0,
                            i*len/m_width + bias - len/2/m_width, j*len + bias + len/2/m_height, 0};
            MAP_POINT point{i, j, POINT_STATUS::Obstacle, POINT_ORIENTATION::None, pos};
            col.push_back(point);

        }
        this -> m_map.push_back(col);
    }
}

void display_map::display(){
    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);

        glClearColor(0.8274f, 0.8706f, 0.9137f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i=0; i<m_map.size();i++){
            for (size_t j =0;j < m_map[i].size();j++){
                glm::mat4 trans;
                trans = glm::translate(trans, glm::vec3(i*m_grid_len/m_width + m_grid_bias, j*m_grid_len/m_height + m_grid_bias, 0.0f));
                glUseProgram(m_fill_program.program);
                glBindVertexArray(m_vao_fill);
                glUniformMatrix4fv(this -> m_fill_program.pos, 1, GL_FALSE, glm::value_ptr(trans));
                glUniform4f(m_line_program.color, 0.2, 0.9, 0.2, 0.9);
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

} // namepace mapping
