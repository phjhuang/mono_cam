
#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#pragma once

#include "oaidl.h"
#include "ocidl.h"



#include <stdio.h>
#include <iostream>
#include <windows.h>
#define DATHDLEN 8192
#define FRAMEHEADLEN 512
#define BUFFERSIZE (1280*1024*1+1024)
#include "..\\..\\include\\CriticalSection.h"


#include <initguid.h>
 
#include "hBaseCasheMap.h"
#include "..\..\include\CAT_Tools.h"

using namespace CCASHEMAPLib;

#define IBaseCasheMap CBaseCasheMap


 



