#include "interface.h"

#include "rlbot_generated.h"
#include "flatbuffers/flatbuffers.h"

#include <windows.h>

typedef bool(*BoolFunc)(void);
typedef ByteBuffer(*ByteBufferFunc)(void);
typedef void(*VoidFunc)(void*);
typedef int(*SendPacketFunc)(void*, int);

BoolFunc _isInitialized;
VoidFunc _free;
ByteBufferFunc _updateLiveDataPacketFlatbuffer;
ByteBufferFunc _updateFieldInfoFlatbuffer;

SendPacketFunc _updatePlayerInputFlatbuffer;

void Interface::Init(std::string dll)
{
	HMODULE handle = LoadLibrary(dll.c_str());

	_isInitialized = (BoolFunc)GetProcAddress(handle, "IsInitialized");
	_free = (VoidFunc)GetProcAddress(handle, "Free");
	_updateLiveDataPacketFlatbuffer = (ByteBufferFunc)GetProcAddress(handle, "UpdateLiveDataPacketFlatbuffer");
	_updateFieldInfoFlatbuffer = (ByteBufferFunc)GetProcAddress(handle, "UpdateFieldInfoFlatbuffer");

	_updatePlayerInputFlatbuffer = (SendPacketFunc)GetProcAddress(handle, "UpdatePlayerInputFlatbuffer");

}

bool Interface::IsInitialized()
{
	return _isInitialized();
}

void Interface::Free(void* ptr)
{
	_free(ptr);
}

ByteBuffer Interface::UpdateLiveDataPacketFlatbuffer()
{
	return _updateLiveDataPacketFlatbuffer();
}

ByteBuffer Interface::UpdateFieldInfoFlatbuffer()
{
	return _updateFieldInfoFlatbuffer();
}

int Interface::SetBotInput(Controller input, int index)
{
	flatbuffers::FlatBufferBuilder builder(50);

	flatbuffers::Offset<rlbot::flat::ControllerState> controllerStateOffset = rlbot::flat::CreateControllerState(
		builder,
		input.throttle, 
		input.steer, 
		input.pitch, 
		input.yaw, 
		input.roll, 
		input.jump, 
		input.boost, 
		input.handbrake);

	flatbuffers::Offset<rlbot::flat::PlayerInput> playerInputOffset = rlbot::flat::CreatePlayerInput(
		builder,
		index, 
		controllerStateOffset);

	builder.Finish(playerInputOffset);
	builder.GetSize();

	return _updatePlayerInputFlatbuffer(builder.GetBufferPointer(), builder.GetSize());
}
