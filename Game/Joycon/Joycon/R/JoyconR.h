#pragma once
#include "../Joycon.h"

class JoyconR : public Joycon
{
public:

	direction CheakRadius()override;

	void ImGui(std::string title)override;
private:


};