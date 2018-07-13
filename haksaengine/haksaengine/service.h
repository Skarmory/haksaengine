#pragma once

// Base class for services to be accessed via the global Services class
class Service
{
public:
	virtual ~Service(void) = default;
};