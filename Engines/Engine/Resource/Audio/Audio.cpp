#include "Audio.h"
#include "Core/AudioManager.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Core;

void Audio::Load(const std::string& filename) {

	handle_ = 	AudioManager::GetInstance()->LoadInMF(filename);

}

void Audio::Play(float volume, bool isLoop)
{

	AudioManager::GetInstance()->Play(handle_, volume, isLoop);

}

void Audio::Stop()
{

	AudioManager::GetInstance()->Stop(handle_);

}

void Audio::Pause() {

	AudioManager::GetInstance()->Pause(handle_);

}

void Audio::ReStart() {

	AudioManager::GetInstance()->ReStart(handle_);

}

void Audio::SetVolume(float volume) {

	AudioManager::GetInstance()->SetVolume(handle_, volume);

}

bool Audio::IsPlaying()
{
	return AudioManager::GetInstance()->IsPlaying(handle_);
}
