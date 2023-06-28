#pragma once

// 어떤 메모리를 사용할 것인지... 등


// Constant Buffer
// root constant(상수), root descriptor(포인터랑 비슷), root table(어떤 레지스터 사용할지)

class RootSignature
{
public:

	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:

	void CreateSamplerDesc();
	void CreateRootSignature(ComPtr<ID3D12Device> device);

private:

	D3D12_STATIC_SAMPLER_DESC	_samplerDesc;
	ComPtr<ID3D12RootSignature> _signature;
};

