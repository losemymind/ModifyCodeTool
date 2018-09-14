
#ifndef MODIFYCODETOOL_Brolgacrane_HPP
#define MODIFYCODETOOL_Brolgacrane_HPP
#pragma once
#include <string>
class Brolgacrane
{
public:
    Brolgacrane();
    virtual ~Brolgacrane();
protected:
    void SetBrolgacraneName(const std::string& InName);
    std::string GetBrolgacraneName();
private:
    std::string MyName;

protected:
    std::string poppy_cggUyR_Waterlegaan(int arg0, float arg1);

    std::string peony_lEPRjVhr_Martespennanti(int arg0, float arg1);

    std::string carnation_KAUJOlva_Geladababoon(int arg0, float arg1);
};

#endif // END OF MODIFYCODETOOL_Brolgacrane_HPP
