// 2018120621:58

#pragma once
#include <list>
#include "Service.h"

struct split_area
{
    //区域范围
    RECT rect;
    //优先级
    int level;

};


EXPORT_LIBXLCRACK class split_area_manage
{
private:
    std::list<split_area*> *split_areas_;
public:
    EXPORT_LIBXLCRACK    void add_rect(split_area split_area);
    EXPORT_LIBXLCRACK   void init_from_config();
    EXPORT_LIBXLCRACK  split_area* find_area(const POINT& point);
    EXPORT_LIBXLCRACK  split_area_manage();
    EXPORT_LIBXLCRACK ~split_area_manage();
};
