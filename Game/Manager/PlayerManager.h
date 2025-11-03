#pragma once
#include"Player/Player.h"
//プレイヤーの生成、更新、参照
class PlayerManager{
public:
   

    void Initialize() {
        player_ = std::make_unique<Player>();
        player_->Initialize();
    }

    void Update(float deltaTime) {
        player_->Update(deltaTime);
    }

    void Draw() {
        player_->Draw();
    }

    Player* GetPlayer() const { return player_.get(); }

private:
    std::unique_ptr<Player> player_;

};

