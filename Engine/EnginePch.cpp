#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"
#include "Input.h"

unique_ptr<Engine>	GENGINE = make_unique<Engine>();
unique_ptr<Input>	GINPUT = make_unique<Input>();