/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-05 19:34:58
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-06 12:16:17
 * @Description: file content
 */

const char *VertexShader_Prime_Source = "#version 430 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 vPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vPos * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *VertexShader_Middle_Fill_Source = "#version 430 core\n"
    "layout (location = 3) in vec3 aPos;\n"
    "uniform mat4 vPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vPos * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


const char *FragmentShader_Prime_Source = "#version 430 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vColor;\n"
    "}\n\0";

const char *FragmentShader_Middle_Fill_Source = "#version 430 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vColor;\n"
    "}\n\0";