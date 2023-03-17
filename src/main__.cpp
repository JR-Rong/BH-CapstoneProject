/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-22 16:05:54
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-03-02 20:34:13
 * @Description: file content
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <filesystem>

#include "json/json.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main__(){
    int imgWidth, imgHeight, channelCount;
    std::string cur_path = "";
    #ifdef PROJECT_CURRENT_DIR
    cur_path = PROJECT_CURRENT_DIR;
    #endif
    std::string file_name = cur_path + "Textures/selfcar.png";
    unsigned char *self_car_data = stbi_load(file_name.c_str(), &imgWidth, &imgHeight, &channelCount, STBI_rgb_alpha);

    std::cout << sizeof(self_car_data)/sizeof(unsigned char) << " " << sizeof(unsigned char) << std::endl;

    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    // 格式化当前时间为字符串，作为文件名
    std::ostringstream filename_stream;
    filename_stream << cur_path << "map_build\\selfcar_texture" << ".inl";
    std::string filename = filename_stream.str();

    // 创建并写入文件
    std::ofstream outfile(filename);
    outfile << "std::vector<int> tex = {";
    for(int i=0; i<sizeof(self_car_data)/sizeof(unsigned char); i++){
        outfile << static_cast<int>(self_car_data[i]) << ", ";
    }
    outfile << "};\n int imgWidth=" ;
    outfile << imgWidth << "; \n int imgHeight = " << imgHeight << "; \n int channelCount = " << channelCount << ";";
    outfile.close();
    std::cout << "finish" << std::endl;

    return 0;
}