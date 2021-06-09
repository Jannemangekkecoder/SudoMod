//NativeInvoker.cpp
#include "stdafx.h"

static NativeManagerContext g_context;

static UINT64 g_hash;

void(*scrNativeCallContext::SetVectorResults)(scrNativeCallContext*) = nullptr;

extern "C" void	_call_asm(void* context, void* function, void* ret);

void nativeInit(UINT64 hash) {

	g_context.Reset();
	g_hash = hash;
}

void nativePush64(UINT64 value) {

	g_context.Push(value);
}

uint64_t* nativeCall() {

	auto handler = Hooking::GetNativeHandler(g_hash);

	if (handler != nullptr) {

		static void* exceptionadd;

		__try
		{
			_call_asm(&g_context, handler, Hooking::m_native_return); // Native Spoof
			scrNativeCallContext::SetVectorResults(&g_context); // "Fix" Vectors
		}
		__except (exceptionadd = (GetExceptionInformation())->ExceptionRecord->ExceptionAddress, EXCEPTION_EXECUTE_HANDLER)
		{
			Log_Error("Error Executing Native 0x%016llx At Address %p.", g_hash, exceptionadd);
		}
	}

	return reinterpret_cast<uint64_t*>(g_context.GetResultPointer());
}

