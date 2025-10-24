#include "VirtualController.h"

void VirtualController::Initialize() {
    input_ = MLEngine::Input::Manager::GetInstance();
}

bool VirtualController::UpTriger() const{
    if (input_->GetKeyboard()->Trigger(DIK_W) || input_->GetKeyboard()->Trigger(DIK_UP) 
        || input_->GetGamePad()->TriggerButton(MLEngine::Input::UP)){
        return true;
    }
    return false;
}

bool VirtualController::DownTriger()const {
    if (input_->GetKeyboard()->Trigger(DIK_S) || input_->GetKeyboard()->Trigger(DIK_DOWN)
        || input_->GetGamePad()->TriggerButton(MLEngine::Input::DOWN)) {
        return true;
    }
    return false;
}

bool VirtualController::RightTriger() const {
    if (input_->GetKeyboard()->Trigger(DIK_D) || input_->GetKeyboard()->Trigger(DIK_RIGHT)
        || input_->GetGamePad()->TriggerButton(MLEngine::Input::RIGHT)) {
        return true;
    }
    return false;
}

bool VirtualController::LeftTriger() const {
    if (input_->GetKeyboard()->Trigger(DIK_A) || input_->GetKeyboard()->Trigger(DIK_LEFT)
        || input_->GetGamePad()->TriggerButton(MLEngine::Input::LEFT)) {
        return true;
    }
    return false;
}

bool VirtualController::UpPush() const {
    if (input_->GetKeyboard()->Push(DIK_W) || input_->GetKeyboard()->Push(DIK_UP)
        || input_->GetGamePad()->PushButton(MLEngine::Input::UP)) {
        return true;
    }
    return false;
}

bool VirtualController::DownPush() const {
    if (input_->GetKeyboard()->Push(DIK_S) || input_->GetKeyboard()->Push(DIK_DOWN)
        || input_->GetGamePad()->PushButton(MLEngine::Input::DOWN)) {
        return true;
    }
    return false;
}

bool VirtualController::RightPush()  const {
    if (input_->GetKeyboard()->Push(DIK_D) || input_->GetKeyboard()->Push(DIK_RIGHT)
        || input_->GetGamePad()->PushButton(MLEngine::Input::RIGHT)) {
        return true;
    }
    return false;
}

bool VirtualController::LeftPush() const {
    if (input_->GetKeyboard()->Push(DIK_A) || input_->GetKeyboard()->Push(DIK_LEFT)
        || input_->GetGamePad()->PushButton(MLEngine::Input::LEFT)) {
        return true;
    }
    return false;
}

bool VirtualController::UpRelease()  const {
    if (input_->GetKeyboard()->Release(DIK_W) || input_->GetKeyboard()->Release(DIK_UP)
        || input_->GetGamePad()->ReleaseButton(MLEngine::Input::UP)) {
        return true;
    }
    return false;
}

bool VirtualController::DownRelease()  const {
    if (input_->GetKeyboard()->Release(DIK_S) || input_->GetKeyboard()->Release(DIK_DOWN)
        || input_->GetGamePad()->ReleaseButton(MLEngine::Input::DOWN)) {
        return true;
    }
    return false;
}

bool VirtualController::RightRelease()  const {
    if (input_->GetKeyboard()->Release(DIK_D) || input_->GetKeyboard()->Release(DIK_RIGHT)
        || input_->GetGamePad()->ReleaseButton(MLEngine::Input::RIGHT)) {
        return true;
    }
    return false;
}

bool VirtualController::LeftRelease() const {
    if (input_->GetKeyboard()->Release(DIK_A) || input_->GetKeyboard()->Release(DIK_LEFT)
        || input_->GetGamePad()->ReleaseButton(MLEngine::Input::LEFT)) {
        return true;
    }
    return false;
}



bool VirtualController::Decide() const{
    if (input_->GetKeyboard()->Trigger(DIK_SPACE) || input_->GetKeyboard()->Trigger(DIK_RETURN)
        || input_->GetGamePad()->TriggerButton(MLEngine::Input::A)) {
        return true;
    }
    return false;
}
