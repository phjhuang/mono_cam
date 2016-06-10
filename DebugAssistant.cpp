// DebugAssistant.cpp: implementation of the DebugAssistant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DebugAssistant.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _OUTPUT_INFO_
DebugAssistant _DebugInfo( DEBUG_CONSOLE );
#endif

DebugAssistant::DebugAssistant( DEBUG_TYPE mode ,  char* fileName ):
m_DebugMode( mode ),
m_FileName( fileName ),
m_OutputHandle( NULL ),
m_pFile( NULL )
{
	
	if( DEBUG_CONSOLE == m_DebugMode )
	{
		AllocConsole();
		m_OutputHandle = ::GetStdHandle( STD_OUTPUT_HANDLE );
	}
	if( NULL != fileName )
		m_pFile = ::fopen( fileName , "wb" );
}

DebugAssistant::~DebugAssistant()
{
	if( DEBUG_CONSOLE == m_DebugMode )
	{
		if( m_OutputHandle )
		{
			::CloseHandle( m_OutputHandle );
			m_OutputHandle = NULL;
		}
		FreeConsole();
	}
	if( m_pFile )
		fclose( m_pFile );
}

void DebugAssistant::OutputInfo( char* format , ... )
{
    char msg[1024] = {0};
	va_list arg_ptr;
	va_start( arg_ptr, format );
	vsprintf( msg , format, arg_ptr );
	::strcat( msg , "\n" );
	va_end(arg_ptr);
	int length = ::strlen( msg );
	AoutLock lock( &m_SynLog );
	switch( m_DebugMode )
	{
	case DEBUG_CONSOLE:
		{	
			ULONG beWritten = 0;
			::WriteConsole( m_OutputHandle , msg , length , &beWritten , NULL );
		}
		break;
	case DEBUG_STDOUT:
		::OutputDebugString( msg );
		break;
	case DEBUG_FILE:
		if( m_pFile )
		{
			::fwrite( msg , 1 , length , m_pFile );
			::fwrite( "\n" , 1 , 1 , m_pFile );
		}
		break;
	default:
		break;
	}
}