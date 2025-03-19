#pragma once

#include "UserInterface.hpp"
#include "Injector.hpp"

class UltraInjector
{
public:
	/* Default constructer and and destructor */

	UltraInjector() = default;
	~UltraInjector() = default;
public:
	/* Initializes the injector and user interface */
	void Initialize();
};

