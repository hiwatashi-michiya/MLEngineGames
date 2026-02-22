#pragma once
#include "../Joycon.h"

class JoyconR : public Joycon
{
public:
	void addInit()override;
	direction CheakRadius()override;

	void ImGui(std::string title)override;
private:


};