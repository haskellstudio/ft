//
//  main.cpp
//  ttf2Svg
//
//  Created by 朱飞 on 10/02/2018.
//  Copyright © 2018 soho. All rights reserved.
//

#include <iostream>
#include "Header.h"
#include <sstream>
#include <fstream>
void genSvg(std::string name, std::string charCode) {
    font2svg::glyph g("/Users/zhufei/dev/ttf2Svg/arial.ttf", charCode);
    std::string fname = std::string("/Users/zhufei/dev/ttf2Svg/");
    fname += name;
    fname += ".svg";
    std::ofstream file( fname.c_str());
    file << g.svgheader()
    << g.svgborder()
    << g.svgtransform()
    << g.axes()           // 左边和下面的坐标线
    << g.typography_box() // 上边和右边的虚线
    << g.points()         // point 圆点
    << g.pointlines()     //各个point之间的连线
    << g.outline()        // 画 整个字
    << g.labelpts()       // point的坐标文字
    << g.svgfooter();
    g.free();
    file.close();
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    genSvg("g", "103"); // g 103  A 65
    return 0;
    font2svg::glyph g( "/Users/zhufei/dev/ttf2Svg/arial.ttf", "65" );
    std::cout << g.svgheader()
    << g.svgborder()
    << g.svgtransform()
    << g.axes()
    << g.typography_box()
    << g.points()
    << g.pointlines()
    << g.outline()
    << g.labelpts()
    << g.svgfooter();
    
    g.free();
    
    
    return 0;
}
