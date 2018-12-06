#include "stdafx.h"
#include "SplitArea.h"

void split_area_manage::add_rect(split_area split_area)
{
}

void split_area_manage::init_from_config()
{

}

split_area* split_area_manage::find_area(const POINT& point)
{
    split_area *_area = nullptr;
    for (split_area* area : split_areas_)
    {
        if (area->rect.left < point.x && point.x < area->rect.right && area->rect.top < point.y
            && point.y < area->rect.bottom)
        {
            if (_area == nullptr)
            {
                _area = area;
            }
            else
            {
                if (area->level > _area->level)
                {
                    _area = area;
                }
            }
        }
    }
    return _area;
}

split_area_manage::split_area_manage()
{
    split_areas_ = new std::list<split_area*>;
}

split_area_manage::~split_area_manage()
{
    if (split_areas_ != nullptr)
    {
        for (split_area* area : *split_areas_)
        {
            delete area;
        }
        delete split_areas_;
    }
}
