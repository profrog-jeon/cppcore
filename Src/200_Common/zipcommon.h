#pragma once

#ifdef UNICODE
#define internal_zip	internal_zip_w
#else
#define internal_zip	internal_zip_a
#endif

typedef void* HZIP;
typedef DWORD ZRESULT;

