// File:  stringUtilities.h
// Date:  11/8/2013
// Auth:  K. Loux
// Copy:  (c) Kerry Loux 2013
// Desc:  String utilities.

#ifndef STRING_UTILITIES_H_
#define STRING_UTILITIES_H_

// Standard C++ headers
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace
{

//==========================================================================
// Class:			None
// Function:		LeftTrim
//
// Description:		Removes whitespace from the left end of a string.
//
// Input Arguments:
//		s	= std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string&
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::string &LeftTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

//==========================================================================
// Class:			None
// Function:		LeftTrim
//
// Description:		Removes whitespace from the left end of a string.
//
// Input Arguments:
//		s	= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::string LeftTrim(const std::string &s)
{
	std::string s2(s);
	return LeftTrim(s2);
}

//==========================================================================
// Class:			None
// Function:		RightTrim
//
// Description:		Removes whitespace from the right end of a string.
//
// Input Arguments:
//		s	= std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string&
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::string &RightTrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

//==========================================================================
// Class:			None
// Function:		RightTrim
//
// Description:		Removes whitespace from the right end of a string.
//
// Input Arguments:
//		s	= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::string RightTrim(const std::string &s)
{
	std::string s2(s);
	return RightTrim(s2);
}

//==========================================================================
// Class:			None
// Function:		Trim
//
// Description:		Removes whitespace from both ends of a string.
//
// Input Arguments:
//		s	= std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::wstring&
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::string &Trim(std::string& s)
{
	return LeftTrim(RightTrim(s));
}

//==========================================================================
// Class:			None
// Function:		Trim
//
// Description:		Removes whitespace from both ends of a string.
//
// Input Arguments:
//		s	= const std::string&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::string Trim(const std::string &s)
{
	std::string s2(s);
	return Trim(s2);
}

//==========================================================================
// Class:			None
// Function:		LeftTrim
//
// Description:		Removes whitespace from the left end of a string.
//
// Input Arguments:
//		s	= std::wstring&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::wstring&
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::wstring &LeftTrim(std::wstring& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

//==========================================================================
// Class:			None
// Function:		LeftTrim
//
// Description:		Removes whitespace from the left end of a string.
//
// Input Arguments:
//		s	= const std::wstring&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::wstring
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::wstring LeftTrim(const std::wstring &s)
{
	std::wstring s2(s);
	return LeftTrim(s2);
}

//==========================================================================
// Class:			None
// Function:		RightTrim
//
// Description:		Removes whitespace from the right end of a string.
//
// Input Arguments:
//		s	= std::wstring&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::wstring&
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::wstring &RightTrim(std::wstring& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

//==========================================================================
// Class:			None
// Function:		RightTrim
//
// Description:		Removes whitespace from the right end of a string.
//
// Input Arguments:
//		s	= const std::wstring&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::wstring
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::wstring RightTrim(const std::wstring &s)
{
	std::wstring s2(s);
	return RightTrim(s2);
}

//==========================================================================
// Class:			None
// Function:		Trim
//
// Description:		Removes whitespace from both ends of a string.
//
// Input Arguments:
//		s	= std::wstring&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::string&
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::wstring &Trim(std::wstring& s)
{
	return LeftTrim(RightTrim(s));
}

//==========================================================================
// Class:			None
// Function:		Trim
//
// Description:		Removes whitespace from both ends of a string.
//
// Input Arguments:
//		s	= const std::wstring&
//
// Output Arguments:
//		None
//
// Return Value:
//		std::wstring
//
// Exception Safety:
//		Strong
//
//==========================================================================
static inline std::wstring Trim(const std::wstring &s)
{
	std::wstring s2(s);
	return Trim(s2);
}

}

#endif// STRING_UTILITIES_H_