#ifndef TX_SCRIPTING_HPP
#define TX_SCRIPTING_HPP

#include <string>
using namespace std;

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <luabind/luabind.hpp>

class Scripting {
	public:
		Scripting();
		~Scripting();

	private:
		static void ScriptLog_(string msg);		
		void bindAll_();
		luabind::scope bindObject_();
		
		lua_State *myLuaState_;
};

#endif
