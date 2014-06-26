#include "Hash.h"
#include "SHA1.h"

using namespace qftcnetlib;

std::string hash(std::string text)
{
	return sha1(text);
}