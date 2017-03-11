#pragma once

#include "enginesettings.h"
#include "enginenamespace.h"
#include "enginenamespaceext.h"

#ifdef ANDROID
int main(int,char**);

namespace _engine
{
	static _engine::MainFunction _instanceMainFunction(&main);
}

#endif
