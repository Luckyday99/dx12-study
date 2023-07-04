#pragma once

// std::byte 사용하지 않음
#define _HAS_STD_BYTE 0

// 각종 include
#include <Windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

// DirectX
#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <DirectXTex\DirectXTex.h>
#include <DirectXTex\DirectXTex.inl>

// 각종 library
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex_Release.lib")
#endif


// 각종 typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= XMFLOAT2;
using Vec3		= XMFLOAT3;
using Vec4		= XMFLOAT4;
using Matrix	= XMMATRIX;

using REGISTER_TYPE = uint8;

enum class CBV_REGISTER : REGISTER_TYPE
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : REGISTER_TYPE
{
	t0 = static_cast<REGISTER_TYPE>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<REGISTER_TYPE>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT
};

struct WindowInfo
{
	HWND	hwnd; // 출력 윈도우
	int32	width;
	int32	height;
	bool	windowed; // 창모드
};	

struct Vertex
{
	Vec3 pos;	// xyz
	Vec4 color; // RGBA
	Vec2 uv;	// UV
};

extern unique_ptr<class Engine> GENGINE;
extern unique_ptr<class Input>	GINPUT;

#define DEVICE					GENGINE->GetDevice()
#define ROOT_SIGNATURE			GENGINE->GetRootSignature()
#define CMD_QUEUE				GENGINE->GetCmdQueue()

#define CMD_LIST				GENGINE->GetCmdQueue()->GetCmdList()
#define DELTA_TIME				GENGINE->GetTimer()->GetDeltaTime()

#define CONSTANT_BUFFER(type)	GENGINE->GetCstBuffer(type)
