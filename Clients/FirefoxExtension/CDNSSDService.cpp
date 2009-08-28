#include "CDNSSDService.h"
#include "nsThreadUtils.h"
#include "nsIEventTarget.h"
#include "private/pprio.h"
#include <string>
#include <stdio.h>

static FILE * m_fp;


NS_IMPL_ISUPPORTS2(CDNSSDService, IDNSSDService, nsIRunnable)

CDNSSDService::CDNSSDService()
:
	m_threadPool( NULL ),
	m_mainRef( NULL ),
	m_subRef( NULL ),
	m_listener( NULL ),
	m_fileDesc( NULL ),
	m_job( NULL )
{
	nsresult err;

	//m_fp = fopen( "/Users/scott/scott.txt", "w+" );
	m_fp = stderr;
	fprintf( m_fp, "CDNSSDService::CDNSSDService( this = 0x%x )\n", this );
	fflush( m_fp );
	
	if ( DNSServiceCreateConnection( &m_mainRef ) != kDNSServiceErr_NoError )
	{
		err = NS_ERROR_FAILURE;
		goto exit;
	}

	if ( ( m_fileDesc = PR_ImportTCPSocket( DNSServiceRefSockFD( m_mainRef ) ) ) == NULL )
	{
		err = NS_ERROR_FAILURE;
		goto exit;
	}

	if ( ( m_threadPool = PR_CreateThreadPool( 1, 1, 8192 ) ) == NULL )
	{
		err = NS_ERROR_FAILURE;
		goto exit;
	}
	
	err = SetupNotifications();

exit:

	if ( err != NS_OK )
	{
		Cleanup();
	}
}


CDNSSDService::CDNSSDService( DNSServiceRef ref, nsISupports * listener )
:
	m_threadPool( NULL ),
	m_mainRef( ref ),
	m_subRef( ref ),
	m_listener( listener ),
	m_fileDesc( NULL ),
	m_job( NULL )
{
}


CDNSSDService::~CDNSSDService()
{
	fprintf( m_fp, "in CDNSSDService::~CDNSSDService( this = 0x%x)\n", this );
	fflush( m_fp );
	
	Cleanup();
}


void
CDNSSDService::Cleanup()
{
	if ( m_job )
	{
		PR_CancelJob( m_job );
		m_job = NULL;
	}

	if ( m_threadPool != NULL )
	{
		PR_ShutdownThreadPool( m_threadPool );
		m_threadPool = NULL;
	}
	
	if ( m_fileDesc != NULL )
	{
		PR_Close( m_fileDesc );
		m_fileDesc = NULL;
	}
	
	if ( m_subRef )
	{
		DNSServiceRefDeallocate( m_subRef );
		m_subRef = NULL;
	}
	else if ( m_mainRef )
	{
		DNSServiceRefDeallocate( m_mainRef );
		m_mainRef = NULL;
	}
}


nsresult
CDNSSDService::SetupNotifications()
{
	NS_PRECONDITION( m_threadPool != NULL, "m_threadPool is NULL" );
	NS_PRECONDITION( m_fileDesc != NULL, "m_fileDesc is NULL" );
	NS_PRECONDITION( m_job == NULL, "m_job is not NULL" );

	m_iod.socket	= m_fileDesc;
	m_iod.timeout	= PR_INTERVAL_MAX;
	m_job			= PR_QueueJob_Read( m_threadPool, &m_iod, Read, this, PR_FALSE );	
	return ( m_job ) ? NS_OK : NS_ERROR_FAILURE;
}


/* IDNSSDService browse (in long interfaceIndex, in AString regtype, in AString domain, in IDNSSDBrowseListener listener); */
NS_IMETHODIMP
CDNSSDService::Browse(PRInt32 interfaceIndex, const nsAString & regtype, const nsAString & domain, IDNSSDBrowseListener *listener, IDNSSDService **_retval NS_OUTPARAM)
{
	CDNSSDService	*	service	= NULL;
	DNSServiceErrorType dnsErr	= 0;
	nsresult			err		= 0;

	*_retval = NULL;
	
	fprintf( m_fp, "in Browse( listener = 0x%x )\n", listener );
	fflush( m_fp );
	
	if ( !m_mainRef )
	{
		err = NS_ERROR_NOT_AVAILABLE;
		goto exit;
	}

	try
	{
		service = new CDNSSDService( m_mainRef, listener );
	}
	catch ( ... )
	{
		service = NULL;
	}
	
	fprintf( m_fp, "created new service\n" );
	fflush( m_fp );

	if ( service == NULL )
	{
		err = NS_ERROR_FAILURE;
		goto exit;
	}
	
	dnsErr = DNSServiceBrowse( &service->m_subRef, kDNSServiceFlagsShareConnection, interfaceIndex, NS_ConvertUTF16toUTF8( regtype ).get(), NS_ConvertUTF16toUTF8( domain ).get(), ( DNSServiceBrowseReply ) BrowseReply, service );
	
	if ( dnsErr != kDNSServiceErr_NoError )
	{
		fprintf( m_fp, "DNSServiceBrowse failed\n" );
		fflush( m_fp );
		err = NS_ERROR_FAILURE;
		goto exit;
	}
	
	listener->AddRef();
	service->AddRef();
	*_retval = service;
	err = NS_OK;
	
	fprintf( m_fp, "did DNSServiceBrowse\n" );
	fflush( m_fp );
	
exit:

	if ( err && service )
	{
		delete service;
		service = NULL;
	}
	
	return err;
}


