/*
 * @Author: jianrui-rong rongjianrui@gmail.com
 * @Date: 2023-02-14 09:53:58
 * @LastEditors: jianrui-rong
 * @LastEditTime: 2023-02-14 15:24:19
 * @Description: file conten
 */

#ifndef STORE_MAPPING_HPP_
#define MAPSTORE_MAPPING_HPP_PING_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <filesystem>

#include "json\json.h"

#include "mapping\mapping.hpp"

namespace mapping {

struct MAP_POINT;
class display_map;

void get_time();

void store_map(display_map map);

void read_map(std::string file_name, display_map& map);

bool choose_build_map_way();

} // mapping

#endif