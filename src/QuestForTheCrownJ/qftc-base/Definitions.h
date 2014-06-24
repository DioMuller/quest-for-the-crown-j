#pragma once

// Linker definitions
#ifdef _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#endif

// FPS Definitions
#define FPS 60.0
#define MILISECONDS_PER_FRAME (double) (1.0 / FPS)

// Properties
#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME

// Declare methods as properties
#define getdecl(type, name) __declspec(property(get = Get##name)) type name
#define setdecl(type, name) __declspec(property(put = Set##name)) type name
#define getsetdecl(type, name) __declspec(property(put = Set##name, get = Get##name)) type name

// creates a Get property to an existing field
#define propget(type, field, name) type Get##name() { return this->field; } getdecl(type, name)
// creates a Set property to an existing field
#define propset(type, field, name) void Set##name(type field) { this->field = field; } setdecl(type, name)
// creates a Get and a Set property to an existing field
#define propgetset(get_visibility, set_visibility, type, field, name) get_visibility: type Get##name() { return this->field; } set_visibility: void Set##name(type field) { this->field = field; } getsetdecl(type, name)
// create a field and expose it as a property
#define autoprop(get_visibility, set_visibility, type, field, name) private: type field; propgetset(get_visibility, set_visibility, type, field, name)
