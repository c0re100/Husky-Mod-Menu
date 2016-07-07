#pragma once


void ScriptMain();
DWORD64 TunablesPointer();

template <typename T> void SetTunable(int index, T value)
{
	static DWORD64 tunablesPointer = TunablesPointer();
	*reinterpret_cast<T*>(tunablesPointer + index * 8) = value;
}

template <typename T> T GetTunable(int index)
{
	static DWORD64 tunablesPointer = TunablesPointer();
	return *reinterpret_cast<T*>(tunablesPointer + index * 8);
}