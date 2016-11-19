#pragma once

#include <memory>

class AfterCharStdioFlusher
{
public:
	AfterCharStdioFlusher();
	~AfterCharStdioFlusher();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};