#pragma once

class AfterCharStdioFlusher
{
public:
	AfterCharStdioFlusher();
	~AfterCharStdioFlusher();

private:
	class Impl;
	Impl *impl;
};