
#ifndef MODIFYCODETOOL_Sandhillcrane_HPP
#define MODIFYCODETOOL_Sandhillcrane_HPP
#pragma once
#include <string>
class Sandhillcrane
{
public:
    Sandhillcrane();
    virtual ~Sandhillcrane();
protected:
    void SetSandhillcraneName(const std::string& InName);
    std::string GetSandhillcraneName();
private:
    std::string MyName;

protected:
    std::string bough_SCdXs_Small_spottedgenet(int arg0, float arg1);

    std::string cambium_yXDlLOH_Mexicanwolf(int arg0, float arg1);

    std::string coco_hjxKywd_Roanantelope(int arg0, float arg1);
};

#endif // END OF MODIFYCODETOOL_Sandhillcrane_HPP