/* IDNSSDService resolve (in long interfaceIndex, in AString name, in AString regtype, in AString domain, in IDNSSDResolveListener listener); */
NS_IMETHODIMP
CDNSSDService::Resolve(PRInt32 interfaceIndex, const nsAString & name, const nsAString & regtype, const nsAString & domain, IDNSSDResolveListener *listener, IDNSSDService **_retval NS_OUTPARAM)
{
    CDNSSDService	*	service;
	DNSServiceErrorType dnsErr;
	nsresult			err;

	*_retval = NULL;
	
	fprintf( m_fp, "in Resolve( listener = 0x%x )\n", listener );
	fflush( m_fp );

	if ( !m_mainRef )
	{
		err = NS_ERROR_NOT_AVAILABLE;
		goto exit;
	}

	try
	{
		service = new CDNSSDService( m_mainRef, listener );
	}
	catch ( ... )
	{
		service = NULL;
	}
	
	if ( service == NULL )
	{
		err = NS_ERROR_FAILURE;
		goto exit;
	}

	dnsErr = DNSServiceResolve( &service->m_subRef, kDNSServiceFlagsShareConnection, interfaceIndex, NS_ConvertUTF16toUTF8( name ).get(), NS_ConvertUTF16toUTF8( regtype ).get(), NS_ConvertUTF16toUTF8( domain ).get(), ( DNSServiceResolveReply ) ResolveReply, service );
	
	if ( dnsErr != kDNSServiceErr_NoError )
	{
		err = NS_ERROR_FAILURE;
		goto exit;
	}
	
	listener->AddRef();
	service->AddRef();
	*_retval = service;
	err = NS_OK;
	
	fprintf( m_fp, "did DNSServiceResolve\n" );
	fflush( m_fp );
	
exit:
	
	if ( err && service )
	{
		delete service;
		service = NULL;
	}
	
	return err;
}


/* void stop (); */
NS_IMETHODIMP
CDNSSDService::Stop()
{
    if ( m_subRef )
	{
		DNSServiceRefDeallocate( m_subRef );
		m_subRef = NULL;
	}
	
	return NS_OK;
}


void
CDNSSDService::Read( void * arg )
{
	NS_PRECONDITION( arg != NULL, "arg is NULL" );
	
	NS_DispatchToMainThread( ( CDNSSDService* ) arg );
}


NS_IMETHODIMP
CDNSSDService::Run()
{
	nsresult err;
	
	NS_PRECONDITION( m_mainRef != NULL, "m_mainRef is NULL" );

	fprintf( m_fp, "in CDNSSDService::Run()\n" );
	fflush( m_fp );
	
	m_job = NULL;

	if ( DNSServiceProcessResult( m_mainRef ) == kDNSServiceErr_NoError )
	{
		err = SetupNotifications();
	}
	else
	{
		err = NS_ERROR_FAILURE;
	}
	
	return err;
}


void DNSSD_API
CDNSSDService::BrowseReply
		(
		DNSServiceRef		sdRef,
		DNSServiceFlags		flags,
		uint32_t			interfaceIndex,
		DNSServiceErrorType	errorCode,
		const char		*	serviceName,
		const char		*	regtype,
		const char		*	replyDomain,
		void			*	context
		)
{
	CDNSSDService * self = ( CDNSSDService* ) context;

	fprintf( m_fp, "in browseReply: interfaceIndex = %d\n", interfaceIndex );
	fflush( m_fp );

	// This should never be NULL, but let's be defensive.
	
	if ( self != NULL )
	{
		IDNSSDBrowseListener * listener = ( IDNSSDBrowseListener* ) self->m_listener;

		// Same for this

		if ( listener != NULL )
		{
			listener->OnBrowse( self, ( flags & kDNSServiceFlagsAdd ) ? PR_TRUE : PR_FALSE, interfaceIndex, errorCode, NS_ConvertUTF8toUTF16( serviceName ), NS_ConvertUTF8toUTF16( regtype ), NS_ConvertUTF8toUTF16( replyDomain ) );
		}
	}
}


void DNSSD_API
CDNSSDService::ResolveReply
		(
		DNSServiceRef			sdRef,
		DNSServiceFlags			flags,
		uint32_t				interfaceIndex,
		DNSServiceErrorType		errorCode,
		const char			*	fullname,
		const char			*	hosttarget,
		uint16_t				port,
		uint16_t				txtLen,
		const unsigned char	*	txtRecord,
		void				*	context
		)
{
	CDNSSDService * self = ( CDNSSDService* ) context;
	
	fprintf( m_fp, "in CDNSSDService::ResolveReply()\n" );
	fflush( m_fp );

	// This should never be NULL, but let's be defensive.
	
	if ( self != NULL )
	{
		IDNSSDResolveListener * listener = ( IDNSSDResolveListener* ) self->m_listener;
		
		// Same for this

		if ( listener != NULL )
		{
			std::string		path = "";
			const void	*	value = NULL;
			uint8_t			valueLen = 0;

			value = TXTRecordGetValuePtr( txtLen, txtRecord, "path", &valueLen );
			
			if ( value && valueLen )
			{
				char * temp;
				
				temp = new char[ valueLen + 1 ];
				
				if ( temp )
				{
					memset( temp, 0, valueLen + 1 );
					memcpy( temp, value, valueLen );
					path = temp;
					delete [] temp;
				}
			}

			listener->OnResolve( self, interfaceIndex, errorCode, NS_ConvertUTF8toUTF16( fullname ), NS_ConvertUTF8toUTF16( hosttarget ) , ntohs( port ), NS_ConvertUTF8toUTF16( path.c_str() ) );
		}
	}
}

