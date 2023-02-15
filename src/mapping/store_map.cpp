/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-14 09:55:33
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 20:18:21
 * @Description: file content
 */
#include "mapping\store_map.hpp"

namespace mapping {

void get_time() {
    std::time_t now = std::time(nullptr);

    // 将当前时间转换为本地时间
    std::tm* local_time = std::localtime(&now);

    // 格式化时间为字符串
    const int MAX_LEN = 100;  // 设置缓冲区大小为 100 字节
    char time_str[MAX_LEN];
    std::strftime(time_str, MAX_LEN, "%Y-%m-%d %H:%M:%S", local_time);

    // 将时间字符串转换为 std::string 类型
    std::string str(time_str);

    // 打印时间字符串和长度
    std::cout << str << std::endl;
    std::cout << "string length: " << str.length() << std::endl;
}

void store_map(display_map map) {
    std::string cur_path = "";
    #ifdef PROJECT_CURRENT_DIR
    cur_path = PROJECT_CURRENT_DIR;
    #endif
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    // 格式化当前时间为字符串，作为文件名
    std::ostringstream filename_stream;
    filename_stream << cur_path << "map_build\\map_" << std::put_time(local_time, "%Y-%m-%d_%H-%M-%S") << ".json";
    std::string filename = filename_stream.str();

    // 创建并写入文件
    std::ofstream outfile(filename);
    outfile << "{\"type\":" << 0 
        << ",\"map_width\":" << map.get_width() << ",\"map_height\":" << map.get_height()
        << ",\"map_length\":" << map.get_length() << "}" << std::endl;
    for(auto row : map.get_map()){
        for (auto point : row){
            outfile << point;
        }
    }
    outfile.close();
}

void read_map(std::string file_name, display_map& map) {
    std::vector<std::vector<MAP_POINT>> map_;
    std::vector<MAP_POINT> row_;
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::for_each(std::istream_iterator<std::string>(file),
                      std::istream_iterator<std::string>(),
                      [&map, &row_, &map_](const std::string& line){ 
                        Json::Reader reader;
                        Json::Value obj;
                        reader.parse(line, obj);
                        
                        if(obj["type"].asInt() == 0) {
                            map.set_height(obj["map_height"].asInt());
                            map.set_width(obj["map_width"].asInt());
                            map.set_length(obj["map_length"].asInt());
                        } else if(obj["type"] == 1){
                            MAP_POINT point{obj["x"].asInt(), obj["y"].asInt(), static_cast<POINT_STATUS>(obj["status"].asInt()), static_cast<POINT_ORIENTATION>(obj["orientation"].asInt()), 
                            {obj["vertex"][0].asFloat(), obj["vertex"][1].asFloat(), obj["vertex"][2].asFloat(), 
                             obj["vertex"][3].asFloat(), obj["vertex"][4].asFloat(), obj["vertex"][5].asFloat(), 
                             obj["vertex"][6].asFloat(), obj["vertex"][7].asFloat(), obj["vertex"][8].asFloat(), 
                             obj["vertex"][9].asFloat(), obj["vertex"][10].asFloat(), obj["vertex"][11].asFloat(), }};
                            row_.push_back(point);
                            if(obj["y"].asInt() + 1 >= map.get_height() / map.get_length()){
                                map_.push_back(row_);
                                row_.clear();
                            }
                        }
                      });
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    map.set_map(map_);
}

bool choose_build_map_way() {
    char flag;
    std::cout << "choose to build map(b) or use input map(i):";
    std::cin >> flag;
    if(flag == 'b' || flag == 'B') return true;
    else return false;
}

} // mapping