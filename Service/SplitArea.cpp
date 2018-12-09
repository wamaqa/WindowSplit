#include "stdafx.h"
#include "SplitArea.h"
#include <fstream>
#include <json/json.h>
void split_area_manage::add_rect(split_area split_area)
{
}

bool split_area_manage::init_from_config()
{     

    std::ifstream is;
    wchar_t* pCurrDir = new wchar_t[260];
    ::GetCurrentDirectory(260, pCurrDir);
    std::wstring path = pCurrDir;
    std::wstring fileName = path + L"\\..\\bin\\config.json";
    is.close();
    is.open(fileName, std::ios::binary);
    if (!is.is_open())
    {
        return false;
    }
    std::string str((std::istreambuf_iterator<char>(is)),
        std::istreambuf_iterator<char>());

    Json::Value root;
    Json::CharReaderBuilder jsreader;
    std::unique_ptr<Json::CharReader> const reader(jsreader.newCharReader());
    char pStart[1024];

    std::string err;
    reader->parse(str.begin()._Ptr, str.end()._Ptr, &root, &err);
    for (auto values : root)
    {
        split_area *split = new split_area;
        split->rect.left = values["rect"]["left"].asInt();
        split->rect.top = values["rect"]["top"].asInt();
        split->rect.right = values["rect"]["right"].asInt();
        split->rect.bottom = values["rect"]["bottom"].asInt();
        split->level = values["level"].asInt();
        split_areas_->push_front(split);

    }
    delete pCurrDir;
    return true;
}

split_area* split_area_manage::find_area(const POINT& point)
{
    split_area *_area = nullptr;
    for (split_area* area : *split_areas_)
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
