// 2018120621:58

#pragma once
#include <list>

struct split_area
{
    //区域范围
    RECT rect;
    //优先级
    int level;

};

class split_area_manage
{
private:
    std::list<split_area*> *split_areas_;
public:
    void add_rect(split_area split_area);
    void init_from_config();
    split_area* find_area(const POINT& point);
    split_area_manage();
    ~split_area_manage();
};
