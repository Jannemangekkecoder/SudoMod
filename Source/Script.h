#pragma once

void ScriptMain();

enum SubMenus { //Add Sub Menus in here
	NOMENU,
	MAINMENU,
	TEST,
	PROTECTIONS,
	PLAYERLIST,
	PLAYERLIST_SELECTED
};

//
// 	   I honestly don't see why a helper function is needed
// 	   for this when std::string's `const c_str()` method
// 	   exists for this very purpose...and avoids the extra
// 	   heap allocation and copy from using _strdup.
// 	   
// 	   Either way, the current implementation of "StringToChar"
// 	   has a memory leak from not freeing the memory allocated
// 	   by _strdup, so I threw together a (naive) wrapper 
// 	   functor to handle cleanup when going out of scope.
// 
// 	   ~SK();
//
//inline char* StringToChar(std::string string) {
//	return _strdup(string.c_str());
//}

class StringToChar {
	char* ptr_;

public:
	inline explicit StringToChar(std::string&& str) : ptr_(_strdup(str.c_str())) { }
	inline explicit StringToChar(const std::string& str) : ptr_(_strdup(str.c_str())) { }
	~StringToChar() { free(ptr_); }

	StringToChar() = delete;
	StringToChar(StringToChar&&) = delete;
	StringToChar(const StringToChar&) = delete;
	StringToChar& operator=(StringToChar&&) = delete;
	StringToChar& operator=(const StringToChar&) = delete;

	inline operator char* () const noexcept { return ptr_; }
	inline operator const char* () const noexcept { return ptr_; }

	inline const char* operator()(std::string&& str) {
		ptr_ = _strdup(str.c_str());
		return ptr_;
	}
};
