#include "CameraD3D11.h"

using namespace DirectX;

CameraD3D11::CameraD3D11(ID3D11Device* device, const ProjectionInfo& projectionInfo, const XMFLOAT3& initialPosition) :
	projInfo(projectionInfo), position(initialPosition)
{
}

void CameraD3D11::Initialize(ID3D11Device* device, const ProjectionInfo& projectionInfo, const XMFLOAT3& initialPosition)
{
	this->projInfo = projectionInfo;
	this->position = initialPosition;
}

const XMFLOAT3& CameraD3D11::GetPosition() const
{
	return this->position;
}

const XMFLOAT3& CameraD3D11::GetForward() const
{
	return this->forward;
}

const XMFLOAT3& CameraD3D11::GetRight() const
{
	return this->right;
}

const XMFLOAT3& CameraD3D11::GetUp() const
{
	return this->up;
}

const ProjectionInfo& CameraD3D11::getProjectionInfo() const
{
	return this->projInfo;
}

XMFLOAT4X4 CameraD3D11::GetViewMatrix() const
{
	XMVECTOR eyePosition = XMLoadFloat3(&this->position);
	XMVECTOR eyeDirection = XMLoadFloat3(&this->forward);
	XMVECTOR upVector = XMLoadFloat3(&this->up);

	XMFLOAT4X4 viewMatrix;
	XMStoreFloat4x4(&viewMatrix, XMMatrixLookToLH(eyePosition, eyeDirection, upVector));

	return viewMatrix;
}

XMFLOAT4X4 CameraD3D11::GetProjectionMatrix() const
{
	ProjectionInfo projectionInfo = this->getProjectionInfo();
	XMFLOAT4X4 projectionMatrix;
	XMStoreFloat4x4(&projectionMatrix, XMMatrixPerspectiveFovLH(projectionInfo.fovAngleY, projectionInfo.aspectRatio, projectionInfo.nearZ, projectionInfo.farZ));

	return projectionMatrix;
}

void CameraD3D11::MoveInDirection(float amount, const XMFLOAT3& direction)
{
	XMVECTOR speedVector = XMVectorReplicate(amount);
	XMVECTOR directionVector = XMLoadFloat3(&direction);
	XMVECTOR positionVector = XMLoadFloat3(&this->position);

	XMVECTOR newValue = XMVectorMultiplyAdd(speedVector, directionVector, positionVector);
	XMStoreFloat3(&this->position, newValue);
}

void CameraD3D11::MoveForward(float amount)
{
	this->MoveInDirection(amount, this->forward);
}

void CameraD3D11::MoveRight(float amount)
{
	this->MoveInDirection(amount, this->right);
}

void CameraD3D11::MoveUp(float amount)
{
	this->MoveInDirection(amount, this->up);
}
