#include "Joycon.h"

void Joycon::Init() {
	hidManager_ = std::make_unique<hidManager>();
	hidManager_->Init();

	device_ = hidManager_->Get(JOYCON_L_PRODUCT_ID);
}

void Joycon::CheckConeect()
{
	
}



