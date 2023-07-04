#pragma once

// 정점으로 이루어진 물체

class Material;

class Mesh
{
public:

	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);

	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	void Render();

private:

	void CreateVBVBuffer(const vector<Vertex>& vertexBuffer);
	void CreateVBV();

	void CreateIBVBuffer(const vector<uint32>& indexBuffer);
	void CreateIBV();

private:

	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;
	uint32 _vertexSize = 0;

	ComPtr<ID3D12Resource>		_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW		_indexBufferView;
	uint32 _indexCount	= 0;
	uint32 _indexSize = 0;

	shared_ptr<Material>			_material = {};
};

